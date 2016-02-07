varying vec3 v;
varying vec3 N;

void main(void)
{
	v = gl_Vertex;
	N = normalize(gl_NormalMatrix * gl_Normal);
  gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
