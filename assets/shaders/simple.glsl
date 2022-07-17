#version 450

#define POSITION   0
#define COLOR      5
#define TEXTURE    4
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
vec2 fragColor;

#ifdef VERTEX_SHADER 
INPUT(POSITION) vec3 inPosition;
INPUT(TEXTURE)  vec2 inColor;

PUSH_CONST() uTransform {
    mat4 MVP;
} transform;

void main() {
    gl_Position = transform.MVP * vec4(inPosition, 1.0);
    fragColor = inColor;
}
#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
#extension GL_ARB_separate_shader_objects : enable
layout(binding = 0) uniform sampler2D texSampler;

OUTPUT(OUT_COLOR) vec4 outColor;

void main() {
    outColor = texture(texSampler, fragColor);
}
#endif // FRAGMENT_SHADER