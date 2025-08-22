#shader vertex
#version 330 core
#extension GL_ARB_separate_shader_objects: enable

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;

out vec2 v_TexCoord;

// only need when plan to do translations
//uniform mat4 u_MVP;
        
void main() {

    // If you want to use a model-view-projection matrix, uncomment the line below
    //gl_Position = u_MVP * position;
    gl_Position = position;
    v_TexCoord = texCoords;
        
};
    
#shader fragment
#version 330 core
#extension GL_ARB_separate_shader_objects: enable
        
out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
        
void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
        
};