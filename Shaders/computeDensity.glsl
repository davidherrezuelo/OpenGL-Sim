#version 460 core

layout(local_size_x = 256) in;

struct Particle
{
	vec2 position;
	vec2 velocity;
	float mass;
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
uniform float kernelRadius;

uvec2 gridPos;
const float PI = 3.14159265359;
vec2 cellsToCheck[9];

void main(){
	uint id = gl_GlobalInvocationID.x;

	if(id < particleCount)
	{
		gridPos.x = id%gridWidth;
		gridPos.y = id/gridWidth;

		for(int i = 0; i<=2; i++){
			for(int j = 0; j<=2; j++){
				cellsToCheck[i*3 + j].x = gridPos.x + j-1;
				cellsToCheck[i*3 + j].y = gridPos.y + i-1;
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