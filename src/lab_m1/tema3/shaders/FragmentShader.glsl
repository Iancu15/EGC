#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Plane lights uniforms
uniform vec3 plane_lights_positions[9];
uniform vec3 plane_lights_colors[9];
uniform int number_of_plane_lights;
uniform float plane_lights_intensity;
uniform bool ring_floor_switch;

// Spotlights uniforms
uniform vec3 spotlights_positions[4];
uniform vec3 spotlights_colors[4];
uniform vec3 spotlights_directions[4];
uniform float spotlights_intensity;
uniform float spot_angle;
uniform bool spotlights_switch;

// Disco globe light uniforms
uniform sampler2D texture_curr;
uniform sampler2D texture_next;
uniform float texture_interpolation_factor;
uniform float disco_rotation_factor;
uniform vec3 disco_ball_light_position;
uniform float disco_ball_light_intensity;
uniform bool apply_texture;
uniform bool disco_globe_switch;

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
    if (ring_floor_switch) {
        for (int i = 0; i < number_of_plane_lights; i++) {
            vec3 light_position = plane_lights_positions[i];
            vec3 L = normalize(light_position - world_position);
            vec3 V = normalize(eye_position - world_position);
            vec3 H = normalize(L + V);
            float att = 0;
            float light_radius = 1;
            float dist = distance(light_position, world_position);
            if (dist < light_radius && object_position.y != 0) {
                att = pow(light_radius - dist, 2);
            }

            float gives_light = 0;
            if (dot(N, L) > 0) {
                gives_light = 1;
            }

            float diffuse_light = material_kd * max(dot(N, L), 0);
            float specular_light = material_ks * gives_light * pow(max(dot(N, H), 0), material_shininess);
            light += plane_lights_colors[i] * att * plane_lights_intensity * (diffuse_light + specular_light);
        }
    }

    if (spotlights_switch) {
        for (int i = 0; i < 4; i++) {
            vec3 light_position = spotlights_positions[i];
            vec3 light_direction = spotlights_directions[i];
            vec3 L = normalize(light_position - world_position);
            vec3 V = normalize(eye_position - world_position);
            vec3 H = normalize(L + V);
            float att = 0;
            float cut_off = spot_angle;
            float spot_light = dot(-L, light_direction);
            float spot_light_limit = cos(radians(cut_off - 12.5f));
            if (spot_light > spot_light_limit) {
                float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
                att = pow(linear_att, 2);
            }

            float gives_light = 0;
            if (dot(N, L) > 0) {
                gives_light = 1;
            }

            float diffuse_light = material_kd * max(dot(N, L), 0);
            float specular_light = material_ks * gives_light * pow(max(dot(N, H), 0), material_shininess);
            light += spotlights_colors[i] * att * spotlights_intensity * (diffuse_light + specular_light);
        }
    }

    vec3 texture_color = vec3(0);
    if (disco_globe_switch) {
        float u = disco_rotation_factor;
        mat4 rotation_matrix = mat4(vec4(cos(u), 0, -sin(u), 0), vec4(0, 1, 0, 0), vec4(sin(u), 0, cos(u), 0), vec4(0, 0, 0, 1));
        vec3 disco_light_direction = world_position - disco_ball_light_position;
        disco_light_direction = (vec4(disco_light_direction, 1) * rotation_matrix).xyz;
        vec2 texcoord;
        texcoord.x = (1.0 / (2 * 3.14159)) * atan(disco_light_direction.x, disco_light_direction.z);
        texcoord.y = (1.0 / 3.14159) * acos(disco_light_direction.y / length(disco_light_direction));
        vec4 texture_curr_color = texture2D(texture_curr, texcoord);
        vec4 texture_next_color = texture2D(texture_next, texcoord);
        vec3 disco_light_color = mix(texture_curr_color, texture_next_color, texture_interpolation_factor).xyz;
        if (apply_texture) {
            texture_color = disco_light_color;
        } else {
            vec3 L = normalize(disco_ball_light_position - world_position);
            vec3 V = normalize(eye_position - world_position);
            vec3 H = normalize(L + V);

            float gives_light = 0;
            if (dot(N, L) > 0) {
                gives_light = 1;
            }

            float diffuse_light = material_kd * max(dot(N, L), 0);
            float specular_light = material_ks * gives_light * pow(max(dot(N, H), 0), material_shininess);
            if (object_position.y == 4) {
                diffuse_light *= 6;
                specular_light *= 0;
            }
            light += disco_light_color * disco_ball_light_intensity * (diffuse_light + specular_light);
        }
    }

    vec3 color = material_ke * (object_color + texture_color) + light;
    out_color = vec4(color, 1);
}
