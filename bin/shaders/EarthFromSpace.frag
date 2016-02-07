/*
 ============================================================================
 Name        : EarthFromSpace.frag
 Author      : Nicolas Hess
 Version     :
 Copyright   : Your copyright notice
 Description : Fragment Shader to display the ground of Earth depending
               of the position of the Sun
 ============================================================================
 */


/* Normal and Position Vector of the current Vertex */
varying vec3 N;
varying vec3 v;

/* Samplers 2D for texturing the Earth (day & Night) */
uniform sampler2D colorMap;
uniform sampler2D colorMapNight;

/* Spatial position of the Sun */
uniform vec3 sunPosition;

/* Spatial position of the Camera */
uniform vec3 camPosition;



varying float fCos;


void main (void)
{
	fCos = -dot(normalize(v), normalize(sunPosition));

	gl_FragColor.a = (0.5-clamp(fCos, -0.5, 0.5))*texture2D(colorMap, gl_TexCoord[0].st) + (0.5+clamp(fCos, -0.5, 0.5))*texture2D(colorMapNight, gl_TexCoord[0].st);
}
