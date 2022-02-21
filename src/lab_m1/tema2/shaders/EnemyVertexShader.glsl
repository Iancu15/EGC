#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 color;
uniform int shouldBeDisplaced;
uniform float displacementTime;

// Output
out vec3 frag_color;
out vec3 frag_normal;

void main()
{
    frag_color = color;
    vec3 new_position = v_position;
    if (shouldBeDisplaced == 1) {
        new_position += v_normal * vec3(displacementTime);
    }

    gl_Position = Projection * View * Model * vec4(new_position, 1.0);
}