#pragma once
namespace Ogre
{
	// 所有物件对象的属性ID
	enum ObjectPropertyID
	{

		ID_NullObject_Begin = 100000,
		ID_General,							// 一般设置
		ID_Name,							// 名称
		ID_Type,							// 类型
		ID_Position,						// 位置
		ID_Rotate,							// 旋转
		ID_Scale,							// 缩放
		ID_Visible,							// 可见
		ID_Parent,							// 父节点
		ID_CanCollide,						// 是否参加碰撞计算
		ID_NullObject_End = 199999,

		ID_EntityObject_Begin = 200000,
		ID_Entity,							// 模型设置
		ID_MeshFile,						// 模型文件
		ID_Alpha,							// 透明度
		ID_CollisionShapeType,				// 碰撞模型类型
		ID_CollisionShapeName,				// 碰撞模型名称
		ID_EntityObject_End = 299999,

		ID_CameraObject_Begin = 300000,
		ID_Camera,							// 摄像机设置
		ID_PolygonMode,						// 几何渲染模式
		ID_LodBias,							// LOD偏移
		ID_NearClipDistance,				// 近裁面
		ID_FarClipDistance,					// 远裁面
		ID_FOVy,							// FOVy角度
		ID_AspectRatio,						// 窗口比率
		ID_ProjectionType,					// 投影方式
		ID_OrthoWindow,						// 正交投影窗口大小
		ID_CameraObject_End = 399999,

		ID_ParticleObject_Begin = 400000,
		ID_Particle,						// 粒子设置
		ID_ParticleTemplate,				// 粒子模板
		ID_ParticleObject_End = 499999,

		ID_TerrainObject_Begin = 500000,
		ID_Terrain,							// 地形设置
		ID_TerrainFile,						// 地形文件
		ID_TerrainObject_End = 599999,

		ID_EnvironmentObject_Begin = 600000,
		ID_Environment,						// 环境设置
		ID_SkyBox,							// 天空盒设置
		ID_SkyBoxEnabled,					// 是否开启天空盒
		ID_SkyBoxMaterialName,				// 天空盒纹理
		ID_SkyBoxRenderDistance,			// 渲染距离
		ID_Fog,								// 雾化设置
		ID_FogMode,							// 雾化方式
		ID_FogColor,						// 雾化颜色
		ID_FogExpDensity,					// 雾化密度
		ID_FogStart,						// 雾化开始距离
		ID_FogEnd,							// 雾化结束距离
		ID_Lighting,						// 光照设置
		ID_AmbientLight,					// 环境光颜色
		ID_EnvironmentObject_End = 699999,


		ID_TriggerObject_Begin = 700000,
		ID_Trigger,							// 触发器设置
		ID_TriggerCondition,				// 满足条件
		ID_TriggerOnEnterEvent,				// 进入事件
		ID_TriggerOnLeaveEvent,				// 退出事件
		ID_TriggerObject_End = 709999,

		ID_BoxTriggerObject_Begin = 750000,
		ID_BoxTrigger,						// 盒子触发器设置
		ID_BoxTriggerAxisAlignedBox,		// 盒子触发器的包围盒
		ID_BoxTriggerObject_End = 759999,

		ID_SphereTriggerObject_Begin = 760000,
		ID_SphereTrigger,					// 球体触发器设置
		ID_SphereRadius,					// 球体半径
		ID_SphereTriggerObject_End = 769999,

		ID_AnimationTrackObject_Begin = 800000,
		ID_AnimationTrack,					// 动画轨迹
		ID_AnimationLoop,					// 是否循环
		ID_AnimationInterpolationMode,		// 插值模式
		ID_AnimationRotationInterpolationMode,		// 旋转插值模式
		ID_AnimationKeyFrames,				// 关键帧列表
		ID_AnimationKeyFrames_Begin = 810000,// 关键帧列表的ID开始
		ID_AnimationKeyFrames_End = 850000,	// 关键帧列表的ID结束
		ID_AnimationTrackObject_End = 899999,


	};
};