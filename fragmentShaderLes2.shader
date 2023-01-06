#version 330 core

out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec4 worldPixel; // FragPos

struct PointLight {
	vec3 position;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Material {
	sampler2D diffuseSample;
	sampler2D specular;
	float     shininess;
};

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform PointLight pointLight;
uniform Material material;

vec3 CalcPointLight(PointLight _light, vec3 _normal, vec3 _fragPos, vec3 _viewDir)
{
	vec3 lightDir = normalize(_light.position - _fragPos);
	// Diffuse Shading
	float diff = max(dot(_normal, -lightDir), 0.0f);
	// Specular Shading
	vec3 reflectDir = reflect(-lightDir, _normal);
	float spec = pow(max(dot(reflectDir, _viewDir), 0.0f), material.shininess);
	// Attenuation
	float distance = length(_light.position - _fragPos);
	float attenuation = 1.0 / (_light.constant + _light.linear * distance + _light.quadratic * (distance * distance));
	// Combined Results
	vec3 ambient = _light.ambientColor * vec3(texture(material.diffuseSample, uv));
	vec3 diffuse = _light.diffuseColor * diff * vec3(texture(material.diffuseSample, uv));
	vec3 specular = _light.specular * spec * vec3(texture(material.specular, uv));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}

void main() {
	vec3 camPos = vec3(0, 3, -3);
	vec3 viewDir = normalize(worldPixel.xyz - camPos);

	vec3 result = vec3(0.0f);

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], normal, worldPixel.xyz, viewDir);
	}

	FragColor = vec4(result, 1.0f);

	//vec3 lightDir = normalize(pointLight.position);
	//float diff = max(dot(normal, -lightDir), 0.0f);
	//vec3 camPos = vec3(0, 3, -3);
	//vec3 viewDir = normalize(worldPixel.xyz - camPos);
	//vec3 reflectDir = normalize(reflect(-lightDir, normal));
	//float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);

	//vec3 ambient = pointLight.ambientColor * vec3(texture(material.diffuseSample, uv));
	//vec3 diffuse = pointLight.diffuseColor * diff * vec3(texture(material.diffuseSample, uv));
	//vec3 specular = pointLight.specular * spec * vec3(texture(material.specular, uv));

	//FragColor = vec4(ambient + diffuse + specular, 1.0f);
}