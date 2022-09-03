#version 420 core               // Windows

//#version 330 core               // Cafe
//#extension GL_EXT_Cafe : enable // ....

layout(std140, binding = 1)
uniform cViewBlock
{
    vec3 viewPos;
    vec4 viewProj[4];
};

layout(std140, binding = 3)
uniform cLightBlock
{
    vec3 lightColor;
    vec3 lightPos;
};

uniform sampler2D texture1;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 o_FragColor;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);

    // ambient
    float ambientStrength = 0.4f;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.4f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * texColor.rgb;
    o_FragColor = vec4(result, texColor.a);
}
