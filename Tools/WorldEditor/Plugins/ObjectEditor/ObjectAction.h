#pragma once

namespace Ogre
{

	// 物件操作基类
	class ObjectAction
	{
	public:
		ObjectAction(void);
		virtual ~ObjectAction(void);

		// 执行这个操作
		virtual void execute(){};
	};

};