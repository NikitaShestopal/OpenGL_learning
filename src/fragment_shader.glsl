#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform PointLight light;

void main() {
    // 1. Ambient (Фонове світло деталі)
    vec3 ambient = light.ambient * material.ambient;

    // 2. Diffuse (Основний колір деталі)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // 3. Specular (Блік, який залежить від типу деталі - матовий чи глянець)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // 4. Затухання світла з відстанню
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    // 🟢 ХАК ДЛЯ СІТКИ: якщо блиск рівний 0, ігноруємо ліхтарик і виводимо чистий колір
    if (material.shininess == 0.0f) {
        result = material.diffuse;
    }

    FragColor = vec4(result, 1.0f);
}