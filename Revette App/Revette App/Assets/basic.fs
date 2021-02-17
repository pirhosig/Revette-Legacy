#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

uniform sampler2DArray tileAtlas;

void main()
{
    vec4 textureColour = texture(tileAtlas, TexCoord);
    if (textureColour.a < 0.1)
        discard;
    FragColor = textureColour;
}