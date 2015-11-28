#include "stdafx.h"
#include "TerrainBrush.h"
#include "ETBrush.h"
namespace Ogre
{

	TerrainBrush::TerrainBrush()
		: mBrush(0)
		, m_strength(1.0f)
		, m_size(10)
		, m_image(0)
	{
	};

	TerrainBrush::~TerrainBrush()
	{
		clear();
		OGRE_DELETE(m_image);
	};

	void TerrainBrush::clear()
	{
		OGRE_DELETE(mBrush);
	};

	// 从一个灰度图载入笔刷
	bool TerrainBrush::loadBrushFromImage(const Ogre::Image& image)
	{
		if(image.getSize() == 0)
			return false;
		clear();
		mBrush = new Brush(Ogre::loadBrushFromImage(image));


		// 回调到侦听器
		ExecuteListener(TerrainBrushListener , updateBrush);

		return true;
	}

	/** Saves a brush to a grayscale image */
	void TerrainBrush::saveBrushToImage(Ogre::Image& image)
	{
		if(!mBrush)
			return;
		Ogre::saveBrushToImage(*mBrush , image);
	}


	// 根据文件读取笔刷
	bool TerrainBrush::loadBrushFromFile(const String &fileName)
	{
		if(m_brushFileName == fileName && m_image && m_image->getWidth() == m_size && m_image->getHeight() == m_size)
			return true;
		m_brushFileName = fileName;
		OGRE_DELETE(m_image);
		m_image = new Image;
		try
		{
			m_image->load(m_brushFileName , "General");

			// 更改笔刷大小
			m_image->resize(m_size , m_size);
			// 直接读取图片到笔刷
			return loadBrushFromImage(*m_image);
		}
		catch(Exception&e)
		{
			e.getLine();
			return false;
		}
		return false;
	}


	// 根据当前笔刷的图像改变大小
	void TerrainBrush::setSize(int size)
	{
		m_size = size;
		// 回调到侦听器
		ExecuteListener(TerrainBrushListener , updateSize);
		if(!m_brushFileName.empty())
		{
			loadBrushFromFile(m_brushFileName);
		}
	}
	void TerrainBrush::setStrength(const float &fStrength)
	{
		m_strength = fStrength;
		// 回调到侦听器
		ExecuteListener(TerrainBrushListener , updateStrength);
	}
	size_t TerrainBrush::getWidth() const
	{
		return mBrush->getWidth();
	}
	size_t TerrainBrush::getHeight() const
	{
		return mBrush->getHeight();
	}

};