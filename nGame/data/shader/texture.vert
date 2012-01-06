/**
	Vertex shader for textured data
*/

varying vec4 texCoord;

void main() 
{
	texCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = ftransform();
}
