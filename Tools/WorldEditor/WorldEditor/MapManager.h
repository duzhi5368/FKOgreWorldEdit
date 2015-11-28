// ============================================================
// 文件名	:	IMapManager.h
// 创建人	:	王宏张
// 说明		:	地图管理器的实现
// 创建时间	:	2008-2-26 22:31:47
// ============================================================

#pragma once
#include "IMapManager.h"
#include "TListener.h"
namespace Ogre
{
	// 地图管理器的实现
	class MapManager : public IMapManager
		, public IMapManager::Listener
		, public TListener<IMapManager::Listener>
	{
	public:
		MapManager(void);
		virtual ~MapManager(void);


		// 添加主窗口侦听器
		virtual void addListener(IMapManager::Listener *listener , bool toBack = true)
		{
			TListener<IMapManager::Listener>::addListener(listener , toBack);
		}
		// 删除主窗口侦听器
		virtual void removeListener(IMapManager::Listener *listener)
		{
			TListener<IMapManager::Listener>::removeListener(listener);
		}

		// 选中所有场景对象
		virtual void selectAllObject();
		// 反选所有场景对象
		virtual void deselectAllObject();

		// 选中一个场景对象
		virtual void selectObject(ISceneObject *obj);
		// 选中一批场景对象
		virtual void selectObject(const ListSceneObject &listSceneObject);
		// 反选一个场景对象
		virtual void deselectObject(ISceneObject *obj);
		// 反选一批场景对象
		virtual void deselectObject(const ListSceneObject &listSceneObject);

		// 检查这个场景对象是否已被选中
		virtual bool isObjectSelected(ISceneObject *obj)const;
		// 检查这个场景对象是否已被选中
		virtual bool isObjectSelected(const String &name)const;

		// 遍历所有已经选择的物件
		virtual void visitSelectedObject(ObjectVisitor *visitor);
		// 获得已经选择的物件的数量
		virtual size_t getSelectedObjectCount()const
		{
			return m_selectedObject.size();
		}

		// 新建场景
		virtual void newScene();
		// 载入一个场景
		virtual bool loadScene(const String &fileName);
		// 保存场景
		virtual bool saveScene(const String &fileName);
		// 销毁场景
		virtual void destroyScene();
		// 获得当前场景
		virtual IScene *getScene()
		{
			return m_scene;
		}


		// 选中一个场景对象
		virtual void onSelectObject(Ogre::ISceneObject *obj);
		// 选中一批场景对象
		virtual void onSelectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject);
		// 反选一个场景对象
		virtual void onDeselectObject(Ogre::ISceneObject *obj);
		// 反选一批场景对象
		virtual void onDeselectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject);

		// 获得选择物体的枚举
		virtual SelectedObjectsIterator getSelectedObjectsIterator()
		{
			return SelectedObjectsIterator(m_selectedObject.begin() , m_selectedObject.end());
		}
		// 获得选择物体的枚举
		virtual ConstSelectedObjectsIterator getConstSelectedObjectsIterator()const
		{
			return ConstSelectedObjectsIterator(m_selectedObject.begin() , m_selectedObject.end());
		}

		// 获得所有选中物体的平均中心点
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center , const ISceneObject *exceptObject = 0)const;


	protected:
		// 被选中的对象列表
		MapSelectedObjects m_selectedObject;

		// 当前载入的场景
		IScene *m_scene;

	};
};