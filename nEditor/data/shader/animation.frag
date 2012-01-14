/**
	Fragment shader for an animation
*/

uniform float uAnimLength, uAnimFrame;
uniform sampler2D tex;

varying vec4 texCoord;

void main()
{
	vec4 texel = texture2D(tex, texCoord.st);
	
	if (texel.a <= 0.0)
		discard;
		
	gl_FragColor = texel;
}