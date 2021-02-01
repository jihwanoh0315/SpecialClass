/*******************************************************************************
filename			: SCShader.hpp
author			: Jihwan Oh
email				: jihwan315@gmail.com, dkdkdkdudrn@gmail.com
date				: 2021.01.06

description:
  hpp file for Shader class.

*******************************************************************************/
#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

/****************************************************************************
\Class Name : Shader
\Role :
	This class contains codes to do this....
	- Create the shader with file io
	- Set values for shader

*****************************************************************************/
class Shader
{
public:
	Shader(const char* vertex_file_path_ = nullptr, const char* fragment_file_path_ = nullptr);
	~Shader();

	void Use(void);

	// Setter for values for shader
	void SetBool(const std::string& name_, bool value_) const;
	void SetInt(const std::string& name_, int value_) const;
	void SetuInt(const std::string& name_, unsigned int value_) const;
	void SetFloat(const std::string& name_, float value_) const;
	void SetVec3(const std::string& name_, const glm::vec3& value_) const;
	void SetVec3(const std::string& name_, float x_, float y_, float z_) const;
	void SetVec4(const std::string& name_, const glm::vec4& value_) const;
	void SetVec4(const std::string& name_, float x_, float y_, float z_, float w_) const;
	void SetMat4(const std::string& name_, const glm::mat4& mat) const;

	// Getter for shader program ID
	unsigned int GetProgramID(void);
private:
	unsigned int m_programID;	//!< ID of this shader
};



#endif // end SHADER_H