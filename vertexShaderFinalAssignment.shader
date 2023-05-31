#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNormal;

uniform mat4 world, view, projection;
uniform vec3 camera, cubeColor;

out vec2 uv;
out vec3 normal;
out vec4 fragPos;
out vec3 camPos;
out vec3 cubeLightColor;

void main() {
	fragPos = world * vec4(vPos, 1.0f);
	gl_Position = projection * view * world * vec4(vPos, 1.0f);
	uv = vUV;
	normal = mat3(world) * vNormal;
	camPos = camera;
	cubeLightColor = cubeColor;
}