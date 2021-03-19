#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D entityTexture;

void main()
{
    vec4 textureColour = texture(entityTexture, TexCoord);
    if (textureColour.a < 0.1)
        discard;
    FragColor = textureColour;
}