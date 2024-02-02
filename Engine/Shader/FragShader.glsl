#version 330
out vec4 FragColor;

//in vec3 outColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;
//uniform sampler2D ourTexture;
//uniform sampler2D ourTexture2;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

vec3 CalDirLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 reflectDir = reflect(-dirLight.direction, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, texCoord));
    return (ambient + diffuse + specular);
}

vec3 CalPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);

    vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = pointLight.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = pointLight.specular * spec * vec3(texture(material.specular, texCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float var = intensity * attenuation;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

    ambient *= attenuation;
    diffuse *= var;
    specular *= var;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalDirLight(dirLight, normal, viewDir);

    result += CalPointLight(pointLight, normal, FragPos, viewDir);
    result += CalSpotLight(spotLight, normal, FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}