/**
	Vertex shader for textured data
*/

varying vec4 color;

void main() 
{
	gl_Position = ftransform();
	color = gl_Color;
}
