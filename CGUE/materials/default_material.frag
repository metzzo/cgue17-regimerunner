#version 330 core
out vec4 FragColor;

#define RT_MESH 0
#define RT_HUD 1
#define RT_WATER 2
#define RT_SKYBOX 3
uniform int renderType;
uniform float blood;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;


in vec4 clipTexProjCoord;
in vec3 eyeDirection;
in vec3 fromLightVector;
in vec2 TexCoords;
in float visible;

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

uniform sampler2D normalSampler;
uniform sampler2D displaceSampler;
uniform sampler2D reflectSampler;
uniform sampler2D refractSampler;

uniform vec3 lightTanSpace;
uniform float waveOffset;
uniform float texOffset;

vec2 texProjCoord;
float dblDispTiling = 50.0;
float normalTiling = 50.0;

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
#define MAX_NR_SPOT_LIGHTS 10

uniform SpotLight spotLights[MAX_NR_SPOT_LIGHTS];
uniform int numSpotLights;
uniform vec3 viewPos;

const float waveStrength = 0.02;
const float shineDamper = 20.0;
const float reflectivity = 0.08;
const vec3 sky = vec3(0.11,0.11,0.11);

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
	
    //float closestDepth = shadowMapColor.r; 
    
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
	
    return ambient + (1.0 - shadow)*(diffuse + specular); // + shadow*vec3(0,0,0);
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


vec3 renderWater() {

	vec2 ndc = (clipTexProjCoord.xy/clipTexProjCoord.w)/2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	vec2 distortedTexCoords = texture(displaceSampler, vec2(fs_in.TexCoords.x + waveOffset, fs_in.TexCoords.y)).rg*0.1;
	distortedTexCoords += TexCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+waveOffset);
	vec2 distortion = (texture(displaceSampler, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;
	reflectTexCoords += distortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	refractTexCoords += distortion;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

	vec4 reflectColor = texture(reflectSampler, reflectTexCoords);
	vec4 refractColor = texture(refractSampler, refractTexCoords);

	vec3 viewVector = normalize(eyeDirection);
	float transparency = dot(viewVector, vec3(0.0,1.0,0.0));
	transparency = pow(transparency, 4.0);

	vec4 normalMap = texture(normalSampler, distortedTexCoords);
	vec3  normal = vec3(normalMap.r * 2.0 - 1.0, normalMap.b, normalMap.g * 2.0 - 1.0);
    normal = normalize(normal);

	vec3 result = vec3(0.0,0.0,0.0);

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	for (int i = 0; i < numDirLights; i++) 
	{
		vec3 lightDir = normalize(-dirLights[i].direction);
		// diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
		// specular shading
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shineDamper);
		// combine results
		vec3 specular = dirLights[i].specular * (spec + 0.1);    
		result += specular;
	}

	for(int i = 0; i < numSpotLights; i++) 
	{
		vec3 lightDir = normalize(spotLights[i].position - fs_in.FragPos);
		// specular shading
		vec3 reflectDir = reflect(viewDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shineDamper);
		// attenuation
		float distance = length(spotLights[i].position - fs_in.FragPos);
		float attenuation = 1.0f / (spotLights[i].constant + spotLights[i].linear * distance + spotLights[i].quadratic * (distance * distance));    
		// spotlight intensity
		float theta = dot(lightDir, normalize(-spotLights[i].direction)); 
		float epsilon = spotLights[i].cutOff - spotLights[i].outerCutOff;
		float intensity = clamp((theta - spotLights[i].outerCutOff) / epsilon, 0.0, 1.0);
		// combine results
		vec3 specular = spotLights[i].specular * spec;
		specular *= attenuation * intensity;
		float shadow = spotLights[i].shadowCasting ? ShadowCalculation(spotLights[i], spotLights[i].spaceMatrix * vec4(fs_in.FragPos, 1.0)) : 0.0;
	
		result += (1.0 - shadow)*(0 + specular); 
	}


	vec4 out_color = mix(reflectColor, refractColor, transparency);

	out_color = mix(out_color, vec4(0.0,0.3,0.5,1.0),0.2);

	vec3 outgoing = vec3(out_color) + result;
	return outgoing;
}

vec3 renderSkyBox() {

	vec3 ambient = vec3(texture(material.diffuse, fs_in.TexCoords));
	vec3 diffuse = vec3(texture(material.diffuse, fs_in.TexCoords));
	return ambient + diffuse;
}

void main()
{    
    // properties    
    vec3 result;
	
	switch(renderType) {
		case RT_MESH:
			result = renderMesh();
			result = mix(sky,result,visible);
			result = mix(result,vec3(0.047, 0.2, 0.352),0.4);
			break;
		case RT_HUD:
			result = renderHud();
			break;
		case RT_WATER:
			result = renderWater();
			result = mix(sky,result,visible*5);
			result = mix(result,vec3(0.047, 0.2, 0.352),0.7);
			break;
		case RT_SKYBOX:
			result = renderSkyBox();
			result = mix(sky,result,visible*2);
			result = mix(result,vec3(0.047, 0.2, 0.352),0.4);
			break;
		default:
			result = vec3(0,1,0);
	}
	
	result.x = min(result.x + blood, 1.0);
	result.y = max(0.0, result.y - blood);
	result.z = max(0.0, result.z - blood);
    
    FragColor = vec4(result, 1.0);
}
