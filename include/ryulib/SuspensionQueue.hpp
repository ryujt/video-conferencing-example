#ifndef RYULIB_SUSPENSIONQUEUE_HPP
#define RYULIB_SUSPENSIONQUEUE_HPP

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SuspensionQueue
{
private:
	bool is_terminated_;
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable condition_;
public:
	SuspensionQueue() :
		is_terminated_(false)
	{

	}

	void push(T item)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		queue_.push(item);
		lock.unlock();

		condition_.notify_all();
	}

	/**
		terminate() 되고나면 NULL이 리턴 됨을 주의해야 한다.
	*/
	T pop() 
	{
		std::unique_lock<std::mutex> lock(mutex_);

		while (queue_.empty()) {
			if (is_terminated_) return NULL;
			condition_.wait(lock);
		}

		auto result = queue_.front();
		queue_.pop();
		return result;
	}

	bool pop(T &item)
	{
		item = NULL;

		std::unique_lock<std::mutex> lock(mutex_);

		while (queue_.empty()) {
			if (is_terminated_) return false;
			condition_.wait(lock);
		}

		item = queue_.front();
		queue_.pop();
		return item;
	}

	/**
		pop에서 무한정 기다리게 되어 종료 처리에서 문제가 될 때, 데이터가 없어도 강제로 깨운다.
	*/
	void terminate() 
	{
		is_terminated_ = true;
		condition_.notify_all();
	}
public:
	bool is_terminated() { return is_terminated_;  }
	
	bool is_empty() 
	{
		std::unique_lock<std::mutex> lock(mutex_);
		return queue_.empty();
	}

	int size() 
	{
		std::unique_lock<std::mutex> lock(mutex_);
		return queue_.size();
	}
};

#endif  // RYULIB_SUSPENSIONQUEUE_HPP