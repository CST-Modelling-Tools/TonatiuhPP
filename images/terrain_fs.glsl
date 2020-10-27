#version 150

uniform vec3 steps;
uniform ivec3 divs;

varying vec3 wPosition3;

// https://github.com/ayamflow/glsl-grid
float getIsoLine(float z, int major)
{
    float g = fract(z + 0.5) - 0.5;
    if (abs(g) < 0.4)  
        g = 1 - clamp(abs(g)/fwidth(g), 0, 1);
    else 
        g = 0.;
        
    float a = 0.05;
    int level = int(round(abs(z)));
    if (level%major == 0) a = 0.2;

    return g*a;
}

float getIsoLineT(float z, int major)
{
    float g = fract(z + 0.5) - 0.5;
    if (abs(g) < 0.4)  
        g = 1 - clamp(abs(g)/5/fwidth(g), 0, 1);
    else 
        g = 0.;
        
    float a = 0.05;
    int level = int(round(abs(z)));
    if (level%major == 0) a = 0.2;

    return g*a;
}

vec4 addAlpha(vec4 dst, vec4 src)
{
    vec4 ans;
    if (src.a == 0.) return dst;
    
    ans.a = dst.a*(1. - src.a) + src.a;
    ans.rgb = (dst.rgb*dst.a*(1. - src.a) + src.rgb*src.a)/ans.a;
    return ans;
}

void main(void) 
{

    vec4 cx = vec4(0., 0, 0, getIsoLine(wPosition3.x/steps.x, divs.x));
    vec4 cy = vec4(0., 0, 0, getIsoLine(wPosition3.y/steps.y, divs.y));
	
	float q = fract(wPosition3.z);
    //vec4 cz = vec4(0.9, 0.89, 0.82, q > 0.5 ? 1:0);
    //vec4 cz = vec4(0.9, 0.89, 0.82, 10.*getIsoLineT(wPosition3.z/1., 5));
    vec4 cz = vec4(1, 1, 1, 3*getIsoLineT(wPosition3.z/steps.z, divs.z));
    
    gl_FragColor = addAlpha(addAlpha(cx, cy), cz);
	
	/*
	float level = wPosition3.z - floor(wPosition3.z);
	if (level > 0.97)
		gl_FragColor = vec4(0, 0, 0, 0.03);
	else
		gl_FragColor = vec4(1., 1., 1., 0.);
		*/
}