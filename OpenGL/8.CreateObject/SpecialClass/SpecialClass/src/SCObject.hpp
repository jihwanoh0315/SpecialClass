/*******************************************************************************
filename			: SCObject.hpp
author			: Jihwan Oh
email				: jihwan315@gmail.com, dkdkdkdudrn@gmail.com
date				: 2021.01.28

description:
  hpp file for Object class

*******************************************************************************/

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <glm/glm.hpp>

struct Model;
class Camera;
class Shader;

class Object
{
public:
	Object();
	~Object();

	void Init(void);

	void SetModel(Model* model_);
	void LoadTexture(const char* texture_file_path_);
	void Update(Shader& shader_, Camera& camera_);
	void SetDebugMode(bool dbMode_);

	glm::vec3 m_position; //!< postion of object
	glm::vec3 m_rotation; //!< rotation of object
	glm::vec3 m_scale;		//!< scale of object
protected:
	// VertexArrayID
	GLuint m_VertexArrayID;
	// VertexBuffer
	GLuint m_vertexBuffer;   //!< VB for Vertex
	GLuint m_indexBuffer;    //!< IB for Indices

	// PPM File Reading
	std::vector<GLuint> m_textureBuffer; //!< texture buffer id
	GLuint m_textureUVBuffer;			//!< texture coordinate buffer
	GLuint m_textureIndexBuffer;			//!< texture coordinate buffer

	Model* m_model; //!< model to draw
	bool m_skeletonMode;
};


#endif // !OBJECT_HPP
