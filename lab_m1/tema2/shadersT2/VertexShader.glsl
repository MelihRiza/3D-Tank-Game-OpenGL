#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float deformationStrength;


// Output
out vec3 frag_color;
out vec3 frag_position;
out vec2 frag_texture;
out vec3 frag_normal;

out vec3 damaged_color_increment;

void main()
{
    frag_color = v_color;
    frag_position = v_position;
    frag_texture = v_texture;
    frag_normal = v_normal;

    vec3 deformedPosition = v_position;
    deformedPosition.x += sin(8.0) * deformationStrength;
    deformedPosition.y += cos(3.0) * deformationStrength / 3;
    deformedPosition.z += sin(5.0) * deformationStrength;

    vec3 damage_color_constant = vec3 (1.2, 1.2, 1.2);
    damaged_color_increment += damage_color_constant * deformationStrength;

    gl_Position = Projection * View * Model * vec4(deformedPosition, 1.0);
}
