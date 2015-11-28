#pragma once

#include "TerrainEditorDll.h"
#include "TListener.h"

namespace Ogre
{
	class Brush;

	// 笔刷侦听器
	struct TerrainBrushListener
	{
		virtual void updateBrush(){};
		virtual void updateSize(){};
		virtual void updateStrength(){};
	};

	// 地形笔刷实现
	class TERRAINEDITOR_API TerrainBrush : public TListener<TerrainBrushListener>
	{
	public:
		TerrainBrush();

		virtual ~TerrainBrush();

		void clear();

		// 从一个灰度图载入笔刷
		bool loadBrushFromImage(const Ogre::Image& image);
		// 保存笔刷到灰度图
		void saveBrushToImage(Ogre::Image& image);

		// 根据文件读取笔刷
		bool loadBrushFromFile(const String &fileName);


		// 根据当前笔刷的图像改变大小
		void setSize(int size);

		void setStrength(const float &fStrength);

		size_t getWidth() const;
		size_t getHeight() const;

		const float &getStrength()const{return m_strength;}
		const float &getSize()const{return m_size;}
		const Brush *getBrush()const{return mBrush;};


		Image *getImage()
		{
			return m_image;
		}
protected:

		Brush *mBrush;
		float m_strength;
		float m_size;
		Image *m_image;

		String m_brushFileName;
	};
};