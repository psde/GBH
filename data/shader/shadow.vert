varying vec4 shadowCoord;

void main(void)
{ 
	mat4 bias = mat4(0.5, 0.0, 0.0, 0.5,
					 0.0, 0.5, 0.0, 0.5,
					 0.0, 0.0, 0.5, 0.5,
					 0.0, 0.0, 0.0, 1.0);
					 
	shadowCoord = gl_Vertex * gl_TextureMatrix[7] * bias;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
