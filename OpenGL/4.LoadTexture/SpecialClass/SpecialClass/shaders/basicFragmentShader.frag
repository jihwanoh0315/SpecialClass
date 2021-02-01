/* Start Header -------------------------------------------------------
File Name: basicFragmentShader.frag
Purpose: fragement shader for basic shader
Language: glsl
Platform: vs2019 , window 10
Project: SpecialClass
Author: Jihwan Oh, jihwan315@gmail.com
Creation date: 01.07.2021
End Header --------------------------------------------------------*/
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}