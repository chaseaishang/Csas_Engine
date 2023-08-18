layout (std140, binding = 0) uniform Matrices
{
    mat4 View;
    mat4 Projection;
}Camera;
#include tools.glsl
//  0          1                     10
//camera    SpotLight           RenderInput