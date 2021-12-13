#version 430 core
out vec4 FragColor;

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D uTexture; //gets texture input from ACTIVETEXTURE0 

uniform vec3 viewPos;

uniform vec3 lightPosition;  
uniform vec3 lightDirection;
uniform float lightCutOff;
uniform float lightOuterCutOff;
  
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
	
uniform float lightConstant;
uniform float lightLinear;
uniform float lightQuadratic;
uniform vec4 u_color = vec4(1.0, 0.0, 0.0, 1.0);

#define NR_POINT_LIGHTS 1

uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform float materialShininess;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
	// ambient
    vec3 ambient = lightAmbient; //* texture(uTexture, TexCoords).rgb;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff; //* texture(uTexture, TexCoords).rgb;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = lightSpecular * spec; //* texture(uTexture, TexCoords).rgb;  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-lightDirection)); 
    float epsilon = (lightCutOff - lightOuterCutOff);
    float intensity = clamp((theta - lightOuterCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(lightPosition - FragPos);
    float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;

     for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  
    FragColor = vec4(result, 1.0) * u_color;
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

  
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    /// combine results
    vec3 ambient = light.ambient; // * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff; //* vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec;// * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);

}