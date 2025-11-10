#include "Application.h"
#include "BufferLayout.h"
#include "Layer.h"
#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods){
	Layer* layer = (Layer*)glfwGetWindowUserPointer(window);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (layer != nullptr){
		layer->OnMouseButtonEvent(button, action, mods, mouseX, mouseY);

	}
}

void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods){
    Layer* layer = (Layer*)glfwGetWindowUserPointer(window);

    if (layer != nullptr){
        layer->OnKeyEvent(key, scancode, action, mods);
    }
}

void PrintHelpMessage(){
	std::cout << "\n========================================\n";
	std::cout << "   2D Paint Program - Controls\n";
	std::cout << "========================================\n\n";
	
	std::cout << "DRAWING MODES:\n";
	std::cout << "  0 - Select Mode\n";
	std::cout << "  1 - Draw Rectangle\n";
	std::cout << "  2 - Draw Triangle\n";
	std::cout << "  3 - Draw Circle\n";
	std::cout << "  4 - Draw Polygon (click to add points, right-click to finish)\n\n";
	
	std::cout << "SELECTION & TRANSFORMATION:\n";
	std::cout << "  Left Click - Select shape (hold Ctrl for multiple selection)\n";
	std::cout << "  Drag Mouse - Move selected shape(s)\n";
	std::cout << "  Q - Rotate selected shape(s) counter-clockwise\n";
	std::cout << "  E - Rotate selected shape(s) clockwise\n";
	std::cout << "  + / = - Scale up selected shape(s)\n";
	std::cout << "  - - Scale down selected shape(s)\n";
	std::cout << "  A - Display area and perimeter of selected shape(s)\n\n";
	
	std::cout << "VIEW CONTROLS:\n";
	std::cout << "  Z - Zoom in\n";
	std::cout << "  X - Zoom out\n";
	std::cout << "  Middle Mouse Button - Pan (drag to move view)\n";
	std::cout << "  Space + Drag Mouse - Pan (hold space and drag)\n\n";
	
	std::cout << "COLOR CONTROLS:\n";
	std::cout << "  R - Change selected shape(s) to RED\n";
	std::cout << "  G - Change selected shape(s) to GREEN\n";
	std::cout << "  B - Change selected shape(s) to BLUE\n";
	std::cout << "  K - Change selected shape(s) to BLACK\n\n";
	
	std::cout << "OTHER CONTROLS:\n";
	std::cout << "  F - Toggle fill mode (filled/outline)\n";
	std::cout << "  Backspace - Delete selected shape(s)\n";
	std::cout << "  Ctrl+Q - Quit application\n\n";
	
	std::cout << "========================================\n\n";
}

int main(){
	Application window (1280, 720 , "My_Window");
	window.Run();

	PrintHelpMessage();

	Layer layer ("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl", &window);

	glfwSetWindowUserPointer(window.GetWindow(), &layer);

	glfwSetMouseButtonCallback(window.GetWindow(), MouseButtonCallBack);

	glfwSetKeyCallback(window.GetWindow(), KeyCallBack);


	while(!glfwWindowShouldClose(window.GetWindow())){
		glClear(GL_COLOR_BUFFER_BIT);

		// renderer.Draw(vertexArray, shader);
		layer.OnUpdate(window.GetWindow());
		layer.OnRender();

		glfwSwapBuffers(window.GetWindow());

		glfwPollEvents();
	}

return 0;
}
