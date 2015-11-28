
#pragma once

#include "EntityRTT.h"
namespace Ogre
{

	class AxisRTT : public EntityRTT
	{
	public:
		AxisRTT();
		virtual ~AxisRTT(void);

		virtual void init(IWorldEditor *worldEditor);
		virtual void destroy();

		// 视口更新
		void preViewportUpdate(const RenderTargetViewportEvent& evt);

		const String &getAxisMaterialName()
		{
			return m_axisMat->getName();
		}
	protected:
		// 创建坐标轴标识
		ManualObject* createAxis();
		// 创建坐标轴界面
		void createAxisText();
		// 调整摄像机到实体面前
		virtual void adjustCamera();

		// 坐标轴对象
		ManualObject *m_axisObject;
		BillboardSet *m_axisText;
		MaterialPtr m_axisMat;

		IWorldEditor *m_worldEditor;

	};

};