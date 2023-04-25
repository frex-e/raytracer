#include "hittablelist.h"

bool HittableList::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
	HitRecord temp_rec;
	bool has_hit = false;
	double closest_so_far = t_max;

	for (const auto& object : objects)
	{
		// Pass closest so far as t_max as we don't need to consider intersections after.
		if (object->hit(ray, t_min, closest_so_far, temp_rec))
		{
			has_hit = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return has_hit;
}
