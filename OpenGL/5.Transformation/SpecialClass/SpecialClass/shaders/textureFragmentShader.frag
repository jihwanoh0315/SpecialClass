/* Start Header -------------------------------------------------------
File Name: textureFragmentShader.frag
Purpose: fragment shader for texture shader
Language: glsl
Platform: vs2019 , window 10
Project: SpecialClass
Author: Jihwan Oh, jihwan315@gmail.com
Creation date: 01.11.2021
End Header --------------------------------------------------------*/
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixRatio;

void main()
{
	// mix with linear interpolation using mix function depends on ratio
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixRatio);
}