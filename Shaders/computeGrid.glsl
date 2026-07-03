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

layout(std430, binding = 1) buffer ParticleIDs
{
	uint particleIDs[];
};

layout(std430, binding = 2) buffer ParticleCount
{
	uint counts[];
};

uniform uint gridHeight;
uniform uint gridWidth;
uniform uint maxCellSize;
uniform uint particleCount;

ivec2 gridPos;

void main(){
	uint id = gl_GlobalInvocationID.x;

    if (id >= particleCount)
        return;

    ivec2 gridPos;

    gridPos.x = clamp(
        int((particles[id].position.x + 1.0) * 0.5 * float(gridWidth)),
        0,
        int(gridWidth) - 1);

    gridPos.y = clamp(
        int((particles[id].position.y + 1.0) * 0.5 * float(gridHeight)),
        0,
        int(gridHeight) - 1);

    uint gridID = uint(gridPos.y) * gridWidth + uint(gridPos.x);

    uint index = atomicAdd(counts[gridID], 1);

    if (index < maxCellSize)
    {
        particleIDs[gridID * maxCellSize + index] = id;
    }

}