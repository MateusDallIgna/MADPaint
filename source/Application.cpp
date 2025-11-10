#include "Application.h"
#include <cstdlib>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>

Application::Application(int windowWidth, int windowHeight, const char* title) :m_Width(windowWidth), m_Height(windowHeight), m_Title(title){
}

Application::~Application(){
	glfwTerminate();
}

void Application::InitGLFW(){
	if(!glfwInit()){
		std::cerr << "Failed on initializa the GLFW" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void Application::CreateGLFWWindow(){

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Compatibility special for you Rodack <3
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
	if(m_Window == NULL){
		std::cerr << "Failed on window creation" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_Window);
}

void Application::InitGLAD(){

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cerr << "Failed to init GLAD" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void Application::MainLoop(){
	while(!glfwWindowShouldClose(m_Window)){
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
}

GLFWwindow* Application::GetWindow(){
	return m_Window;
}

int Application::GetHeight(){
	return m_Height;
}

int Application::GetWidth(){
	return m_Width;
}

void Application::SetupGL(){
	glViewport(0, 0, m_Width, m_Height);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	float aspectRatio = (float)m_Width / (float)m_Height;
    float worldHeight = 10.0f; 
    float worldWidth = worldHeight * aspectRatio;

    float left   = -worldWidth / 2.0f;
    float right  =  worldWidth / 2.0f;
    float bottom = -worldHeight / 2.0f;
    float top    =  worldHeight / 2.0f;

    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

const glm::mat4& Application::GetProjectionMatrix() const{
    return m_ProjectionMatrix;
}

void Application::Run(){
	InitGLFW();
	CreateGLFWWindow();
	InitGLAD();
	SetupGL();
}
