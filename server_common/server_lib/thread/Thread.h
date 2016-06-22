#ifndef _TASK_H_
#define _TASK_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/bind.hpp>
#include <deque>
#include <set>

#include "lib/noncopyable.h"
#include "lib/log.h"

namespace server { namespace thread {

enum TaskType
{
	ENUM_TASK_EXIT = 0,
	ENUM_TASK_TYPE_1,
	ENUM_TASK_TYPE_2,
	ENUM_TASK_TYPE_3,
	ENUM_TASK_TYPE_4,
	ENUM_TASK_TYPE_5
};


class Task
{
	public:
		Task(TaskType t):type(t){ }
		virtual ~Task(){}
		virtual void run() { }
		TaskType GetTaskType()
		{
			return type;
		}
	private:
		TaskType type;	

};

class ThreadManger;

class BoostThread:public server::lib::noncopyable    //每个线程对象都是唯一的，不应该被复制
{
	public:
		BoostThread(ThreadManger * p_thread_manger);
		void ThreadMain(ThreadManger * p_thread_manger);

	private:
		std::auto_ptr<boost::thread> m_thread;
};

class ThreadManger
{
	public:
		~ThreadManger()
		{
            server::log::logger(server::log::LOG_INFO, "ThreadManger start destruct!"); 
			JoinAll();
		}

		void IncreaseThread();                      //创建一个线程  
		void DecreaseThread();                      //释放一个线程
		void ThreadExit(BoostThread * p_thread);
		void SetThreadCount(const int32_t count);   //创建一定数量的线程池
		void JoinAll();                             //释放线程池
	
		void ThreadRun();
		void SetQueueLimit(int32_t max_limit);
		void PushTask(Task* task);                 

	private:
		std::deque<Task *> task_queue;    //任务队列
		boost::mutex queue_mutex;        //公共任务队列的互斥锁
		boost::condition queue_cond;     //任务队列的条件变量
		int32_t queue_limit;             //队列容量，超过直接丢弃

		std::set<BoostThread *> thread_pool;  //线程池
		boost::mutex thread_mutex;
		boost::condition thread_cond;   //用来等待线程池释放掉

};

class MulThreadHelper
{
	public:
		void Init(int32_t thread_num, int32_t queue_limit);
		void PushTask(Task* task);

	private:
		ThreadManger _thread_manger;		
		
};

}}




#endif
