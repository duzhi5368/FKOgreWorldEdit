// ============================================================
// 文件名	:	IMapManager.h
// 创建人	:	王宏张
// 说明		:	地图管理器的接口
// 创建时间	:	2008-2-26 22:31:47
// ============================================================
#pragma once

namespace Ogre
{
	// 地图管理器的接口
	class IMapManager
	{
	public:

		IMapManager(void)
		{
		}

		virtual ~IMapManager(void)
		{
		}
		typedef std::list<ISceneObject*> ListSceneObject;

		// 事件侦听器
		struct Listener
		{
			// 选中一个场景对象
			virtual void onSelectObject(ISceneObject *obj){}
			// 选中一批场景对象
			virtual void onSelectObject(const ListSceneObject &listSceneObject){}
			// 反选一个场景对象
			virtual void onDeselectObject(ISceneObject *obj){}
			// 反选一批场景对象
			virtual void onDeselectObject(const ListSceneObject &listSceneObject){}

			// 新建场景
			virtual void onNewScene(IScene *scene){};
			// 载入一个场景
			virtual void onPreLoadScene(const String &fileName ,IScene *scene){};
			// 载入一个场景
			virtual void onPostLoadScene(const String &fileName ,IScene *scene){};
			// 保存场景
			virtual void onPreSaveScene(const String &fileName ,IScene *scene){};
			// 保存场景
			virtual void onPostSaveScene(const String &fileName ,IScene *scene){};
			// 销毁场景
			virtual void onDestroyScene(IScene *scene){};
		};

		// 添加侦听器
		virtual void addListener(Listener *listener , bool toBack = true) = 0;
		// 删除侦听器
		virtual void removeListener(Listener *listener) = 0;


		// 选中所有场景对象
		virtual void selectAllObject() = 0;
		// 反选所有场景对象
		virtual void deselectAllObject() = 0;


		// 选中一个场景对象
		virtual void selectObject(ISceneObject *obj) = 0;
		// 选中一批场景对象
		virtual void selectObject(const ListSceneObject &listSceneObject) = 0;
		// 反选一个场景对象
		virtual void deselectObject(ISceneObject *obj) = 0;
		// 反选一批场景对象
		virtual void deselectObject(const ListSceneObject &listSceneObject) = 0;

		// 检查这个场景对象是否已被选中
		virtual bool isObjectSelected(ISceneObject *obj)const = 0;
		// 检查这个场景对象是否已被选中
		virtual bool isObjectSelected(const String &name)const = 0;

		// 对象遍历器
		struct ObjectVisitor
		{
			// 访问这个对象，当返回false时不再遍历后面的对象
			virtual bool visit(ISceneObject *obj) = 0;
		};
		// 遍历所有已经选择的物件
		virtual void visitSelectedObject(ObjectVisitor *visitor) = 0;
		// 获得已经选择的物件的数量
		virtual size_t getSelectedObjectCount()const = 0;

		typedef std::map<String , ISceneObject*> MapSelectedObjects;
		typedef MapIterator<MapSelectedObjects> SelectedObjectsIterator;
		typedef ConstMapIterator<MapSelectedObjects> ConstSelectedObjectsIterator;
		// 获得选择物体的枚举
		virtual SelectedObjectsIterator getSelectedObjectsIterator() = 0;
		virtual ConstSelectedObjectsIterator getConstSelectedObjectsIterator()const = 0;

		// 获得所有选中物体的平均中心点
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center , const ISceneObject *exceptObject = 0)const = 0;

		// 新建场景
		virtual void newScene() = 0;
		// 载入一个场景
		virtual bool loadScene(const String &fileName) = 0;
		// 保存场景
		virtual bool saveScene(const String &fileName) = 0;
		// 销毁场景
		virtual void destroyScene() = 0;

		// 获得当前场景
		virtual IScene *getScene() = 0;
	};
};