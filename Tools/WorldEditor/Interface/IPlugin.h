// ============================================================
// 文件名	:	IPlugin.h
// 创建人	:	王宏张
// 说明		:	编辑器插件的接口，编辑器插件需要从这里继承
// 创建时间	:	2008-1-20 22:45:05
// ============================================================
#pragma once

namespace Ogre
{
	class IWorldEditor;
	// 编辑器插件的接口，编辑器插件需要从这里继承
	// 插件侦听4视图的RenderTarget事件
	class IPlugin : public RenderTargetListener
	{
	public:

		// 插件描述
		struct Description
		{
			Description()
				: version(0)
			{

			};
			// 插件名称
			String name;
			// 插件文件名
			String fileName;
			// 插件说明
			String desc;
			// 插件发行信息
			String publish;
			// 必须依赖的插件名称列表
			StringVector dependentPlugins;
			// 版本信息
			uint version;
		};

		IPlugin(void)
		{
		}

		virtual ~IPlugin(void)
		{
		}

		// 初始化插件，传进来的worldEditor是世界编辑器的接口
		virtual bool init(IWorldEditor *worldEditor) = 0;
		// 关闭插件
		virtual void shutdown() = 0;
		// 是否已经初始化
		virtual bool isInited()const = 0;

		// 激活插件
		virtual void activate() = 0;
		// 反激活插件
		virtual void deactivate() = 0;
		// 是否已经激活
		virtual bool isActivate()const = 0;

		// 获得插件描述
		virtual const Description &getDescription()const = 0;

		// 查询一些数据，然后返回到result中
		virtual bool query(const String &name , const NameValuePairList *params , void *result) = 0;
	};

};