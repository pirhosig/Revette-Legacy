#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in uint aTextureIndex;
layout (location = 2) in vec2 aTexCoord;

out uint TextureIndex;
out vec2 TexCoord;

uniform vec2 vertexOffset;

void main()
{
    vec2 actualPosition = aPos + vertexOffset;
    vec2 finalPosition = vec2(actualPosition.x / 10.0f, actualPosition.y / 10.0f);
    gl_Position = vec4(finalPosition, 0.0f, 1.0f);
    TextureIndex = aTextureIndex;
    TexCoord = aTexCoord;
}