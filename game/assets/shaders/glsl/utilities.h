#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//--------------------------------------------------------------------------------------

highp float4 pack_depth(const highp float depth)
{
	const highp float4 bit_shift = float4(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);
	const highp float4 bit_mask  = float4(0.0, 1.0/256.0, 1.0/256.0, 1.0/256.0);
	highp float4 res = fract(depth * bit_shift);
	res -= res.xxyz * bit_mask;
	return res;
}

highp float unpack_depth( const highp float4 rgba_depth )
{
	const highp float4 bit_shift = float4(1.0/(256.0*256.0*256.0), 1.0/(256.0*256.0), 1.0/256.0, 1.0);
	highp float depth = dot(rgba_depth, bit_shift);
	return depth;
}