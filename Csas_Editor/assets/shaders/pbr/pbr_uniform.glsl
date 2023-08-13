#ifndef PBR_UNIFORM_H
#define PBR_UNIFORM_H



// sampler binding points (texture units) 17-19 are reserved for PBR IBL
layout(binding = 17) uniform samplerCube irradiance_map;
layout(binding = 18) uniform samplerCube prefilter_map;
layout(binding = 19) uniform sampler2D BRDF_LUT;











#endif