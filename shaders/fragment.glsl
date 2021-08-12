#version 330

// basic Phong shader

const vec4 light_ambient = vec4(vec3(0.5f), 1.0f);
const vec4 light_diffuse = vec4(1.0f);
const vec4 light_specular = vec4(1.0f);
const float shininess = 50.0f;

in V2F {
    vec3 position;
    vec3 normal;
    vec3 color;
};

in vec3 light_direction;

layout(location = 0) out vec4 fragment_color;

void main() {
    vec3 L = normalize(light_direction);
    vec3 N = normalize(normal);
    vec3 V = normalize(-position);

    vec3 R = normalize(reflect(-L, N));
    float cos_theta = dot(L, N);
    float cos_phi = dot(V, R);

    vec3 ambient = (light_ambient * vec4(color, 1.0)).xyz;
    vec3 diffuse = (light_diffuse * vec4(color, 1.0) * max(cos_theta, 0.0)).xyz;
    vec3 specular = (light_specular * vec4(color, 1.0) * pow(max(cos_phi, 0.0), shininess)).xyz;

    fragment_color = vec4(ambient + diffuse + specular, 1.0);
}
