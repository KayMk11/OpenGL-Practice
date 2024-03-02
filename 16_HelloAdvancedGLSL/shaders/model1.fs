#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

uniform Material material;

void main()
{
    // if(gl_FrontFacing)
    //     FragColor = texture(material.specular, TexCoord);
    // else
    //     FragColor = vec4(1., 1., 0., 1.);
    FragColor = vec4(1., 0., 0., 1.);

}
