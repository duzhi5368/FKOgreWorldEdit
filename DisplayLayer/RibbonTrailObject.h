#pragma once
#include "NullObject.h"
namespace Ogre
{
	class EntityObject;

	// 场景物件的对象工厂
	class DISPLAYLAYER_API RibbonTrailObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// 获得该工厂的类型名称
		virtual const String &getType() const;
		// 工厂对象的创建方法
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// 工厂对象的销毁方法
		virtual void destroy(ISceneObject *obj);
	};

	// 模型实体对象
	class DISPLAYLAYER_API RibbonTrailObject : public NullObject
	{
	public:
		RibbonTrailObject(const String &name);
		virtual ~RibbonTrailObject(void);
		// 获得对象类型
		virtual const String &getType()const
		{
			return RibbonTrailObjectFactory::FACTORY_TYPE;
		}
		// 创建对象实体
		virtual void create(IScene *scene , const NameValuePairList* params);
		// 销毁对象实体
		virtual void destroy();
		// 获得MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_ribbonTrail;
		}
		// 获得RibbonTrail
		virtual RibbonTrail *getOgreRibbonTrail()const
		{
			return m_ribbonTrail;
		}

		// 用条带跟踪这个场景对象，返回chainIndex
		size_t trail(ISceneObject *obj);
		size_t trail(const String &objName);

		// 用条带跟踪这个骨骼，返回chainIndex
		size_t trail(EntityObject *obj , const String &boneName);
		size_t trail(const String &entityObjName , const String &boneName);

		// 停止跟踪
		void stopTrail(ISceneObject *obj);
		void stopTrail(const String &objName);
		void stopTrail(size_t chainIndex);

		// 停止跟踪
		void stopTrail(EntityObject *obj , const String &boneName);
		void stopTrail(const String &entityObjName , const String &boneName);


		// 设置条带初始颜色
		void setInitialColour(size_t chainIndex, const ColourValue& col);
		const ColourValue& getInitialColour(size_t chainIndex) const;

		// 设置条带颜色每秒减去的颜色
		void setColourChange(size_t chainIndex, const ColourValue& valuePerSecond);
		const ColourValue& getColourChange(size_t chainIndex) const;

		// 设置条带默认宽度
		void setInitialWidth(size_t chainIndex, float width);
		float getInitialWidth(size_t chainIndex) const;

		// 设置条带每秒减去的宽度
		void setWidthChange(size_t chainIndex, float widthDeltaPerSecond);
		float getWidthChange(size_t chainIndex) const;

		// 设置条带的长度
		void setTrailLength(float len);
		float getTrailLength(void) const;

		// 设置条带的材质
		void setMaterialName(const String &name);
		const String &getMaterialName(void) const;

		// 被跟踪的对象
		struct TrailObject
		{
			TrailObject(ISceneObject *obj , const String &bone = StringUtil::BLANK , TagPoint *tag = 0)
				: object(obj)
				, boneName(bone)
				, boneTag(tag)
			{

			}
			union
			{
				ISceneObject *object;
				EntityObject *entity;
			};			
			// 骨头名称，仅当挂接到骨头时才有数据
			String boneName;
			// 骨头节点
			TagPoint *boneTag;

			bool operator==(ISceneObject *obj)const
			{
				return object == obj && boneName.empty() && boneTag == 0;
			}
		};
		typedef std::vector<TrailObject> ListTrailObject;
		typedef ConstVectorIterator<ListTrailObject> TrailObjectIterator;
		// 获得被跟踪的对象列表
		TrailObjectIterator getTrailObjectIterator()const
		{
			return TrailObjectIterator(m_listTrailObject.begin() , m_listTrailObject.end());
		}

	protected:
		// Ogre实体
		RibbonTrail *m_ribbonTrail;

		// 正在跟踪的对象
		ListTrailObject m_listTrailObject;
	};


};