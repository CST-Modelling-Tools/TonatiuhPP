#version 120

varying vec3 wPosition3;

void main(void)
{
	wPosition3 = gl_Vertex.xyz;
	
	gl_Position = ftransform();
}