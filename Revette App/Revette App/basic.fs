#version 330 core
out vec4 FragColor;

in uint TextureIndex;
in vec2 TexCoord;

uniform sampler2D tileAtlas;

void main()
{
    vec4 textureColour = texture(tileAtlas, TexCoord);
    FragColor = textureColour;
}