#version 430

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;
in vec2 tc;

out vec4 fragColor;

struct PositionalLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

layout (binding=0) uniform sampler2D samp;

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void)
{
    // fragColor = varyingColor;
    vec3 L = normalize(varyingLightDir);
    vec3 N = normalize(varyingNormal);
    vec3 V = normalize(-varyingVertPos);
    vec3 H = normalize(varyingHalfVector);

    // vec3 R = normalize(reflect(-L, N));
    float cosTheta = dot(L, N);
    // float cosPhi = dot(V, R);
    float cosPhi = dot(H, N);

    vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz * vec3(texture(samp, tc));
    vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0.0);
    // vec3 specular = light.specular.xyz *  material.specular.xyz * pow(max(cosPhi, 0.0), material.shininess);
    vec3 specular = light.specular.xyz *  material.specular.xyz * pow(max(cosPhi, 0.0), material.shininess * 3.0);

    fragColor = vec4((ambient + diffuse + specular), 1.0);
}