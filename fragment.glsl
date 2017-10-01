#version 330

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 Color;

uniform float time;

void main()
{
	Color = vec4(fragmentColor.r * (cos(fragmentPosition.x*4.0 + time) + 1.0) * 0.5,
				 fragmentColor.g * (cos(fragmentPosition.y*8.0 + time) + 1.0) * 0.5,
				 fragmentColor.b * (cos(fragmentPosition.x*8.0 + time) + 1.0) * 0.5, fragmentColor.a);
}
