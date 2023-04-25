#include "raycast.h"
#include "hittable.h"
#include "material.h"

Vec3 ray_cast(const Ray& ray, State* STATE, int depth)
{
	HitRecord rec;
	if (depth <= 0)
		return Vec3(0, 0, 0);

	if (STATE->objects.hit(ray,0.001,infinity,rec))
	{
		Ray scattered;
		Vec3 attenuation;
		if (rec.mat_ptr->scatter(ray, rec, attenuation, scattered))
			return attenuation * ray_cast(scattered, STATE, depth - 1);
		return Vec3(0, 0, 0);
	}

	double t;

	Vec3 unit_dir = unit_vector(ray.direction());

	t = 0.5 * (unit_dir.y() + 1.0);
	return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}
