#include <GL/glew.h>

void Init_light() {
	// differents paramètres
	GLfloat ambient[] = {0.15f,0.15f,0.15f,1.0f};
	GLfloat diffuse[] = {0.5f,0.5f,0.5f,1.0f};
	GLfloat light0_position [] = {0.0f, -10.0f, 0.0f, 0.0f};
	GLfloat specular_reflexion[] = {0.8f,0.8f,0.8f,1.0f};
	GLubyte shiny_obj = 128;

	//positionnement de la lumière avec les différents paramètres
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse); 
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
	glEnable(GL_LIGHT0); 

	//spécification de la réflexion sur les matériaux 
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular_reflexion);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,shiny_obj);
}

