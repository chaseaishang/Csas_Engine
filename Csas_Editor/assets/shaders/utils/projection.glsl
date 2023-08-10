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

// convert a vector v in Cartesian coordinates to spherical coordinates
vec2 Cartesian2Spherical(vec3 v) {
    float phi = atan(v.z, v.x);          // ~ [-PI, PI] (assume v is normalized)
    float theta = acos(v.y);             // ~ [0, PI]
    return vec2(phi / PI2, theta / PI);  // ~ [-0.5, 0.5], [0, 1]
}

// remap a spherical vector v into equirectangle texture coordinates
vec2 Spherical2Equirect(vec2 v) {
    return vec2(v.x + 0.5, v.y);  // ~ [0, 1]
}

// convert a vector v in spherical polar angles to Cartesian coordinates
vec3 Spherical2Cartesian(vec2 v) {
    float z = sin(v.y);
    return vec3(z * cos(v.x), z * sin(v.x), cos(v.y));
}


// convert a 2D texture coordinate st on a cubemap face to its equivalent 3D
// texture lookup vector v such that `texture(cubemap, v) == texture(face, st)`
vec3 UV2Cartesian(vec2 st, uint face) {
    vec3 v = vec3(0.0);  // texture lookup vector in world space
    vec2 uv = 2.0 * vec2(st.x, 1.0 - st.y) - 1.0;  // convert [0, 1] to [-1, 1] and invert y

    // https://en.wikipedia.org/wiki/Cube_mapping#Memory_addressing
    switch (face) {
        case 0: v = vec3( +1.0,  uv.y, -uv.x); break;  // posx
        case 1: v = vec3( -1.0,  uv.y,  uv.x); break;  // negx
        case 2: v = vec3( uv.x,  +1.0, -uv.y); break;  // posy
        case 3: v = vec3( uv.x,  -1.0,  uv.y); break;  // negy
        case 4: v = vec3( uv.x,  uv.y,  +1.0); break;  // posz
        case 5: v = vec3(-uv.x,  uv.y,  -1.0); break;  // negz
    }

    return normalize(v);
}


// convert an ILS image coordinate w to its equivalent 3D texture lookup
// vector v such that `texture(samplerCube, v) == imageLoad(imageCube, w)`
vec3 ILS2Cartesian(ivec3 w, vec2 resolution) {
    // w often comes from a compute shader in the form of `gl_GlobalInvocationID`
    vec2 st = w.xy / resolution;  // tex coordinates in [0, 1] range
    return UV2Cartesian(st, w.z);
}
