#version 460 core

layout(local_size_x = 256) in;

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

layout(std430, binding = 1) buffer ParticleIDs
{
	uint particleIDs[];
};

layout(std430, binding = 2) buffer ParticleCount
{
	uint counts[];
};

uniform float dt;
uniform float gravity;
uniform uint particleCount;
uniform float damping;

uniform uint gridHeight;
uniform uint gridWidth;
uniform uint maxCellSize;

void main(){
	uint id = gl_GlobalInvocationID.x;

	if(id < particleCount)
	{
		particles[id].position.y += particles[id].velocity.y * dt + gravity * dt * dt * 0.5;
		particles[id].velocity.y += gravity * dt;
		
		if (particles[id].position.y <= -0.9 && particles[id].velocity.y != 0.0)
		{
			particles[id].position.y = -0.9;

			if(abs(particles[id].velocity.y)<0.5)
				particles[id].velocity.y=0.0;
			
			else if (particles[id].velocity.y < 0)
				particles[id].velocity.y *= -damping;
		}
	}
}