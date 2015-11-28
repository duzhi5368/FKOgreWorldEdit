#include "StdAfx.h"
#include "SelectionRectangle.h"
namespace Ogre
{
	SelectionRectangle::SelectionRectangle(const String &name)
		: ManualObject(name)
	{
		setUseIdentityProjection(true);
		setUseIdentityView(true);
		setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
		setQueryFlags(0);
		setVisibilityFlags(0xFF00);
	}

	/**
	* Sets the corners of the SelectionRectangle.  Every parameter should be in the
	* range [0, 1] representing a percentage of the screen the SelectionRectangle
	* should take up.
	*/
	void SelectionRectangle::setCorners(float left, float top, float right, float bottom)
	{
		left = left * 2 - 1;
		right = right * 2 - 1;
		top = 1 - top * 2;
		bottom = 1 - bottom * 2;

		clear();
		begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_STRIP);
		position(left, top, -1);
		position(right, top, -1);
		position(right, bottom, -1);
		position(left, bottom, -1);
		position(left, top, -1);
		end();

		AxisAlignedBox box;
		box.setInfinite();
		setBoundingBox(box);
	}

	void SelectionRectangle::setCorners(const Vector2 &topLeft, const Vector2 &bottomRight)
	{
		setCorners(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	}
};