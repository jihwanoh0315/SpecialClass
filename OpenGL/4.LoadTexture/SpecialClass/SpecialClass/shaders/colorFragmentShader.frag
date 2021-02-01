/* Start Header -------------------------------------------------------
File Name: colorFragmentShader.frag
Purpose: fragement shader for color shader
Language: glsl
Platform: vs2019 , window 10
Project: SpecialClass
Author: Jihwan Oh, jihwan315@gmail.com
Creation date: 01.09.2021
End Header --------------------------------------------------------*/
#version 330 core
out vec4 FragColor;
  
uniform vec4 ourColor;

void main()
{
    FragColor = ourColor;
}   