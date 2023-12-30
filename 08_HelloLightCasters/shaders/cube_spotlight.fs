#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light{
    vec3 direction;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutoff;
    float outercutoff;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
    float distance = length(light.position - FragPos);
    vec3 lightDir = normalize(light.position - FragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outercutoff;
    float intensity = clamp((theta - light.outercutoff) / epsilon, 0., 1.);
    
        //ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

        // Diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).xyz;

        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoord).xyz;

        diffuse = diffuse * intensity;
        specular = specular * intensity;
        FragColor = vec4(ambient + diffuse + specular, 1.);
}