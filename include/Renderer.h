#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"

class Renderer{

public:
	void Draw(const VertexArray& VertexArray, const IndexBuffer& IndexBuffer, GLenum DrawnMode);
	void Unbind(const VertexArray& vertexarray, const IndexBuffer& IndexBuffer, const Shader& shader);

private:

};
