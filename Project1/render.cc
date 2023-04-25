#include "render.h"
#include "utility.h"
#include "sphere.h"
#include <cmath>
#include <memory>
#include "raycast.h"

#define POS(x,y) ((S->h-y-1)*S->w + x)
#define CAST_COLOR(a) (static_cast<int>(255.999 * a))

const int samples_per_pixel =1000;
const int max_depth = 100;
const int num_threads = 6;


inline uint32_t RGB(double r, double g, double b, double a) {
    return CAST_COLOR(a) << 24 | CAST_COLOR(b) << 16 | CAST_COLOR(g) << 8 | CAST_COLOR(r);
};

inline uint32_t vector_color(const Vec3& v)
{
	return RGB(v.x(), v.y(), v.z(), 1.0f);
}


void render(uint32_t* pixels, State* S)
{

	// RENDERING
	int i = 100;
	while (i--)
	{
		int x = S->i % S->w;
		int y = S->i / S->w;
		S->i++;
		if (S->i >= S->h * S->w) S->i = 0;

		Vec3 current_color(0,0,0);

		for (int i = 0; i < samples_per_pixel; i++)
		{
			double u = (double(x) + random_double() ) / (S->w - 1);
			double v = (double(y) + random_double() ) / (S->h - 1);

			current_color += ray_cast(S->camera.get_ray(u, v),S,max_depth)/samples_per_pixel;
		}

		current_color = Vec3(
			sqrt(current_color.x()),
			sqrt(current_color.y()),
			sqrt(current_color.z())
		);

		pixels[POS(x,y)] = vector_color(current_color);
	}
}
