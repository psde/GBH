uniform sampler2D colorTexture;
uniform sampler2DShadow shadowMap;

varying vec4 shadowCoord;

float shadowLookup(float x, float y)
{
	shadowCoord.z -= 0.1;
	float depth = shadow2DProj(shadowMap, shadowCoord );
	return depth != 1.0 ? 0.25 : 1.0;
	//return depth;
}

void main(void)
{
	vec4 color = texture2D(colorTexture, gl_TexCoord[0].xy);
	if(color.a <= 0.6) discard;
	
	gl_FragColor = vec4(shadowLookup(0, 0) * color.rgb, color.a); 	  	
}
