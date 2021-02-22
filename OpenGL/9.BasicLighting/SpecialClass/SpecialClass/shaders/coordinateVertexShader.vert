/* Start Header -------------------------------------------------------
File Name: coordinateVertexShader.vert
Purpose: vertex shader for coordinate shader
Language: glsl
Platform: vs2019 , window 10
Project: SpecialClass
Author: Jihwan Oh, jihwan315@gmail.com
Creation date: 01.15.2021
End Header --------------------------------------------------------*/
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    // get position with each matrix; MVP for each step
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
} 