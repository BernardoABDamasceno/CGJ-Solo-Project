#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

uniform mat4 ModelMatrix;
uniform vec3 inColor;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

out vec3 exPosition;
out vec2 exTexCoord;
out vec3 exNormal;
out vec3 exColour;

out vec3 FragPos;
out vec3 camPosition;

void main(void)
{
	exPosition = inPosition;
	exTexCoord = inTexcoord;
	exNormal = mat3(transpose(inverse(ModelMatrix))) * inNormal;

	exColour = inColor;

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
    FragPos = vec3(ModelMatrix * vec4(inPosition, 1.0f));
    mat4 invViewMatrix = inverse(ViewMatrix);
    camPosition = vec3(invViewMatrix[3][0],invViewMatrix[3][1],invViewMatrix[3][2]);
}
