#ifndef RYU_WORKER_HPP
#define RYU_WORKER_HPP


#include <ryulib/SimpleThread.hpp>
#include <ryulib/ThreadQueue.hpp>

using namespace std;

class TaskOfWorker
{
public:
	int task;
	void* data;
	int size;
	int tag;

	TaskOfWorker(int t, void* d, int s, int g) {
		task = t;
		data = d;
		size = s;
		tag = g;
	}
};

typedef function<void(int, void*, int, int)> TaskEvent;
typedef function<void()> RepeatEvent;

class Worker {
public:
	Worker() {
		thread_ = new SimpleThread(on_thread_execute);
	}

	~Worker() {
		stop();
		delete thread_;
	}

	void terminateAndWait()
	{
		stop();
		thread_->terminateAndWait();
	}

	void terminateNow()
	{
		stop();
		thread_->terminateNow();
	}

	void start() {
		started_ = true;
	}

	void stop() {
		started_ = false;
	}
	
	void add(int task) {
		TaskOfWorker* t = new TaskOfWorker(task, nullptr, 0, 0);
		queue_.push(t);
	}

	void add(int task, void* data) {
		TaskOfWorker* t = new TaskOfWorker(task, data, 0, 0);
		queue_.push(t);
	}

	void add(int task, void* data, int size, int tag) {
		TaskOfWorker* t = new TaskOfWorker(task, data, size, tag);
		queue_.push(t);
	}

	void sleep(int millis)
	{
		thread_->sleep(millis);
	}

	void setOnTask(const TaskEvent& value) { on_task_ = value; }
	void setOnRepeat(const RepeatEvent& value) { on_repeat_ = value; }

private:
	bool started_ = false;
	ThreadQueue<TaskOfWorker*> queue_;
	SimpleThread* thread_;
	SimpleThreadEvent on_thread_execute = [&](SimpleThread * simpleThread) {
		while (simpleThread->isTerminated() == false) {
			TaskOfWorker* t = queue_.pop();
			if (t != NULL) {
				if (on_task_ != nullptr) on_task_(t->task, t->data, t->size, t->tag);
				delete t;
			}

			if (started_) {
				if (on_repeat_ != nullptr) {
					on_repeat_();
				} else {
					thread_->sleep(1);
				}
			}
		}
	};

	TaskEvent on_task_ = nullptr;
	RepeatEvent on_repeat_ = nullptr;
};


#endif  // RYU_WORKER_HPP