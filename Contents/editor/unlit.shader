#shader vertex
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

void main()
{
    gl_Position = ubo_Projection * ubo_View * ubo_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 430 core

out vec4 FragColor;

uniform vec4 u_Diffuse;

void main()
{
    FragColor = vec4(u_Diffuse);
}