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


uniform uint particleCount;
uniform uint gridHeight;
uniform uint gridWidth;
uniform uint maxCellSize;
uniform float smoothingRadius;

ivec2 gridPos;
const float PI = 3.14159265359;
ivec2 cellsToCheck[9];


float poly6Kernel(float r, float h);

void main(){
	uint cellID = gl_GlobalInvocationID.x;

	if(cellID < particleCount)
	{
		gridPos.x = int(cellID%gridWidth);
		gridPos.y = int(cellID/gridWidth);

		for(int i = 0; i<=2; i++)
		{
			for(int j = 0; j<=2; j++)
			{
				cellsToCheck[i*3 + j].x = gridPos.x + j-1;
				cellsToCheck[i*3 + j].y = gridPos.y + i-1;
			}
		}

		uint count = counts[cellID];
		for (int i = 0; i < count; i++)
		{
			uint id = particleIDs[cellID + i];
			for(int j = 0; j < 9; j++)
			{
				uint otherCellCount
			}
		}

	}


}

float poly6Kernel(float r, float h)
{
    if (r >= h)
        return 0.0;

    float h2 = h * h;
    float diff = h2 - r * r;

    return (4.0 / (PI * pow(h, 8.0))) * diff * diff * diff;
}