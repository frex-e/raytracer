#pragma once

#include "utility.h"
#include "vec3.h"
#include "ray.h"

struct HitRecord;

class Material
{
public:
	virtual bool scatter(
		const Ray& ray, const HitRecord& rec, Vec3& attenuation, Ray& scattered
	) const = 0;
};
