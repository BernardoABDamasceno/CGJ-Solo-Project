#version 330 core

in vec3 inPosition;
in vec3 inNormal;

uniform float stylised;
out float exStylised;

uniform mat4 ModelMatrix;
uniform vec3 inColor;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

out vec3 exPosition;
out vec3 exNormal;
out vec3 exColour;

out vec3 worldSpace;

out vec3 camPosition;

void main(void)
{
	exPosition = inPosition;
	exNormal = mat3(transpose(inverse(ModelMatrix))) * inNormal;

	exColour = inColor;

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
    mat4 invViewMatrix = inverse(ViewMatrix);
    camPosition = vec3(invViewMatrix[3][0],invViewMatrix[3][1],invViewMatrix[3][2]);

	worldSpace = (ModelMatrix * MCPosition).xyz;

	exStylised = stylised;
}
