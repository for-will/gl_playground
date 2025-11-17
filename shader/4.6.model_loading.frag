// Fragment shader:
// ================
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

//uniform sampler2D texture_diffuse1;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};

uniform Material material;

// 点光源
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

// 聚光
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight spotLight;

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 FragPos, vec3 viewDir);

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    //    FragColor = texture(material.texture_normal1, TexCoords);

    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
    //    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    vec3 result= vec3(0.0, 0.0, 0.0);
    // 第二阶段：点光源
    for (int i = 0; i < NR_POINT_LIGHTS; i++){
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    // 第三阶段：聚光
     result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position-fragPos);
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse =  light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;

    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;

    vec3 reflectDir = reflect(norm, -lightDir);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

    float distance = length(light.position-fragPos);
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);

    return (ambient + diffuse + specular)*attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光颜色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);

    // 合并结果
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // 聚光
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return (ambient+diffuse+specular)*intensity;
}