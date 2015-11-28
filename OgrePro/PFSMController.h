#pragma once
namespace Ogre
{
	// 用来设置所有状态成不可达状态
	#define ALL_STATE -1
	// 并行有限状态机实现类
	template<class T>
	class PFSMController
	{
	public:
		//回调函数
		typedef void (T::*ProcCommand)(uint32 state);
		typedef void (T::*StateCommand)(float timeSinceLastFrame);
		typedef void (T::*RefuseCommand)(uint32 oldState , uint32 newState);
	protected:

		//状态结构
		struct State
		{
			State(){OnEnter=NULL; OnLeave=NULL;}
			uint32 state;
			ProcCommand OnEnter;
			ProcCommand OnLeave;
			StateCommand OnState;
		};

		typedef std::list<State> ASheet;		//一个状态表

		//并行态结构
		struct PState
		{
			ASheet Sheet;
			uint32 CurrState;
			StateCommand OnState;
		};

		std::map<uint32, PState> m_Cont;		//并行状态表

		//不可转换状态结构
		struct InAccessible
		{
			InAccessible(){OnRefuse=NULL;}
			uint32 Sheet;
			uint32 StateFrom;
			uint32 StateTo;
			RefuseCommand OnRefuse;
		};
		std::list<InAccessible> m_InAccessible;	//不可转换状态列表

		T* m_CallbackClass;	

	public:

		/**
		得到当前状态
		@param 如果没有此状态机，返回0xFFFFFFFF
		*/
		uint32 getCurrentState(uint32 fsm);


		/**
		设置回调函数类
		*/
		void setCallBackClass(T* callback)
		{
			m_CallbackClass = callback;
		}

		/**
		添加一个并行态
		@param fsm 状态ID
		*/
		void addFSM(uint32 fsm);

		/**
		删除一个并行态
		@param fsm 状态ID
		*/
		void deleteFSM(uint32 fsm);

		/**
		添加一个子态到一个并行态
		@param fsm 并行态ID
		@param state 子态ID
		@param onEnter 进入回调
		@param onLeave 离开回调
		*/
		void addState(uint32 fsm, uint32 state,
			ProcCommand onEnter = 0,
			ProcCommand onLeave = 0,
			StateCommand onState = 0);

		/**
		从并行态中删除一个子态
		@param fsm 并行态ID
		@param state 子态ID
		*/
		void deleteState(uint32 fsm, uint32 state);

		/**
		改变状态
		@param fsm 并行态ID
		@param state 新子态ID
		@param force 是否强制变换状态（不理会不可达条件）
		@note 强制变换时，不会调用回调函数
		*/
		void changeState(uint32 fsm, uint32 newState, bool force = false);

		/**
		设置不可到达条件
		@param fsm 并行态ID
		@param stateFrom 上一子态
		@param stateTo	 需变化到的子态
		@param onResult  条件发生时回调
		*/
		void addInaccessible(uint32 fsm,
			uint32 stateFrom,
			uint32 stateTo,
			RefuseCommand onRefuse = 0);

		/**
		删除不可到达条件
		@param fsm 并行态ID
		@param stateFrom 上一子态
		@param stateTo   需变化到的子态
		*/
		void deleteInaccessible(uint32 fsm,
			uint32 stateFrom,
			uint32 stateTo);


		/**
		设置不可到达条件
		@param fsm 并行态ID
		@param onResult  条件发生时回调
		*/
		void addInaccessible(uint32 fsm,
			RefuseCommand onRefuse = 0);

		/**
		删除不可到达条件
		@param fsm 并行态ID
		*/
		void deleteInaccessible(uint32 fsm);

		/**
		当前状态下
		*/
		void update(float timeSinceLastFrame);

		// 清空所有数据
		void clearFSM()
		{
			m_Cont.clear();
			m_InAccessible.clear();
			m_CallbackClass = 0;
		}
	};

	template<class T> 
	void PFSMController<T>::addFSM(uint32 fsm)
	{
		//查找当前fsm是否已经存在
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it==m_Cont.end())
		{
			//添加进去
			PState newPState;
			m_Cont.insert(std::pair<uint32, PState>(fsm, newPState));
		}
		else
			return;
	}

	template<class T> 
	void PFSMController<T>::addState(uint32 fsm, uint32 state,
		ProcCommand onEnter = 0,
		ProcCommand onLeave = 0,
		StateCommand onState = 0)
	{
		//查找当前fsm
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it!=m_Cont.end())
		{

			PState* pState = &(it->second);
			ASheet::iterator it1 = (pState->Sheet).begin();
			for(;it1!=(pState->Sheet).end();++it1)
			{
				//有重复
				if(state == it1->state)
					return;
			}
			//添加新状态
			State newState;
			newState.OnEnter = onEnter;
			newState.OnLeave = onLeave;
			newState.OnState = onState;
			newState.state = state;
			(pState->Sheet).push_back(newState);
			pState->CurrState = state;		//改变当前状态
			pState->OnState = onState;		//此状态回调函数
			return;
		}
	}

	template<class T> 
	void PFSMController<T>::changeState(uint32 fsm, uint32 newState, bool force)
	{
		//查找当前fsm是否存在
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it!=m_Cont.end())
		{
			//查找新状态和旧状态是否存在
			PState* pState = &(it->second);
			ASheet::iterator it1 = (pState->Sheet).begin();
			bool oldExist = false;
			bool newExist = false;
			ProcCommand newOnEnter = NULL;
			ProcCommand oldOnLeave = NULL;
			StateCommand newOnState = NULL;
			uint32 oldState = pState->CurrState;
			for(;it1!=(pState->Sheet).end();++it1)
			{
				//新状态与当前状态重复
				if(newState == pState->CurrState)
					return;

				if(it1->state == pState->CurrState)
				{
					oldExist = true;
					oldOnLeave = it1->OnLeave;
				}
				if(it1->state == newState)
				{
					newExist = true;
					newOnEnter = it1->OnEnter;
					newOnState = it1->OnState;
				}
			}
			//如果是强制转换
			if(force == true && newExist == true)
			{
				if(oldOnLeave)
					(m_CallbackClass->*oldOnLeave)(newState);
				pState->CurrState = newState;
				pState->OnState = newOnState;
				if(newOnEnter!=NULL)
				{
					(m_CallbackClass->*newOnEnter)(oldState);
				}
				return;
			}

			if(newExist == true && oldExist == true)
			{
				//查找是否是不可到达条件
				std::list<InAccessible>::iterator it2 = m_InAccessible.begin();
				for(;it2!=m_InAccessible.end();++it2)
				{
					if((it2->Sheet == fsm &&
						it2->StateFrom == pState->CurrState &&
						it2->StateTo == newState)
						|| (it2->Sheet == fsm && 
						it2->StateFrom == ALL_STATE &&
						it2->StateTo == ALL_STATE)
						)
					{
						if(it2->OnRefuse)
							(m_CallbackClass->*(it2->OnRefuse))(oldState , newState);
						return;
					}
				}
				if(oldOnLeave)
					(m_CallbackClass->*oldOnLeave)(newState);
				pState->CurrState = newState;
				pState->OnState = newOnState;
				if(newOnEnter)
					(m_CallbackClass->*newOnEnter)(oldState);
			}
			return;
		}
	}

	template<class T> 
	void PFSMController<T>::addInaccessible(uint32 fsm, uint32 stateFrom, uint32 stateTo,
		RefuseCommand onRefuse)
	{
		//检查条件是否已经存在
		std::list<InAccessible>::iterator it = m_InAccessible.begin();
		for(;it!=m_InAccessible.end();++it)
		{
			if(fsm == it->Sheet &&
				stateFrom == it->StateFrom &&
				stateTo == it->StateTo)
				return;
		}

		InAccessible tmp;
		tmp.Sheet = fsm;
		tmp.StateFrom = stateFrom;
		tmp.StateTo = stateTo;
		tmp.OnRefuse = onRefuse;

		m_InAccessible.push_back(tmp);
	}

	template<class T> 
	void PFSMController<T>::deleteInaccessible(uint32 fsm, uint32 stateFrom, uint32 stateTo)
	{
		//确认条件已经存在了
		std::list<InAccessible>::iterator it = m_InAccessible.begin();
		for(;it!=m_InAccessible.end();++it)
		{
			if(fsm == it->Sheet &&
				stateFrom == it->StateFrom &&
				stateTo == it->StateTo)
			{
				m_InAccessible.erase(it);
				return;
			}
		}
	}



	template<class T> 
	void PFSMController<T>::addInaccessible(uint32 fsm, RefuseCommand onRefuse)
	{
		//检查条件是否已经存在
		std::list<InAccessible>::iterator it = m_InAccessible.begin();
		for(;it!=m_InAccessible.end();++it)
		{
			if(fsm == it->Sheet &&
				ALL_STATE == it->StateFrom &&
				ALL_STATE == it->StateTo)
				return;
		}

		InAccessible tmp;
		tmp.Sheet = fsm;
		tmp.StateFrom = ALL_STATE;
		tmp.StateTo = ALL_STATE;
		tmp.OnRefuse = onRefuse;

		m_InAccessible.push_back(tmp);
	}

	template<class T> 
	void PFSMController<T>::deleteInaccessible(uint32 fsm)
	{
		//确认条件已经存在了
		std::list<InAccessible>::iterator it = m_InAccessible.begin();
		for(;it!=m_InAccessible.end();++it)
		{
			if(fsm == it->Sheet &&
				ALL_STATE == it->StateFrom &&
				ALL_STATE == it->StateTo)
			{
				m_InAccessible.erase(it);
				return;
			}
		}
	}

	template<class T> 
	void PFSMController<T>::deleteFSM(uint32 fsm)
	{
		//确认已经存在了
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it!=m_Cont.end())
		{	
			m_Cont.erase(it);
			return;
		}
	}

	template<class T> 
	void PFSMController<T>::deleteState(uint32 fsm, uint32 state)
	{
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it!=m_Cont.end())
		{
			PState* pState = &(it->second);
			ASheet::iterator it1 = pState->Sheet.begin();
			for(;it1!=pState->Sheet.end();++it1)
			{
				if(state == it1->state)
				{
					pState->Sheet.erase(it1);
					return;
				}
			}
		}
	}

	template<class T>
	void PFSMController<T>::update(float timeSinceLastFrame)
	{
		std::map<uint32, PState>::iterator it = m_Cont.begin();
		for(;it!=m_Cont.end();++it)
		{
			PState* pState = &(it->second);
			if(pState->OnState)
			{
				(m_CallbackClass->*(pState->OnState))(timeSinceLastFrame);
			}
		}
	}

	template<class T>
	uint32 PFSMController<T>::getCurrentState(uint32 fsm)
	{
		std::map<uint32, PState>::iterator it = m_Cont.find(fsm);
		if(it == m_Cont.end())
			return 0xFFFFFFFF;

		PState* pState = &(it->second);
		return pState->CurrState;
	}

};