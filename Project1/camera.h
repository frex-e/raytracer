#pragma once

#include "utility.h"
#include "ray.h"

class Camera
{
private:
	Vec3 position;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;

	double viewport_width, viewport_height;
	double focal_length = 1.0;

public:
	Camera(double WIDTH, double HEIGHT, Vec3 pos)
	{
		double aspect_ratio = WIDTH / HEIGHT;
		viewport_height= 2.0;
		viewport_width = aspect_ratio * viewport_height;
		position = pos;

		horizontal = Vec3(viewport_width, 0, 0);
		vertical = Vec3(0.0, viewport_height, 0.0);
		lower_left_corner = position - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
	}

	Ray get_ray(double u, double v)
	{
		return Ray(position, lower_left_corner + u * horizontal + v * vertical - position);
	}


	void set_pos(Vec3 pos)
	{
		position = pos;
		horizontal = Vec3(viewport_width, 0, 0);
		vertical = Vec3(0.0, viewport_height, 0.0);
		lower_left_corner = position - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
	}
};
