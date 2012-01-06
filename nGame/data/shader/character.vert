/**
	Vertex shader for the player character
*/

uniform float anim_length, anim_frame;
varying vec4 texCoord;

void main()
{
	texCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	
	if (texCoord.s == 1.0) {
		texCoord.s = 1.0 / anim_length * (anim_frame + 1.0);
	}
	
	if (texCoord.s == 0.0) {
		texCoord.s = 1.0 / anim_length * anim_frame;
	}
	
	
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
}
