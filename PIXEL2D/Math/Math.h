#pragma once
#include "..//Core/ValueTypes.h"
#include "MathTypes.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Rectangle.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

namespace PIXL { namespace math{

	inline int Clamp(int value, int min, int max)
	{
		return value > max ? max : (value < min ? min : value);
	}

	inline float Clamp(float value, float min, float max)
	{
		return value > max ? max : (value < min ? min : value);
	}

	inline float Abs(float value)
	{
		return value < 0.0f ? -value : value;
	}

	inline Vector2 Abs(Vector2 value)
	{
		return Vector2(Abs(value.x), Abs(value.y));
	}

	inline float ToRadians(float degrees)
	{
		return (float)(degrees * (M_PI / 180.0f));
	}

} }