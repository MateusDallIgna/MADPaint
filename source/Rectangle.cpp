#include "Rectangle.h"
#include "BufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>                    
#include <glm/gtc/matrix_transform.hpp>  

Rectangle::Rectangle(float x1, float y1, float x2, float y2, bool m_IsFilled) : m_AnchorX(x1) , m_AnchorY(y1), m_CurrentX(x2), m_CurrentY(y2), m_ShapeIsFilled(m_IsFilled), m_ModelMatrix(glm::mat4(1.0f)){

	m_Vertices = {
		x1,y1,1.0f,0.0f,0.0f,1.0f,
		x2,y1,1.0f,0.0f,0.0f,1.0f,
		x2,y2,1.0f,0.0f,0.0f,1.0f,
		x1,y2,1.0f,0.0f,0.0f,1.0f
	};

	m_Indices = {
		0,1,2,
		2,3,0
	};

	m_BufferLayout.PushFloat(2);
	m_BufferLayout.PushFloat(4);

	m_VertexArray = new VertexArray();

	m_VertexArray->SetData(m_Vertices.data(), m_Vertices.size()*sizeof(float) , &m_BufferLayout);

	m_IndexBuffer = new IndexBuffer(m_Indices.data(), m_Indices.size());

}

void Rectangle::UpdateVertices(double mouseX, double mouseY){
	
    m_CurrentX = (float)mouseX;
    m_CurrentY = (float)mouseY;


	m_Vertices = {
		(float)m_AnchorX, (float)m_AnchorY, 1.0f,0.0f,0.0f,1.0f,
		(float)mouseX, (float)m_AnchorY, 1.0f,0.0f,0.0f,1.0f,
		(float)mouseX, (float)mouseY, 1.0f,0.0f,0.0f,1.0f,
		(float)m_AnchorX, (float)mouseY, 1.0f,0.0f,0.0f,1.0f,
	};

	m_VertexArray->SetData(m_Vertices.data(), m_Vertices.size()*sizeof(float) , &m_BufferLayout);

}

GLenum Rectangle::GetDrawnMode(){
	if(m_ShapeIsFilled){
		return GL_TRIANGLES;
	}
	else{
		return GL_LINE_LOOP;
	}

}

void Rectangle::SetColor(float r, float g, float b){
    
	for (size_t i = 0; i < m_Vertices.size(); i+=6){
        m_Vertices[i+2] = r;
        m_Vertices[i+3] = g;
        m_Vertices[i+4] = b;
		m_Vertices[i+5] = 1.0f;
    }
	m_VertexArray->SetData(m_Vertices.data(), m_Vertices.size()*sizeof(float) , &m_BufferLayout);
}

bool Rectangle::IsInside(float ndcX, float ndcY) const{
	float leftX   = std::min(m_AnchorX, m_CurrentX);
    float rightX  = std::max(m_AnchorX, m_CurrentX);
    float bottomY = std::min(m_AnchorY, m_CurrentY);
    float topY    = std::max(m_AnchorY, m_CurrentY);

    return (ndcX >= leftX   && ndcX <= rightX &&
            ndcY >= bottomY && ndcY <= topY);
}

VertexArray& Rectangle::GetVAO() const{
	return *m_VertexArray;
}

IndexBuffer& Rectangle::GetIBO() const{
return *m_IndexBuffer;
}

const glm::mat4& Rectangle::GetModelMatrix() const{
    return m_ModelMatrix;
}

void Rectangle::Translate(float dx, float dy)
{
    glm::vec3 translationVector(dx, dy, 0.0f);

	m_ModelMatrix = glm::translate(m_ModelMatrix, translationVector);

}

void Rectangle::Rotate(float angle) {
    float centerX = (m_AnchorX + m_CurrentX) / 2.0f;
    float centerY = (m_AnchorY + m_CurrentY) / 2.0f;
    
    glm::mat4 rotateMatrix = glm::mat4(1.0f);
    rotateMatrix = glm::translate(rotateMatrix, glm::vec3(centerX, centerY, 0.0f));
    rotateMatrix = glm::rotate(rotateMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    rotateMatrix = glm::translate(rotateMatrix, glm::vec3(-centerX, -centerY, 0.0f));
    
    m_ModelMatrix = m_ModelMatrix * rotateMatrix;
}

void Rectangle::Scale(float sx, float sy) {
    float centerX = (m_AnchorX + m_CurrentX) / 2.0f;
    float centerY = (m_AnchorY + m_CurrentY) / 2.0f;
    
    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::translate(scaleMatrix, glm::vec3(centerX, centerY, 0.0f));
    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(sx, sy, 1.0f));
    scaleMatrix = glm::translate(scaleMatrix, glm::vec3(-centerX, -centerY, 0.0f));
    
    m_ModelMatrix = m_ModelMatrix * scaleMatrix;
}

float Rectangle::GetArea() const {
    float width = std::abs(m_CurrentX - m_AnchorX);
    float height = std::abs(m_CurrentY - m_AnchorY);
    return width * height;
}

float Rectangle::GetPerimeter() const {
    float width = std::abs(m_CurrentX - m_AnchorX);
    float height = std::abs(m_CurrentY - m_AnchorY);
    return 2.0f * (width + height);
}

Rectangle::~Rectangle(){
	delete m_VertexArray;
	delete m_IndexBuffer;
}


void Rectangle::AddPoint(double x, double y){}
void Rectangle::FinalizeShape(){}
