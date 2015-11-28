#include "StdAfx.h"
#include "OpcodeCollisionSystem.h"
#include "OpcodeCollisionMeshShape.h"
#include "OpcodeCollisionWorld.h"
#include "OpcodeCollisionSubMeshShape.h"
#include "OpcodeCollisionVertexShape.h"
namespace Ogre
{
	OpcodeCollisionSystem::OpcodeCollisionSystem(void)
	{
	}

	OpcodeCollisionSystem::~OpcodeCollisionSystem(void)
	{
		m_collisionShapeMap.clear();
		for(CollisionWorldMap::iterator iter = m_worldMap.begin() ; iter != m_worldMap.end() ; iter ++)
		{
			delete iter->second;
		}
		m_worldMap.clear();
	}

	// 添加一个碰撞模型到碰撞系统中
	ICollisionShapePtr OpcodeCollisionSystem::getCollisionShape(const String &type , const String& name)
	{
		CollisionShapeMapMap::iterator itr = m_collisionShapeMap.find(type);
		if(itr != m_collisionShapeMap.end())
		{
			CollisionShapeMap::iterator iter = itr->second.find(name);
			if(iter != itr->second.end())
			{
				return iter->second;
			}
			else
			{
				return ICollisionShapePtr();
			}
		}
		else
		{
			return ICollisionShapePtr();
		}
	}

	// 根据名称获得碰撞模型
	void OpcodeCollisionSystem::addCollisionShape(ICollisionShapePtr& shape)
	{
		m_collisionShapeMap[shape->getType()][shape->getName()] = shape;
	}


	// 从一个Mesh创建碰撞模型
	ICollisionShapePtr OpcodeCollisionSystem::createMeshCollisionShape(const String& meshName , const Vector3& position, const Quaternion& orient, const Vector3 &scale)
	{
		//check if a shape with this name already exists
		ICollisionShapePtr shape = getCollisionShape(OpcodeCollisionMeshShape::SHAPE_TYPE , meshName);

		if(shape.isNull())
		{
			shape.bind(new OpcodeCollisionMeshShape());
			if(!static_cast<OpcodeCollisionMeshShape*>(shape.getPointer())->init(meshName , position , orient , scale))
			{
				shape.setNull();
				return ICollisionShapePtr();
			}
			addCollisionShape(shape);
		}

		return shape;
	}


	// 从一个Mesh创建碰撞模型
	ICollisionShapePtr OpcodeCollisionSystem::createMeshCollisionShape(const MeshPtr &mesh , const Vector3& position, const Quaternion& orient, const Vector3 &scale)
	{
		return createMeshCollisionShape(mesh->getName() , position , orient , scale);
	}


	// 从一个Mesh创建碰撞模型
	ICollisionShapePtr OpcodeCollisionSystem::createSubMeshCollisionShape(const MeshPtr &mesh , const String &subMeshName , const Vector3& position, const Quaternion& orient, const Vector3 &scale)
	{
		return createSubMeshCollisionShape(mesh->getName() , subMeshName , position , orient , scale);
	}

	// 从一个Mesh创建碰撞模型
	ICollisionShapePtr OpcodeCollisionSystem::createSubMeshCollisionShape(const String &meshName , const String &subMeshName , const Vector3& position, const Quaternion& orient, const Vector3 &scale)
	{
		//check if a shape with this name already exists
		ICollisionShapePtr shape = getCollisionShape(OpcodeCollisionSubMeshShape::SHAPE_TYPE , meshName + "_" + subMeshName);

		if(shape.isNull())
		{
			shape.bind(new OpcodeCollisionSubMeshShape());
			if(!static_cast<OpcodeCollisionSubMeshShape*>(shape.getPointer())->init(meshName , subMeshName , position , orient , scale))
			{
				shape.setNull();
				return ICollisionShapePtr();
			}
			addCollisionShape(shape);
		}

		return shape;
	}

	// 从一个顶点和索引缓冲区创建碰撞模型
	ICollisionShapePtr OpcodeCollisionSystem::createVertexCollisionShape(const String &name , const Vector3 *vertexBuffer , size_t vertexCount , bool autoDeleteVertex , const uint16 *indexBuffer , size_t indexCount , bool autoDeleteIndex)
	{
		//check if a shape with this name already exists
		ICollisionShapePtr shape = getCollisionShape(OpcodeCollisionVertexShape::SHAPE_TYPE , name);

		if(shape.isNull())
		{
			shape.bind(new OpcodeCollisionVertexShape());
			if(!static_cast<OpcodeCollisionVertexShape*>(shape.getPointer())->init(name , vertexBuffer , vertexCount , autoDeleteVertex , indexBuffer , indexCount , autoDeleteVertex))
			{
				shape.setNull();
				return ICollisionShapePtr();
			}
			addCollisionShape(shape);
		}

		return shape;
	}

	// 销毁碰撞模型
	void OpcodeCollisionSystem::destroyCollisionShape(ICollisionShapePtr &shape)
	{
		CollisionShapeMapMap::iterator itr = m_collisionShapeMap.find(shape->getType());
		if(itr != m_collisionShapeMap.end())
		{
			CollisionShapeMap::iterator iter = itr->second.find(shape->getName());
			if(iter != itr->second.end())
			{
				shape.setNull();
				// 最后一个，就删除之
				if(iter->second.unique())
				{
					itr->second.erase(iter);
				}
			}
		}
	}



	// 创建一个碰撞世界
	ICollisionWorld *OpcodeCollisionSystem::createWorld(const String &name)
	{
		if(hasWorld(name))
		{
			return 0;
		}
		ICollisionWorld *world = new OpcodeCollisionWorld(name);
		m_worldMap[name] = world;
		return world;
	}

	// 获得一个碰撞世界
	ICollisionWorld *OpcodeCollisionSystem::getWorld(const String &name)
	{
		CollisionWorldMap::iterator iter = m_worldMap.find(name);
		if(iter != m_worldMap.end())
		{
			return iter->second;
		}
		else
		{
			return 0;
		}
	}

	// 是否有这个名称的碰撞世界
	bool OpcodeCollisionSystem::hasWorld(const String &name)
	{
		return m_worldMap.find(name) != m_worldMap.end();
	}

	// 销毁一个碰撞世界
	void OpcodeCollisionSystem::destroyWorld(const String &name)
	{
		CollisionWorldMap::iterator iter = m_worldMap.find(name);
		if(iter != m_worldMap.end())
		{
			delete iter->second;
			m_worldMap.erase(iter);
		}
	}

	// 销毁一个碰撞世界
	void OpcodeCollisionSystem::destroyWorld(ICollisionWorld *world)
	{
		destroyWorld(world->getName());
	}

};