#pragma once

#include "BufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <glm/ext/matrix_float4x4.hpp>

class VertexArray;
class IndexBuffer;

class Shape {
public:
	virtual ~Shape(){}

	//Render
	virtual VertexArray& GetVAO() const = 0;
	virtual IndexBuffer& GetIBO() const = 0;
	virtual GLenum GetDrawnMode() = 0;

	//Iteration
	virtual void UpdateVertices(double mouseX, double mouseY) = 0;
	virtual void AddPoint(double mouseX, double mouseY) = 0;
	virtual void FinalizeShape() = 0;

	//Selection An Color
	
	virtual bool IsInside(float ndcX, float ndcY) const = 0;
	virtual void SetColor(float r, float g, float b) = 0;

	//u_Model Matrix
	
	virtual const glm::mat4& GetModelMatrix() const = 0;
	virtual void Translate(float dx, float dy) = 0;
	virtual void Rotate(float angle) = 0;
	virtual void Scale(float sx, float sy) = 0;

	//Area and Perimeter
	virtual float GetArea() const = 0;
	virtual float GetPerimeter() const = 0;


};
