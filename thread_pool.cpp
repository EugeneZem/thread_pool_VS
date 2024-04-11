#include "thread_pool.h"

/*----------------------->> safe_queue <<-----------------------*/
void safe_queue::push(std::function<void()> funct)
{
	{
		std::unique_lock<std::mutex> lock(_mt);
		_queue.push(funct);
	}
	_notification.notify_one();
}

std::function<void(void)> safe_queue::pop()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(_mt);
		_notification.wait(lock, [&] { return !(_queue.empty()); });
		std::function<void()> res = _queue.front();
		lock.unlock();
		return res;
	}
}

bool safe_queue::empty()
{
	std::unique_lock<std::mutex> lock(_mt);
	return _queue.empty();
}

/*----------------------->> thread_pool <<-----------------------*/
thread_pool::thread_pool()
{
	_squeue = new safe_queue;
}

thread_pool::~thread_pool()
{
	delete _squeue;
}

void thread_pool::work_one_thread()
{
	while (!_squeue->empty())
	{
		std::function<void()> runing_funct = _squeue->pop();
		runing_funct();
	}
}

void thread_pool::work()
{
	const auto cores_count = std::thread::hardware_concurrency();
	for (unsigned int i = 0; i < cores_count; ++i)
	{
		std::unique_lock<std::mutex> lk(_squeue->_mt);
		_pool.push_back(std::thread(work_one_thread));
	}

	for (std::thread& thr : _pool)
	{
		thr.join();
	}

}

void thread_pool::submit(std::function<void(void)> funct)
{
//	std::lock_guard<std::mutex> lock(_squeue->_mt);
	_squeue->push(funct);
}

