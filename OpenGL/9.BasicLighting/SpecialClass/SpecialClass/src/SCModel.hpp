/*******************************************************************************
filename			: SCModel.hpp
author			: Jihwan Oh
email				: jihwan315@gmail.com, dkdkdkdudrn@gmail.com
date				: 2021.02.03

description:
  hpp file for Model class.

*******************************************************************************/
#ifndef MODEL_HPP
#define MODEL_HPP

#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct Model
{
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_vertexNormal;
	std::vector<glm::vec2> m_texture;
	std::vector<unsigned int> m_indicies;
	std::vector<unsigned int> m_texIndicies;
	std::vector<unsigned int> m_vnIndicies;
};

struct Texture
{
	unsigned int m_textureID;

};

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	void LoadOBJ(const char* obj_file_path_, const char* name_, bool normalize_ = false);
	Model* GetModel(const char* name_);

private:
	std::map<std::string, Model> m_modelList;
};

#endif // !MODEL_HPP