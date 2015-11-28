#include "StdAfx.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "SceneSerializer.h"
#include "TerrainObject.h"
#include "ETTerrain.h"

namespace Ogre
{

	SceneSystem::SceneSystem(void)
		: m_sceneMgr(0)
		, m_collisionWorld(0)
		, m_triggerWorld(0)
	{
	}

	SceneSystem::~SceneSystem(void)
	{
		for(MapScene::iterator iter = m_mapScene.begin() ; iter != m_mapScene.end() ; iter ++)
		{
			delete iter->second;
		}
		m_mapScene.clear();
	}
	// 根据实例名称和文件名载入一个场景
	IScene* SceneSystem::loadScene(const String &name , const String &fileName)
	{
		IScene *scene = createScene(name);
		if(!scene)
		{
			return 0;
		}
		SceneSerializer serializer;
		if(!serializer.importScene(fileName , scene))
		{
			destroyScene(scene);
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"载入场景" + fileName + "失败", 
				"SceneSystem::loadScene");
			return 0;
		}
		return scene;
	}
	// 根据文件名载入一个场景
	IScene* SceneSystem::loadScene(const String &fileName , IScene *scene)
	{
		if(scene == 0)
		{
			return loadScene(getUniqueName() , fileName);
		}
		else
		{
			SceneSerializer serializer;
			if(!serializer.importScene(fileName , scene))
			{
				OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
					"载入场景" + fileName + "失败", 
					"SceneSystem::loadScene");
				return 0;
			}
			return scene;
		}
	}


	// 根据文件名保存一个场景
	bool SceneSystem::saveScene(IScene* scene , const String &fileName)
	{
		SceneSerializer serializer;
		if(!serializer.exportScene(scene , fileName))
		{
			OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE, 
				"保存场景" + fileName + "失败", 
				"SceneSystem::saveScene");
			return false;
		}
		return true;
	}
	// 根据实例名称和文件名保存一个场景
	bool SceneSystem::saveScene(const String &name , const String &fileName)
	{
		IScene *scene = getScene(name);
		return saveScene(scene , fileName);
	}

	// 创建一个场景
	IScene* SceneSystem::createScene()
	{
		return createScene(getUniqueName());
	}

	// 获得一个全局唯一ID
	uint32 SceneSystem::getUniqueID()
	{
		static uint32 s_index = 0;
		return ++s_index;
	}

	// 获得一个全局唯一名称
	String SceneSystem::getUniqueName()
	{
		String name;
		do 
		{
			name = "_scene_" + StringConverter::toString(getUniqueID());
		}while(hasScene(name));
		return name;
	}

	// 创建一个场景
	IScene* SceneSystem::createScene(const String &name)
	{
		if(hasScene(name))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"场景名称为 '" + name + "' 的场景已经存在", 
				"SceneSystem::createScene");
			return 0;
		}
		IScene *scene = new Scene(m_sceneMgr , name , m_collisionWorld , m_triggerWorld);
		m_mapScene[name] = scene;
		return scene;
	}

	// 销毁场景
	void SceneSystem::destroyScene(const String &name)
	{
		MapScene::iterator iter = m_mapScene.find(name);
		if(iter == m_mapScene.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景名称为 '" + name + "' 的场景不存在", 
				"SceneSystem::destroyScene");
			return;
		}
		delete (iter->second);
		m_mapScene.erase(iter);
	}
	
	void SceneSystem::destroyScene(IScene *scene)
	{
		if(scene == 0)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景指针为空", 
				"SceneSystem::destroyScene");
			return;
		}
		MapScene::iterator iter = m_mapScene.find(scene->getName());
		if(iter == m_mapScene.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景名称为 '" + scene->getName() + "' 的场景不存在", 
				"SceneSystem::destroyScene");
			return;
		}
		delete (iter->second);
		m_mapScene.erase(iter);
	}
	// 获得一个场景
	IScene *SceneSystem::getScene(const String &name)
	{
		MapScene::iterator iter = m_mapScene.find(name);
		if(iter == m_mapScene.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景名称为 '" + name + "' 的场景不存在", 
				"SceneSystem::getScene");
			return 0;
		}
		return iter->second;
	}
	// 是否存在指定名字的场景
	bool SceneSystem::hasScene(const String &name)const
	{
		return m_mapScene.find(name) != m_mapScene.end();
	}


	// 为一个Viewport开启运动模糊
	void SceneSystem::setMotionBlurEnabled(Viewport *vp , bool enable)
	{
		// 动态创建运动模糊的混合器
		if(m_motionBlur.isNull() && enable)
		{
			// 运动模糊特效
			m_motionBlur = CompositorManager::getSingleton().create(
				"Motion Blur", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
				);
			{
				CompositionTechnique *t = m_motionBlur->createTechnique();
				{
					CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("scene");
					def->width = 0;
					def->height = 0;
					def->format = PF_R8G8B8;
				}
				{
					CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("sum");
					def->width = 0;
					def->height = 0;
					def->format = PF_R8G8B8;
				}
				{
					CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("temp");
					def->width = 0;
					def->height = 0;
					def->format = PF_R8G8B8;
				}
				// 渲染场景
				{
					CompositionTargetPass *tp = t->createTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
					tp->setOutputName("scene");
				}
				// 为累积贴图初始化
				{
					CompositionTargetPass *tp = t->createTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
					tp->setOutputName("sum");
					tp->setOnlyInitial(true);
				}
				// 运动模糊
				{
					CompositionTargetPass *tp = t->createTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_NONE);
					tp->setOutputName("temp");
					{ CompositionPass *pass = tp->createPass();
					pass->setType(CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Compositor/Combine");
					pass->setInput(0, "scene");
					pass->setInput(1, "sum");
					}
				}
				// 拷贝back纹理
				{
					CompositionTargetPass *tp = t->createTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_NONE);
					tp->setOutputName("sum");
					{ CompositionPass *pass = tp->createPass();
					pass->setType(CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Compositor/Copyback");
					pass->setInput(0, "temp");
					}
				}
				// 显示结果
				{
					CompositionTargetPass *tp = t->getOutputTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_NONE);
					{ CompositionPass *pass = tp->createPass();
					pass->setType(CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Compositor/MotionBlur");
					pass->setInput(0, "sum");
					}
				}
			}
		}
		// 开启动感模糊
		if(!m_motionBlur.isNull())
		{
			setCompositorEnabled(vp , m_motionBlur->getName() , enable);
		}
	}
	// 确保一个混合器已经添加到vp中
	void SceneSystem::ensureCompositorAdd(Viewport *vp , const String &compositor)
	{
		bool added = false;
		CompositorChain *chain = CompositorManager::getSingleton().getCompositorChain(vp);
		if(chain)
		{
			for(size_t i = 0 ; i < chain->getNumCompositors() ; i ++)
			{
				if(chain->getCompositor(i)->getCompositor()->getName() == compositor)
				{
					added = true;
					break;
				}
			}
		}
		if(!added)
		{
			// 添加到混合器
			CompositorManager::getSingleton().addCompositor(vp, m_motionBlur->getName());
			// 默认禁止
			CompositorManager::getSingleton().setCompositorEnabled(vp, m_motionBlur->getName(), false);
		}
	}

	// 在vp上允许或禁用某个compositor
	void SceneSystem::setCompositorEnabled(Viewport *vp , const String &compositor , bool enable)
	{
		ensureCompositorAdd(vp , compositor);
		CompositorManager::getSingleton().setCompositorEnabled(vp, compositor, enable);
	}


}