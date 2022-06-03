#version 450

#define POSITION   0
#define COLOR      5
#define FRAG_COLOR 0
#define OUT_COLOR  0

#define INPUT(loc)   layout(location = loc) in
#define OUTPUT(loc)  layout(location = loc) out
#define PUSH_CONST() layout(push_constant)  uniform

#ifdef VERTEX_SHADER
OUTPUT(FRAG_COLOR)
#elif defined(FRAGMENT_SHADER) 
INPUT(FRAG_COLOR) 
#endif
vec3 fragColor;

#ifdef VERTEX_SHADER 
INPUT(POSITION) vec2 inPosition;
INPUT(COLOR)    vec3 inColor;

PUSH_CONST() uTransform {
    mat4 MVP;
} transform;

void main() {
    gl_Position = transform.MVP * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}
#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
#extension GL_ARB_separate_shader_objects : enable

OUTPUT(OUT_COLOR) vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
#endif // FRAGMENT_SHADER