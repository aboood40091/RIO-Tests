#version 330 core

uniform vec4 mvp[4];

layout(location = 0) in vec3 v_inPos;
layout(location = 1) in vec2 v_inTexCoord;

out vec2 TexCoord;

void main()
{
    TexCoord = v_inTexCoord;

    vec4 pos = vec4(v_inPos, 1.0f);
    gl_Position = vec4(dot(mvp[0], pos), dot(mvp[1], pos), dot(mvp[2], pos), dot(mvp[3], pos));
}
