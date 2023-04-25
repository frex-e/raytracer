#pragma once

#include <cstdint>
#include "vec3.h"
#include "hittablelist.h"
#include "camera.h"
#include "state.h"


void render(uint32_t* pixels, State* S);
