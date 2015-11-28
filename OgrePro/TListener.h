// ============================================================
// 工程项目	:	MMORPG
// 文件名	:	TListener.h
// 创建人	:	FreeKnight(王宏张)
// 说明		:	一个Listener设计模式的帮助类
// 创建时间	:	2007-8-20 10:54:10
// ============================================================
#pragma once

#define ExecuteListenerStart(T) {\
	const Ogre::TListener<T>::ListListener &listeners = Ogre::TListener<T>::getListenerList();\
	for(size_t i = 0 ; i < listeners.size() ; i ++)\
	{

#define ExecuteListenerEnd(T) }\
	}

#define ExecuteListener(T , fun) ExecuteListenerStart(T) \
	listeners[i]->fun();ExecuteListenerEnd(T)
#define ExecuteListener1(T , fun , p1) ExecuteListenerStart(T) \
	listeners[i]->fun(p1);ExecuteListenerEnd(T)
#define ExecuteListener2(T , fun , p1 , p2) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2);ExecuteListenerEnd(T)
#define ExecuteListener3(T , fun , p1 , p2 , p3) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3);ExecuteListenerEnd(T)
#define ExecuteListener4(T , fun , p1 , p2 , p3 , p4) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4);ExecuteListenerEnd(T)
#define ExecuteListener5(T , fun , p1 , p2 , p3 , p4 , p5) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5);ExecuteListenerEnd(T)
#define ExecuteListener6(T , fun , p1 , p2 , p3 , p4 , p5 , p6) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6);ExecuteListenerEnd(T)
#define ExecuteListener7(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7);ExecuteListenerEnd(T)
#define ExecuteListener8(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8);ExecuteListenerEnd(T)
#define ExecuteListener9(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9);ExecuteListenerEnd(T)
#define ExecuteListener10(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10);ExecuteListenerEnd(T)
#define ExecuteListener11(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11);ExecuteListenerEnd(T)
#define ExecuteListener12(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12);ExecuteListenerEnd(T)
#define ExecuteListener13(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13);ExecuteListenerEnd(T)
#define ExecuteListener14(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13 , p14) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13 , p14);ExecuteListenerEnd(T)
#define ExecuteListener15(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13 , p14 , p15) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13 , p14 , p15);ExecuteListenerEnd(T)


#define ExecuteListener(T , fun) ExecuteListenerStart(T) \
	listeners[i]->fun();ExecuteListenerEnd(T)
#define ExecuteListener1(T , fun , p1) ExecuteListenerStart(T) \
	listeners[i]->fun(p1);ExecuteListenerEnd(T)
#define ExecuteListener2(T , fun , p1 , p2) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2);ExecuteListenerEnd(T)
#define ExecuteListener3(T , fun , p1 , p2 , p3) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3);ExecuteListenerEnd(T)
#define ExecuteListener4(T , fun , p1 , p2 , p3 , p4) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4);ExecuteListenerEnd(T)
#define ExecuteListener5(T , fun , p1 , p2 , p3 , p4 , p5) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5);ExecuteListenerEnd(T)
#define ExecuteListener6(T , fun , p1 , p2 , p3 , p4 , p5 , p6) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6);ExecuteListenerEnd(T)
#define ExecuteListener7(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7);ExecuteListenerEnd(T)
#define ExecuteListener8(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8);ExecuteListenerEnd(T)
#define ExecuteListener9(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9);ExecuteListenerEnd(T)
#define ExecuteListener10(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10);ExecuteListenerEnd(T)
#define ExecuteListener11(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11);ExecuteListenerEnd(T)
#define ExecuteListener12(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12);ExecuteListenerEnd(T)
#define ExecuteListener13(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13);ExecuteListenerEnd(T)
#define ExecuteListener14(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13 , p14) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13 , p14);ExecuteListenerEnd(T)
#define ExecuteListener15(T , fun , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13 , p14 , p15) ExecuteListenerStart(T) \
	listeners[i]->fun(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 , p9 , p10 , p11 , p12 , p13 , p14 , p15);ExecuteListenerEnd(T)


namespace Ogre
{

	template<typename T>
	class TListener
	{
	public:
		typedef std::deque<T*> ListListener;
		TListener(){};
		virtual ~TListener()
		{
			removeAllListener();
		};

		// 添加侦听器
		template<typename T>
		void addListener(T * listener , bool toBack = true)
		{
			removeListener(listener);
			if(toBack)
				m_listListener.push_back(listener);
			else
				m_listListener.push_front(listener);
		}

		// 删除侦听器
		template<typename T>
		void removeListener(T * listener)
		{
			for(ListListener::iterator iter = m_listListener.begin() ; iter != m_listListener.end() ; iter ++)
			{
				if(*iter == listener)
				{
					m_listListener.erase(iter);
					break;
				}
			}
		}
		void removeAllListener()
		{
			m_listListener.clear();
		}

		const ListListener &getListenerList()const{return m_listListener;}

	protected:
		// 保存所有Listener的指针
		ListListener m_listListener;
	};
};