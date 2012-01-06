/**
	Fragment shader for the textured data
*/

uniform float dt;
uniform vec4 colorA, colorB;
uniform sampler2D texA, texB;
uniform bool isTextured;
varying vec4 texCoord;

void main()
{	
	vec4 fragA = colorA * texture2D(texA, texCoord.st);
	vec4 fragB = colorB * texture2D(texB, texCoord.st);
	
	gl_FragColor = fragA * (1.0 - dt) + fragB * dt;
}