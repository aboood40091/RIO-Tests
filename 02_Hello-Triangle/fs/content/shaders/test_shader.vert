#version 330 core
layout(location = 0) in vec3 v_inPos;

void main()
{
    gl_Position = vec4(v_inPos, 1.0);
}
