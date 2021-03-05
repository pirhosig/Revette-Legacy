#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in int aTextureIndex;
layout (location = 2) in vec2 aTexCoord;

out vec3 TexCoord;

uniform mat4 vertexTransformationMatrix;

void main()
{
    vec4 actualPosition = vec4(aPos, 0.0f, 1.0f);
    gl_Position = vertexTransformationMatrix * actualPosition;
    TexCoord = vec3(aTexCoord / 128.0f, aTextureIndex);
}