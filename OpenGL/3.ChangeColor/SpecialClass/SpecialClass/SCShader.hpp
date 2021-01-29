#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const char* vertex_file_path_ = nullptr, const char* fragment_file_path_ = nullptr);
	~Shader();

	void Use(void);

	void SetBool(const std::string& name_, bool value_) const;
	void SetInt(const std::string& name_, int value_) const;
	void SetuInt(const std::string& name_, unsigned int value_) const;
	void SetFloat(const std::string& name_, float value_) const;
	void SetVec3(const std::string& name_, const glm::vec3& value_) const;
	void SetVec3(const std::string& name_, float x_, float y_, float z_) const;
	void SetVec4(const std::string& name_, const glm::vec4& value_) const;
	void SetVec4(const std::string& name_, float x_, float y_, float z_, float w_) const;
	void SetMat4(const std::string& name_, const glm::mat4& mat) const;

	unsigned int GetProgramID(void);
private:
	unsigned int m_programID;
};



#endif // end SHADER_H