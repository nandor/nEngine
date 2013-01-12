/**
	Vertex shader for fonts
	Passes the texture coordinate and the color
	to the fragment shader
*/

varying vec4 texCoord;
varying vec4 color;

void main() 
{
	texCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	color = gl_Color;
	
	gl_Position = ftransform();
}
