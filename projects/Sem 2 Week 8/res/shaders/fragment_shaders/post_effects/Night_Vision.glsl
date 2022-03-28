#version 430
#include "../../fragments/frame_uniforms.glsl"

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

uniform layout(binding = 0) sampler2D s_Image;
uniform layout(binding = 1) sampler3D s_Lut;

uniform float u_Strength;

float hash(in float n) { return fract(sin(n)*43758.5453123); }

void main() {

	vec2 position = inUV.xy / u_ScreenSize.xy;
	
	vec2 u = position * 2. - 1.;

	vec2 n = u * vec2(u_ScreenSize.x / u_ScreenSize.y, 1.0);

	vec3 c = texture(s_Image, position).xyz;
    
    
	// flicker, grain, vignette, fade in
	c += sin(hash(u_Time)) * 0.01;

	c += hash((hash(n.x) + n.y) * u_Time) * 0.5;
	c *= smoothstep(length(n * n * n * vec2(0.075, 0.4)), 1.0, 0.4);
    c *= smoothstep(0.001, 3.5, u_Time) * 1.5;
	
	c = dot(c, vec3(0.2126, 0.7152, 0.0722)) 
	  * vec3(0.2, 1.5 - hash(u_Time) * 0.1,0.4);
	
	outColor = vec4(c,1.0);

}
