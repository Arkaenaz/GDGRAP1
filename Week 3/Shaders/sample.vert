#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 transform, projection;

void main() {
	//vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z);
	gl_Position = projection * transform * vec4(aPos, 1.0);
}