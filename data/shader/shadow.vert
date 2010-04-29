uniform mat4 test;

varying vec4 shadowCoord;

void main(void)
{ 
	mat4 bias = mat4(0.5, 0.0, 0.0, 0.0,
					 0.0, 0.5, 0.0, 0.0,
					 0.0, 0.0, 0.5, 0.0,
					 0.5, 0.5, 0.5, 1.0);
					 
	shadowCoord = bias * gl_TextureMatrix[7] * gl_Vertex;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
