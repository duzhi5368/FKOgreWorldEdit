#pragma once
namespace Ogre
{
	class IScene;
	class ISceneSerializer
	{
	public:

		ISceneSerializer(void)
		{
		}

		virtual ~ISceneSerializer(void)
		{
		}
		// 导入场景
		virtual bool importScene(DataStreamPtr& stream, IScene* scene) = 0;
		// 导出场景
		virtual bool exportScene(IScene* scene, const String& filename) = 0;
	};
};