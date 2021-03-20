#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 vertexTransformationMatrix;
uniform vec2 scaleVector;

void main()
{
    vec2 scaledPosition = aPos * scaleVector;
    vec4 actualPosition = vec4(scaledPosition, 0.1f, 1.0f);
    gl_Position = vertexTransformationMatrix * actualPosition;
    TexCoord = vec2(aTexCoord / 255.0f);
}