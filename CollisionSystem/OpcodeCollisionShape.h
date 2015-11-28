#pragma once
#include "ICollisionShape.h"
//forward declarations
namespace Opcode
{
	class Model;
}

namespace Ogre
{
	// 基本的碰撞模型
	class OpcodeCollisionShape : public ICollisionShape
	{
	public:
		OpcodeCollisionShape(void){};
		virtual ~OpcodeCollisionShape(void){};

		// 返回碰撞模型
		virtual Opcode::Model* getCollisionModel(void) const = 0;

	};

};