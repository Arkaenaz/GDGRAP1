#version 330 core

layout(location = 0) in vec3 aPos;

uniform float x;
uniform float y;

void main() {
	vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z);
	gl_Position = vec4(newPos, 1.0);
}