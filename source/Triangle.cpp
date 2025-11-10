#include "Triangle.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>                   
#include <glm/gtc/matrix_transform.hpp>    
										  
Triangle::Triangle(float x1, float y1, float x2, float y2, bool m_IsFilled) : m_AnchorX(x1) , m_AnchorY(y1), m_ShapeIsFilled(m_IsFilled), m_ModelMatrix(glm::mat4(1.0f)){

	float leftX = std::min(x1, x2);
    float rightX = std::max(x1, x2);
    float topY = std::max(y1, y2);
    float bottomY = std::min(y1, y2);

	m_Vertices = {
		(leftX+rightX)/2, topY, 0.0f,1.0f,0.0f,1.0f,
		leftX, bottomY, 0.0f,1.0f,0.0f,1.0f,
        rightX, bottomY, 0.0f,1.0f,0.0f,1.0f
		
    };

    m_Indices = {
        0,1,2
    };

    m_BufferLayout.PushFloat(2);
    m_BufferLayout.PushFloat(4);

    m_VertexArray = new VertexArray();

    m_VertexArray->SetData(m_Vertices.data(), m_Vertices.size()*sizeof(float) , &m_BufferLayout);

    m_IndexBuffer = new IndexBuffer(m_Indices.data(), m_Indices.size());
}

void Triangle::UpdateVertices(double mouseX, double mouseY){

	float x1 = (float)m_AnchorX;
    float y1 = (float)m_AnchorY;
    float x2 = (float)mouseX;
    float y2 = (float)mouseY;

    float leftX = std::min(x1, x2);
    float rightX = std::max(x1, x2);
    float topY = std::max(y1, y2);
    float bottomY = std::min(y1, y2);

	m_Vertices = {

        (leftX+rightX)/2, topY, 0.0f,1.0f,0.0f,1.0f,
        leftX, bottomY, 0.0f,1.0f,0.0f,1.0f,
        rightX, bottomY, 0.0f,1.0f,0.0f,1.0f
	};

	m_VertexArray->SetData(m_Vertices.data(), m_Vertices.size()*sizeof(float) , &m_BufferLayout);
}


GLenum Triangle::GetDrawnMode(){
	if(m_ShapeIsFilled){
		return GL_TRIANGLES;
	}
	else{
		return GL_LINE_LOOP;
	}

}

void Triangle::SetColor(float r, float g, float b){
    
	for (size_t i = 0; i < m_Vertices.size(); i+=6){
        m_Vertices[i+2] = r;
        m_Vertices[i+3] = g;
        m_Vertices[i+4] = b;
		m_Vertices[i+5] = 1.0f;
    }
	m_VertexArray->SetData(m_Vertices.data(), m_Vertices.size()*sizeof(float) , &m_BufferLayout);
}

float sign(float x1, float y1, float x2, float y2, float x3, float y3) {
    return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}

bool Triangle::IsInside(float ndcX, float ndcY) const {

    float v0_x = m_Vertices[0]; 
    float v0_y = m_Vertices[1]; 
    
    float v1_x = m_Vertices[6]; 
    float v1_y = m_Vertices[7]; 
    
    float v2_x = m_Vertices[12]; 
    float v2_y = m_Vertices[13]; 

    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(ndcX, ndcY, v0_x, v0_y, v1_x, v1_y);
    d2 = sign(ndcX, ndcY, v1_x, v1_y, v2_x, v2_y);
    d3 = sign(ndcX, ndcY, v2_x, v2_y, v0_x, v0_y);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

VertexArray& Triangle::GetVAO() const{
    return *m_VertexArray;
}

IndexBuffer& Triangle::GetIBO() const{
    return *m_IndexBuffer;
}

const glm::mat4& Triangle::GetModelMatrix() const{
    return m_ModelMatrix;
}

void Triangle::Translate(float dx, float dy) {

    glm::vec3 translationVector(dx, dy, 0.0f);

    m_ModelMatrix = glm::translate(m_ModelMatrix, translationVector); 
}

void Triangle::Rotate(float angle) {
    float centerX = (m_Vertices[0] + m_Vertices[6] + m_Vertices[12]) / 3.0f;
    float centerY = (m_Vertices[1] + m_Vertices[7] + m_Vertices[13]) / 3.0f;
    
    glm::mat4 rotateMatrix = glm::mat4(1.0f);
    rotateMatrix = glm::translate(rotateMatrix, glm::vec3(centerX, centerY, 0.0f));
    rotateMatrix = glm::rotate(rotateMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    rotateMatrix = glm::translate(rotateMatrix, glm::vec3(-centerX, -centerY, 0.0f));
    
    m_ModelMatrix = m_ModelMatrix * rotateMatrix;
}

void Triangle::Scale(float sx, float sy) {
    float centerX = (m_Vertices[0] + m_Vertices[6] + m_Vertices[12]) / 3.0f;
    float centerY = (m_Vertices[1] + m_Vertices[7] + m_Vertices[13]) / 3.0f;
    
    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::translate(scaleMatrix, glm::vec3(centerX, centerY, 0.0f));
    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(sx, sy, 1.0f));
    scaleMatrix = glm::translate(scaleMatrix, glm::vec3(-centerX, -centerY, 0.0f));
    
    m_ModelMatrix = m_ModelMatrix * scaleMatrix;
}

float Triangle::GetArea() const {
    float x1 = m_Vertices[0], y1 = m_Vertices[1];
    float x2 = m_Vertices[6], y2 = m_Vertices[7];
    float x3 = m_Vertices[12], y3 = m_Vertices[13];
    
    return std::abs((x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2)) / 2.0f);
}

float Triangle::GetPerimeter() const {
    float x1 = m_Vertices[0], y1 = m_Vertices[1];
    float x2 = m_Vertices[6], y2 = m_Vertices[7];
    float x3 = m_Vertices[12], y3 = m_Vertices[13];
    
    float side1 = std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    float side2 = std::sqrt((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2));
    float side3 = std::sqrt((x1-x3)*(x1-x3) + (y1-y3)*(y1-y3));
    
    return side1 + side2 + side3;
}

Triangle::~Triangle(){
    delete m_VertexArray;
    delete m_IndexBuffer;
}

void Triangle::AddPoint(double x, double y){}
void Triangle::FinalizeShape(){}
