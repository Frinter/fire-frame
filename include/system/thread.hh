#pragma once

namespace System
{
	class thread
	{
	public:
		class IThreadImplementation
		{
		public:
			virtual void Run() = 0;
		};
		
	private:
		template <typename Callable>
		class ThreadImplementation : IThreadImplementation
		{
		public:
			ThreadImplementation(Callable *function)
				: m_function(function)
			{
			}

			ThreadImplementation(ThreadImplementation &other) = delete;
			ThreadImplementation(ThreadImplementation &&other) = delete;

			void Run()
			{
				m_function();
			}

		private:
			Callable *m_function;
		};
		
		template <typename Callable>
		IThreadImplementation *createImplementation(Callable function)
		{
			return NULL;
		}
		
	public:
		template <class Callable>
		thread(Callable &&function)
			: m_joinable(false)
		{
			m_implementation = createImplementation(function);
			m_implementation->Run();
		}
		
		~thread()
		{
			if (m_joinable)
				join();
		};
		
		void join()
		{
		}
		
	private:
		void StartThread();
		
	private:
		IThreadImplementation *m_implementation;
		bool m_joinable;
	};
}