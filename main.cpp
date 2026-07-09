#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#include "Shader.h"
#include "Input.h"
#include "Draw.h"

#define WIDTH 800
#define HEIGHT 600
#define MAX_PARTICLES 32

GLFWwindow* window;

float dampingFactor = 0.67;
float radius = 15.0f;
unsigned int nPart = 5;

const int cellSize = 5;
glm::vec2 gridDims = glm::vec2(WIDTH / cellSize, HEIGHT / cellSize);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;

GLuint VAO;
GLuint VertexSSBO;
GLuint PartcileIdSSBO;
GLuint CountsSSBO;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void bindSsboAndData(GLuint SSBO, GLuint binding, unsigned int size, const void* data);
int initGLFW();
int initGLAD();

class Random {
public:
	static std::mt19937& engine() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return gen;
	}
};

struct Particle
{
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;	
	float mass;
	float density;
	float pressure;
	float padding;
};

std::vector<Particle> particles;
std::vector<int> particleID;
std::vector<unsigned int> particleCount;

int main() {
	//inicializamos glfw y glad
	if (initGLFW()) return -1;

	if (initGLAD()) return -1;

	const char* vertexShaderPath = "shaders/vertex.glsl";
	const char* fragmentShaderPath = "shaders/fragment.glsl";
	const char* computeShaderPath = "shaders/compute.glsl";
	const char* computeGridShaderPath = "shaders/computeGrid.glsl";
	const char* clearGridShaderPath = "shaders/clearGrid.glsl";
	
	Shader myShader = Shader(vertexShaderPath, fragmentShaderPath);
	myShader.use();

	cShader clearGrid = cShader(clearGridShaderPath);
	clearGrid.use();
	clearGrid.setUint("nCells", gridDims.x * gridDims.y);

	cShader computeGrid = cShader(computeGridShaderPath);
	computeGrid.use();
	computeGrid.setUint("gridWidth", gridDims.x);
	computeGrid.setUint("gridHeigt", gridDims.y);
	computeGrid.setUint("maxCellSize", MAX_PARTICLES);
	computeGrid.setUint("particleCount", nPart);

	cShader computeShader = cShader(computeShaderPath);
	computeShader.use();
	computeShader.setUint("gridWidth", gridDims.x);
	computeShader.setUint("gridHeigt", gridDims.y);
	computeShader.setUint("maxCellSize", MAX_PARTICLES);
	computeShader.setUint("particleCount", nPart);

	//reserva espacio
	particles.resize(nPart);
	particleID.resize(gridDims.x * gridDims.y * MAX_PARTICLES);
	particleCount.resize(gridDims.x * gridDims.y);

	//creo generador de n aleatorios
	std::uniform_real_distribution<float> dist(-0.7f, 0.7f);

	//inicializar particulas
	for (int i = 0; i < nPart; i++) {
		particles[i].position.x = dist(Random::engine());
		particles[i].position.y = dist(Random::engine());
		particles[i].velocity = glm::vec2(0.0f);
		particles[i].mass = 0.01;	
	}

	//inicializar celdas grid
	int index;
	for (int i = 0; i < gridDims.y; i++) {
		for (int j = 0; j < gridDims.x; j++) {
			index = i * gridDims.x + j;
			for (int k = 0; k < MAX_PARTICLES; k++) {
				particleID[index * MAX_PARTICLES + k] = -1;
			}
			particleCount[index] = 0;
		}
	}

	std::cout << particles.size() << std::endl;
	for (int i = 0; i < particles.size(); i++)
	{
		std::cout
			<< i << " "
			<< particles[i].position.x << " "
			<< particles[i].position.y << std::endl;
	}

#include <cstddef>

	std::cout << offsetof(Particle, position) << '\n';
	std::cout << offsetof(Particle, velocity) << '\n';
	std::cout << offsetof(Particle, mass) << '\n';

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VertexSSBO);
	glGenBuffers(1, &PartcileIdSSBO);
	glGenBuffers(1, &CountsSSBO);

	glBindVertexArray(VAO);

	//PARTICULAS
	bindSsboAndData(VertexSSBO, 0, particles.size() * sizeof(Particle), particles.data());

	//IDS DEL GRID
	bindSsboAndData(PartcileIdSSBO, 1, particleID.size() * sizeof(int), particleID.data());

	//IDS DEL GRID
	bindSsboAndData(CountsSSBO, 2, particleCount.size() * sizeof(unsigned int), particleCount.data());


	glEnable(GL_PROGRAM_POINT_SIZE);

	GLuint groups = (nPart + 255) / 256;
	GLuint clearGroups = (gridDims.x * gridDims.y + 255) / 256;

	lastFrame = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		processKeyboardInput(window);

		glfwFocusWindow(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//clearGrid.useAndDispatch(clearGroups, 1, 1);

		//computeGrid.useAndDispatch(groups, 1, 1);

		computeShader.use();
		computeShader.setFloat("gravity", -9.8f);
		computeShader.setFloat("dt", deltaTime);
		computeShader.setFloat("damping", dampingFactor);

		computeShader.dispatch(groups,1,1);

		myShader.use();
		myShader.setFloat("size", radius);

		DrawParticles(VAO, nPart);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}


int initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//contexto de la ventana sea le principal del thread y ajusta tamaño
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	return 0;
}

int initGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		return -1;
	}
	else return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void bindSsboAndData(GLuint SSBO, GLuint binding, unsigned int size, const void *data) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, SSBO);
}