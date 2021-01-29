/*******************************************************************************
filename			: main.cpp
author			: Jihwan Oh
email				: jihwan315@gmail.com, dkdkdkdudrn@gmail.com
date				: 2021.01.04

description:
  Main File of Special classs.
  We will make simple graphic frameworks.

*******************************************************************************/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream> 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(void)
{
    // Init with version of OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Set and Create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "SCWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Check the environment is ready for GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set viewport. This is Opengl, so 0,0 is bottom left
    glViewport(0, 0, 800, 600);

    // Set something when size of window is changed
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // do the loop of the window until it close
    while (!glfwWindowShouldClose(window))
    {
        //!< Inputs
        processInput(window);

        //!< Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //!< GLFW Functions
        glfwPollEvents(); // Check all events
        glfwSwapBuffers(window); // Swap the buffer
    }

    // deallocate all and close the window before close
    glfwTerminate();
	return 0;
}

/****************************************************************************
\Function Name : framebuffer_size_callback
\Inputs :
    window_ - window to info
    width_ - width of window
    height_ - height of window
\Output :
    NONE
\Role :
    Reset the viewport of the window.
    This is callback function for the window.
    This will call when window is resized.

*****************************************************************************/
void framebuffer_size_callback(GLFWwindow* /*window_*/, int width_, int height_)
{
    glViewport(0, 0, width_, height_);
}

/****************************************************************************
\Function Name : processInput
\Inputs :
    window_ - window to get key info
\Output :
    NONE
\Role :
    Get the key info from window_ and react with this function

*****************************************************************************/
void processInput(GLFWwindow* window_)
{
    // when ESC is pressed
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // Close the window
        glfwSetWindowShouldClose(window_, true);
    }
}