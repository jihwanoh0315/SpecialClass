/*******************************************************************************
filename			: SCShader.cpp
author			: Jihwan Oh
email				: jihwan315@gmail.com, dkdkdkdudrn@gmail.com
date				: 2021.01.06

description:
    Codes for Shader class.
    Funcions in this shader.

    void Use(void);
    void SetBool(const std::string& name_, bool value_) const;
    void SetInt(const std::string& name_, int value_) const;
    void SetuInt(const std::string& name_, unsigned int value_) const;
    void SetFloat(const std::string& name_, float value_) const;
    void SetVec3(const std::string& name_, const glm::vec3& value_) const;
    void SetVec3(const std::string& name_, float x_, float y_, float z_) const;
    void SetVec4(const std::string& name_, const glm::vec4& value_) const;
    void SetVec4(const std::string& name_, float x_, float y_, float z_, float w_) const;

    void SetMat4(const std::string& name_, const glm::mat4& mat) const;

*******************************************************************************/
#include <glad/glad.h>
#include <glm/vector_relational.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "SCShader.hpp"
/****************************************************************************
\Function Name : Shader(constructor)
\Inputs :
    vertex_file_path_   - path of vertex shader file
    fragment_file_path_ - path of fragment shader file

\Role :
    Constructor of shader class

*****************************************************************************/
Shader::Shader(const char* vertex_file_path_, const char* fragment_file_path_)
{
    // Read the shader file 
    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream;
    vertexShaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexShaderStream.open(vertex_file_path_, std::ios::in);
        std::stringstream sstr;
        sstr << vertexShaderStream.rdbuf();
        vertexShaderCode = sstr.str();
        vertexShaderStream.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR : VERTEX SHADER READING IS FAILED!!" << std::endl;
        std::cout << "Impossible to open " << vertex_file_path_ << ". Are you in the right directory ?" << std::endl;
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream;
    fragmentShaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        fragmentShaderStream.open(fragment_file_path_, std::ios::in);
        std::stringstream sstr;
        sstr << fragmentShaderStream.rdbuf();
        fragmentShaderCode = sstr.str();
        fragmentShaderStream.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR : FRAGMENT SHADER READING IS FAILED!!" << std::endl;
        std::cout << "Impossible to open " << fragment_file_path_ << ". Are you in the right directory ?" << std::endl;
        getchar();
    }

    /*********************************************************/
    //* SHADER 
    //      - functions to compile graphic codes
    //* How to set Shader in GLFW; shader is also 'Object'
    //    1. Generate the ID and shader
    //    2. Set source of shader and compile
    //    2-2. Check compilation is done successful
    //    3. Create the program for shaders
    //    4. Attach and link the shaders to program
    //    4-2. Check linking is done successful
    //    5. Detach and delete the shaders
    /*********************************************************/

    // For Vertex Shader
    //    1. Generate the ID
    unsigned int vertexShaderID;
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    //    2. Set source of shader and compile
    char const* vertexShaderSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexShaderSourcePointer, NULL);
    glCompileShader(vertexShaderID);
    //    2-2. Check compilation is done successful
    GLint success = GL_FALSE;
    int InfoLogLength = 0;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (!success)
    {
        std::vector<char> errorMessage(static_cast<__int64>(InfoLogLength) + 1);
        glGetShaderInfoLog(vertexShaderID, InfoLogLength, NULL, &errorMessage[0]);
        std::cout << "ERROR : VERTEX SHADER COMPILATION IS FAILED\n" << &errorMessage[0] << std::endl;
    }

    // For Fragment Shader
    //    1. Generate the ID and shader
    unsigned int fragmentShaderID;
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    //    2. Set source of shader and compile
    char const* fragmentShaderSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSourcePointer, NULL);
    glCompileShader(fragmentShaderID);
    //    2-2. Check compilation is done successful
    success = GL_FALSE;
    InfoLogLength = 0;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (!success)
    {
        std::vector<char> errorMessage(static_cast<__int64>(InfoLogLength) + 1);
        glGetShaderInfoLog(fragmentShaderID, InfoLogLength, NULL, &errorMessage[0]);
        std::cout << "ERROR : FRAGMENT SHADER COMPILATION IS FAILED\n" << &errorMessage[0] << std::endl;
    }

    //Linking Shader Program
    //    3. Create the program for shaders
    m_programID = glCreateProgram();
    //    4. Attach and link the shaders
    glAttachShader(m_programID, vertexShaderID);
    glAttachShader(m_programID, fragmentShaderID);
    glLinkProgram(m_programID);
    //    4-2. Check linking is done successful
    glGetShaderiv(m_programID, GL_LINK_STATUS, &success);
    glGetShaderiv(m_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (!success)
    {
        std::vector<char> errorMessage(static_cast<__int64>(InfoLogLength) + 1);
        glGetShaderInfoLog(m_programID, InfoLogLength, NULL, &errorMessage[0]);
        std::cout << "ERROR : SHADER LINKING COMPILATION IS FAILED\n" << &errorMessage[0] << std::endl;
    }

    //    5. Detach and delete the shaders
    glDetachShader(m_programID, fragmentShaderID);
    glDetachShader(m_programID, vertexShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

Shader::~Shader()
{
}

/****************************************************************************
\Function Name : Use

\Role :
    Use the shader with ID

*****************************************************************************/
void Shader::Use(void)
{
    glUseProgram(m_programID);
}

/****************************************************************************
\Function Name : Set...
\Inputs :
    name_ - name of value to set
    value_ - actual value

\Role :
    Setter of values for shader

*****************************************************************************/
void Shader::SetBool(const std::string& name_, bool value_) const
{
    glUniform1i(glGetUniformLocation(m_programID, name_.c_str()), static_cast<int>(value_));
}

void Shader::SetInt(const std::string& name_, int value_) const
{
    glUniform1i(glGetUniformLocation(m_programID, name_.c_str()), value_);
}

void Shader::SetuInt(const std::string& name_, unsigned int value_) const
{
    glUniform1ui(glGetUniformLocation(m_programID, name_.c_str()), value_);
}

void Shader::SetFloat(const std::string& name_, float value_) const
{
    glUniform1f(glGetUniformLocation(m_programID, name_.c_str()), value_);
}

void Shader::SetVec3(const std::string& name_, const glm::vec3& value_) const
{
    glUniform3fv(glGetUniformLocation(m_programID, name_.c_str()), 1, &value_[0]);
}

void Shader::SetVec3(const std::string& name_, float x_, float y_, float z_) const
{
    glUniform3f(glGetUniformLocation(m_programID, name_.c_str()), x_, y_, z_);
}

void Shader::SetVec4(const std::string& name_, const glm::vec4& value_) const
{
    glUniform4fv(glGetUniformLocation(m_programID, name_.c_str()), 1, &value_[0]);
}

void Shader::SetVec4(const std::string& name_, float x_, float y_, float z_, float w_) const
{
    glUniform4f(glGetUniformLocation(m_programID, name_.c_str()), x_, y_, z_, w_);
}

void Shader::SetMat4(const std::string& name_, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_programID, name_.c_str()), 1, false, &mat[0][0]);
}

unsigned int Shader::GetProgramID(void)
{
    return m_programID;
}
