/**
	Vertex shader for an animation
*/

uniform float uAnimLength, uAnimFrame;
varying vec4 texCoord;

void main()
{
	texCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	
	if (texCoord.s == 1.0) {
		texCoord.s = 1.0 / uAnimLength * (uAnimFrame + 1.0);
	}
	
	if (texCoord.s == 0.0) {
		texCoord.s = 1.0 / uAnimLength * uAnimFrame;
	}
	
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
}
