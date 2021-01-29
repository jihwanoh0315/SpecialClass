/* Start Header -------------------------------------------------------
File Name: basicVertexShader.vert
Purpose: vertex shader for basic shader
Language: glsl
Platform: vs2019 , window 10
Project: SpecialClass
Author: Jihwan Oh, jihwan315@gmail.com
Creation date: 01.07.2021
End Header --------------------------------------------------------*/
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}