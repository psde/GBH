uniform sampler2D colorTexture;

void main(void)
{
	vec4 color = texture2D(colorTexture, gl_TexCoord[0].xy);
	if(color.a <= 0.6) discard;
	gl_FragColor = color;
}
