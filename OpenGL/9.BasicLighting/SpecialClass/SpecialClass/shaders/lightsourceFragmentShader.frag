/* Start Header -------------------------------------------------------
File Name: coordinateFragmentShader.frag
Purpose: fragment shader for  coordinate shader
Language: glsl
Platform: vs2019 , window 10
Project: SpecialClass
Author: Jihwan Oh, jihwan315@gmail.com
Creation date: 01.12.2021
End Header --------------------------------------------------------*/
#version 330 core
out vec4 FragColor;

in vec3 objectColor;

void main()
{
	FragColor = vec4(objectColor, 1.0f);
}