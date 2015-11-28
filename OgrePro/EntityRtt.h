/******************************************************************
** 文件名:	EntityRtt.h
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-13 11:53:28
** 描  述:	模型实体RTT
******************************************************************/

#pragma once
#include "EntityAnimationController.h"
namespace Ogre
{
	class EntityRTT;
	// RTT的渲染目标侦听器，开始更新的时候才显示需要渲染的东西，渲染完毕后全部隐藏掉
	class OGREPRO_API EntityRTTListener : public RenderTargetListener
	{
	public:
		EntityRTTListener(EntityRTT *rtt)
			: m_rtt(rtt)
		{

		}
		virtual void preRenderTargetUpdate(const RenderTargetEvent& evt);
		virtual void postRenderTargetUpdate(const RenderTargetEvent& evt);
	protected:
		EntityRTT *m_rtt;
	};

	// 模型实体RTT
	class OGREPRO_API EntityRTT : public FrameListener
	{
	public :
		friend class EntityRTTListener;
		//构造和析构
		EntityRTT(const String &name);
		virtual ~EntityRTT();

		// 初始化RTT
		virtual void init(SceneManager *sceneMgr
			, ushort textureWidth = 512
			, ushort textureHeight = 512
			, PixelFormat textureFormat = PF_R8G8B8A8
			, const ColourValue& bgColor = ColourValue::ZERO
			, float nearclipDist = 0.1f
			, float farclipDist = 100.0f
			, const Radian & fovy = Degree(60)
			, float ratio = 1.333f
			);
		// 销毁RTT
		virtual void destroy();

		// 设置要渲染的实体
		virtual void setEntity(Entity *entity , const String &defaultAnimationName = StringUtil::BLANK);
		// 获得实体
		virtual Entity *getEntity()const
		{
			return m_entity;
		}
		// 获得克隆后的实体
		virtual Entity *getCloneEntity()const
		{
			return m_cloneEntity;
		}

		// 开启RTT渲染
		virtual void setEnabled(bool enable);

		// 是否已经开启RTT渲染
		virtual bool isEnabled()const;

		// 获得RTT纹理
		virtual TexturePtr &getRTT();

		// 获得此RTT的名称
		virtual const String &getName()const
		{
			return m_name;
		};

		// 增量旋转
		void rotateEntity(const Quaternion &rotate);
		// 设置旋转量
		void setEntityRotate(const Quaternion &rotate);
		// 获得旋转量
		const Quaternion &getEntityRotate()const;

		// 设置RTT纹理渲染比例
		void setRatio(float ratio);

		// 设置窗口大小，会根据这个大小自动设置纹理大小和摄像机比率
		void setWindowSize(ushort width , ushort height);
		// 设置RTT纹理大小
		void setTextureSize(ushort width , ushort height);
		// 每帧更新动画
		virtual bool frameStarted(const FrameEvent& evt);

		// 获得动画控制器接口
		IEntityAnimationController *getEntityAnimationController()
		{
			return m_animationController;
		}
	protected:
		// 创建RTT纹理和视口
		void createViewport();
		// 延迟初始化
		virtual void delayInit();
		// 调整摄像机到实体面前
		virtual void adjustCamera();
		// 销毁视口
		virtual void destroyViewport();
		// 销毁实体
		virtual void destroyEntity();
		// 获得一个不重复的实体名字
		virtual String getUniqueEntityName(const String &preName = StringUtil::BLANK);

		// 此RTT的名字
		String m_name;
		// 是否已经初始化
		bool m_inited;
		// 是否已经创建RTT纹理
		bool m_createdTexture;
		// 是否已经setEntity过了
		bool m_setedEntity;
		// 是否允许渲染
		bool m_enabled;

		// RTT使用的场景管理器
		SceneManager *m_sceneMgr;
		// RTT纹理宽度
		ushort m_textureWidth;
		// RTT纹理高度
		ushort m_textureHeight;
		// RTT纹理格式
		PixelFormat m_textureFormat;
		// RTT的视口背景颜色
		ColourValue m_bgColor;
		// RTT摄像机近裁面
		float m_nearclipDist;
		// RTT摄像机远裁面
		float m_farclipDist;
		// RTT摄像机FOV角度
		Radian m_fovy;
		// RTT摄像机宽高比
		float m_ratio;

		// 需要渲染的Rtt实体的克隆
		Entity *m_cloneEntity;
		// 需要渲染的Rtt实体
		Entity *m_entity;
		// 实体旋转
		Quaternion m_rotate;
		// 动画控制器
		IEntityAnimationController *m_animationController;

		// RTT纹理
		TexturePtr m_rtt;
		// 小场景的视口
		Viewport* m_viewport;
		// 小场景的摄像机
		Camera* m_camera;
		// 渲染目标
		RenderTarget *m_target;
		// RTT所在的node
		SceneNode *m_node;
		// Entity所在的node，挂在m_node上
		SceneNode *m_nodeEntity;
		// RTT目标的侦听器
		RenderTargetListener *m_listener;
	};
};