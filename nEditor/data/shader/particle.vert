/**
	Vertex shader for particles
*/

varying vec4 texCoord;

void main() 
{
	texCoord = gl_Vertex;
	if (texCoord.x > 0.0) {
		texCoord.x = 1.0;
	}
	
	if (texCoord.y > 0.0) {
		texCoord.y = 1.0;
	}
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}