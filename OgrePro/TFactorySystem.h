#pragma once

namespace Ogre
{
	// 对象工厂的实现模板
	template<typename T>
	class TFactorySystem
	{
	public:

		TFactorySystem(void)
		{
		}

		virtual ~TFactorySystem(void)
		{
			removeAllFactory();
		}

		// 添加一个工厂
		virtual void addFactory(T *factory)
		{
			m_mapFactory[factory->getType()] = factory;
		}
		// 删除一个工厂
		virtual void removeFactory(T *factory)
		{
			removeFactory(factory->getType());
		}
		// 删除一个工厂
		virtual void removeFactory(const String &typeName)
		{
			MapFactory::iterator iter = m_mapFactory.find(typeName);
			if(iter != m_mapFactory.end())
			{
				delete iter->second;
				m_mapFactory.erase(iter);
			}
		}
		// 删除所有工厂
		virtual void removeAllFactory()
		{
			for(MapFactory::iterator iter = m_mapFactory.begin() ; iter != m_mapFactory.end() ; iter ++)
			{
				delete iter->second;
			}
			m_mapFactory.clear();
		}
		// 获得一个工厂
		virtual T* getFactory(const String &typeName)const
		{
			MapFactory::const_iterator iter = m_mapFactory.find(typeName);
			if(iter == m_mapFactory.end())
			{
				return 0;
			}
			else
			{
				return iter->second;
			}
		}
		// 获得工厂名称列表
		virtual StringVector getFactoriesName()const
		{
			StringVector str;
			for(MapFactory::const_iterator iter = m_mapFactory.begin() ; iter != m_mapFactory.end() ; iter ++)
			{
				str.push_back(iter->second->getType());
			}
			return str;
		}
	protected:
		typedef std::map<String , T*> MapFactory;
		// 对象工厂注册集合
		MapFactory m_mapFactory;
	};

};