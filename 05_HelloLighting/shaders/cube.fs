#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = .1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = .5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm); 
    float spec = pow(max(dot(reflectDir, viewDir), 0.), 32);
    vec3 specular = specularStrength * spec * lightColor;
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}