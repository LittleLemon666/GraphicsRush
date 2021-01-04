#version 430 core
out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 center;
} f_in;

uniform vec3 u_color;

uniform float time;

uniform float seed;

float distance3( in vec3 a, in vec3 b ) { return dot(a-b,a-b) * 50.0; }

float rand(int val){
	return cos(val*sin(val*seed)*seed);	
}

uniform mat3 rrz = mat3( cos(1.0), -sin(1.0), 0.0,
	sin(1.0), cos(1.0), 0.0,
	0.0, 0.0, 1.0 );
uniform mat3 rrx = mat3( 1.0, 0.0, 0.0,
	0.0, cos(1.0), -sin(1.0),
	0.0, sin(1.0), cos(1.0));

vec3 drawParticles(float t, float gravity, float timelength){
    vec3 color= vec3(0.0);
    vec3 pp = vec3(1.0, 0.0, 0.0);
    for(int i = 1; i <= 128; i++){
        float d = rand(i);
        float fade = (i/128.0) * t;
        vec3 particpos = f_in.center + t * pp * d;
        pp = rrz*pp;
		color = particpos;
        //color = mix(u_color/fade, color, smoothstep(0.0, 0.1, distance3(f_in.center, f_in.position)));
    }
    //color*=smoothstep(0.0,1.0,(timelength-time)/timelength/0.05);
    return color;
}

// range [0, 50.0) for up, range [50.0, 100.0) for bomb
vec3 drawFireworks(){
	
	float timeoffset = 10.0;
	vec3 color = vec3(0.0);
	if (time <= 0.){
		return color;	
	}
	if (time < timeoffset)
	{
		color = u_color;
	}
	else
	{
		color = drawParticles(time -timeoffset, 0.98, ceil(time/6.0));
	}
	return color;	
}

void main()
{   
    vec3 color = drawFireworks();
	//if (color == vec3(0.0)) discard;
    f_color = vec4(color, 1.0f);
}