#pragma once
#include "ICollisionShape.h"
namespace Ogre
{
	class ICollisionWorld;
	// 碰撞系统接口
	class COLLISIONSYSTEM_API ICollisionSystem : public Singleton<ICollisionSystem>
	{
	public:
		ICollisionSystem(void){};
		virtual~ICollisionSystem(void){};

		// 单间模式接口
		static ICollisionSystem& getSingleton(void);
		static ICollisionSystem* getSingletonPtr(void);

		// 初始化碰撞系统
		static void init(SceneManager *sceneMgr);
		// 销毁碰撞系统
		static void shutdown();

		// 从一个Mesh创建碰撞模型
		virtual ICollisionShapePtr createMeshCollisionShape(const String& meshName, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE) = 0;
		// 从一个Mesh创建碰撞模型
		virtual ICollisionShapePtr createMeshCollisionShape(const MeshPtr &mesh, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE) = 0;
		// 从一个SubMesh创建碰撞模型
		virtual ICollisionShapePtr createSubMeshCollisionShape(const MeshPtr &mesh , const String &subMeshName , const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE) = 0;
		// 从一个SubMesh创建碰撞模型
		virtual ICollisionShapePtr createSubMeshCollisionShape(const String& meshName, const String &subMeshName , const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE) = 0;
		// 从一个顶点和索引缓冲区创建碰撞模型
		virtual ICollisionShapePtr createVertexCollisionShape(const String &name , const Vector3 *vertexBuffer , size_t vertexCount , bool autoDeleteVertex , const uint16 *indexBuffer , size_t indexCount , bool autoDeleteIndex) = 0;

		// 销毁碰撞模型
		virtual void destroyCollisionShape(ICollisionShapePtr &shape) = 0;

		// 创建一个碰撞世界
		virtual ICollisionWorld *createWorld(const String &name) = 0;
		// 获得一个碰撞世界
		virtual ICollisionWorld *getWorld(const String &name) = 0;
		// 是否有这个名称的碰撞世界
		virtual bool hasWorld(const String &name) = 0;
		// 销毁一个碰撞世界
		virtual void destroyWorld(const String &name) = 0;
		// 销毁一个碰撞世界
		virtual void destroyWorld(ICollisionWorld *world) = 0;
	};
};