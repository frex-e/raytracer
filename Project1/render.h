#pragma once

#include <cstdint>
#include "vec3.h"
#include "hittablelist.h"
#include "camera.h"
#include "state.h"
#include <functional>
#include <future>
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <vector>


void render(uint32_t* pixels, State* S);

class MultiRenderer
{
public:
	MultiRenderer(const MultiRenderer&) = delete;
	MultiRenderer(MultiRenderer&&) = delete;
	MultiRenderer& operator=(const MultiRenderer&) = delete;
	MultiRenderer& operator=(MultiRenderer&&) = delete;

	MultiRenderer(size_t num_threads, uint32_t* pix, State* state);
	~MultiRenderer();

	void render(int time);
private:
	State* S;
	uint32_t* pixels;

	int queued_pixel = 0;
	int running_threads = 0;

	std::vector<std::thread> threads;
	std::mutex mtx;
	std::condition_variable worker_cv;
	std::condition_variable main_cv;

	std::vector<std::vector < Vec3 >> sum_pixels;
	std::vector<std::vector< int>> samples;

	bool pause = true;
	bool stop = false;
	
};
