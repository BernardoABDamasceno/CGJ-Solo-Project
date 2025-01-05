#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

in vec3 exColour;

out vec4 FragmentColor;

vec3 constantColor(void) {
    return vec3(0.5);
}

vec3 positionColor(void) {
    return (exPosition + vec3(1.0)) * 0.5;
}

vec3 uvColor(void) {
    return vec3(exTexcoord, 0.0);
}

vec3 normalColor(void) {
    //return normalize((exColour)*0.7 - (exNormal * 0.3)); 
    return (normalize(exNormal) + exColour) * 0.5;
}

vec3 diffuseColor(void) {
    vec3 N = normalize(exNormal);
    vec3 direction = vec3(0.5, 0.5, 0.5);
    float intensity = max(dot(direction, N), 0.25);
    vec3 colorIntensity = intensity * exColour;
    return colorIntensity;
}

void main(void)
{
    vec3 color;
    // color = constantColor();
     color = positionColor();
    // color = uvColor();
    //color = normalColor();
    // color = diffuseColor();
    // color = exColour;
    FragmentColor = vec4(color, 1.0);
}
