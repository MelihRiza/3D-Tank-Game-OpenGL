#version 330

// Input
in vec3 frag_color;
in vec3 frag_position;
in vec2 frag_texture;
in vec3 frag_normal;

in vec3 damaged_color_increment;

// Output
layout(location = 0) out vec4 out_color;


void main()
{   
    
    vec3 final_color = frag_color - damaged_color_increment;
    out_color = vec4(final_color, 1);

}
