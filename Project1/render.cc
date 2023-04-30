#include "render.h"
#include "utility.h"
#include "sphere.h"
#include <cmath>
#include <memory>
#include "raycast.h"
#include <vector>
#include <chrono>
#include<iostream>

#define POS(x,y) ((S->h-y-1)*S->w + x)
#define CAST_COLOR(a) (static_cast<int>(255.999 * a))

const int samples_per_pixel = 1000;
const int max_depth = 50;


inline uint32_t RGB(double r, double g, double b, double a) {
    return CAST_COLOR(a) << 24 | CAST_COLOR(b) << 16 | CAST_COLOR(g) << 8 | CAST_COLOR(r);
};

inline uint32_t vector_color(const Vec3& v)
{
	return RGB(v.x(), v.y(), v.z(), 1.0f);
}

inline Vec3 gamma_correct(const Vec3& v)
{
	return Vec3(sqrt(v.x()), sqrt(v.y()), sqrt(v.z()));
}


MultiRenderer::MultiRenderer(size_t num_threads, uint32_t* pix, State* state) : S(state), pixels(pix), running_threads(num_threads), sum_pixels(S->w,std::vector<Vec3>(S->h,Vec3(0,0,0))), samples(S->w,std::vector<int>(S->h,0))
{	
	for (size_t i = 0; i < num_threads; i++)
	{	
		std::thread thread([&,i]() {
			while (true)
			{	
				int cur_pixel;
				{	
					// Obtain lock
					std::unique_lock<std::mutex> lock(mtx);
					
					// Decrease running threads, waits and releases lock.
					running_threads -= 1;

					if (running_threads == 0) main_cv.notify_all(); // Notifies main_thread that all threads have stopped.
					worker_cv.wait(lock, [&]() {
						return !pause || stop;
					});


					// Ends thread.
					if (stop)
					{
						return;
					}
					running_threads += 1;

					// Grabs current pixel
					cur_pixel = queued_pixel++;
					queued_pixel %= (S->h * S->w);
				}

				// Render Pixel Stuff
				int x = cur_pixel % S->w;
				int y = cur_pixel / S->w;
				y = S->h - y - 1;

				for (int j = 0; j < samples_per_pixel; j++)
				{
					double u = (double(x) + random_double()) / (S->w - 1);
					double v = (double(y) + random_double()) / (S->h - 1);
					sum_pixels[x][y] += ray_cast(S->camera.get_ray(u, v), S, max_depth);
					samples[x][y] += 1;
				}

				pixels[cur_pixel] = vector_color(gamma_correct(sum_pixels[x][y]/samples[x][y]));
			}
		});

		threads.emplace_back(std::move(thread));
	}
	
	// Waits for threads to initialze.
	std::unique_lock<std::mutex> lock(mtx);
	main_cv.wait(lock, [&]() {return running_threads == 0; });
}

void MultiRenderer::render(int time)
{
	// Grabs the lock, and updates pause to false.
	{
		std::unique_lock<std::mutex> lock(mtx);
		pause = false;
	}
	// Wakes up all threads.
	worker_cv.notify_all();

	// Waits set amount of time.
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
	
	// Blocks main thread until everything else has stopped.
	std::unique_lock<std::mutex> lock(mtx);
	pause = true;
	worker_cv.notify_all();

	main_cv.wait(lock, [&]() { return running_threads == 0; });
}

MultiRenderer::~MultiRenderer()
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		stop = true;
	}
	worker_cv.notify_all();
	for (auto& thread : threads)
		thread.join();
}
