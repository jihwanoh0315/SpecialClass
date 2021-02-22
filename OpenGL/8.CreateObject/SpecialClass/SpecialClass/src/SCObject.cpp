/*******************************************************************************
filename			: SCObject.cpp
author			: Jihwan Oh
email				: dkdkdkdudrn@gmail.com
date				: 2021.01.28

description:
    Codes for Shader class.

*******************************************************************************/
#include <glad/glad.h>
#include <glm/vector_relational.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

// Load texture
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "SCObject.hpp"
#include "SCModel.hpp"
#include "SCShader.hpp"
#include "SCCamera.hpp"

Object::Object()
{
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
    glGenVertexArrays(1, &m_VertexArrayID);
    //// FOR VBO - V
    // V1. Generate the buffer
    glGenBuffers(1, &m_vertexBuffer);
    //// FOR INDEX - E
    // E1. Generate the buffer
    glGenBuffers(1, &m_indexBuffer);
    // T1. Generate texture buffer
    glGenBuffers(1, &m_textureUVBuffer);
    glGenBuffers(1, &m_textureIndexBuffer);

    m_model = nullptr;
    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

    m_skeletonMode = false;
}

Object::~Object()
{
    m_model = 0;

    glDeleteVertexArrays(1, &m_VertexArrayID);
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_indexBuffer);
    glDeleteBuffers(1, &m_textureUVBuffer);
    glDeleteBuffers(1, &m_textureIndexBuffer);
}

void Object::Init(void)
{

}

void Object::SetModel(Model* model_)
{
    m_model = model_;

    /***********************************************************

        Vertex
    
    ***********************************************************/
    // V2. Bind the buffer to proper target
    glBindVertexArray(m_VertexArrayID);
    // V3. Copy arrays to buffer data
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_model->m_vertices.size() * sizeof(glm::vec3),
                                                                    &m_model->m_vertices[0], GL_STATIC_DRAW);
    // E3. Copy arrays to buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_model->m_indicies.size() * sizeof(unsigned int),
                                                                                    &m_model->m_indicies[0], GL_STATIC_DRAW);

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
    //    6. Set the attribute of texture coordinate
    /*********************************************************/
    //  T6. Set the attribute of texture coordinate
    glBindBuffer(GL_ARRAY_BUFFER, m_textureUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_model->m_texture.size() * sizeof(glm::vec2),
                                                                    &m_model->m_texture[0], GL_STATIC_DRAW);
    // E3. Copy arrays to buffer data
    //glBindBuffer(GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING, m_textureIndexBuffer);
    //glBufferData(GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING, m_model->m_texIndicies.size() * sizeof(unsigned int),
    //                                                                                      &m_model->m_texIndicies[0], GL_STATIC_DRAW);

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

    
}

void Object::LoadTexture(const char* texture_file_path_)
{
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
    unsigned int textureID;
    glGenTextures(1, &textureID);
    // texture 1
    //  T2. Bind the buffer to proper target
    glBindTexture(GL_TEXTURE_2D, textureID);
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
    unsigned char* data = stbi_load(texture_file_path_, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture : "<< texture_file_path_ << std::endl;
    }
    //  T5. Free the image after loading
    stbi_image_free(data);

    m_textureBuffer.push_back(textureID);
    glTexCoordPointer(2, GL_FLOAT, 0, &m_model->m_texIndicies[0]);

}

void Object::Update(Shader& shader_, Camera& camera_)
{
    shader_.Use();

    /*********************************************************/
    //* Coordinate System 
    //      - Move the object using coordinate system
    //      Clip = Projection x View x Model x Local
    //* Set
    //    1. Set model matrix (to World space)
    //    2. Set view matrix (Camera) - from camera
    //    3. Set projection matrix (Screen) - from camera
    //    4. Send it to the shader
    /*********************************************************/
    // C1. Set model matrix (to World space)
    // Create model space with position, rotation of object
    glm::mat4 model = glm::mat4(1.0f);
    // rotate with x-axis
    model = glm::rotate(model, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    // rotate with y-axis
    model = glm::rotate(model, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    // rotate with z-axis
    model = glm::rotate(model, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    // set scale
    model[0][0] = m_scale.x;
    model[1][1] = m_scale.y;
    model[2][2] = m_scale.z;

    // set position
    model[3][0] = m_position.x;
    model[3][1] = m_position.y;
    model[3][2] = m_position.z;

    // Set model matrix
    shader_.SetMat4("model", model);

    //  C2. Set view matrix (Camera) with camera
    //  C3. Set projection matrix (Screen) with camera

    //  C4. Send it to the shader
    shader_.SetMat4("view", camera_.GetViewMat());
    shader_.SetMat4("projection", camera_.GetProjectionMat());

    /*********************************************************/
    //* TEXTURE 
    //      - Load the texture(image) from the file
    //* Draw
    //    7. Bind the texture before draw
    /*********************************************************/
    //  Texture 
    for (int i = 0; i < m_textureBuffer.size(); ++i)
    {
        //  T7. Bind the texture before draw
        std::string texName = std::string("texture") + std::to_string(i + 1);
        shader_.SetInt(texName.c_str(), i);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textureBuffer[i]);
    }

    if (m_skeletonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(m_VertexArrayID);
    glDrawElements(GL_TRIANGLES, m_model->m_indicies.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Don't need to unbind everytime
}

void Object::SetDebugMode(bool dbMode_)
{
    m_skeletonMode = dbMode_;
}
