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

const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;
const unsigned int SHADER_COUNT = 4;

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

    Shader basicShader("shaders/basicVertexShader.vert", "shaders/basicFragmentShader.frag");
    Shader colorShader("shaders/colorVertexShader.vert", "shaders/colorFragmentShader.frag");
    Shader textureShader("shaders/textureVertextShader.vert", "shaders/textureFragmentShader.frag");
    Shader transformShader("shaders/transformVertexShader.vert", "shaders/transformFragmentShader.frag");

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

    textureShader.Use(); 
    textureShader.SetInt("texture1", 0);
    textureShader.SetInt("texture2", 1);

    transformShader.Use(); 
    transformShader.SetInt("texture1", 0);
    transformShader.SetInt("texture2", 1);

    /*********************************************************/
    //* TRANSFORM 
    //      - transform the object with matrices
    /*********************************************************/
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime() / 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    int changeShader = SHADER_COUNT;
    bool skeletonMode = false;
    float boxCol[3] = { 0.0f, 1.0f, 0.0f };
    float mixRatio = 0.2f;

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

        //  T7. Bind the texture before draw
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //// BUFFER Drawing
        // VBO with EBO
        // V5. Use shader whatever you want
        //// BUFFER Drawing
        if (changeShader == 1) // basic window
        {
            basicShader.Use();
        }
        else if (changeShader == 2) // color change
        {
            colorShader.Use();
            float time = sin(glfwGetTime());
            colorShader.SetVec4("ourColor", boxCol[0] * time, boxCol[1] * time, boxCol[2] * time, 1.0f);
        }
        else if (changeShader == 3) // texture
        {
            textureShader.Use();
            textureShader.SetFloat("mixRatio", mixRatio);
        }
        else if (changeShader == 4)
        {
            transformShader.Use();
            transformShader.SetFloat("mixRatio", mixRatio);
            transformShader.SetMat4("transform", trans);
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
        //ImGui::ShowDemoWindow();
        {
            ImGui::Begin("Basic Window");
            if(ImGui::Button("Change Shader"))
            {
                ++changeShader;
                if (changeShader > SHADER_COUNT)
                    changeShader = 1;
            }
            ImGui::SameLine();
            ImGui::Checkbox("Skeleton Mode", &skeletonMode);
            if (changeShader == 2)
            {
                ImGui::Separator();
                ImGui::ColorEdit3("Box Color", boxCol);
            }
            else if (changeShader == 3)
            {
                ImGui::Separator();
                if(ImGui::SliderFloat("Mix Ratio", &mixRatio, 0.0f, 1.0f, "%.1f"))
                    textureShader.SetFloat("mixRatio", mixRatio);
            }
            else
            {
                ImGui::Separator();
                if (ImGui::SliderFloat("Mix Ratio", &mixRatio, 0.0f, 1.0f, "%.1f"))
                    transformShader.SetFloat("mixRatio", mixRatio);
            }
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