#version 330

// basic Phong shader

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 normal_matrix;
uniform vec4 light_position;

out V2F {
    vec3 position;
    vec3 normal;
    vec3 color;
} fragment;

out vec3 light_direction;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    fragment.position = (mv * vec4(position, 1.0)).xyz;
    fragment.normal = (normal_matrix * vec4(normal, 1.0)).xyz;
    fragment.color = color;
    light_direction = light_position.xyz - fragment.position;
}
