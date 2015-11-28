#include "StdAfx.h"
#include "Scene.h"
#include "NullObject.h"
#include "ISceneSystem.h"
#include "AnimationStateController.h"
#include "TerrainObject.h"
#include "CameraObject.h"
#include "ETTerrain.h"
#include "ETTile.h"

namespace Ogre
{

	Scene::Scene(SceneManager *mgr , const String &name , ICollisionWorld *collisionWorld , ITriggerWorld *triggerWorld)
		: m_name(name)
		, m_sceneMgr(mgr)
		, m_index(0)
		, m_rootObject(0)
		, m_collisionWorld(collisionWorld)
		, m_triggerWorld(triggerWorld)
		, m_rayQuery(0)
	{
		Root::getSingleton().addFrameListener(this);
		m_rootObject = new NullObject("root_object");
		NameValuePairList params;
		params["isRootNode"] = "";
		m_rootObject->create(this , &params);

		m_rayQuery = m_sceneMgr->createRayQuery(Ray());
	}

	Scene::~Scene(void)
	{
		clear();
		m_sceneMgr->destroyQuery(m_rayQuery);
		m_rayQuery = 0;
		OGRE_DELETE(m_rootObject);
		Root::getSingleton().removeFrameListener(this);
	}

	// 通知这个场景是在哪个文件载入的
	void Scene::_notifyOrigin(const String &origin)
	{
		m_origin = origin;
	}
	// 获得用于创建这个场景的文件名
	const String &Scene::getOrigin()const
	{
		return m_origin;
	}

	// 销毁所有物件
	void Scene::clear()
	{
		// 删除所有动画控制器
		for(MapAnimationStateController::iterator iter = m_mapAnimationStateController.begin() ; iter != m_mapAnimationStateController.end() ; iter ++)
		{
			m_sceneMgr->destroyAnimation(iter->first);
			delete iter->second;
		}
		m_mapAnimationStateController.clear();
		// 删除所有物件
		for(MapSceneObject::iterator iter = m_mapSceneObject.begin() ; iter != m_mapSceneObject.end() ; iter ++)
		{
			ISceneObjectFactory* factory = ISceneSystem::getSingleton().getFactory(iter->second->getType());
			if(!factory)
			{
				OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
					"找不到类型为 '" + iter->second->getType() + "' 的对象工厂", 
					"Scene::destroySceneObject");
				return;
			}
			// 回调到侦听器
			ExecuteListener1(IScene::Listener , onDestroySceneObject , iter->second);
			// 删除对象
			factory->destroy(iter->second);
		}
		m_mapSceneObject.clear();
	}

	// 获得一个场景内唯一ID
	uint32 Scene::getUniqueID()
	{
		return ++m_index;
	}

	// 获得一个场景内唯一名称
	String Scene::getUniqueName()
	{
		String name;
		do 
		{
			name = "_object_" + StringConverter::toString(getUniqueID());
		}while(hasSceneObject(name));
		return name;	
	}

	// 根据类型和名称创建一个场景对象
	ISceneObject *Scene::createSceneObject(const String &type, const NameValuePairList* params)
	{
		return createSceneObject(getUniqueName() , type , params);
	}

	// 根据类型和名称创建一个场景对象
	ISceneObject *Scene::createSceneObject(const String &name , const String &type , const NameValuePairList* params)
	{
		ISceneObjectFactory* factory = ISceneSystem::getSingleton().getFactory(type);
		if(!factory)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"找不到类型为 '" + type + "' 的对象工厂", 
				"Scene::createSceneObject");
			return 0;
		}

		if(hasSceneObject(name))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"场景对象名称为 '" + name + "' 的场景对象已经存在", 
				"Scene::createSceneObject");
			return 0;
		}
		ISceneObject *obj = factory->create(name , this , params);
		m_mapSceneObject[name] = obj;
		// 回调到侦听器
		ExecuteListener1(IScene::Listener , onCreateSceneObject , obj);
		return obj;		
	}


	// 销毁场景对象
	void Scene::destroySceneObject(const String &name)
	{
		MapSceneObject::iterator iter = m_mapSceneObject.find(name);
		if(iter == m_mapSceneObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景对象名称为 '" + name + "' 的场景对象不存在", 
				"Scene::destroyScene");
			return;
		}
		ISceneObjectFactory* factory = ISceneSystem::getSingleton().getFactory(iter->second->getType());
		if(!factory)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"找不到类型为 '" + iter->second->getType() + "' 的对象工厂", 
				"Scene::destroySceneObject");
			return;
		}
		// 回调到侦听器
		ExecuteListener1(IScene::Listener , onDestroySceneObject , iter->second);

		factory->destroy(iter->second);
		m_mapSceneObject.erase(iter);
	}

	void Scene::destroySceneObject(ISceneObject *obj)
	{
		destroySceneObject(obj->getName());
	}
	// 获得一个场景对象
	ISceneObject *Scene::getSceneObject(const String &name)
	{
		MapSceneObject::iterator iter = m_mapSceneObject.find(name);
		if(iter == m_mapSceneObject.end())
		{
			// 返回根对象
			if(name == m_rootObject->getName())
			{
				return m_rootObject;
			}

			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景对象名称为 '" + name + "' 的场景对象不存在", 
				"Scene::getScene");
			return 0;
		}
		return iter->second;
	}
	// 是否存在指定名字的场景对象
	bool Scene::hasSceneObject(const String &name)const
	{
		return m_mapSceneObject.find(name) != m_mapSceneObject.end();
	}
	// 是否存obj这个场景对象
	bool Scene::hasSceneObject(ISceneObject *obj)const
	{
		if(!obj)
			return false;
		return hasSceneObject(obj->getName());
	}

	// 这个指针相对于此场景是否有效
	bool Scene::isSceneObjectValid(ISceneObject *obj)const
	{
		for(MapSceneObject::const_iterator iter = m_mapSceneObject.begin() ; iter != m_mapSceneObject.end() ; iter ++)
		{
			if(iter->second == obj)
				return true;
		}
		return false;
	}

	// 获得Ogre的场景管理器
	SceneManager *Scene::getOgreSceneManager()const
	{
		return m_sceneMgr;
	}

	// 创建一个动画控制器
	IAnimationStateController* Scene::createAnimationStateController(const String &name , const String &animationName)
	{
		if(!m_sceneMgr->hasAnimation(animationName))
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景动画名称为 '" + animationName + "' 的场景动画不存在", 
				"Scene::createAnimationStateController");
			return 0;
		}
		MapAnimationStateController::iterator iter = m_mapAnimationStateController.find(name);
		if(iter != m_mapAnimationStateController.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景动画控制器名称为 '" + name + "' 的场景动画控制器已经存在", 
				"Scene::createAnimationStateController");
			return 0;
		}

		AnimationState *state = m_sceneMgr->createAnimationState(animationName);
		AnimationStateController* ani = new AnimationStateController;
		ani->create(name , state);
		// 添加到自动更新列表
		m_mapAnimationStateController[name] = ani;
		return ani;
	}

	// 创建一个动画控制器
	IAnimationStateController* Scene::createAnimationStateController(const String &animationName)
	{
		return createAnimationStateController(getUniqueName() , animationName);
	}

	// 销毁一个动画控制器
	void Scene::destroyAnimationStateController(const String &name)
	{
		MapAnimationStateController::iterator iter = m_mapAnimationStateController.find(name);
		if(iter == m_mapAnimationStateController.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景动画控制器名称为 '" + name + "' 的场景动画控制器不存在", 
				"Scene::destroyAnimationStateController");
			return;
		}
		m_sceneMgr->destroyAnimationState(iter->second->getAnimationName());
		delete iter->second;
		m_mapAnimationStateController.erase(iter);
	}

	// 销毁一个动画控制器
	void Scene::destroyAnimationStateController(IAnimationStateController *ani)
	{
		destroyAnimationStateController(ani->getName());
	}
	// 获得一个动画控制器
	IAnimationStateController* Scene::getAnimationStateController(const String &name)
	{
		MapAnimationStateController::iterator iter = m_mapAnimationStateController.find(name);
		if(iter == m_mapAnimationStateController.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"场景动画控制器名称为 '" + name + "' 的场景动画控制器不存在", 
				"Scene::getAnimationStateController");
			return 0;
		}
		return iter->second;

	}
	// 是否存在这个动画控制器
	bool Scene::hasAnimationStateController(const String &name)
	{
		return m_mapAnimationStateController.find(name) != m_mapAnimationStateController.end();
	}
	// 是否存在这个动画
	bool Scene::hasAnimation(const String &animationName)
	{
		return m_sceneMgr->hasAnimation(animationName);
	}

	// 帧侦听器的每帧执行
	bool Scene::frameStarted( const FrameEvent& evt )
	{
		for(MapAnimationStateController::iterator iter = m_mapAnimationStateController.begin() ; iter != m_mapAnimationStateController.end() ; iter ++)
		{
			if(iter->second->isPlaying())
			{
				iter->second->update(evt.timeSinceLastFrame);
			}
		}
		return true;
	}
	// 重命名对象
	bool Scene::renameSceneObject(const String &oldName , const String &newName)
	{
		MapSceneObject::iterator iter = m_mapSceneObject.find(oldName);
		if(iter == m_mapSceneObject.end())
			return false;
		if(hasSceneObject(newName))
			return false;
		ISceneObject *obj = iter->second;
		m_mapSceneObject.erase(iter);
		m_mapSceneObject[newName] = obj;
		return true;
	}


	// 射线碰撞检测
	// @ray 射线
	// @collideWorld 是否跟世界碰撞（如：地形）
	// @collideObject 是否跟物件碰撞
	// @info 输出射线碰撞检测信息
	bool Scene::collide(const Ray& ray , bool collideWorld , bool collideObject , RayCheckInfo& info)
	{
		ListRayCheckInfo listInfo;
		bool ret = collide(ray , collideWorld , collideObject , true , true , listInfo);
		if(!ret)
		{
			return false;
		}
		else
		{
			if(listInfo.empty())
			{
				return false;
			}
			else
			{
				info = listInfo.front();
				return true;
			}
		}
	}

	// 射线碰撞检测，仅返回第一个发生碰撞的信息
	// @ray 射线
	// @collideWorld 是否跟世界碰撞（如：地形）
	// @collideObject 是否跟物件碰撞
	// @intersection 输出交点
	bool Scene::collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection)
	{
		RayCheckInfo info;
		if(!collide(ray , collideWorld , collideObject , info))
			return false;
		intersection = ray.getPoint(info.info.distance);
		return true;
	}

	// 射线碰撞检测
	// @ray 射线
	// @collideWorld 是否跟世界碰撞（如：地形）
	// @collideObject 是否跟物件碰撞
	// @infoList 输出射线碰撞检测信息
	// @sortByDistance 输出的infoList按距离排序
	bool Scene::collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , ListRayCheckInfo& info)
	{
		return collide(ray , collideWorld , collideObject , sortByDistance , false , info);
	}

	// 射线碰撞检测
	// @ray 射线
	// @collideWorld 是否跟世界碰撞（如：地形）
	// @collideObject 是否跟物件碰撞
	// @infoList 输出射线碰撞检测信息
	// @sortByDistance 输出的infoList按距离排序
	// @returnFirstOnly 只返回第一个碰撞数据
	bool Scene::collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , bool returnFirstOnly , ListRayCheckInfo& infoList)
	{
		m_rayQuery->setRay(ray);
		m_rayQuery->execute();
		RaySceneQueryResult& result = m_rayQuery->getLastResults();
		if(result.empty())
		{
			return false;
		}
		// 遍历所有结果
		for(RaySceneQueryResult::iterator iter = result.begin() ; iter != result.end() ; iter ++)
		{
			// 如果只返回一个，并且已经有碰撞数据了，就退出了
			if(returnFirstOnly && !infoList.empty())
				return true;
			// 摄像机对象不碰撞
			if(iter->movable->getMovableType() == "Camera")
				continue;
			// 转换成场景对象
			ISceneObject *obj = obj_cast(iter->movable->getUserAny());
			// 如果有场景对象，并且此场景对象参加碰撞计算
			if(obj && obj->getCanCollide())
			{
				// 如果要碰撞世界，就跟地形求交
				if(collideWorld)
				{
					// 如果是地形Tile
					if(iter->movable->getMovableType() == Tile::MOVABLE_OBJECT_TYPE)
					{
						Tile *tile = static_cast<Tile*>(iter->movable);
						RayCheckInfo info;
						// 碰撞检测
						if(tile->collide(ray , info.info.distance))
						{
							info.obj = obj;
							infoList.push_back(info);
						}
						continue;
					}
				}


				// 如果要跟物件求交
				if(collideObject)
				{
					// 地形不参与物件求交
					if(obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
					{
						continue;
					}

					// 如果有精确碰撞体，就用精确碰撞体来计算
					if(obj->getCollisionEntity() && obj->getCollisionEntity()->isInited())
					{
						RayCheckInfo info;
						// 如果发生了碰撞
						if(obj->getCollisionEntity()->collide(ray , info.info))
						{
							info.obj = obj;
							infoList.push_back(info);
						}
					}
					else
					{
						RayCheckInfo info;
						info.obj = obj;
						info.info.distance = iter->distance;
						infoList.push_back(info);
					}
				}
			}
		}
		// 按距离排序
		if(sortByDistance && !infoList.empty())
		{
			std::sort(infoList.begin() , infoList.end());
		}

		return !infoList.empty();
	}
};