#include "vec3.h"

Vec3 random_in_unit_sphere()
{
	while(true)
	{
		Vec3 p = Vec3::random(-1, 1);
		if (p.length_squared() < 1) return p;
	}
}

Vec3 random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

Vec3 random_in_hemisphere(const Vec3& normal)
{
	Vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(normal, in_unit_sphere) > 0.0)
		return in_unit_sphere;
	else
		return -1*in_unit_sphere;
}
