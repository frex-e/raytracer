#pragma once

#include "camera.h"
#include "hittablelist.h"

class State
{
public:
	int h, w, i=0;
	Camera camera;
	HittableList objects;

	State(int width, int height) : h(height), w(width), camera(width, height, Vec3(0, -0.3, 0))
	{
	};
};
