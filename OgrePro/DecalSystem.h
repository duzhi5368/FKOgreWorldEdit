/******************************************************************
** 文件名:	DecalSystem.h
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:30
** 描  述:
******************************************************************/
#pragma once
#include "IDecalSystem.h"

namespace Ogre
{
	class TerrainSceneManager;
	class MeshDecal;

	class DecalSystem : public IDecalSystem
		, public FrameListener
	{
	public:
		DecalSystem(SceneManager *mgr);
		virtual ~DecalSystem();


		// 使用这个场景管理器，调用这个函数之后，createDecal将使用这个场景管理器
		virtual void acceptSceneMgr(SceneManager *mgr);
		// 获得场景管理器
		virtual SceneManager* getSceneManager()const
		{
			return m_sceneMgr;
		}

		// 根据参数和实例名创建一个贴花，如果实例名为空就自动生成
		virtual IDecal *createDecal(DecalType decalType
			, const String &name = StringUtil::BLANK
			, const NameValuePairList *params = 0
			);

		// 销毁一个贴花
		virtual void destroyDecal(IDecal *decal);
		virtual void destroyDecal(const String &name);

		// 销毁所有贴花
		virtual void destroyAllDecal();

		// 根据实例名获得一个贴花
		virtual IDecal *getDecal(const String &name);
		// 检查是否有这个实例名的贴花
		virtual bool hasDecal(const String &name)const;

		// 获得所有贴花枚举
		virtual DecalIterator getDecalIterator();

		// 获得所有贴花枚举
		virtual ConstDecalIterator getDecalIterator()const;


		// 对世界模型做射线检测（只针对地形）
		ETTerrain* worldRayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd);
		ETTerrain* worldRayQuery(Vector3 &posIntersect , const Ray &ray);

		// 对所有物体进行射线检测（包括地形、地上物体、玩家、怪物等等）
		bool rayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd);

		// 鼠标点击测试，返回世界交点坐标（地形）
		ETTerrain* hitTestWorld(Vector3 &posIntersect , short cursorX, short cursorY , Camera *camera);

		// 每帧更新所有贴花
		virtual bool frameStarted(const FrameEvent& evt);
	protected:
		// 获得唯一的贴花名字
		String getUniqueDecalName(const String &preName = StringUtil::BLANK)
		{
			static uint s_index = 0;
			return preName + "_AutoDecal_" + StringConverter::toString(++s_index);
		}
		// 所有贴花列表
		MapDecal m_mapDecal;
		// 室外地形场景管理器
		SceneManager *m_sceneMgr;

		// 射线查询
		RaySceneQuery* m_rayQuery;

	};
};