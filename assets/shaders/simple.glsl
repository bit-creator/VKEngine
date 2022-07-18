#version 450

/**
 * Macro definitions
 *
 */

#define INPUT(loc)   layout(location = loc) in
#define OUTPUT(loc)  layout(location = loc) out
#define PUSH_CONST() layout(push_constant)  uniform
#define UNIFORM(loc) layout(binding = loc)  uniform

#ifdef VERTEX_SHADER
#define VERTEX_OUT_FRAGMENT_IN(loc) OUTPUT(loc)
#elif defined(FRAGMENT_SHADER)
#define VERTEX_OUT_FRAGMENT_IN(loc) INPUT(loc)
#endif

/**
 * Shader common data
 *
 */

VERTEX_OUT_FRAGMENT_IN(TEXTURE) vec2 ioTexCoord;


/**
 * Vertex shader
 *
 */

#ifdef VERTEX_SHADER
INPUT(POSITION) vec3 inPosition;
INPUT(TEXTURE)  vec2 inTexture;

PUSH_CONST() uTransform {
    mat4 MVP;
} transform;

void main() {
    gl_Position = transform.MVP * vec4(inPosition, 1.0);
    ioTexCoord = inTexture;
}
#endif // VERTEX_SHADER


/**
 * Fragment shader 
 *
 */

#ifdef FRAGMENT_SHADER
UNIFORM(ALBEDO) sampler2D uAlbedo;

OUTPUT(OUT_COLOR) vec4 outColor;

void main() {
    outColor = texture(uAlbedo, ioTexCoord);
}
#endif // FRAGMENT_SHADER