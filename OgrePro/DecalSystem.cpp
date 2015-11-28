/******************************************************************
** 文件名:	DecalSystem.cpp
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:29
** 描  述:
******************************************************************/
#include "stdafx.h"
#include "DecalSystem.h"
#include "MeshDecal.h"
#include "NormalDecal.h"
#include "GridDecal.h"
#include "ETTerrain.h"

namespace Ogre
{
	DecalSystem::DecalSystem(SceneManager *mgr)
		: m_sceneMgr(0)
		, m_rayQuery(0)

	{
		acceptSceneMgr(mgr);
		Root::getSingleton().addFrameListener(this);
	}

	DecalSystem::~DecalSystem()
	{
		destroyAllDecal();
		if(m_rayQuery)
		{
			m_sceneMgr->destroyQuery(m_rayQuery);
			m_rayQuery = 0;
		}
		Root::getSingleton().removeFrameListener(this);
	}


	// 使用这个场景管理器，调用这个函数之后，createDecal将使用这个场景管理器
	void DecalSystem::acceptSceneMgr(SceneManager *mgr)
	{
		m_sceneMgr = mgr;
		if(m_rayQuery)
		{
			m_sceneMgr->destroyQuery(m_rayQuery);
			m_rayQuery = 0;
		}
		m_rayQuery = m_sceneMgr->createRayQuery(Ray());
		m_rayQuery->setSortByDistance(true);
	}


	// 根据参数和实例名创建一个贴花，如果实例名为空就自动生成
	IDecal *DecalSystem::createDecal(DecalType decalType
		, const String &name
		, const NameValuePairList *params
		)
	{
		String nameCreate(name);
		if(nameCreate.empty())
		{
			nameCreate = getUniqueDecalName();
		}
		if(hasDecal(nameCreate))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"指定的贴花名字在贴花系统中已经存在：" + nameCreate,
				"DecalSystem::createDecal");
			return 0;
		}
		IDecal *decal = 0;
		switch(decalType)
		{
			// 法线贴花
		case DT_NORMAL:
			decal = new NormalDecal(m_sceneMgr , nameCreate);
			break;
			// 网格贴花
		case DT_MESH:
			decal = new MeshDecal(m_sceneMgr , nameCreate);
			break;
		case DT_PROJECTIVE:
		case DT_GRID:
			decal = new GridDecal(m_sceneMgr , nameCreate);
			if(params)
			{
				NameValuePairList::const_iterator iter;

				// 初始化属性
				iter = params->find("WidthGrid");
				if(iter != params->end())
				{
					static_cast<GridDecal*>(decal)->setWidthGridCount(StringConverter::parseLong(iter->second));
				}

				iter = params->find("WidthGrid");
				if(iter != params->end())
				{
					static_cast<GridDecal*>(decal)->setHeightGridCount(StringConverter::parseLong(iter->second));
				}
			}
			break;
		default:
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"不支持的DecalType，相关decalType为：" + StringConverter::toString(decalType),
				"DecalSystem::createDecal");
			return 0;
			break;
		}
		m_mapDecal[nameCreate] = decal;
		return decal;
	}

	// 销毁一个贴花
	void DecalSystem::destroyDecal(IDecal *decal)
	{
		destroyDecal(decal->getName());
	}

	void DecalSystem::destroyDecal(const String &name)
	{
		MapDecal::iterator iter = m_mapDecal.find(name);
		if(iter == m_mapDecal.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"指定的贴花名字在贴花系统中不存在：" + name,
				"DecalSystem::destroyDecal");
			return;
		}
		delete iter->second;
		m_mapDecal.erase(iter);
	}

	// 销毁所有贴花
	void DecalSystem::destroyAllDecal()
	{
		for(MapDecal::iterator iter = m_mapDecal.begin() ; iter != m_mapDecal.end() ; iter ++)
		{
			delete iter->second;
		}
		m_mapDecal.clear();
	}

	// 根据实例名获得一个贴花
	IDecal *DecalSystem::getDecal(const String &name)
	{
		MapDecal::iterator iter = m_mapDecal.find(name);
		if(iter == m_mapDecal.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"指定的贴花名字在贴花系统中不存在：" + name,
				"DecalSystem::getDecal");
			return 0;
		}
		return iter->second;
	}
	// 检查是否有这个实例名的贴花
	bool DecalSystem::hasDecal(const String &name)const
	{
		return m_mapDecal.find(name) != m_mapDecal.end();
	}

	// 获得所有贴花枚举
	DecalSystem::DecalIterator DecalSystem::getDecalIterator()
	{
		return DecalIterator(m_mapDecal.begin() , m_mapDecal.end());
	}

	// 获得所有贴花枚举
	DecalSystem::ConstDecalIterator DecalSystem::getDecalIterator()const
	{
		return ConstDecalIterator(m_mapDecal.begin() , m_mapDecal.end());
	}

	// 每帧更新所有贴花
	bool DecalSystem::frameStarted(const FrameEvent& evt)
	{
		for(MapDecal::iterator iter = m_mapDecal.begin() ; iter != m_mapDecal.end() ; iter ++)
		{
			iter->second->update(evt.timeSinceLastFrame);
		}
		return true;
	}


	// 射线检测
	ETTerrain* DecalSystem::worldRayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd)
	{
		Ray ray(posStart , (posEnd - posStart).normalisedCopy());
		return worldRayQuery(posIntersect , ray);
	}

	// 射线检测
	ETTerrain* DecalSystem::worldRayQuery(Vector3 &posIntersect , const Ray &ray)
	{
		typedef SharedPtr<RaySceneQuery> RaySceneQueryPtr;
		RaySceneQueryPtr query(m_sceneMgr->createRayQuery(ray));
		query->setSortByDistance(true);
		query->execute();
		RaySceneQueryResult& result = query->getLastResults();
		if(result.empty())
		{
			return 0;
		}
		for(RaySceneQueryResult::iterator iter = result.begin() ; iter != result.end() ; iter ++)
		{
			// 如果是地形
			if(iter->movable->getMovableType() == ETTerrain::MOVABLE_TYPE)
			{
				// 转换成地形对象
				ETTerrain *terrain = static_cast<TerrainImpl*>(iter->movable)->getTerrain();
				if(!terrain)
				{
					return 0;
				}
				std::pair<bool, Vector3> res = terrain->rayIntersects(ray);
				if(!res.first)
					return 0;
				posIntersect = res.second;
				return terrain;
			}
		}
		return 0;
	}
	// 射线检测
	bool DecalSystem::rayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd)
	{
		Ray ray(posStart , (posEnd - posStart).normalisedCopy());
		m_rayQuery->setRay(ray);
		RaySceneQueryResult& result = m_rayQuery->execute();
		for(RaySceneQueryResult::iterator iter = result.begin() ; iter != result.end() ; iter ++)
		{
			// 使用世界点来算平面法线
			if(iter->worldFragment)
			{
				posIntersect = iter->worldFragment->singleIntersection;
				return true;
			}
			// 使用模型点来算平面法线
			else
			{
				posIntersect = ray.getPoint(iter->distance);
				return true;
			}
		}
		return false;
	}

	// 鼠标点击测试，返回世界交点坐标（地形）
	ETTerrain* DecalSystem::hitTestWorld(Vector3 &posIntersect , short cursorX, short cursorY , Camera *camera)
	{
		// 得到鼠标处的一条射线
		float fx,fy;
		RenderTarget* target = camera->getViewport()->getTarget();
		fx = (float)cursorX / (target->getWidth());
		fy = (float)cursorY / (target->getHeight());
		Ray hitRay = camera->getCameraToViewportRay(fx, fy); 

		return worldRayQuery(posIntersect , hitRay);
	}

};