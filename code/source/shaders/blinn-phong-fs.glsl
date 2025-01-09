#version 330 core

in vec3 exPosition;
in vec3 exNormal;

in vec3 exColour;

in vec3 camPosition;
in vec3 worldSpace;

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

float sawWave(float x, float period, float min, float max)
{
    return mix(min, max, fract(x / period));
}  
float invmax(float x){
    return pow(x, x/(x-1))/(x-1);
}
float ring(float x, float y){
    return invmax(x)*(x-pow(y,x));
}
float sineSmooth(float t) {
    return 0.5 - 0.5 * cos(t * 3.14159);
}


vec3 random3(vec3 st){

    st = vec3( dot(st,vec3(127.1,311.7, 513.7) ),
               dot(st,vec3(269.5,183.3, 396.5) ),
               dot(st,vec3(419.2,314.1, 119.7) ) );

    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

float perlin3d(vec3 st){
    vec3 gridIndex = floor(st);
    vec3 gridFract = fract(st);

    vec3 blur = smoothstep(0.0, 1.0, gridFract);

    vec3 blb = gridIndex + vec3(0.0, 0.0, 0.0);
    vec3 brb = gridIndex + vec3(1.0, 0.0, 0.0);
    vec3 tlb = gridIndex + vec3(0.0, 1.0, 0.0);
    vec3 trb = gridIndex + vec3(1.0, 1.0, 0.0);
    vec3 blf = gridIndex + vec3(0.0, 0.0, 1.0);
    vec3 brf = gridIndex + vec3(1.0, 0.0, 1.0);
    vec3 tlf = gridIndex + vec3(0.0, 1.0, 1.0);
    vec3 trf = gridIndex + vec3(1.0, 1.0, 1.0);
    

    vec3 gradBLB = random3(blb);
    vec3 gradBRB = random3(brb);
    vec3 gradTLB = random3(tlb);
    vec3 gradTRB = random3(trb);
    vec3 gradBLF = random3(blf);
    vec3 gradBRF = random3(brf);
    vec3 gradTLF = random3(tlf);
    vec3 gradTRF = random3(trf);

    vec3 distToPixelFromBLB = gridFract - vec3(0.0, 0.0, 0.0);
    vec3 distToPixelFromBRB = gridFract - vec3(1.0, 0.0, 0.0);
    vec3 distToPixelFromTLB = gridFract - vec3(0.0, 1.0, 0.0);
    vec3 distToPixelFromTRB = gridFract - vec3(1.0, 1.0, 0.0);
    vec3 distToPixelFromBLF = gridFract - vec3(0.0, 0.0, 1.0);
    vec3 distToPixelFromBRF = gridFract - vec3(1.0, 0.0, 1.0);
    vec3 distToPixelFromTLF = gridFract - vec3(0.0, 1.0, 1.0);
    vec3 distToPixelFromTRF = gridFract - vec3(1.0, 1.0, 1.0);

    float dotBLB = dot(gradBLB, distToPixelFromBLB);
    float dotBRB = dot(gradBRB, distToPixelFromBRB);
    float dotTLB = dot(gradTLB, distToPixelFromTLB);
    float dotTRB = dot(gradTRB, distToPixelFromTRB);
    float dotBLF = dot(gradBLF, distToPixelFromBLF);
    float dotBRF = dot(gradBRF, distToPixelFromBRF);
    float dotTLF = dot(gradTLF, distToPixelFromTLF);
    float dotTRF = dot(gradTRF, distToPixelFromTRF);


    return mix(
        mix(
            mix(dotBLB, dotBRB, blur.x),
            mix(dotTLB, dotTRB, blur.x),
            blur.y
        ),
        mix(
            mix(dotBLF, dotBRF, blur.x),
            mix(dotTLF, dotTRF, blur.x),
            blur.y
        ),
        blur.z
    ) + 0.5;
}


float scale = 10.0;
void main(void)
{
    //vec2 uv =  gl_FragCoord.xy / vec2(640, 480);
    // vec3 uvw =  vec3(1+worldSpace.x, 0.1+worldSpace.y, 1+worldSpace.z)/2;
    vec3 uvw = exPosition;

    //1 - noiseless saw wave
    float saw = sawWave(uvw.z, 0.2, 0.0, 1.0);

    //2 - sin wave smoothing
    float smoothSaw = sineSmooth(saw);

    //3 - add perlin noise
    float noise = perlin3d(uvw * scale * 0.5);

    //4 - interpolate noise
    float perlinSaw = mix(smoothSaw, noise, 0.5);

    //5 ring noise
    float ringNoise = ring(perlinSaw, noise);

    //6 - combine noises
    float woodpattern = ringNoise + 0.5*noise;

    //7 - function retouching
    woodpattern = clamp(woodpattern, 0.0, 1.0);


    vec3 color;
    color = gooch() ;
    color = mix(exColour, exColour-vec3(0.2, 0.2, 0.2),vec3(woodpattern));
    FragmentColor = vec4(color, 1.0);
}
