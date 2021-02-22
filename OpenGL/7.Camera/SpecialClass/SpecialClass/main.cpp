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

// Load texture
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Transform
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream> 

#include "SCShader.hpp" // Class 2
#include "SCCamera.hpp" // Class 7

const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;

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

    Shader coordinateShader("shaders/coordinateVertexShader.vert", "shaders/coordinateFragmentShader.frag");

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

    /*********************************************************/
    //* TEXTURE 
    //      - Load the texture(image) from the file
    //* Set
    //    1. Generate the texture
    //    2. Bind the buffer to proper target
    //    3. set the texture wrapping/filtering options
    //    4. load and generate the texture
    //    5. Free the image after loading
    //    6. Set the attribute of texture coordinate
    //* Draw
    //    7. Bind the texture before draw
    /*********************************************************/
    //  T1. Generate the texture
    unsigned int texture1;
    glGenTextures(1, &texture1);
    // texture 1
    //  T2. Bind the buffer to proper target
    glBindTexture(GL_TEXTURE_2D, texture1);
    //  T3. Set the texture wrapping/filtering options
    //  wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //  filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //  T4. load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("textures/ice_texture_2.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    //  T5. Free the image after loading
    stbi_image_free(data);

    //  T1. Generate the texture
    unsigned int  texture2;
    glGenTextures(1, &texture2);
    //  T2. Bind the buffer to proper target
    glBindTexture(GL_TEXTURE_2D, texture2);
    //  T3. Set the texture wrapping/filtering options
    //  wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //  filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //  T4. load and generate the texture
    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    //  T5. Free the image after loading
    stbi_image_free(data);

    //  T6. Set the attribute of texture coordinate
    unsigned int texCoords;
    glGenBuffers(1, &texCoords);
    glBindBuffer(GL_ARRAY_BUFFER, texCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture), texture, GL_STATIC_DRAW);
    glVertexAttribPointer(
        2,								// attribute 0. No particular reason for 0, but must match the layout in the shader.
        2,								// size
        GL_FLOAT,				// type
        GL_FALSE,				// normalized?
        2 * sizeof(float),	// stride - space between attributes
        (void*)0					// array buffer offset
    );
    glEnableVertexAttribArray(2);

    // Unbind vertex array
    glBindVertexArray(0);

    coordinateShader.Use(); 
    coordinateShader.SetInt("texture1", 0);
    coordinateShader.SetInt("texture2", 1);


    /*********************************************************/
    //* TRANSFORM 
    //      - Transform(translate, rotate) the object with glm
    /*********************************************************/
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    /*********************************************************/
    //* Coordinate System 
    //      - Move the object using coordinate system
    //      Clip = Projection x View x Model x Local
    //* Set
    //    1. Set model matrix (to World space)
    //    2. Set view matrix (Camera)
    //    3. Set projection matrix (Screen)
    //    4. Send it to the shader
    /*********************************************************/
    //  C1. Set model matrix (to World space)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    ////  C2. Set view matrix (Camera)
    //glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    ////  C3. Set projection matrix (Screen)
    //glm::mat4 projection;
    //projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    //  C4. Send it to the shader
    coordinateShader.Use();
    coordinateShader.SetMat4("model", model);
    //coordinateShader.SetMat4("view", view);
    //coordinateShader.SetMat4("projection", projection);

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
    coordinateShader.SetMat4("view", myCamera.GetViewMat());
    coordinateShader.SetMat4("projection", myCamera.GetProjectionMat());

    // Values for ImGui
    bool skeletonMode = false;
    float boxCol[3] = { 0.0f, 1.0f, 0.0f };
    float mixRatio = 0.2f;
    float FoV = 45.0f;

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

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
        glClear(GL_COLOR_BUFFER_BIT);

        //  T7. Bind the texture before draw
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //// BUFFER Drawing
        // VBO with EBO
        // V5. Use shader whatever you want
        //// BUFFER Drawing
        coordinateShader.Use();
        coordinateShader.SetFloat("mixRatio", mixRatio);
        coordinateShader.SetMat4("model", model);
        // Set v p with camera
        coordinateShader.SetMat4("view", myCamera.GetViewMat());
        coordinateShader.SetMat4("projection", myCamera.GetProjectionMat());

        myCamera.Update(window, deltaTime);

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
                coordinateShader.SetFloat("mixRatio", mixRatio);

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
    glDeleteBuffers(1, &EBO);

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