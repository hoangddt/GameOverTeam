#pragma once

#include <stdio.h>
#include <string.h>

#define FREE_2D_ARRAY(ptr, n) {if(ptr != NULL){for(unsigned int i = 0; i < n; i++){ delete ptr[i];} delete[] ptr; ptr = NULL;}}
#define FREE_1D_ARRAY(ptr) {if(ptr != NULL){delete[] ptr; ptr = NULL;}}
#define SAFE_FREE(ptr) {if(ptr != NULL){delete ptr; ptr = NULL;}}

#define MATH_GRAVITY 9.8f

#define DESIGN_SCREEN_WIDTH		480.0f
#define DESIGN_SCREEN_HEIGHT	800.0f

typedef unsigned int uint;

struct Rect
{
	float top;
	float left;
	float right;
	float bottom;
};

struct Point
{
	float x;
	float y;
};