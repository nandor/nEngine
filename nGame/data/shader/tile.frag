/**
	Fragment shader for the textured data
*/

uniform sampler2D tex;
varying vec4 texCoord;
uniform int isExplored, tileWidth, tileHeight;

vec2 dir[9] = vec2[](vec2(1, 1), vec2(1, -1), vec2(-1, 1), vec2(-1, -1),
			         vec2(0, 1), vec2(0, -1), vec2(1, 0), vec2(-1, 0),
				     vec2(0, 0));

					 
vec4 composite(vec4 a, vec4 b, float t)
{
	return a * t + b * (1.0 - t);
}					 
					 
vec4 blurTexture(sampler2D texture, vec2 center, float radius)
{
	float blurSizeX = radius / tileWidth;
	float blurSizeY = radius / tileHeight;

	vec4 sum = vec4(0.0);
	int num = 0;
	
	for (int i = 0; i < dir.length(); ++i) {
		vec4 val = texture2D(texture, center + vec2(blurSizeX * dir[i].x, blurSizeY * dir[i].y));
		
		if (val.a > 0.0) {
			sum += val;
			num++;
		}
	}
	
	return sum / num;
}


void main()
{
	vec4 texel = texture2D(tex, texCoord.st);
	if (texel.a <= 0.0)
		discard;
	
	if (isExplored == 0) {
		gl_FragColor = composite(blurTexture(tex, texCoord.st, 4.0), vec4(0.0, 0.0, 0.0, 1.0), 0.1);
		return;
	}

	if (isExplored == 1) {
		gl_FragColor = composite(blurTexture(tex, texCoord.st, 2.0), vec4(0.0, 0.0, 0.0, 1.0), 0.5);
		return;
	}
	
	if (isExplored == 2) {
		gl_FragColor = texel;
		return;
	}
}
