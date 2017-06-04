#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

#define RT_MESH 0
#define RT_HUD 1
#define RT_WATER 2
uniform int renderType;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 hudProjection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	if (renderType == RT_HUD) {
		gl_Position = hudProjection * model * vec4(position, 1.0f);
	} else {
		gl_Position = projection * view * model * vec4(position, 1.0f);
	}
    vs_out.FragPos = vec3(model * vec4(position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoords = texCoords;
}