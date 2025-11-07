// Fragment shader:
// ================
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform vec3 objectColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

uniform float matrixmove;

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos);// the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

    // emission
    vec3 emisson = (sin(matrixmove)+1)/2 * texture(material.emission, vec2(TexCoords.x, TexCoords.y+matrixmove)).rgb;
    vec3 result = ambient + diffuse + specular + emisson;

    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear*distance + light.quadratic*distance*distance);

    float theta = dot(lightDir, normalize(-light.direction));
    if (theta > light.cutOff){
        vec3 result = ambient + diffuse*attenuation + specular*attenuation;
        result = result * (1- (sin(matrixmove)+1)/2) + emisson;
        FragColor = vec4(result, 1.0);
    } else {
        FragColor = vec4(ambient, 1.0);
    }
}