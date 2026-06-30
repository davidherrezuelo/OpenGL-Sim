#version 460 core

layout(local_size_x = 256) in;

struct Particle
{
	vec2 position;
	vec2 velocity;
};

layout(std430, binding = 0) buffer Particles
{
	Particle particles[];
};

uniform float dt;
uniform float gravity;
uniform uint particleCount;
uniform float damping;


void main(){
	uint id = gl_GlobalInvocationID.x;

	if(id < particleCount){
		particles[id].position.y += particles[id].velocity.y * dt + gravity * dt * dt * 0.5;
		particles[id].velocity.y += gravity * dt;

		if(particles[id].position.y <= -0.9)
			particles[id].velocity.y *= ((-1.0) * damping);
	}
}