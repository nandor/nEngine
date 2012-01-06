/**
	Fragment shader for the fonts
	It takes the alpha from the texture
	and the color from glColor();
*/

uniform sampler2D tex;
varying vec4 texCoord;
varying vec4 color;

void main()
{
	vec4 texel = texture2D(tex, texCoord.st);
		
	gl_FragColor = vec4(color.rgb, texel.a);
}
