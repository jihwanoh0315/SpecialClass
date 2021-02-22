/* Start Header -------------------------------------------------------
File Name: basicLightingFragmentShader.frag
Purpose: fragment shader for  basic lighting shader
Language: glsl
Platform: vs2019 , window 10
Project: SpecialClass
Author: Jihwan Oh, jihwan315@gmail.com
Creation date: 02.12.2021
End Header --------------------------------------------------------*/
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 objectColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float mixRatio;
uniform bool isLight;

uniform float ambientStrength;
uniform float specularStrength;

void main()
{
    if(isLight)
    {
	    FragColor = vec4(objectColor, 1.0f);
    }
    else
    {
	    // ambient
        vec3 ambient = ambientStrength * lightColor;

	    // diffuse
	    vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;

        vec3 result = (ambient + diffuse + specular) * objectColor;
	    FragColor = vec4(result, 1.0f);
    }
}