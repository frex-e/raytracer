#pragma once

#include "utility.h"
#include "ray.h"

class Material;

struct HitRecord
{
	Vec3 point;
	Vec3 normal;
	double t;
	bool front_face;
	shared_ptr<Material> mat_ptr;


	inline void set_face_normal(const Ray& ray, const Vec3& outward_normal)
	{
		front_face = dot(ray.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -1*outward_normal;
	}
};

class Hittable
{
public:
	virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const = 0;
};
