#version 420 core               // Windows

//#version 330 core               // Cafe
//#extension GL_EXT_Cafe : enable // ....

layout(std140, binding = 1)
uniform cViewBlock
{
    vec3 viewPos;
    vec4 viewProj[4];
};

layout(std140, binding = 2)
uniform cModelBlock
{
    vec4 model[3];
    vec4 normalMtx[3];
};

layout(location = 0) in vec3 v_inPos;
layout(location = 1) in vec2 v_inTexCoord;
layout(location = 2) in vec3 v_inNormal;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
    TexCoord = v_inTexCoord;

    vec4 normal = vec4(v_inNormal, 0.0);

    Normal = normalize(vec3(
        dot(normalMtx[0], normal),
        dot(normalMtx[1], normal),
        dot(normalMtx[2], normal)
    ));

    vec4 pos = vec4(v_inPos, 1.0f);

    FragPos = vec3(
        dot(model[0], pos),
        dot(model[1], pos),
        dot(model[2], pos)
    );
    vec4 wpos = vec4(FragPos, 1.0);

    gl_Position = vec4(
        dot(viewProj[0], wpos),
        dot(viewProj[1], wpos),
        dot(viewProj[2], wpos),
        dot(viewProj[3], wpos)
    );
}
