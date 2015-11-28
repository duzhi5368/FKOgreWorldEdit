#include "StdAfx.h"
#include "TerrainActionImpl.h"


namespace Ogre
{

	SingleSelectionAction::SingleSelectionAction(Camera *pCamera , SelectionRenderer *pSelectionRenderer , ETTerrain* pTerrain , InputListener *pListener , Selection *pSelection , TerrainBrush *pBrush ,int paintChannel)
	: TerrainAction(pCamera , pSelectionRenderer , pTerrain , pListener , pSelection , pBrush, paintChannel)
	{
		// add by 李锦俊 2007-7-27
		// 开始就清除掉所有数据
		mSelection->clear();
	}

	SingleSelectionAction::~SingleSelectionAction(void)
	{
	}

	void SingleSelectionAction::update(float deltaTime)
	{
		InputListener::update(deltaTime);
	}


	bool SingleSelectionAction::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
	{
		return InputListener::mousePressed(arg , id , consumed);
	}


	bool SingleSelectionAction::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
	{
		return InputListener::mouseReleased(arg , id , consumed);
	}


	bool SingleSelectionAction::mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
	{
		if(!isLocked())
		{
			float screenX = (float)arg.state.X.abs / (float)arg.state.width;
			float screenY = (float)arg.state.Y.abs / (float)arg.state.height;

			// add by 李锦俊 2007-7-12
			// 根据鼠标位置找到地形坐标
			std::pair<bool, Ogre::Vector3> pos = mTerrain->rayIntersects(mCamera , screenX , screenY);
			if(pos.first)
			{
				update();
				// add by 李锦俊 2007-7-23
				// 获得网格索引
				Vector2 grid = mTerrain->getTerrainGrid(pos.second);
				// add by 李锦俊 2007-7-27
				// 更新位置
				int iX = (int)grid.x - (int)mBrush->getWidth() / 2;
				int iY = (int)grid.y - (int)mBrush->getHeight() / 2;
				GridVector2 posNew(iX < 0 ? grid.x : iX , iY < 0 ? grid.y : iY);

				mSelection->setPosition(posNew);

			}
			else
			{
				mSelection->clear();
			}
		}
		return InputListener::mouseMoved(arg , consumed);
	}

	
	void SingleSelectionAction::update()
	{
		// add by 李锦俊 2007-7-27
		// 只有空的时候才添加
		if(mSelection->empty())
		{
			mSelection->addSel((ushort)mBrush->getWidth() / 2 , (ushort)mBrush->getHeight() / 2 , *mBrush->getBrush() , mBrush->getStrength());
		}
	}



	MultiSelectionAction::MultiSelectionAction(Camera *pCamera , SelectionRenderer *pSelectionRenderer , ETTerrain* pTerrain , InputListener *pListener , Selection *pSelection , TerrainBrush *pBrush ,int paintChannel)
	: TerrainAction(pCamera , pSelectionRenderer , pTerrain , pListener , pSelection , pBrush, paintChannel)
	{
		mSelection->clear();
		mSelection->setPosition(GridVector2(0 , 0));
	}

	MultiSelectionAction::~MultiSelectionAction(void)
	{
	}
			

	void MultiSelectionAction::update(float deltaTime)
	{
		InputListener::update(deltaTime);
	}


	bool MultiSelectionAction::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
	{
		return InputListener::mousePressed(arg , id , consumed);
	}


	bool MultiSelectionAction::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
	{
		float screenX = (float)arg.state.X.abs / (float)arg.state.width;
		float screenY = (float)arg.state.Y.abs / (float)arg.state.height;

		// add by 李锦俊 2007-7-12
		// 根据鼠标位置找到地形坐标
		std::pair<bool, Ogre::Vector3> pos = mTerrain->rayIntersects(mCamera , screenX , screenY);
		if(pos.first)
		{
			// add by 李锦俊 2007-7-23
			// 获得网格索引
			Vector2 grid = mTerrain->getTerrainGrid(pos.second);
			// add by 李锦俊 2007-7-23
			// 左键就添加
			if(id == OIS::MB_Left)
			{
				mSelection->addSel(grid.x , grid.y , *mBrush->getBrush() , mBrush->getStrength());
			}
			// add by 李锦俊 2007-7-23
			// 右键就减去
			else if(id == OIS::MB_Right)
			{
				mSelection->addSel(grid.x , grid.y , *mBrush->getBrush() , -mBrush->getStrength());
			}
		}

		return InputListener::mouseReleased(arg , id , consumed);
	}


	bool MultiSelectionAction::mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
	{
		return InputListener::mouseMoved(arg , consumed);
	}

static const int VERTEX_BINDING = 0;
static const int TestDistanceAction_VBCount = 4096;

	TestDistanceAction::TestDistanceAction(Camera *pCamera , ETTerrain* pTerrain , InputListener *pListener)
		: TerrainAction(pCamera , 0 , pTerrain , pListener , 0 , 0, 0)
	{

		mSceneMgr->getRootSceneNode()->attachObject(this);
	
		mRenderOp.vertexData = new VertexData();
		mRenderOp.indexData = 0;

		this->setMaterial("BaseWhiteNoLighting");

		// Initialization stuff
		mRenderOp.vertexData->vertexCount = TestDistanceAction_VBCount;
		mRenderOp.vertexData->vertexStart = 0;
		mRenderOp.operationType = RenderOperation::OT_LINE_STRIP; // OT_LINE_LIST, OT_LINE_STRIP
		mRenderOp.useIndexes = false;

		VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
		VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

		size_t offset = 0;
		offset += decl->addElement (VERTEX_BINDING, offset, VET_FLOAT3, VES_POSITION).getSize();
		offset += decl->addElement (VERTEX_BINDING, offset, VET_COLOUR, VES_DIFFUSE).getSize();

		HardwareVertexBufferSharedPtr vbuf =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(VERTEX_BINDING),
			mRenderOp.vertexData->vertexCount,
			//HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);

		bind->setBinding(VERTEX_BINDING, vbuf);

		setRenderQueueGroup(RENDER_QUEUE_9);

		mRenderOp.vertexData->vertexCount = 0;

		pushPoint(Vector3::ZERO);

	}
	TestDistanceAction::~TestDistanceAction(void)
	{
		clearAllPoints();
		mSceneMgr->getRootSceneNode()->detachObject(this);
		delete mRenderOp.vertexData;
	};

	void TestDistanceAction::clearAllPoints()
	{
		for(PointList::iterator iter = mPointList.begin() ; iter != mPointList.end() ; iter ++)
		{
			delete *iter;
		}
		mPointList.clear();
		mRenderOp.vertexData->vertexCount = 0;
		pushPoint(Vector3::ZERO);

	}


	//-----------------------------------------------------------------------
	Real TestDistanceAction::getSquaredViewDepth(const Camera *cam) const
	{
		const Vector3 vMin = mBox.getMinimum();
		const Vector3 vMax = mBox.getMaximum();
		const Vector3 vMid = ((vMin - vMax) * 0.5) + vMin;
		const Vector3 vDist = cam->getDerivedPosition() - vMid;

		return vDist.squaredLength();
	}

	//-----------------------------------------------------------------------
	Real TestDistanceAction::getBoundingRadius(void) const
	{
		return Math::Sqrt(max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
	}

	//-----------------------------------------------------------------------
	const Quaternion &TestDistanceAction::getWorldOrientation(void) const
	{
		return Quaternion::IDENTITY;
	}

	//-----------------------------------------------------------------------
	const Vector3 &TestDistanceAction::getWorldPosition(void) const
	{
		return Vector3::ZERO;
	}



	void TestDistanceAction::update(float deltaTime)
	{
		InputListener::update(deltaTime);
	}


	bool TestDistanceAction::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
	{
		// add by 李锦俊 2007-7-27
		// 检测鼠标是否在GUI上面，如果在就不处理
		if(consumed || CEGUI::System::getSingleton().injectMousePosition(arg.state.X.abs, arg.state.Y.abs))
			return true;

		if(id == OIS::MB_Left)
		{

		}
		else if(id == OIS::MB_Right)
		{
		}
		return TerrainAction::mousePressed(arg , id , consumed);
	}

	bool TestDistanceAction::keyReleased(const OIS::KeyEvent &arg, bool& consumed)
	{
		if(arg.key == OIS::KC_DELETE)
		{
			popPoint();
		}
		else if(arg.key == OIS::KC_ESCAPE)
		{
			clearAllPoints();
		}
		return TerrainAction::keyReleased(arg , consumed);
	}

	bool TestDistanceAction::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
	{
		if(id == OIS::MB_Left)
		{
			float screenX = (float)arg.state.X.abs / (float)arg.state.width;
			float screenY = (float)arg.state.Y.abs / (float)arg.state.height;

			// add by 李锦俊 2007-7-12
			// 根据鼠标位置找到地形坐标
			std::pair<bool, Ogre::Vector3> pos = mTerrain->rayIntersects(mCamera , screenX , screenY);
			if(pos.first)
			{
				pushPoint(pos.second);
			}
		}
		else if(id == OIS::MB_Right)
		{
		}
		return TerrainAction::mouseReleased(arg , id , consumed);
	}


	bool TestDistanceAction::mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
	{
		float screenX = (float)arg.state.X.abs / (float)arg.state.width;
		float screenY = (float)arg.state.Y.abs / (float)arg.state.height;

		// add by 李锦俊 2007-7-12
		// 根据鼠标位置找到地形坐标
		std::pair<bool, Ogre::Vector3> pos = mTerrain->rayIntersects(mCamera , screenX , screenY);
		if(pos.first)
		{
			updateLastPoint(pos.second);
		}
		return TerrainAction::mouseMoved(arg , consumed);
	}

	
	void TestDistanceAction::pushPoint(const Vector3 &pos)
	{
		// add by 李锦俊 2007-8-13
		// 越界检测
		if(mRenderOp.vertexData->vertexCount >= TestDistanceAction_VBCount)
			return;
	
		// add by 李锦俊 2007-8-10
		// 保存到列表
		mPointList.push_back(new TestDistanceAction::PointText(mSceneMgr));
		// add by 李锦俊 2007-8-13
		// 更新顶点数据和位置
		setPointPosition(mRenderOp.vertexData->vertexCount , pos);
		mRenderOp.vertexData->vertexCount++;

	}

	void TestDistanceAction::setPointPosition(size_t index , const Vector3 &pos)
	{
		struct Vertex
		{
			Vector3 pos;
			uint32 colour;
		};
		// add by 李锦俊 2007-8-8
		// 锁定顶点缓冲区
		HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(VERTEX_BINDING);             
		Vertex *pPos = static_cast<Vertex*>(vbuf->lock(index * sizeof(Vertex) , sizeof(Vertex) , HardwareBuffer::HBL_DISCARD));

		pPos->pos = pos;
		Root::getSingleton().convertColourValue(ColourValue::Red , &pPos->colour);


		if(index == 0)
		{
			mBox.setExtents(pos, pos);
		}
		else
		{
			Vector3 &vaabMin(mBox.getMinimum());
			Vector3 &vaabMax(mBox.getMaximum());

			// add by 李锦俊 2007-8-8
			// 计算包围盒
			if(pos.x < vaabMin.x)
				vaabMin.x = pos.x;
			if(pos.y < vaabMin.y)
				vaabMin.y = pos.y;
			if(pos.z < vaabMin.z)
				vaabMin.z = pos.z;

			if(pos.x > vaabMax.x)
				vaabMax.x = pos.x;
			if(pos.y > vaabMax.y)
				vaabMax.y = pos.y;
			if(pos.z > vaabMax.z)
				vaabMax.z = pos.z;
		}

		vbuf->unlock();
		TestDistanceAction::PointText &point = *mPointList[index];
		// add by 李锦俊 2007-8-10
		// 创建场景节点
		if(point.mMovableTextNode == 0)
		{
			point.mMovableTextNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		}


		// add by 李锦俊 2007-8-13
		// 更新位置
		point.mMovableTextNode->setPosition(pos);
		String strText;
		strText = StringConverter::toString(point.mMovableTextNode->getPosition());

		if(point.mMovableText == 0)
		{
			static int s_ID = 0;
			s_ID ++;
			// add by 李锦俊 2007-8-10
			// 创建移动文字
			String strNodeName("TestDistancePoint_" + StringConverter::toString(s_ID));
			
			point.mMovableText = new MovableText(strNodeName , strText , "BlueHighway" , 1 
				, ColourValue(Math::RangeRandom(0.5f , 1.0f) , Math::RangeRandom(0.5f , 1.0f) , Math::RangeRandom(0.5f , 1.0f)));
			point.mMovableText->setAdditionalHeight(1);
			point.mMovableText->setTextAlignment(MovableText::H_CENTER, MovableText::V_ABOVE); // Center horizontally and display above the node
			point.mMovableTextNode->attachObject(point.mMovableText);
		}
		// add by 李锦俊 2007-8-13
		// 更新文字
		if(index == 0)
		{
			point.mMovableText->setCaption(strText);
			point.mLen = 0.0f;
			point.mAllLen = 0.0f;
		}
		else
		{
			// add by 李锦俊 2007-8-13
			// 获得上一个点
			TestDistanceAction::PointText &prePoint = *mPointList[index - 1];

			// add by 李锦俊 2007-8-13
			// 计算长度
			point.mLen = (point.mMovableTextNode->getPosition() - prePoint.mMovableTextNode->getPosition()).length();
			point.mHeightLen = point.mMovableTextNode->getPosition().y - prePoint.mMovableTextNode->getPosition().y;
			// add by 李锦俊 2007-8-13
			// 累计到一起
			point.mAllLen = prePoint.mAllLen + point.mLen;
			point.mAllHeightLen = prePoint.mAllHeightLen + point.mHeightLen;
			// add by 李锦俊 2007-8-13
			// 显示出来
			point.mMovableText->setCaption(strText + "\n"
				+ "Distance " + StringConverter::toString(point.mLen) + " " + StringConverter::toString(point.mAllLen) + "\n"
				+ "Height " + StringConverter::toString(point.mHeightLen) + " " + StringConverter::toString(point.mAllHeightLen)
				);
		}
	}

	void TestDistanceAction::popPoint()
	{
		if(mPointList.size() > 1)
		{
			delete mPointList.back();
			mPointList.pop_back();
			mRenderOp.vertexData->vertexCount--;

		}
	}

	void TestDistanceAction::updateLastPoint(const Vector3 &pos)
	{
		setPointPosition(mRenderOp.vertexData->vertexCount - 1 , pos);
	}

	
	FixVertexTestDistanceAction::FixVertexTestDistanceAction(Camera *pCamera , ETTerrain* pTerrain , InputListener *pListener)
		: TestDistanceAction(pCamera , pTerrain , pListener)
	{
	}
	bool FixVertexTestDistanceAction::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id, bool& consumed)
	{
		if(id == OIS::MB_Left)
		{
			float screenX = (float)arg.state.X.abs / (float)arg.state.width;
			float screenY = (float)arg.state.Y.abs / (float)arg.state.height;

			// add by 李锦俊 2007-7-12
			// 根据鼠标位置找到地形坐标
			std::pair<bool, Ogre::Vector3> pos = mTerrain->rayIntersects(mCamera , screenX , screenY);
			if(pos.first)
			{
				// add by 李锦俊 2007-8-13
				// 对齐到网格
				Vector2 grid = mTerrain->getTerrainGrid(pos.second);
				pushPoint(mTerrain->getTerrainPos(grid.x , grid.y));
			}
		}
		else if(id == OIS::MB_Right)
		{
		}
		return TerrainAction::mouseReleased(arg , id , consumed);
	}


	
	bool FixVertexTestDistanceAction::mouseMoved(const OIS::MouseEvent &arg, bool& consumed)
	{
		// add by 李锦俊 2007-7-12
		// 根据鼠标位置找到地形坐标
		float screenX = (float)arg.state.X.abs / (float)arg.state.width;
		float screenY = (float)arg.state.Y.abs / (float)arg.state.height;

		// add by 李锦俊 2007-7-12
		// 根据鼠标位置找到地形坐标
		std::pair<bool, Ogre::Vector3> pos = mTerrain->rayIntersects(mCamera , screenX , screenY);
		if(pos.first)
		{
			// add by 李锦俊 2007-8-13
			// 对齐到网格
			Vector2 grid = mTerrain->getTerrainGrid(pos.second);
			updateLastPoint(mTerrain->getTerrainPos(grid.x , grid.y));
		}

		return TerrainAction::mouseMoved(arg , consumed);
	}

}