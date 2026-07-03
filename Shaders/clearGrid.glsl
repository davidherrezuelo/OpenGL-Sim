#version 460 core

layout(local_size_x = 256) in;

layout(std430, binding = 2) buffer ParticleCount
{
	uint counts[];
};

uniform uint nCells;

uint id;

void main(){
	id = gl_GlobalInvocationID.x;
	if(id < nCells)
		counts[id] = 0;
}