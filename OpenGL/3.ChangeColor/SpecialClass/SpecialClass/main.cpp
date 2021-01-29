/*******************************************************************************
filename			: main.cpp
author			: Jihwan Oh
email				: jihwan315@gmail.com, dkdkdkdudrn@gmail.com
date				: 2021.01.04

description:
  Main File of Special classs.
  We will make simple graphic frameworks.

*******************************************************************************/
// imgui Class 3
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream> 

#include "SCShader.hpp" // Class 2

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window_);

/* Vertices for the Square */
float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

int main(void)
{
    // Initialize the window
    glfwInit();
    const char* glsl_version = "#version 130";
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

    // Change size of window during runtime
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);
    int nrAttributes;

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // Set viewport
    glViewport(0, 0, width, height);
    /*********************************************************/
    //* ImGui
    //      - Easy UI created with c++
    //* Set
    //    1. Create context and link to window
    //    2. Initialize the frame
    //    3. Design a window with codes
    //      a. Each window starts with ImGui::Begin("window's name");
    //      b. End with ImGui::End();
    //    4. Render
    //    5. Shutdown and clean
    //    
    /*********************************************************/
    /////ImGui - I
    // I1. Create context and link to window
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool show_demo_window = true;

    Shader basicShader("shaders/basicVertexShader.vert", "shaders/basicFragmentShader.frag");
    Shader colorShader("shaders/colorVertexShader.vert", "shaders/colorFragmentShader.frag");

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
    unsigned int VBO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    //// FOR VBO - V
    // V1. Generate the buffer
    glGenBuffers(1, &VBO);
    //// FOR EBO - E
    // E1. Generate the buffer
    glGenBuffers(1, &EBO);
    // V2. Bind the buffer to proper target
    glBindVertexArray(VAO);
    // V3. Copy arrays to buffer data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // E3. Copy arrays to buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ////VBO
    // V4. Set the vertex attributes pointers
    glVertexAttribPointer(
        0,								// attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,								// size
        GL_FLOAT,				// type
        GL_FALSE,				// normalized?
        3 * sizeof(float),	// stride - space between attributes
        (void*)0					// array buffer offset
    );
    glEnableVertexAttribArray(0); // Set attribue same with layout in the shader

    // Unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind vertex array
    glBindVertexArray(0);

    bool changeShader = false;
    bool skeletonMode = false;
    float boxCol[3] = { 0.0f, 1.0f, 0.0f };

    // do the loop of the window until it close
    while (!glfwWindowShouldClose(window))
    {
        // Input checking
        processInput(window);

        // I2. Initialize the frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering Commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Color of screen when clear
        glClear(GL_COLOR_BUFFER_BIT);


        //// BUFFER Drawing
        // VBO with EBO
        // V5. Use shader whatever you want
        if (changeShader)
        {
            basicShader.Use();
        }
        else
        {
            colorShader.Use();
            float time = sin(glfwGetTime());
            colorShader.SetVec4("ourColor", boxCol[0] * time, boxCol[1] * time, boxCol[2] * time, 1.0f);
        }
        if (skeletonMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        // glBindVertexArray(0); // Don't need to unbind everytime

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // I3. Design a window with codes
        //if (show_demo_window)
        //    ImGui::ShowDemoWindow(&show_demo_window);
        {
            ImGui::Begin("Basic Window");
            ImGui::Checkbox("Change Shader", &changeShader);
            ImGui::SameLine();
            ImGui::Checkbox("Skeleton Mode", &skeletonMode);
            ImGui::Separator();
            ImGui::ColorEdit3("Box Color", boxCol);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        //    I4. Draw
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

         // Swap the buffer (for Double Buffering)
        glfwSwapBuffers(window);
        // Get events
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // deallocate all and close the window before close
    //  I5. Shutdown and clean
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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