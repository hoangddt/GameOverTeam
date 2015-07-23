#pragma once

#include <stdio.h>
#include <string.h>

#define FREE_2D_ARRAY(ptr, n) {if(ptr != NULL){for(unsigned int i = 0; i < n; i++){ delete ptr[i];} delete[] ptr; ptr = NULL;}}
#define FREE_1D_ARRAY(ptr) {if(ptr != NULL){delete[] ptr; ptr = NULL;}}
#define SAFE_FREE(ptr) {if(ptr != NULL){delete ptr; ptr = NULL;}}

typedef unsigned int uint;