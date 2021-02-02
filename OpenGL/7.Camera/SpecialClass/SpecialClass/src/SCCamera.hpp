/*******************************************************************************
filename			: SCCamera.hpp
author			: Jihwan Oh
email				: jihwan315@gmail.com, dkdkdkdudrn@gmail.com
date				: 2021.01.22

description:
  hpp file for Camera class.

*******************************************************************************/
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

struct GLFWwindow;

class Camera
{
public:
	Camera();
	~Camera();

	void SetFirstValue(void);
	void Init(void);

	void SetTarget(glm::vec3 target_);

	void Update(GLFWwindow* window_, float dt_);
	void SetMove(bool move_);
	void SetFoV(float FoV_);

	glm::mat4 GetProjectionMat();
	glm::mat4 GetViewMat();

private:
	void Moving(GLFWwindow* window_, float dt_);
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;

	glm::vec3 m_position;
	glm::vec3 m_target;
	glm::vec3 m_direction;
	glm::vec3 m_right;
	glm::vec3 m_up;

	float m_FoV;
	float m_cameraSpeed;
	float m_mouseSpeed;

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;

	bool m_isMoving;
};

#endif // !CAMERA_HPP
