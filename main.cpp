#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#include "Shader.h"
#include "Input.h"
#include "Draw.h"

#define WIDTH 800
#define HEIGHT 800

GLFWwindow* window;

float dampingFactor = 0.95;
float radius = 30.0f;
unsigned int nPart = 5;


const int grid_size = 1;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;

GLuint VAO;
GLuint SSBO;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
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
};

std::vector<Particle> particles;

int main() {
	//inicializamos glfw y glad
	if (initGLFW()) return -1;

	if (initGLAD()) return -1;

	const char* vertexShaderPath = "shaders/vertex.glsl";
	const char* fragmentShaderPath = "shaders/fragment.glsl";
	const char* computeShaderPath = "shaders/compute.glsl";
	
	Shader myShader = Shader(vertexShaderPath, fragmentShaderPath);
	myShader.use();

	cShader computeShader = cShader(computeShaderPath);
	std::cout << "Compute program ID: " << computeShader.ID << '\n';

	//reserva espacio
	particles.resize(nPart);

	//creo generador de n aleatorios
	std::uniform_real_distribution<float> dist(-0.7f, 0.7f);

	//inicializar particulas
	for (int i = 0; i < nPart; i++) {
		particles[i].position.x = dist(Random::engine());
		particles[i].position.y = dist(Random::engine());
	}


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &SSBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

	glBufferData(GL_SHADER_STORAGE_BUFFER, nPart * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);

	//importante le dice que buffer usar al shader, sustituye al glVertexAttribPointer
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

	glEnable(GL_PROGRAM_POINT_SIZE);

	GLuint groups = (nPart + 255) / 256;

	while (!glfwWindowShouldClose(window)) {
		processKeyboardInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		computeShader.use();
		computeShader.setFloat("gravity", -9.8f);
		computeShader.setFloat("dt", deltaTime);
		computeShader.setUint("particleCount", nPart);
		computeShader.setFloat("damping", dampingFactor);

		computeShader.dispatch(groups,1,1);
		computeShader.dispatch(groups, 1, 1);

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