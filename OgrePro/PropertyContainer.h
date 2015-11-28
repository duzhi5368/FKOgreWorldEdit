#pragma once

namespace Ogre
{
	// 属性容器
	class OGREPRO_API PropertyContainer
	{
	public:
		PropertyContainer(){};
		virtual ~PropertyContainer(){};
		// 属性类型
		enum PropertyType
		{
			PT_GROUP,	// 分组，不允许设置属性
			PT_BOOL,
			PT_REAL,
			PT_INT,
			PT_UINT,
			PT_SHORT,
			PT_USHORT,
			PT_LONG,
			PT_ULONG,
			PT_STRING,
			PT_VECTOR2,
			PT_VECTOR3,
			PT_MATRIX3,
			PT_MATRIX4,
			PT_QUATERNION,
			PT_COLOURVALUE,
			PT_AXISALIGNEDBOX,
			PT_UNKNOWN = 0xFFFF,
		};
		// 属性
		class OGREPRO_API Property
		{
		public:
			Property();
			Property(uint inID , PropertyType inType , const String &inName , const String &inDesc , bool inShowable , bool inEditable , bool inSaveable);
			// 属性ID
			uint id;
			// 在界面上显示的名称
			String name;
			// 属性类型
			PropertyType type;
			// 属性说明
			String desc;
			// 是否允许显示给用户
			bool showable;
			// 是否允许用户修改
			bool editable;
			// 是否允许保存
			bool saveable;
			// 子属性
			typedef std::vector<Property> VTProperty;
			VTProperty childs;
			// 父属性
			Property *parent;

			inline void addChild(Property &property)
			{
				property.parent = this;
				childs.push_back(property);
			}
		};
		typedef std::map<uint , Property> MapProperty;
		typedef std::vector<Property> VTProperty;

		struct PropertyVisitor
		{
			virtual bool visit(const Property &property) = 0;
		};
		// 获得所有属性，以树的形式
		virtual const VTProperty* getPropertiesTree()const = 0;
		// 获得所有属性，以列表的形式
		virtual const MapProperty* getPropertiesList()const = 0;

		// 把当前属性集中所有属性拷贝到target中
		void cloneProperties(PropertyContainer *target);

		// 设置属性
		virtual bool setProperty(uint id , const Any &value) = 0;
		// 获取属性值
		virtual Any getProperty(uint id)const = 0;
		// 获取属性值
		Any getProperty(const String &name)const;

		// 设置属性
		bool setPropertyByName(const String &name , const String &value);
		// 获取属性值
		String getPropertyByName(const String &name)const;
		// 设置属性
		bool setPropertyByName(const String &name , const Any &value);


		// 设置属性
		bool setPropertyByID(uint id , const String &value);
		// 获取属性值
		String getPropertyByID(uint id)const;

		// 根据属性名称获取属性ID，返回0则找不到
		uint getPropertyID(const String &name);
		// 根据属性ID获取属性名称
		String getPropertyName(uint id);

		// 遍历访问所有属性
		void visitProperties(PropertyVisitor *visitor);
		void visitProperties(PropertyVisitor *visitor)const;
		// 遍历访问一个属性数组
		static void visitProperties(PropertyVisitor *visitor , const VTProperty &properties);

		// 把Any值转换到String类型
		static String toString(const Any &any , PropertyType type);
		// 把String类型根据类型转换成Any
		static Any toAny(const String &value , PropertyType type);

		// 写入Any值到文件file
		static void writeAny(FILE* file , const Any &any , PropertyType type);
		// 从文件file读取一个Any值
		static Any readAny(FILE* file , PropertyType type);
		// 从数据流stream读取一个Any值
		static Any readAny(DataStreamPtr &stream , PropertyType type);
	protected:
		// 枚举所有属性，以树的形式
		virtual void enumPropertiesTree(VTProperty &properties)const = 0;
		// 枚举所有属性，以列表的形式
		void enumPropertiesList(MapProperty &properties)const;
	};
}