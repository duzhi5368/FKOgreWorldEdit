#pragma once
namespace Ogre
{
	class ITriggerWorld;
	// 触发器系统接口
	class TRIGGERSYSTEM_API ITriggerSystem : public Singleton<ITriggerSystem>
	{
	public:
		ITriggerSystem(void){};
		virtual~ITriggerSystem(void){};

		// 单间模式接口
		static ITriggerSystem& getSingleton(void);
		static ITriggerSystem* getSingletonPtr(void);

		// 初始化触发器系统
		static void init();
		// 销毁触发器系统
		static void shutdown();

		// 创建一个触发器世界
		virtual ITriggerWorld *createWorld(const String &name) = 0;
		// 获得一个触发器世界
		virtual ITriggerWorld *getWorld(const String &name) = 0;
		// 是否有这个名称的触发器世界
		virtual bool hasWorld(const String &name) = 0;
		// 销毁一个触发器世界
		virtual void destroyWorld(const String &name) = 0;
		// 销毁一个触发器世界
		virtual void destroyWorld(ITriggerWorld *world) = 0;
	};
};