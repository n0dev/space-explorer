/*
 ============================================================================
 Name        : SkyFromSpace.frag
 Author      : Nicolas Hess
 Version     :
 Copyright   : Your copyright notice
 Description : Vertex Shader to display the ground of Earth depending
               of the position of the Sun
 ============================================================================
 */


/* Position Vector of the Camera */
uniform vec3 Pc1;

/* Position Vector of the Sun */
uniform vec3 Ps1;

/* Position Vector of the Vertex */
varying vec3 v;

/* Normal Vector of the Vertex */
varying vec3 N;

// ----------------------------------------------------------------------------
// PHYSICAL MODEL PARAMETERS
// ----------------------------------------------------------------------------

/* Earth radius and atmosphere radius from the center of Earth */
const float Rg = 6360000.0;
const float Ra = 6420000.0;

/* Pi */
const float M_PI = 3.141592657;

// Rayleigh βs = (5.8, 13.5, 33.1)10−6 m−1 for λ = (680, 550, 440) nm
const float Hr = 7994.0;
const vec3 betaR = {5.8*1e-6, 13.5*1e-6, 33.1*1e-6};

// Mie
// Hm: Clear Sky: 1.2   | Partly Cloudy: 3.0
// betaMSca: Clear Sky: 20e-3 | Partly Cloudy:3e-3
// mieG: Clear Sky: -0.76 | Partly Cloudy: -0.65
const float Hm = 1200.0;
const vec3 betaM = vec3(21e-6);
const float mieG = -0.8;

const float Yz = 1.0;
const float A = 1.0;
const float B = 1.0;
const float C = 1.0;
const float D = 1.0;
const float E = 1.0;

// ----------------------------------------------------------------------------
// LUMINANCE
// ----------------------------------------------------------------------------

float Yc(float gamma, float theta, float thetas) {
	return Yz;
}

float Yoc(float theta) {
	return Yz * (1 + 2.0 * cos(theta)) / 3.0;
}

float F(float theta, float gamma) {
	return (1.0 + A * exp(B/cos(theta))) * (1.0 + C * exp(D*gamma) + E * pow(cos(gamma), 2.0));
}

float Yp(float gamma, float theta, float thetas) {
	return Yz * F(theta, gamma) / F(0.0, thetas);
}


// ----------------------------------------------------------------------------
// MAIN FUNCTION
// ----------------------------------------------------------------------------

varying vec3 Pc;
varying vec3 Pa;

void main()
{
	/* r = distance for center of earth */
	float r = length(Pc1);

	/* h is the altitude */
	float h = r - Rg;

	vec4 test;

	/* Compute Pa and Pc according to the position of the camera */
	if(r > Ra) {
		Pc = v;

		/* RayTracing */
		/* http://www.games-creators.org/wiki/Intersection_dans_l'espace_entre_une_sph%C3%A8re_et_une_droite*/
		/* http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection */
		vec3 o = Pc1;
		vec3 d = normalize(v - o);

		float a = dot(d, d);
		float b = 2.0 * dot(d, o);
		float c = dot(o, o) - Ra * Ra;

		//Find discriminant
		float disc = b * b - 4.0 * a * c;
		if(disc > 0.0) {
			float q = 0.0;
			if(b < 0.0) {
				q = 0.5 * (-b + sqrt(disc));
			}
			else {
				q = 0.5 * (-b - sqrt(disc));
			}

			float tmin = ((q/a) < (c/q)) ? (q/a) : (c/q);
			float tmax = ((q/a) > (c/q)) ? (q/a) : (c/q);

			float c1 = dot(o,o)-Rg*Rg;
			float disc1 = b * b - 4.0 * a * c1;
			// projection on the ground
			if(disc1 > 0.0) {
				float q1 = 0.0;
				if(b < 0.0) {
					q1 = 0.5 * (-b + sqrt(disc1));
				}
				else {
					q1 = 0.5 * (-b - sqrt(disc1));
				}
				tmin = ((q1/a) > (c1/q1)) ? (q1/a) : (c1/q1);
				tmax = ((q1/a) < (c1/q1)) ? (q1/a) : (c1/q1);
			}
			test = vec4(0.1, 0.3, 1.0, clamp((tmax-tmin)/6000000.0, 0.0, 1.0));
		}
		else {
			test = vec4(0.0, 0.0, 0.0, 0.0);
		}
	}
	else {
		Pa = v;
		Pc = Pc1;
		test = vec4(0.0, 0.0, 0.0, 0.0);
	}
	gl_FragColor = test;
}
