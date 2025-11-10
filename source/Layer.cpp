#include "Layer.h" 
#include "Application.h"
#include "Circle.h"
#include "Polygon.h"
#include "Rectangle.h"
#include <GLFW/glfw3.h>
#include "Triangle.h"
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Layer::Layer(const char* vertPath, const char* fragPath, Application* windowContext) : m_Shader(vertPath, fragPath), m_CurrentMode(0), m_CurrentDrawingShape(nullptr), m_WindowContext(windowContext), m_IsFilled(true),m_ViewMatrix(glm::mat4(1.0f)), m_IsTranslating(false), m_IsPanning(false), m_LastMouseX(0.0), m_LastMouseY(0.0), m_LastPanMouseX(0.0), m_LastPanMouseY(0.0), m_ZoomLevel(1.0f), m_PanOffset(0.0f, 0.0f, 0.0f){
	UpdateViewMatrix();
};

void Layer::OnRender(){

	m_Shader.Bind();

	const glm::mat4& proj = m_WindowContext->GetProjectionMatrix();
	const glm::mat4& view = m_ViewMatrix; 

	m_Shader.SetUniformMat4f("u_Projection", proj);
	m_Shader.SetUniformMat4f("u_View", view);

	for(Shape* shape : m_Shape){
		const glm::mat4& model = shape->GetModelMatrix();

		m_Shader.SetUniformMat4f("u_Model", model);
		m_Renderer.Draw(shape->GetVAO(),shape->GetIBO(),shape->GetDrawnMode());
	}
}

void Layer::ConvertScreenToNDC(double mouseX, double mouseY, float& outX, float& outY){

	float WindowWidth = m_WindowContext->GetWidth();
	float WindowHeight = m_WindowContext->GetHeight();
	
	outX = (mouseX / WindowWidth) * 2.0f - 1.0f;
	outY = 1.0f - (mouseY / WindowHeight) * 2.0f;

}

void Layer::ConvertScreenToWorld(double mouseX, double mouseY, float& outX, float& outY){
	float ndcX, ndcY;
	ConvertScreenToNDC(mouseX, mouseY, ndcX, ndcY);
	
	const glm::mat4& proj = m_WindowContext->GetProjectionMatrix();
	glm::mat4 invProj = glm::inverse(proj);
	glm::mat4 invView = glm::inverse(m_ViewMatrix);
	
	glm::vec4 ndcPoint(ndcX, ndcY, 0.0f, 1.0f);
	glm::vec4 viewPoint = invProj * ndcPoint;
	glm::vec4 worldPoint = invView * viewPoint;
	
	if (worldPoint.w != 0.0f) {
		outX = worldPoint.x / worldPoint.w;
		outY = worldPoint.y / worldPoint.w;
	} else {
		outX = worldPoint.x;
		outY = worldPoint.y;
	}
}

void Layer::OnUpdate(GLFWwindow* window){

	if (m_CurrentDrawingShape != nullptr){
		double mouseX, mouseY;
		float outX, outY;

		glfwGetCursorPos(window, &mouseX, &mouseY);
		ConvertScreenToWorld(mouseX, mouseY, outX, outY);
		m_CurrentDrawingShape->UpdateVertices(outX, outY);
		
	}

	if (m_IsTranslating && !m_SelectedShape.empty()) {
		double mouseX, mouseY;
		float outX, outY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		ConvertScreenToWorld(mouseX, mouseY, outX, outY);

		float dx = (float)(outX - m_LastMouseX);
		float dy = (float)(outY - m_LastMouseY);

		for (Shape* shape : m_SelectedShape) {
			shape->Translate(dx, dy);
		}

		m_LastMouseX = outX;
		m_LastMouseY = outY;
	}

	if (m_IsPanning) {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		
		double dx = mouseX - m_LastPanMouseX;
		double dy = mouseY - m_LastPanMouseY;
		
		float windowWidth = (float)m_WindowContext->GetWidth();
		float windowHeight = (float)m_WindowContext->GetHeight();
		const glm::mat4& proj = m_WindowContext->GetProjectionMatrix();
		
		float worldWidth = (proj[0][0] != 0.0f) ? (2.0f / proj[0][0]) : 10.0f;
		float worldHeight = (proj[1][1] != 0.0f) ? (2.0f / proj[1][1]) : 10.0f;
		
		float pixelToWorldScaleX = worldWidth / windowWidth;
		float pixelToWorldScaleY = worldHeight / windowHeight;
		
		float worldDx = (float)(dx * pixelToWorldScaleX) / m_ZoomLevel;
		float worldDy = (float)(-dy * pixelToWorldScaleY) / m_ZoomLevel;
		
		m_PanOffset.x -= worldDx;
		m_PanOffset.y -= worldDy;
		
		UpdateViewMatrix();
		
		m_LastPanMouseX = mouseX;
		m_LastPanMouseY = mouseY;
	}
}

void Layer::UpdateViewMatrix() {
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / m_ZoomLevel, 1.0f / m_ZoomLevel, 1.0f));
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_PanOffset.x, -m_PanOffset.y, 0.0f));
	m_ViewMatrix = scaleMat * translateMat;
}

void Layer::OnKeyEvent(int key, int /*scancode*/, int action, int mods){

	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		m_IsPanning = false;
		std::cout << "Panning mode deactivated" << std::endl;
		return;
	}

	if (!(action == GLFW_PRESS)){
        return;
    }

	switch (key) {

		case GLFW_KEY_F:{
			m_IsFilled = !m_IsFilled;
			if(!m_IsFilled){
				std::cout << "Fill Mode: Deactivate" <<std::endl;
				return;
			}
			std::cout << "Fill Mode: Activated" <<std::endl;
			break;
		}

		case GLFW_KEY_K:{

			if (m_SelectedShape.empty()){ 
				break; 
			}
			for (Shape* shape : m_SelectedShape) {
				shape->SetColor(0.0f, 0.0f, 0.0f); 
			}
			std::cout << "The color of the selected objects has been changed to BLACK." << std::endl;
			break;
		}

		case GLFW_KEY_R:{

			if (m_SelectedShape.empty()){ 
				break; 
			}
			for (Shape* shape : m_SelectedShape) {
				shape->SetColor(1.0f, 0.0f, 0.0f); 
			}
			std::cout << "The color of the selected objects has been changed to RED." << std::endl;
			break;
		}

		case GLFW_KEY_G:{

			if (m_SelectedShape.empty()){ 
				break; 
			}
			for (Shape* shape : m_SelectedShape) {
				shape->SetColor(0.0f, 1.0f, 0.0f); 
			}
			std::cout << "The color of the selected objects has been changed to GREEN." << std::endl;
			break;
		}

		case GLFW_KEY_B:{

			if (m_SelectedShape.empty()){ 
				break; 
			}
			for (Shape* shape : m_SelectedShape) {
				shape->SetColor(0.0f, 0.0f, 1.0f); 
			}
			std::cout << "The color of the selected objects has been changed to BLUE." << std::endl;
			break;
		}

		case GLFW_KEY_BACKSPACE:{
			if(!(m_SelectedShape.empty())){
				for(Shape* shapeToDelete : m_SelectedShape){
					m_Shape.erase(std::remove(m_Shape.begin(), m_Shape.end(), shapeToDelete), m_Shape.end());
					delete shapeToDelete;
				}
				m_SelectedShape.clear();
				std::cout << "Selected shapes erased" << std::endl;
			} else {
				std::cout << "Nothing selected to be erased" << std::endl;
			}
			break;
		}

		case GLFW_KEY_Q: {
			if (mods & GLFW_MOD_CONTROL) {
				std::cout << "Quitting application..." << std::endl;
				GLFWwindow* window = m_WindowContext->GetWindow();
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			} else {
				if (m_SelectedShape.empty()) {
					break;
				}
				float rotationAngle = 0.1f;
				for (Shape* shape : m_SelectedShape) {
					shape->Rotate(rotationAngle);
				}
				std::cout << "Rotated selected shapes counter-clockwise" << std::endl;
			}
			break;
		}

		case GLFW_KEY_0:{
            m_CurrentMode = 0;
			std::cout << "Mode 0: Select Mode Active" <<std::endl;
            break;
		}

        case GLFW_KEY_1:{
            m_CurrentMode = 1;
            std::cout << "Mode 1: Drawn Rectangle Active" <<std::endl;
            break;
        }

        case GLFW_KEY_2:{
            m_CurrentMode = 2;
            std::cout << "Mode 2: Drawn Triangle Active" <<std::endl;
            break;
        }

		case GLFW_KEY_3: {
			m_CurrentMode = 3;
			std::cout << "Mode 3: Drawn Cirvle Active" <<std::endl;
			break;
		}

        case GLFW_KEY_4: {
            m_CurrentMode = 4;
            std::cout << "Mode 4: Drawn Polygon Active" <<std::endl;
            break;
        }


		case GLFW_KEY_E: {
			if (m_SelectedShape.empty()) {
				break;
			}
			float rotationAngle = -0.1f;
			for (Shape* shape : m_SelectedShape) {
				shape->Rotate(rotationAngle);
			}
			std::cout << "Rotated selected shapes clockwise" << std::endl;
			break;
		}

		case GLFW_KEY_EQUAL:
		case GLFW_KEY_KP_ADD: {
			if (m_SelectedShape.empty()) {
				break;
			}
			float scaleFactor = 1.1f;
			for (Shape* shape : m_SelectedShape) {
				shape->Scale(scaleFactor, scaleFactor);
			}
			std::cout << "Scaled up selected shapes" << std::endl;
			break;
		}

		case GLFW_KEY_MINUS:
		case GLFW_KEY_KP_SUBTRACT: {
			if (m_SelectedShape.empty()) {
				break;
			}
			float scaleFactor = 1.0f / 1.1f;
			for (Shape* shape : m_SelectedShape) {
				shape->Scale(scaleFactor, scaleFactor);
			}
			std::cout << "Scaled down selected shapes" << std::endl;
			break;
		}

		case GLFW_KEY_Z: {
			m_ZoomLevel *= 1.2f;
			if (m_ZoomLevel > 10.0f) m_ZoomLevel = 10.0f;
			UpdateViewMatrix();
			std::cout << "Zoomed in. Zoom level: " << m_ZoomLevel << std::endl;
			break;
		}

		case GLFW_KEY_X: {
			m_ZoomLevel /= 1.2f;
			if (m_ZoomLevel < 0.1f) m_ZoomLevel = 0.1f;
			UpdateViewMatrix();
			std::cout << "Zoomed out. Zoom level: " << m_ZoomLevel << std::endl;
			break;
		}

		case GLFW_KEY_SPACE: {
			GLFWwindow* window = m_WindowContext->GetWindow();
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);
			m_IsPanning = true;
			m_LastPanMouseX = mouseX;
			m_LastPanMouseY = mouseY;
			std::cout << "Panning mode activated (drag mouse)" << std::endl;
			break;
		}

		case GLFW_KEY_A: {
			if (m_SelectedShape.empty()) {
				std::cout << "No shapes selected" << std::endl;
				break;
			}
			float totalArea = 0.0f;
			float totalPerimeter = 0.0f;
			for (Shape* shape : m_SelectedShape) {
				totalArea += shape->GetArea();
				totalPerimeter += shape->GetPerimeter();
			}
			std::cout << "Selected shapes - Total Area: " << totalArea 
			          << ", Total Perimeter: " << totalPerimeter << std::endl;
			break;
		}
	}
}

void Layer::OnMouseButtonEvent(int button, int action, int mods, double mouseX, double mouseY){

	float worldX, worldY;
	ConvertScreenToWorld(mouseX, mouseY, worldX, worldY);

	if (action == GLFW_RELEASE){
		if(m_IsTranslating){
            m_IsTranslating = false;
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && m_IsPanning) {
			m_IsPanning = false;
		}
    }

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		m_IsPanning = true;
		double mouseX, mouseY;
		GLFWwindow* window = m_WindowContext->GetWindow();
		glfwGetCursorPos(window, &mouseX, &mouseY);
		m_LastPanMouseX = mouseX;
		m_LastPanMouseY = mouseY;
		return;
	}

	if (m_CurrentMode == 4){
		if(action != GLFW_PRESS){
			return;
		}
		if(button != GLFW_MOUSE_BUTTON_LEFT){
			m_CurrentDrawingShape = nullptr;
			return;
		}
		if(m_CurrentDrawingShape){
			m_CurrentDrawingShape->AddPoint(worldX, worldY);
		}
		else{
			m_CurrentDrawingShape = new Polygon(worldX, worldY, m_IsFilled);
			m_Shape.push_back(m_CurrentDrawingShape);
		}
		return;
	}


	if (action != GLFW_PRESS){
		m_CurrentDrawingShape = nullptr;
		return;
	}

	switch (m_CurrentMode) {

		case 0:{
				   if(!(mods & GLFW_MOD_CONTROL)){
					   m_SelectedShape.clear();
				   }

				   for(int i = m_Shape.size() - 1; i >= 0; i--){
					   const glm::mat4& modelMatrix = m_Shape[i]->GetModelMatrix();
					   glm::mat4 invModel = glm::inverse(modelMatrix);
					   glm::vec4 testPoint(worldX, worldY, 0.0f, 1.0f);
					   glm::vec4 localPoint = invModel * testPoint;
					   
					   if(m_Shape[i]->IsInside(localPoint.x, localPoint.y)){
						   m_SelectedShape.push_back(m_Shape[i]);
						   std::cout << "The " << i<<"rd last form created has been selected" <<std::endl; 
						   break;
					   }
				   }

				   if (!m_SelectedShape.empty()) {
					   m_IsTranslating = true;

					   m_LastMouseX = worldX; 
					   m_LastMouseY = worldY;
				   }

				   break; 
			   }

		case 1:{
				   m_CurrentDrawingShape = new Rectangle(worldX, worldY, worldX, worldY, m_IsFilled);
				   m_Shape.push_back(m_CurrentDrawingShape);

				   break;
			   }

		case 2:{
				   m_CurrentDrawingShape = new Triangle(worldX, worldY, worldX, worldY, m_IsFilled);
				   m_Shape.push_back(m_CurrentDrawingShape);

				   break;
			   }

		case 3:{
				   m_CurrentDrawingShape = new Circle(worldX, worldY, worldX, worldY, m_IsFilled);
				   m_Shape.push_back(m_CurrentDrawingShape);

				   break;
			   }

		case 4:{

				   break;
			   }

		case 5:{

				   break;
			   }
	}
}

Layer::~Layer(){
	for(Shape* shape : m_Shape){
		delete shape;
	}
}
