#ifndef  SHADER_H
#define SHADER_H

#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

class Shader
{
public:
	GLuint ID;

public:
	Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);
	void useShader();
	inline void setUniformValue(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
	}
	inline void setUniformValue(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	inline void setUniformValue(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value); // 注：所有的setUniformaValue函数如果在在cpp文件中实现，在main函数调用会报错，不知原因
	}
	inline void setUniformValue(const std::string &name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	inline void setUniformValue(const std::string &name, glm::mat4 trans) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
	}
	inline void setUniformValue(const std::string &name, const GLfloat *value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
	}

private:
	void checkCompileError(const GLuint shader, const std::string &type);
};

#endif // ! SHADER_H

