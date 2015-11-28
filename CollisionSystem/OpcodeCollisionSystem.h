#pragma once
#include "ICollisionSystem.h"

namespace Ogre
{
	// 碰撞系统实现
	class OpcodeCollisionSystem : public ICollisionSystem
	{
	public:
		OpcodeCollisionSystem(void);
		virtual ~OpcodeCollisionSystem(void);


		// 从一个Mesh创建碰撞模型
		virtual ICollisionShapePtr createMeshCollisionShape(const String& meshName, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);
		// 从一个Mesh创建碰撞模型
		virtual ICollisionShapePtr createMeshCollisionShape(const MeshPtr &mesh, const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);
		// 从一个SubMesh创建碰撞模型
		virtual ICollisionShapePtr createSubMeshCollisionShape(const MeshPtr &mesh , const String &subMeshName , const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);
		// 从一个SubMesh创建碰撞模型
		virtual ICollisionShapePtr createSubMeshCollisionShape(const String& meshName, const String &subMeshName , const Vector3& position = Vector3::ZERO, const Quaternion& orient = Quaternion::IDENTITY, const Vector3& scale = Vector3::UNIT_SCALE);
		// 从一个顶点和索引缓冲区创建碰撞模型
		virtual ICollisionShapePtr createVertexCollisionShape(const String &name , const Vector3 *vertexBuffer , size_t vertexCount , bool autoDeleteVertex , const uint16 *indexBuffer , size_t indexCount , bool autoDeleteIndex);

		// 销毁碰撞模型
		virtual void destroyCollisionShape(ICollisionShapePtr& shape);

		// 创建一个碰撞世界
		virtual ICollisionWorld *createWorld(const String &name);
		// 获得一个碰撞世界
		virtual ICollisionWorld *getWorld(const String &name);
		// 是否有这个名称的碰撞世界
		virtual bool hasWorld(const String &name);
		// 销毁一个碰撞世界
		virtual void destroyWorld(const String &name);
		// 销毁一个碰撞世界
		virtual void destroyWorld(ICollisionWorld *world);

	protected:

		// 根据名称获得碰撞模型
		ICollisionShapePtr getCollisionShape(const String &type , const String& name);
		// 添加一个碰撞模型到碰撞系统中
		void addCollisionShape(ICollisionShapePtr& shape);

		typedef std::map<String, ICollisionShapePtr> CollisionShapeMap;
		typedef std::map<String, CollisionShapeMap> CollisionShapeMapMap;
		CollisionShapeMapMap m_collisionShapeMap;


		typedef std::map<String , ICollisionWorld*> CollisionWorldMap;
		CollisionWorldMap m_worldMap;

	};
};