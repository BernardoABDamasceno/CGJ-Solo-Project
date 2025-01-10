#version 330 core

in vec3 exPosition;
in vec3 exNormal;

in vec3 exColour;

in vec3 camPosition;

in float exStylised;

out vec4 FragmentColor;

vec3 lightColor = vec3(1, 1, 1);
vec3 lightPos = vec3(10.0, 10.0, 0.0);

float shininess = 16;

vec3 blinnPhong(void){    
    
    vec3 norm = normalize(exNormal);

    vec3 lightDir = normalize(lightPos - exPosition);
    vec3 viewDir = normalize(camPosition - exPosition);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;
    
    float spec = pow(max(dot(halfwayDir, norm), 0.0), 128);
    vec3 specular = lightColor * spec;

    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * exColour;

   return result;
}

vec3 warm = vec3(0.906, 0.524, 0.466);
vec3 cool = vec3(0.391, 0.229, 0.585);
float alpha = 0.5;
float beta = 0.5;

vec3 gooch(void){
    vec3 norm = normalize(exNormal);

    vec3 lightDir = normalize(lightPos - exPosition);
    vec3 viewDir = normalize(camPosition - exPosition);

    float gooch = (1.0 + dot(lightDir, norm)) / 2.0;

    vec3 kCool = cool + alpha*exColour;
    vec3 kWarm = warm + beta*exColour;

    vec3 goochDiffuse = gooch * kWarm + (1.0 - gooch) * kCool;
    
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess * 500);
    vec3 specular = lightColor * spec;

    vec3 result = (goochDiffuse + specular);

    return result;
}

void main(void)
{
    vec3 color;
    if(exStylised == 0.0){
        color = blinnPhong();
    }
    else{
        color = gooch();
    }

    FragmentColor = vec4(color, 1.0);
}
