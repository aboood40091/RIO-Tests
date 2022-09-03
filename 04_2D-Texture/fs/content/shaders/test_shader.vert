#version 330 core

layout(location = 0) in vec3 v_inPos;
layout(location = 1) in vec2 v_inTexCoord;

out vec2 TexCoord;

void main()
{
    TexCoord = v_inTexCoord;
    gl_Position = vec4(v_inPos, 1.0);
}
