#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct DirectionalLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outercutoff;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLight[4];
uniform SpotLight spotLight;

vec3 CalculateDirectionLight(DirectionalLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.), material.shininess);
    
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).xyz;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).xyz;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).xyz;

    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 FragPos)
{
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = dot(norm, lightDir);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.), material.shininess);
    
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).xyz;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).xyz;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).xyz;

    float distance = length(light.position - FragPos);
    float attenuation = 1. / (light.constant + light.linear * distance + light.quadratic * distance * distance); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir, vec3 FragPos)
{
        vec3 lightDir = normalize(light.position - FragPos);

    float diff = dot(norm, lightDir);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.), material.shininess);
    
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).xyz;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).xyz;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).xyz;

    float distance = length(light.position - FragPos);
    float attenuation = 1. / (light.constant + light.linear * distance + light.quadratic * distance * distance); 

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outercutoff;
    float intensity = clamp((theta - light.outercutoff)/ epsilon, 0.0, 1.0); 

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalculateDirectionLight(dirLight, norm, viewDir);

    for(int i = 0; i < 4; i++)
    {
        result += CalculatePointLight(pointLight[i], norm, viewDir, FragPos);
    }

    result += calculateSpotLight(spotLight, norm, viewDir, FragPos);

    FragColor = vec4(result, 1.);
}