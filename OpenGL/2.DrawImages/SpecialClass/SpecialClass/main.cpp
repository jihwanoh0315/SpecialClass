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

#include "SCShader.hpp"    // Class 2

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/* Vertices for the triangle */
float vertices[] = {
    -0.5f, -0.5f, 0.0f, // left  
     0.5f, -0.5f, 0.0f, // right 
     0.0f,  0.5f, 0.0f  // top   
};

int main(void)
{
    // Initialize the window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    int width = 1280, height = 720;
    GLFWwindow* window = glfwCreateWindow(width, height, "MainWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD with 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set viewport
    glViewport(0, 0, width, height);

    // Change size of window during runtime
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    Shader basicShader("Shaders/basicVertexShader.vert", "Shaders/basicFragmentShader.frag");

    //==================TWO======================================
    /*********************************************************/
    //* BUFFER 
    //      - one of 'Object' in OpenGL to send vertices to graphic card
    //* How to Change value of OBJECT in GLFW
    //* Set
    //    1. Generate the buffer
    //    2. Bind the buffer to proper target
    //    3. Copy arrays to buffer data
    //    4. Set the vertex attributes pointers
    //* Draw
    //    5. Use shader whatever you want
    /*********************************************************/
    //// FOR VAO
    unsigned int VAO;
    //// FOR VBO - V
    // V1. Generate the buffer
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // V2. Bind the buffer to proper target
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // V3. Copy arrays to buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ////VBO
    // V4. Set the vertex attributes pointers
    glEnableVertexAttribArray(0); // Set attribue same with layout in the shader
    glVertexAttribPointer(
        0,								// attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,								// size
        GL_FLOAT,				// type
        GL_FALSE,				// normalized?
        3 * sizeof(float),	// stride - space between attributes
        (void*)0					// array buffer offset
    );

    // Unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind vertex array
    glBindVertexArray(0);

    //==================TWO END=================================

    // do the loop of the window until it close
    while (!glfwWindowShouldClose(window))
    {
        // Input checking
        processInput(window);

        // Rendering Commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Color of screen when clear
        glClear(GL_COLOR_BUFFER_BIT);

        //// BUFFER Drawing
        // VBO with EBO
        // V5. Use shader whatever you want
        basicShader.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // Don't need to unbind everytime
         //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

         // Swap the buffer (for Double Buffering)
        glfwSwapBuffers(window);
        // Get events
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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