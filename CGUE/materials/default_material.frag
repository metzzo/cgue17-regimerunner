#version 330 core
out vec4 FragColor;

#define RT_MESH 0
#define RT_HUD 1
#define RT_WATER 2
uniform int renderType;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;


in vec4 clipTexProjCoord;
in vec3 eyeDirection;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
uniform Material material;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define MAX_NR_DIR_LIGHTS 1
uniform DirLight dirLights[MAX_NR_DIR_LIGHTS];
uniform int numDirLights;

uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform sampler2D shadowMap3;
uniform sampler2D shadowMap4;
uniform sampler2D shadowMap5;
uniform sampler2D shadowMap6;
uniform sampler2D shadowMap7;
uniform sampler2D shadowMap8;
uniform sampler2D shadowMap9;
uniform sampler2D shadowMap10;
uniform sampler2D shadowMap11;
uniform sampler2D shadowMap12;
uniform sampler2D shadowMap13;
uniform sampler2D shadowMap14;
uniform sampler2D shadowMap15;

uniform sampler2D normalSampler;
uniform sampler2D displaceSampler;
uniform sampler2D reflectSampler;

uniform vec3 lightTanSpace;
uniform float waveOffset;
uniform float texOffset;

vec2 texProjCoord;
float dblDispTiling = 2.0;
float normalTiling = 2.0;

struct SpotLight {
	vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	bool shadowCasting;
	int shadowMapIndex;
	mat4 spaceMatrix;
};  
#define MAX_NR_SPOT_LIGHTS 50

uniform SpotLight spotLights[MAX_NR_SPOT_LIGHTS];
uniform int numSpotLights;
uniform vec3 viewPos;

const float waveStrength = 0.02;

#define SHADOW_MAP(A,B,C,X) \
	if (B == 0) { \
		X = A(shadowMap0, C); \
	} else if (B == 1) { \
		X = A(shadowMap1, C); \
	} else if (B == 2) { \
		X = A(shadowMap2, C); \
	} else if (B == 3) { \
		X = A(shadowMap3, C); \
	} else if (B == 4) { \
		X = A(shadowMap4, C); \
	} else if (B == 5) { \
		X = A(shadowMap5, C); \
	} else if (B == 6) { \
		X = A(shadowMap6, C); \
	} else if (B == 7) { \
		X = A(shadowMap7, C); \
	} else if (B == 8) { \
		X = A(shadowMap8, C); \
	} else if (B == 9) { \
		X = A(shadowMap9, C); \
	} else if (B == 10) { \
		X = A(shadowMap10, C); \
	} else if (B == 11) { \
		X = A(shadowMap11, C); \
	} else if (B == 12) { \
		X = A(shadowMap12, C); \
	} else if (B == 13) { \
		X = A(shadowMap13, C); \
	} else if (B == 14) { \
		X = A(shadowMap14, C); \
	} else if (B == 15) { \
		X = A(shadowMap15, C); \
	}

float ShadowCalculation(SpotLight light, vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	
	vec4 shadowMapColor;
	SHADOW_MAP(texture, light.shadowMapIndex, projCoords.xy, shadowMapColor);
	
    float closestDepth = shadowMapColor.r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF
    float shadow = 0.0;
	
	vec2 shadowMapSize;
	SHADOW_MAP(textureSize, light.shadowMapIndex, 0, shadowMapSize);
	
	
    vec2 texelSize = 1.0 / shadowMapSize;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
			SHADOW_MAP(texture, light.shadowMapIndex, projCoords.xy + vec2(x, y) * texelSize, shadowMapColor);
            float pcfDepth = shadowMapColor.r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
	float shadow = light.shadowCasting ? ShadowCalculation(light, light.spaceMatrix * vec4(fs_in.FragPos, 1.0)) : 0.0;
	
    return ambient + (1.0 - shadow)*(diffuse + specular); // + shadow*vec3(0,1,0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));    
    return (ambient + diffuse + specular);
}

vec3 renderMesh() {
    vec3 result = vec3(0,0,0);
	
	vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	
	for (int i = 0; i < numDirLights; i++) 
	{
		result += CalcDirLight(dirLights[i], norm, viewDir);
	}
	
    for(int i = 0; i < numSpotLights; i++) 
	{
        result += CalcSpotLight(spotLights[i], norm, fs_in.FragPos, viewDir); 
	}
	
	return result;
}

vec3 renderHud() {
	/*float depthValue = texture(material.diffuse, fs_in.TexCoords).r;
	float near_plane = 1.0;
	float far_plane = 1000.0;
	float z = depthValue * 2.0 - 1.0;
	return vec3(vec3(((2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane))) / far_plane));*/
	
	return vec3(texture(material.diffuse, fs_in.TexCoords));
}

vec2 GetMapCoord(in vec2 offset, in float scale)
{
	return vec2(fract(fs_in.FragPos[0] * scale + offset.x), fract(fs_in.FragPos[2] * scale + offset.y));
}

float GetWaterDepth(in vec2 displacedCoord)
{
	float waterDepth = ((1.0/(1.0 - texture2D(shadowMap0, displacedCoord).r)) - (1.0/(1.0 - texture2D(shadowMap0, displacedCoord).r))) * 0.1;
	return clamp(waterDepth, 0.001, 0.999);
}

vec3 renderWater() {

	vec2 ndc = (clipTexProjCoord.xy/clipTexProjCoord.w)/2.0 + 0.5;
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	vec2 distortion = (texture(displaceSampler, vec2(TexCoords.x + waveOffset,TexCoords.y)).rg * 2.0 - 1.0) * sin(waveStrength);
	reflectTexCoords += distortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 reflectColor = texture(reflectSampler, reflectTexCoords);

	return vec3(reflectColor);
}

void main()
{    
    // properties    
    vec3 result;
	
	switch(renderType) {
		case RT_MESH:
			result = renderMesh();
			break;
		case RT_HUD:
			result = renderHud();
			break;
		case RT_WATER:
			result = renderWater();
			break;
		default:
			result = vec3(0,1,0);
	}
    
    FragColor = vec4(result, 1.0);
}
