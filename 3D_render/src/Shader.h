#pragma once
#include <GL/glew.h>
#include <SDL_opengl.h> 
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader {
public:
	GLuint id;

	Shader();
	Shader(std::string* vertexPath, std::string* fragmentPath, std::string* geometryPath = nullptr);
	void use();
	void setModel(glm::mat4 viewMatrix);
	void setView(glm::mat4 viewMatrix);
	void setProjection(glm::mat4 projectionMatrix);
	void setFloat(std::string const& name, glm::float32 value);
	void setInt(std::string const& name, glm::int32 value);

	void setVector2(std::string const& name, glm::vec2 vector);
	void setVector3(std::string const& name, glm::vec3 vector);


private:
	void checkCompileErrors(GLuint shader, std::string type);
};