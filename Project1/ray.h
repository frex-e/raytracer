#pragma once

#include "vec3.h"

class Ray
{
	Vec3 orig, dir;
public:

	Ray() { }
	Ray(const Vec3& origin, const Vec3& direction) : orig(origin), dir(direction) { }

	Vec3 origin() const
	{
		return orig;
	}

	Vec3 direction() const
	{
		return dir;
	}

	Vec3 at(double t) const
	{
		return orig + t * dir;
	}

};
