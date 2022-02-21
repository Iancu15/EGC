#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Spotlights uniforms
uniform vec3 spotlights_positions[4];
uniform vec3 spotlights_colors[4];
uniform vec3 spotlights_directions[4];
uniform float spotlights_intensity;
uniform float spot_angle;
uniform bool spotlights_switch;

// Uniforms for light properties
uniform vec3 eye_position;

uniform float material_ke;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;
uniform vec3 object_position;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 light = vec3(0);
    vec3 N = normalize(world_normal);
    float alpha = 0;
    if (spotlights_switch && world_position.y > 0 && world_position.z > 0 && world_position.z < 8 && world_position.x < 8) {
        alpha = 0.5f;
        for (int i = 0; i < 4; i++) {
            vec3 light_position = spotlights_positions[i];
            vec3 light_direction = spotlights_directions[i];
            vec3 L = normalize(light_position - world_position);
            vec3 V = normalize(eye_position - world_position);
            vec3 H = normalize(L + V);
            float att = 0;
            float cut_off = spot_angle;
            float spot_light = dot(-L, light_direction);
            float spot_light_limit = cos(radians(cut_off));
            if (spot_light > spot_light_limit) {
                float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
                att = pow(linear_att, 2);
            }

            float gives_light = 0;
            if (dot(N, L) > 0) {
                gives_light = 1;
            }

            float cone_intensity = 1;
            light += spotlights_colors[i] * att * cone_intensity * spotlights_intensity * (material_kd * max(dot(N, L), 0) + material_ks * gives_light * pow(max(dot(N, H), 0), material_shininess));
        }
    }

    vec3 color = light;
    out_color = vec4(color, alpha);
}
