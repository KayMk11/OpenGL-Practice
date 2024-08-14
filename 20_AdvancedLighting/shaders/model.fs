

#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool blinn; 

void main()
{           
    vec3 planeColor = texture(floorTexture, fs_in.TexCoords).rgb;

    float ambientStrength = .1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = .5;
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.;
    if (blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.), 32);
    }
    else 
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(reflectDir, viewDir), 0.), 32);
    }
    vec3 specular = specularStrength * spec * lightColor;

    FragColor = vec4((ambient + specular) * planeColor, 1.0);
}

