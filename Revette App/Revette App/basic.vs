#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in uint aTextureIndex;
layout (location = 2) in vec2 aTexCoord;

out uint TextureIndex;
out vec2 TexCoord;

uniform mat4 vertexTransformationMatrix;

void main()
{
    vec4 actualPosition = vec4(aPos, 0.0f, 1.0f);
    gl_Position = vertexTransformationMatrix * actualPosition;
    TextureIndex = aTextureIndex;
    TexCoord = aTexCoord;
}