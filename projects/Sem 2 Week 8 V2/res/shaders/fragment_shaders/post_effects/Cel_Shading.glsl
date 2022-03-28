#version 430

#include "../fragments/fs_common_inputs.glsl"
#include "../fragments/frame_uniforms.glsl"

// We output a single color to the color buffer
layout(location = 0) out vec4 albedo_specPower;
layout(location = 1) out vec4 normal_metallic;
layout(location = 2) out vec4 emissive;
layout(location = 3) out vec3 view_pos;

uniform float u_Strength;



void main() {
    
}
