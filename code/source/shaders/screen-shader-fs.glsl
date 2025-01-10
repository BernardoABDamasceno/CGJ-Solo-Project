#version 330 core

out vec4 FragmentColor;

in vec2 exTexCoord;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

// Kernels
const float offset = 1.0 / 600.0;

uniform float near;
uniform float far;

in float exLineOn;

float LinearizeDepth(float depth){
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}


vec3 screenColour(void){
    vec3 color = texture(screenTexture, exTexCoord).rgb;
    return color;
}

vec3 screenDepth(void){
    float depth = texture(depthTexture, exTexCoord).r;
    depth = LinearizeDepth(depth) / far;
    return vec3(depth);
}

vec3 invertColour(void){
    return vec3(1.0 - texture(screenTexture, exTexCoord).rgb);
}

vec3 grayscale(void){
    vec3 color = texture(screenTexture, exTexCoord).rgb;
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    return vec3(average);
}

vec3 kernel(void){
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top-left
        vec2(0.0f, offset), // top-center
        vec2(offset, offset), // top-right
        vec2(-offset, 0.0f), // center-left
        vec2(0.0f, 0.0f), // center-center
        vec2(offset, 0.0f), // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f, -offset), // bottom-center
        vec2(offset, -offset) // bottom-right
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, exTexCoord + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }

    return col;
}

vec3 blur(void){
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top-left
        vec2(0.0f, offset), // top-center
        vec2(offset, offset), // top-right
        vec2(-offset, 0.0f), // center-left
        vec2(0.0f, 0.0f), // center-center
        vec2(offset, 0.0f), // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f, -offset), // bottom-center
        vec2(offset, -offset) // bottom-right
    );

    float kernel[9] = float[](
        1.0/16, 2.0/16, 1.0/16,
        2.0/16, 4.0/16, 2.0/16,
        1.0/16, 2.0/16, 1.0/16
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, exTexCoord + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }

    return col;
}

vec3 kernelEdges(void){
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top-left
        vec2(0.0f, offset), // top-center
        vec2(offset, offset), // top-right
        vec2(-offset, 0.0f), // center-left
        vec2(0.0f, 0.0f), // center-center
        vec2(offset, 0.0f), // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f, -offset), // bottom-center
        vec2(offset, -offset) // bottom-right
    );

    float kernel[9] = float[](
        1,  1, 1,
        1, -8, 1,
        1,  1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, exTexCoord + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }

    return col;
}

vec3 cannyEdgeDetector(void){
    vec3 col = texture(screenTexture, exTexCoord).rgb;
    float depth = texture(depthTexture, exTexCoord).r;
    depth = LinearizeDepth(depth) / far;

    float deapths;

    float n = LinearizeDepth(texture(depthTexture, exTexCoord + offset * vec2(0, 1)).r) / far;
    float e = LinearizeDepth(texture(depthTexture, exTexCoord + offset * vec2(1, 0)).r) / far;
    float s = LinearizeDepth(texture(depthTexture, exTexCoord + offset * vec2(0, -1)).r) / far;
    float w = LinearizeDepth(texture(depthTexture, exTexCoord + offset * vec2(-1, 0)).r) / far;

    float ne = LinearizeDepth(texture(depthTexture, exTexCoord + offset * vec2(1, 1)).r) / far;
    float nw = LinearizeDepth(texture(depthTexture, exTexCoord + offset * vec2(-1, 1)).r) / far;
    float se = LinearizeDepth(texture(depthTexture, exTexCoord + offset * vec2(1, -1)).r) / far;
    float sw = LinearizeDepth(texture(depthTexture, exTexCoord + offset * vec2(-1, -1)).r) / far;

    if(n-s > 0.1 || w - e > 0.1 || e - w > 0.1 || s - n > 0.1){
        col = vec3(1.0);
    }
    if(nw - se > 0.1 || ne - sw > 0.1 || se - nw > 0.1 || sw - ne > 0.1){
        col = vec3(1.0);
    }
    
    return col;
}

void main(void)
{
    if(exLineOn == 1.0){
        FragmentColor = vec4(cannyEdgeDetector(), 1.0);
    }
    else{
        FragmentColor = vec4(screenColour(), 1.0);
    }
}
