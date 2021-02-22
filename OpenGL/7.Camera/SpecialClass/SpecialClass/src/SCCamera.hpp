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
/****************************************************************************
\Class Name : Camera
\Role :
	This class contains codes to do this....
	- Create view, projection matrix depends on camera's position
	- Move with keyboard input

*****************************************************************************/
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

	// Matrix for coordinate system
	glm::mat4 m_projectionMatrix;	//!< projection matrix
	glm::mat4 m_viewMatrix;				//!< view matrix


	glm::vec3 m_position;		//!< camera position
	glm::vec3 m_target;			//!< target position
	glm::vec3 m_direction;	//!< direction camera see
	glm::vec3 m_right;			//!< right direction of cam
	glm::vec3 m_up;				//!< up direction of cam

	float m_FoV;						//!< angle of field of view
	float m_cameraSpeed;	//!< camera moving speed
	float m_mouseSpeed;		//!< mouse moving speed

	unsigned int m_screenWidth;	//!< width of current screen
	unsigned int m_screenHeight;	//!< height of current screen

	bool m_isMoving;	// check camera need to move
};

#endif // !CAMERA_HPP
