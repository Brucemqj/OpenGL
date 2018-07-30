#version 330 core

//in vec3 ourColor;
in vec2 TexCoord;
out vec4 fragColor;

uniform float val;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	//fragColor = vec4(ourColor, 1.0);
	//fragColor = vec4(val, 0.0, 0.0, 1.0);
	fragColor = mix( texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2 );
}