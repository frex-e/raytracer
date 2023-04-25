#pragma once

#include "material.h"
#include "ray.h"
#include "hittable.h"

class Lambertian : public Material
{
public:
	Vec3 albedo;

	Lambertian(const Vec3& a) : albedo(a) { }

	virtual bool scatter(
		const Ray& ray_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered
	) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();
		
		// Catch Zero case
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.point, scatter_direction);
		attenuation = albedo;
		return true;
	}
};
