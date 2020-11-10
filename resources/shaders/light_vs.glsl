#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 matrixProjectionViewModel;

uniform int stride;
out vec3 fColor;

void main(void)
{
	int nx = gl_InstanceID / stride;
	int ny = gl_InstanceID % stride;
	float sep = 2.;

	vec4 temp = position;
	temp.x += nx*sep;
	temp.y += ny*sep;

	gl_Position = matrixProjectionViewModel * temp;
	fColor = vec3(0.5, float(gl_InstanceID)/stride/stride, 0.);
}