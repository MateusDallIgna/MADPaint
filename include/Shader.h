#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader{

public:

	Shader(const char* VertexShader, const char* FragmentShader);	
	~Shader();

	void Bind() const;
	void Unbind() const;
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);


private:

	unsigned int m_ID;
};

