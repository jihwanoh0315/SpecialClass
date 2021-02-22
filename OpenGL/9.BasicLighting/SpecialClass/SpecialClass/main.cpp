/*******************************************************************************
filename			: main.cpp
author			: Jihwan Oh
email				: dkdkdkdudrn@gmail.com
date				: 2021.01.04

description:
  Main File of Special classs.
  We will learn basic graphic with LearnOpenGL Website.

*******************************************************************************/
// imgui, Class 3
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>

// OpenGL API, Class 1
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Transform
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream> 

#include "SCShader.hpp"     // Class 2
#include "SCCamera.hpp"   // Class 7
#include "SCObject.hpp"     // Class 8
#include "SCModel.hpp"     // Class 8


const unsigned int SCR_WIDTH = 900, SCR_HEIGHT = 800;

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
float texture[] = {
    // texture coords
    1.0f, 1.0f,   // top right
    1.0f, 0.0f,   // bottom right
    0.0f, 0.0f,   // bottom left
    0.0f, 1.0f    // top left 
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MainWindow", NULL, NULL);
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
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

    Shader basiclightShader("shaders/basicLightingVertexShader.vert", "shaders/basicLightingFragmentShader.frag");
    Shader lightsourceShader("shaders/lightsourceVertexShader.vert", "shaders/lightsourceFragmentShader.frag");


    ModelLoader modelList;
    modelList.LoadOBJ("models/orca.obj", "orca", true);
    modelList.LoadOBJ("models/simpleSphere.obj", "sphere", true);


    Object orca;
    orca.SetModel(modelList.GetModel("orca"));
    orca.m_color = glm::vec3(0.3f, 0.8f, 1.0f);


    Object lightSphere;
    lightSphere.SetModel(modelList.GetModel("sphere"));
    lightSphere.m_color = glm::vec3(1.0f, 1.0f, 1.0f);

    lightSphere.m_position.x += 0.5f;
    //lightSphere.m_position.y += 0.5f;
    lightSphere.m_scale *= 0.25f;
    //exampleObj.LoadTexture("textures/awesomeface.png");

    /*********************************************************/
    //* TRANSFORM 
    //      - Transform(translate, rotate) the object with glm
    /*********************************************************/
    //exampleObj.m_position += glm::vec3(0.5f, -0.5f, 0.0f);
    //exampleObj.m_rotation.z = (float)glfwGetTime();

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
    Camera myCamera;

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    myCamera.SetMove(false);
    //cubeObject.SetDebugMode(true);
    //orca.SetDebugMode(true);

    // Values for ImGui
    bool skeletonMode = false;
    bool lightMove = true;
    float lightCol[3] = { 1.0f, 1.0f, 1.0f };
    float mixRatio = 0.2f;
    float FoV = 45.0f;
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    float startAngle = 0.0f;

    basiclightShader.Use();
    basiclightShader.SetFloat("ambientStrength", ambientStrength);
    basiclightShader.SetVec3("lightColor", lightSphere.m_color);

    // do the loop of the window until it close
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input checking
        processInput(window);

        // I2. Initialize the frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering Commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Color of screen when clear
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myCamera.Update(window, deltaTime);

        basiclightShader.SetBool("isLight", false);
        orca.Update(basiclightShader, myCamera);
        basiclightShader.SetBool("isLight", true);
        lightSphere.Update(basiclightShader, myCamera);
        // I3. Design a window with codes
        //ImGui::ShowDemoWindow();
        {
            ImGui::Begin("Camera Control");
            if (ImGui::Button("Camera Moving"))
            {
                myCamera.SetMove(true);
            }
            ImGui::SameLine();
            if (ImGui::Button("Camera Reset"))
            {
                myCamera.SetFirstValue();
            }
            if (ImGui::SliderFloat("Camera FoV", &FoV, 1.0f, 90.0f, "%.1f"))
                myCamera.SetFoV(FoV);
            ImGui::Separator();
            if (ImGui::SliderFloat("Mix Ratio", &mixRatio, 0.0f, 1.0f, "%.1f"))
                basiclightShader.SetFloat("mixRatio", mixRatio);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        {
            ImGui::Begin("Light Control");
            ImGui::Checkbox("Light Moving", &lightMove);
            
            if (ImGui::SliderFloat("Ambient", &ambientStrength, 0.0f, 1.0f, "%.1f"))
                basiclightShader.SetFloat("ambientStrength", ambientStrength);
            if (ImGui::SliderFloat("Specular", &specularStrength, 0.0f, 1.0f, "%.1f"))
                basiclightShader.SetFloat("specularStrength", specularStrength);
            if (ImGui::ColorEdit3("LightColor", lightCol))
                lightSphere.m_color = glm::vec3(lightCol[0], lightCol[1], lightCol[2]);
            ImGui::End();
        }

        basiclightShader.SetVec3("lightColor", lightSphere.m_color);
        basiclightShader.SetVec3("lightPos", lightSphere.m_position);
        basiclightShader.SetVec3("viewPos", myCamera.m_position);


        if (lightMove)
        {
            // Spin the light sphere with dt
            startAngle += deltaTime;
            lightSphere.m_position = 0.5f * glm::vec3(glm::cos(startAngle), 0.0f, glm::sin(startAngle));
        }

        //    I4. Draw
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

         // Swap the buffer (for Double Buffering)
        glfwSwapBuffers(window);
        // Get events
        glfwPollEvents();
    }

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