#include "sphere.h"

bool Sphere::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
	Vec3 oc = ray.origin() - centre;
	double a = ray.direction().length_squared();
	double half_b = dot(oc, ray.direction());
	auto c = oc.length_squared() - radius * radius;

	auto disc = half_b * half_b - a * c;
	if (disc < 0) return false;

	auto sqrtd = sqrt(disc);

	// Finds the nearest root in range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root)
	{
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root) return false;
	}

	rec.t = root;
	rec.point = ray.at(root);
	Vec3 outward_normal = (rec.point - centre) / radius;
	rec.set_face_normal(ray, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
};
