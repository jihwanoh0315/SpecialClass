/* Start Header -------------------------------------------------------
File Name: colorVertexShader.vert
Purpose: vertex shader for color shader
Language: glsl
Platform: vs2019 , window 10
Project: SpecialClass
Author: Jihwan Oh, jihwan315@gmail.com
Creation date: 01.09.2021
End Header --------------------------------------------------------*/
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}