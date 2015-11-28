/******************************************************************
** 文件名:	DecalSystem.h
** 版  权:	(C)  
** 创建人:	FreeKnight(王宏张)
** 日  期:	2008-1-10 9:53:30
** 描  述:
******************************************************************/
#pragma once


namespace Ogre
{

	class ETTerrain;
	// 贴花的类型
	enum DecalType
	{
		DT_NORMAL,		// 法线贴花，根据所在位置的法线自动调整贴花朝向
		DT_MESH,		// 模型贴花，根据贴花所在的网格自动生成模型
		DT_PROJECTIVE,	// 投影贴花，利用投影纹理的方式实现的贴花（未实现）
		DT_GRID,		// 网格贴花，创建指定大小的网格贴花，然后自动调整网格的位置
	};

	// 一个贴花
	struct IDecal
	{
		virtual ~IDecal(){};
		// 获得这个贴花的名字
		virtual const String &getName()const = 0;

		// 获得贴花的类型
		virtual const DecalType getType()const = 0;

		// 获得场景管理器
		virtual SceneManager* getSceneManager()const = 0;

		// 设置贴花材质名字
		virtual void setMaterialName(const String &materialName) = 0;
		// 获得贴花材质名字
		virtual const String &getMaterialName()const = 0;

		// 设置贴花所在位置
		virtual void setPosition(const Vector3 &pos) = 0;
		// 获取贴花所在位置
		virtual const Vector3 &getPosition()const = 0;

		// 设置贴花大小
		virtual void setSize(const Vector2 &size) = 0;
		// 获取贴花大小
		virtual const Vector2 &getSize()const = 0;

		// 获取贴花的包围盒（本地坐标）
		virtual const AxisAlignedBox &getBoundingBox()const = 0;

		// 获取贴花的世界包围盒（本地坐标）
		virtual const AxisAlignedBox &getWorldBoundingBox()const = 0;

		// 设置贴花的深度，指最大拉伸的高度
		virtual void setDepth(float depth) = 0;
		// 获取贴花深度
		virtual float getDepth()const = 0;
		// 设置贴花沿Y轴的旋转量
		virtual void setRotate(const Radian &angle) = 0;
		// 获取贴花沿Y轴的旋转量
		virtual const Radian &getRotate()const = 0;

		// 设置贴花可视属性
		virtual void setVisible(bool visible) = 0;
		// 获得贴花是否显示
		virtual bool isVisible()const = 0;

		// 设置挂接物体相对偏移
		virtual void setAttachObjectsOffset(const Vector3 &offsetPosition = Vector3::ZERO
			, const Quaternion &offsetOrientation = Quaternion::IDENTITY
			, const Vector3 &offsetScale = Vector3(1.0f , 1.0f , 1.0f)
			) = 0;
		// 获得挂接物体相对旋转
		virtual const Quaternion &getOffsetOrientation()const = 0;
		// 获得挂接物体相对位置
		virtual const Vector3 &getOffsetPosition()const = 0;
		// 获得挂接物体相对缩放
		virtual const Vector3 &getOffsetScale()const = 0;

		// 挂接一个物件到贴花中心点上
		virtual void attachObject(MovableObject *obj) = 0;
		// 取消挂接物件
		virtual void dettachObject(MovableObject *obj) = 0;
		// 根据索引号取消挂接物件
		virtual void dettachObject(ushort index) = 0;
		// 取消挂接所有物件
		virtual void detachAllObjects() = 0;

		// 获得贴花节点
		virtual SceneNode* getSceneNode() = 0;
		// 获得用来附加物件的节点
		virtual SceneNode* getAttachObjectSceneNode() = 0;

		// 根据索引号获得挂接了的物件
		virtual MovableObject* getAttachObject(ushort index)const = 0;

		// 获得已挂接的物件枚举
		virtual SceneNode::ObjectIterator getAttachedObjectIterator(void) = 0;

		// 每帧更新贴花
		virtual void update(float timeSinceLastFrame) = 0;
	};

	// 贴花系统
	class OGREPRO_API IDecalSystem : public Singleton<IDecalSystem>
	{
	public:
		IDecalSystem();
		virtual ~IDecalSystem();

		// 单身模式实现
		static IDecalSystem& getSingleton(void);
		static IDecalSystem* getSingletonPtr(void);

		// 初始化贴花系统
		static void init(SceneManager *mgr);
		// 销毁贴花系统
		static void shutdown();

		// 使用这个场景管理器，调用这个函数之后，createDecal将使用这个场景管理器
		virtual void acceptSceneMgr(SceneManager *mgr) = 0;
		// 获得场景管理器
		virtual SceneManager* getSceneManager()const = 0;

		// 根据参数和实例名创建一个贴花，如果实例名为空就自动生成
		virtual IDecal *createDecal(DecalType decalType
			, const String &name = StringUtil::BLANK
			, const NameValuePairList *params = 0
			) = 0;

		// 销毁一个贴花
		virtual void destroyDecal(IDecal *decal) = 0;
		virtual void destroyDecal(const String &name) = 0;

		// 销毁所有贴花
		virtual void destroyAllDecal() = 0;

		// 根据实例名获得一个贴花
		virtual IDecal *getDecal(const String &name) = 0;
		// 检查是否有这个实例名的贴花
		virtual bool hasDecal(const String &name)const = 0;

		typedef std::map<String , IDecal*> MapDecal;

		// 获得所有贴花枚举
		typedef MapIterator<MapDecal> DecalIterator;
		virtual DecalIterator getDecalIterator() = 0;

		// 获得所有贴花枚举
		typedef ConstMapIterator<MapDecal> ConstDecalIterator;
		virtual ConstDecalIterator getDecalIterator()const = 0;

		// 对世界模型做射线检测（只针对地形）
		virtual ETTerrain* worldRayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd) = 0;
		virtual ETTerrain* worldRayQuery(Vector3 &posIntersect , const Ray &ray) = 0;

		// 对所有物体进行射线检测（包括地形、地上物体、玩家、怪物等等）
		virtual bool rayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd) = 0;

		// 鼠标点击测试，返回世界交点坐标（地形）
		virtual ETTerrain* hitTestWorld(Vector3 &posIntersect , short cursorX, short cursorY , Camera *camera) = 0;

	};
};