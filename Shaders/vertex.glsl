#version 460 core

struct Particle
{
	vec2 position;
	vec2 velocity;
	vec2 acceleration;	
	float mass;
	float density;
	float pressure;
	float padding;
};

layout(std430, binding = 0) buffer Particles
{
	Particle particles[];
};

uniform float size;

void main(){
	Particle p = particles[gl_VertexID];

	gl_Position = vec4(p.position, 0.0, 1.0);
	gl_PointSize = size;
}