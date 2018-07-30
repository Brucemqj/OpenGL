#include "Shader.h"

Shader::Shader(const char * vertexPath, const char * fragmentPath, const char * geometryPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string		vertexCode,	fragmentCode;
	std::ifstream vertexFile,		fragmentFile;
	// ensure ifstream objects can throw exceptions:
	vertexFile.exceptions		(std::ifstream::failbit | std::fstream::badbit);
	fragmentFile.exceptions  (std::ifstream::failbit | std::fstream::badbit);
	try
	{
		// open files
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		std::stringstream vertexStream, fragmentStream;
		// read file's buffer contents into streams
		vertexStream		<< vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();
		// close file handlers
		vertexFile.close();
		fragmentFile.close();
		// convert stream into string
		vertexCode	 = vertexStream.str();
		fragmentCode = fragmentStream.str();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompileError(vertexShader, "Vertex");

	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkCompileError(fragmentShader, "Fragment");

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkCompileError(ID, "Program");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::useShader()
{
	glUseProgram(ID);
}

//inline void Shader::setUniformValue(const std::string & name, bool value) const
//{
//	glUniform1i( glGetUniformLocation( ID, name.c_str() ), static_cast<int>(value));
//}
//
//inline void Shader::setUniformValue(const std::string & name, int value) const
//{
//	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
//}
//
//inline void Shader::setUniformValue(const std::string & name, float value) const
//{
//	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
//}
//
//inline void Shader::setUniformValue(const std::string & name, float x, float y, float z, float w) const
//{
//	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
//}

void Shader::checkCompileError(const GLuint shader, const std::string &type)
{
	int success;
	char infoLog[1024];
	if ("Program" == type)
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Error::Program_Linking_Error of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Error::Shader_Compilation_Error of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
