#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform int isLight;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 objectColor = texture(ourTexture, TexCoord);

    if (isLight == 1)
    {
        FragColor = objectColor;
    }
    else
    {
        FragColor = (vec4(ambient, 1.0) + vec4(diffuse, 1.0)) * objectColor;
    }
}