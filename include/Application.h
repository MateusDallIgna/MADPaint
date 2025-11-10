#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

class Application{

public:

	Application(int windowWidth, int windowHeight, const char* title);

	~Application();

	void Run();
	GLFWwindow* GetWindow();
	int GetWidth();
	int GetHeight();

	const glm::mat4& GetProjectionMatrix() const;

private:

	void InitGLFW();
	void InitGLAD();
	void CreateGLFWWindow();
	void MainLoop();
	void SetupGL();

	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
	const char* m_Title;

	glm::mat4 m_ProjectionMatrix;
};
