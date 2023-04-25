#pragma once

#include "hittable.h"
#include "material.h"
#include "vec3.h"

class Sphere : public Hittable
{
public:
	Vec3 centre;
	double radius;
	shared_ptr<Material> mat_ptr;

	Sphere() { }
	Sphere(Vec3 cen, double rad, shared_ptr<Material> mat) : centre(cen), radius(rad), mat_ptr(mat)
	{
	};

	virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
};

