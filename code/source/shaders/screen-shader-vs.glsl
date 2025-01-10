#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;

out vec2 exTexCoord;

uniform float lineOn;

out float exLineOn;

void main(void)
{
	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);
	exTexCoord = inTexcoord;

	exLineOn = lineOn;
}
