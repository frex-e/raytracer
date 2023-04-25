#pragma once

#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>
#include "raycast.h"

class ThreadPool
{
public:
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	ThreadPool(size_t number_threads);
	virtual ~ThreadPool();

	template <class F, class ... Args>
	std::future<std::result_of_t<F(Args...)>> enqueue(F&& f, Args && ... args);

private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;

	std::mutex mtx;
	std::condition_variable cv;
	bool stop;
};
