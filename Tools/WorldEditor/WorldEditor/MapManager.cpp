#include "StdAfx.h"
#include "MapManager.h"
#include "ISceneObjectVisible.h"

namespace Ogre
{

	MapManager::MapManager(void)
		: m_scene(0)
	{
		addListener(this);
	}

	MapManager::~MapManager(void)
	{
		removeListener(this);
	}

	// 选中一个场景对象
	void MapManager::selectObject(ISceneObject *obj)
	{
		if(m_selectedObject.find(obj->getFullName()) == m_selectedObject.end())
		{
			m_selectedObject[obj->getFullName()] = obj;

			// 回调到侦听器
			ExecuteListener1(IMapManager::Listener , onSelectObject , obj);
		}
	}
	// 选中一批场景对象
	void MapManager::selectObject(const ListSceneObject &listSceneObject)
	{
		ListSceneObject listAdded;
		for(ListSceneObject::const_iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			if(m_selectedObject.find((*iter)->getFullName()) == m_selectedObject.end())
			{
				m_selectedObject[(*iter)->getFullName()] = *iter;
				listAdded.push_back(*iter);
			}
		}

		if(!listAdded.empty())
		{
			if(listAdded.size() == 1)
			{
				// 回调到侦听器
				ExecuteListener1(IMapManager::Listener , onSelectObject , listAdded.back());
			}
			else
			{
				// 回调到侦听器
				ExecuteListener1(IMapManager::Listener , onSelectObject , listAdded);
			}
		}
	}

	// 反选一个场景对象
	void MapManager::deselectObject(ISceneObject *obj)
	{
		MapSelectedObjects::iterator iter = m_selectedObject.find(obj->getFullName());
		if(iter != m_selectedObject.end())
		{
			m_selectedObject.erase(iter);
			// 回调到侦听器
			ExecuteListener1(IMapManager::Listener , onDeselectObject , obj);
		}
	}
	// 反选一批场景对象
	void MapManager::deselectObject(const ListSceneObject &listSceneObject)
	{
		ListSceneObject listRemoved;
		for(ListSceneObject::const_iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			MapSelectedObjects::iterator iterFind = m_selectedObject.find((*iter)->getFullName());
			if(iterFind != m_selectedObject.end())
			{
				m_selectedObject.erase(iterFind);
				listRemoved.push_back(*iter);
			}
		}

		if(!listRemoved.empty())
		{
			if(listRemoved.size() == 1)
			{
				// 回调到侦听器
				ExecuteListener1(IMapManager::Listener , onDeselectObject , listRemoved.back());
			}
			else
			{
				// 回调到侦听器
				ExecuteListener1(IMapManager::Listener , onDeselectObject , listRemoved);
			}
		}
	}


	// 遍历所有已经选择的物件
	void MapManager::visitSelectedObject(ObjectVisitor *visitor)
	{
		MapSelectedObjects selectedObject(m_selectedObject);
		for(MapSelectedObjects::iterator iter = selectedObject.begin() ; iter != selectedObject.end() ; iter ++)
		{
			visitor->visit(iter->second);
		}
	}


	// 选中所有场景对象
	void MapManager::selectAllObject()
	{
		if(!m_scene)
			return;
		IScene::SceneObjectIterator iterator = m_scene->getSceneObjectIterator();
		ListSceneObject listAdded;
		while(iterator.hasMoreElements())
		{
			ISceneObject *obj = iterator.getNext();
			listAdded.push_back(obj);
		}
		selectObject(listAdded);
	}

	// 检查这个场景对象是否已被选中
	bool MapManager::isObjectSelected(ISceneObject *obj)const
	{
		return isObjectSelected(obj->getFullName());
	}

	// 检查这个场景对象是否已被选中
	bool MapManager::isObjectSelected(const String &name)const
	{
		return m_selectedObject.find(name) != m_selectedObject.end();
	}

	// 反选所有场景对象
	void MapManager::deselectAllObject()
	{
		ListSceneObject listRemoved;
		for(MapSelectedObjects::iterator iter = m_selectedObject.begin() ; iter != m_selectedObject.end() ; iter ++)
		{
			listRemoved.push_back(iter->second);
		}
		m_selectedObject.clear();

		if(!listRemoved.empty())
		{
			if(listRemoved.size() == 1)
			{
				// 回调到侦听器
				ExecuteListener1(IMapManager::Listener , onDeselectObject , listRemoved.back());
			}
			else
			{
				// 回调到侦听器
				ExecuteListener1(IMapManager::Listener , onDeselectObject , listRemoved);
			}
		}
	}


	// 新建场景
	void MapManager::newScene()
	{
		destroyScene();
		m_scene = ISceneSystem::getSingleton().createScene();
		// 回调到侦听器
		ExecuteListener1(IMapManager::Listener , onNewScene , m_scene);
	}
	// 载入一个场景
	bool MapManager::loadScene(const String &fileName)
	{
		destroyScene();
		m_scene = ISceneSystem::getSingleton().createScene();
		// 回调到侦听器
		ExecuteListener2(IMapManager::Listener , onPreLoadScene , fileName , m_scene);
		// 载入场景
		ISceneSystem::getSingleton().loadScene(fileName , m_scene);
		// 回调到侦听器
		ExecuteListener2(IMapManager::Listener , onPostLoadScene , fileName , m_scene);

		return m_scene != 0;
	}
	// 保存场景
	bool MapManager::saveScene(const String &fileName)
	{
		// 回调到侦听器
		ExecuteListener2(IMapManager::Listener , onPreSaveScene , fileName , m_scene);

		bool ret = ISceneSystem::getSingleton().saveScene(m_scene , fileName);

		// 回调到侦听器
		ExecuteListener2(IMapManager::Listener , onPostSaveScene , fileName , m_scene);

		return ret;
	}
	// 销毁场景
	void MapManager::destroyScene()
	{
		// 回调到侦听器
		ExecuteListener1(IMapManager::Listener , onDestroyScene , m_scene);

		deselectAllObject();
		if(m_scene)
		{
			ISceneSystem::getSingleton().destroyScene(m_scene);
			m_scene = 0;
		}
	}


	// 选中一个场景对象
	void MapManager::onSelectObject(Ogre::ISceneObject *obj)
	{
		obj->getSceneObjectVisible()->showBorder(true);
	}

	// 反选一个场景对象
	void MapManager::onDeselectObject(Ogre::ISceneObject *obj)
	{
		obj->getSceneObjectVisible()->showBorder(false);
	}

	// 选中一批场景对象
	void MapManager::onSelectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject)
	{
		for(IMapManager::ListSceneObject::const_iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			onSelectObject(*iter);
		}
	}

	// 反选一批场景对象
	void MapManager::onDeselectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject)
	{
		for(IMapManager::ListSceneObject::const_iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			onDeselectObject(*iter);
		}
	}


	// 获得所有选中物体的平均中心点
	bool MapManager::getSelectedObjectsAvgCenter(Vector3 &center , const ISceneObject *exceptObject)const
	{
		if(m_selectedObject.empty())
		{
			return false;
		}
		//get the min/max of x,y,z and choose the center of the bounding box
		Vector3 minVec = Vector3::ZERO;
		Vector3 maxVec = Vector3::ZERO;

		bool first = true;

		for(MapSelectedObjects::const_iterator iter = m_selectedObject.begin() ; iter != m_selectedObject.end() ; iter ++)
		{
			if(iter->second == exceptObject)
				continue;
			const Vector3& vec = iter->second->getWorldPosition();
			if(first)
			{
				minVec = vec;
				maxVec = vec;
				first = false;
			}
			else
			{
				if(vec.x < minVec.x)
					minVec.x = vec.x;

				if(vec.y < minVec.y)
					minVec.y = vec.y;

				if(vec.z < minVec.z)
					minVec.z = vec.z;

				if(vec.x > maxVec.x)
					maxVec.x = vec.x;

				if(vec.y > maxVec.y)
					maxVec.y = vec.y;

				if(vec.z > maxVec.z)
					maxVec.z = vec.z;
			}
		}

		//add and divide by 2
		//Vector3 avg = Vector3(maxVec.x + minVec.x / 2.0f, maxVec.y + minVec.y / 2.0f, maxVec.z + minVec.z / 2.0f);
		center = (maxVec + minVec) / 2.0f;
		return true;
	}
};