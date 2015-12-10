#pragma once
#ifndef ENGINE_CORE_TYPES_H
#define ENGINE_CORE_TYPES_H

#include <glm\glm.hpp>

// Signed-bits
typedef char int8;
typedef short int16;
typedef int int32;
typedef long int64;

// Unsigned-bits
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

// Unsigned-Named
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

/*
Defines the type of the index buffer.
*/
typedef unsigned int index_buffer_t;

/*
Defines the OGL type of the index buffer.
*/
#define OGL_INDEX_T GL_UNSIGNED_INT

/*
Defines the vertex data type.
*/
typedef float vertex_buffer_t;

#endif
