#ifndef _EXT_H
#define _EXT_H


////////////////////////////////////////////////////////////////////////////////

#define EPS      1e-5
#define PI       3.141592653589793
#define PI2      6.283185307179586
#define INV_PI   0.318309886183791  // 1 over PI
#define HLF_PI   1.570796326794897  // half PI
#define SQRT2    1.414213562373095
#define SQRT3    1.732050807568877
#define SQRT5    2.236067977499789
#define CBRT2    1.259921049894873  // cube root 2
#define CBRT3    1.442249570307408  // cube root 3
#define G_PHI    1.618033988749894  // golden ratio
#define EULER_E  2.718281828459045  // natural exponent e
#define LN2      0.693147180559945
#define LN10     2.302585092994046
#define INV_LN2  1.442695040888963  // 1 over ln2
#define INV_LN10 0.434294481903252  // 1 over ln10

#define clamp01(x) clamp(x, 0.0, 1.0)
////////////////////////////////////////////////////////////////////////////////

// computes the min/max component of a vec2/vec3/vec4
float min2(const vec2 v) { return min(v.x, v.y); }
float max2(const vec2 v) { return max(v.x, v.y); }
float min3(const vec3 v) { return min(min(v.x, v.y), v.z); }
float max3(const vec3 v) { return max(max(v.x, v.y), v.z); }
float min4(const vec4 v) { return min(min3(v.xyz), v.w); }
float max4(const vec4 v) { return max(max3(v.xyz), v.w); }

// logarithm base 10 and logarithm base 2
float log10(float x) { return log(x) * INV_LN10; }
float log2(float x) { return log(x) * INV_LN2; }

// checks if value x is in range (a, b), returns value type to avoid branching
float step3(float a, float x, float b) { return step(a, x) * step(x, b); }
vec2 step3(const vec2 a, const vec2 x, const vec2 b) { return step(a, x) - step(b, x); }
vec3 step3(const vec3 a, const vec3 x, const vec3 b) { return step(a, x) - step(b, x); }
vec4 step3(const vec4 a, const vec4 x, const vec4 b) { return step(a, x) - step(b, x); }

// optimizes lower power functions, removes the implicit `exp/log` call
float pow2(float x) { return x * x; }
float pow3(float x) { return x * x * x; }
float pow4(float x) { return x * x * x * x; }
float pow5(float x) { return x * x * x * x * x; }

#endif