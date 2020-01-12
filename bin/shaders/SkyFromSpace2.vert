/*
 ============================================================================
 Name        : SkyFromSpace.vert
 Author      : Nicolas Hess
 Version     :
 Copyright   : Your copyright notice
 Description : Vertex Shader to display the ground of Earth depending
               of the position of the Sun
 ============================================================================
 */

/* Position Vector of the Vertex */
varying vec3 v;

/* Normal Vector of the Vertex */
varying vec3 N;

void main(void)
{
	v = gl_Vertex;
	N = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = ftransform();
}
