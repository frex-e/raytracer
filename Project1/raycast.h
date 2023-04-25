#pragma once

#include "utility.h"
#include "state.h"

Vec3 ray_cast(const Ray& ray, State* STATE, int depth);
