/*******************************************************************************
filename			: SCCamera.cpp
author			: Jihwan Oh
email				: jihwan315@gmail.com, dkdkdkdudrn@gmail.com
date				: 2021.01.22

description:
	In camera, we can calculate the view matrix.
	Also, we will get projection matrix in this camera class.

	Codes for Camera class.
	Funcions in this camera.

	void SetFirstValue(void);
	void Init(void);
	void SetTarget(glm::vec3 target_);
	void Update(GLFWwindow* window_, float dt_);
	void SetMove(bool move_);
	void SetFoV(float FoV_);
	glm::mat4 GetProjectionMat();
	glm::mat4 GetViewMat();
	void Moving(GLFWwindow* window_, float dt_);

*******************************************************************************/
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "SCCamera.hpp"

/****************************************************************************
\Function Name : Camera(constructor)
\Role :
	Constructor of camera class

*****************************************************************************/
Camera::Camera()
{
	/*********************************************************/
	//* Camera
	//      - Make the camera for view matrix
	//* Set
	//    1. Set the setting values
	//    2. Set the position and target
	//    3. Get direction with position and target
	//    4. Get right and up direction of camera with world up dir (0,1,0)
	//    5. Set the view matrix with look at function
	//    6. Set the projection matrix with screen and FoV angle
	/*********************************************************/
	//    1. Set the setting values
	m_screenHeight = 1280;
	m_screenWidth = 720;

	m_FoV = 45.0f;
	m_cameraSpeed = 5.0f;
	m_mouseSpeed = 0.0007f;
	m_isMoving = true;

	//    2. Set the position and target
	m_position = glm::vec3(0.0f, 0.0f, 3.0f);
	m_target = glm::vec3(0.0f);

	//    3. Get direction with position and target
	m_direction = glm::normalize(m_target - m_position);

	//... in Init function
	Init();
}

Camera::~Camera()
{

}
/****************************************************************************
\Function Name : SetFirstValue
\Role :
	Set the first value (initialize the camera before starting) of camera

*****************************************************************************/
void Camera::SetFirstValue(void)
{
	//    2. Set the position and target
	m_position = glm::vec3(0.0f, 0.0f, 3.0f);
	m_target = glm::vec3(0.0f);

	//    3. Get direction with position and target
	m_direction = glm::normalize(m_target - m_position);

	//... in Init function
	Init();
}
/****************************************************************************
\Function Name : Init
\Role :
	Initialize the camera with position, target of camera

*****************************************************************************/
void Camera::Init(void)
{

	//    4. Get right and up direction of camera with world up dir (0,1,0)
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//m_right = glm::normalize(glm::cross(worldUp, m_direction));
	m_right = glm::normalize(glm::cross(m_direction, worldUp));
	m_up = glm::cross(m_right, m_direction);
	//m_up = glm::cross(m_direction, m_right);

	//    5. Set the view matrix with look at function
	m_viewMatrix = glm::lookAt(m_position,  // Camera is here
		m_position + m_direction, // Position to watch
		m_up); // Up direction of camera

	//    6. Set the projection matrix with screen and FoV angle
	m_projectionMatrix
		= glm::perspective(glm::radians(m_FoV), static_cast<float>(m_screenHeight / m_screenWidth), 0.1f, 100.0f);
}
/****************************************************************************
\Function Name : SetTarget
\Inputs :
	target_ - position of target

\Role :
	Set the target positon arbitrary.
	Of course, need to initialize again.

*****************************************************************************/
void Camera::SetTarget(glm::vec3 target_)
{
	m_target = target_;
	//    3. Get direction with position and target
	m_direction = glm::normalize(m_target - m_position);
	Init();
}
/****************************************************************************
\Function Name : Update
\Inputs :
	window_ - window object of this program
	dt_ - current delta time of this program

\Role :
	Update the camera with delta time in window.

*****************************************************************************/
void Camera::Update(GLFWwindow* window_, float dt_)
{
	// when camera is moving
	if (m_isMoving)
	{
		Moving(window_, dt_);
	}
	Init();
}
/****************************************************************************
\Function Name : Moving
\Inputs :
	window_ - window object of this program
	dt_ - current delta time of this program

\Role :
	Update the camera with delta time in window.

*****************************************************************************/
void Camera::Moving(GLFWwindow* window_, float dt_)
{
	// Move with Mouse Cursor
	double xPos, yPos;
	glfwGetCursorPos(window_, &xPos, &yPos);
	//glfwSetCursorPos(window_, m_screenWidth / 2, m_screenHeight / 2);

	// Compute new orientation
	m_direction -= (m_mouseSpeed * dt_ * static_cast<float>(static_cast<double>(m_screenWidth) / 2 - xPos)) * m_right;
	m_direction += (m_mouseSpeed * dt_ * static_cast<float>(static_cast<double>(m_screenHeight) / 2 - yPos)) * m_up;

	// Move with WASD
	float moveSpeed = m_cameraSpeed * dt_;

	// Move the camera with key input
	//	W	- up
	if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_position += moveSpeed * m_direction;
	}
	//	S	- down
	if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_position -= moveSpeed * m_direction;
	}
	//	A	- left
	if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_position -= moveSpeed * m_right;
	}
	//	D	- right
	if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_position += moveSpeed * m_right;
	}
	//	Space - stop the moving
	if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_isMoving = false;
	}
}
/****************************************************************************
\Function Name : SetMove
\Inputs :
	move_ - boolean to set with input

\Role :
	Set movement boolean with input

*****************************************************************************/
void Camera::SetMove(bool move_)
{
	m_isMoving = move_;
}
/****************************************************************************
\Function Name : SetFoV
\Inputs :
	FoV_ - float to set with input

\Role :
	Set angle of field of view with input

*****************************************************************************/
void Camera::SetFoV(float FoV_)
{
	m_FoV = FoV_;
}
/****************************************************************************
\Function Name : GetProjectionMat
\Role :
	Getter of projection matrix

*****************************************************************************/
glm::mat4 Camera::GetProjectionMat()
{
	return m_projectionMatrix;
}
/****************************************************************************
\Function Name : GetProjectionMat
\Role :
	Getter of view matrix

*****************************************************************************/
glm::mat4 Camera::GetViewMat()
{
	return m_viewMatrix;
}
