#pragma once

namespace Ogre
{
	// 渲染屏幕中的选框
	class SelectionRectangle : public ManualObject
	{
	public:
		SelectionRectangle(const String &name);		
		// 设置左上右下的坐标，0~1
		void setCorners(float left, float top, float right, float bottom);
		void setCorners(const Vector2 &topLeft, const Vector2 &bottomRight);
	};
};