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

out vec3 eyeDirection;
out vec3 fromLightVector;
out vec4 clipTexProjCoord;

uniform mat4 projection;
uniform mat4 hudProjection;
uniform mat4 view;
uniform mat4 model;

uniform vec4 clippingPlane;
uniform bool enableClipping;

uniform vec3 eyeTanSpace;
uniform vec3 lightTanSpace;

const float tiling = 18.0;

void main()
{
    vs_out.FragPos = vec3(model * vec4(position, 1.0));
	
	if (enableClipping) {
		gl_ClipDistance[0] = dot(vec4(vs_out.FragPos, 1.0), clippingPlane);
	}
	

	if (renderType == RT_HUD) {
		gl_Position = hudProjection * model * vec4(position, 1.0f);
	}
	 else {
		gl_Position = projection * view * model * vec4(position, 1.0f);
	}
	
    if(renderType == RT_WATER) {
        vs_out.Normal = transpose(inverse(mat3(model))) * normal;
        vs_out.TexCoords = texCoords;	
		eyeDirection = eyeTanSpace - vs_out.FragPos; 

		vec4 clipPosition = projection * view * model * vec4(position, 1.0f);
		clipTexProjCoord = clipPosition;	

		TexCoords = vec2(vs_out.TexCoords[0]/2.0 + 0.5, vs_out.TexCoords[0]/2.0 + 0.5) * tiling;

		vec4 worldPosition = model * vec4(position, 1.0f);
		eyeDirection = eyeTanSpace - worldPosition.xyz;

		fromLightVector = worldPosition.xyz - lightTanSpace;


    } else {
        vs_out.Normal = transpose(inverse(mat3(model))) * normal;
        vs_out.TexCoords = texCoords;
    }

}