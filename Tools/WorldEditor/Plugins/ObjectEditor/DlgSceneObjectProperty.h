#pragma once
#include "DlgObjectProperty.h"

class DlgSceneObjectProperty :
	public DlgObjectProperty
	, public IMapManager::Listener
{
public:
	DlgSceneObjectProperty(CWnd* pParent = NULL);
	virtual ~DlgSceneObjectProperty(void);


	// 选中一个场景对象
	virtual void onSelectObject(ISceneObject *obj);
	// 选中一批场景对象
	virtual void onSelectObject(const IMapManager::ListSceneObject &listSceneObject);
	// 反选一个场景对象
	virtual void onDeselectObject(ISceneObject *obj);
	// 反选一批场景对象
	virtual void onDeselectObject(const IMapManager::ListSceneObject &listSceneObject);

};
