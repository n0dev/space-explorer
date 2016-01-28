#include <GL/glew.h>
#include <SDL/SDL.h>
#include <iostream>
#include <FTGL/ftgl.h>

#include "../include/gui.h"
#include "../include/music.h"

using namespace std;

SDL_Surface * pScreen;
int videoFlags;
const SDL_VideoInfo *videoInfo;

FTPixmapFont *font;

void gui_resize(int width, int height)
{
	/* Height / width ration */
	GLfloat ratio;

	/* Protect against a divide by zero */
	if (height == 0)
		height = 1;

	ratio = (GLfloat) width / (GLfloat) height;

	/* change to the projection matrix and set our viewing volume. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Setup our viewport. */
	glViewport(0, 0, (GLint) width, (GLint) height);

	/* Set our perspective */
	gluPerspective(45.0f, ratio, 10000, 10e25);
	//glFrustum(0,1.0,0,1.0,10, 10e25);

	/* Make sure we're chaning the model view and not the projection */
	glMatrixMode(GL_MODELVIEW);
}

void gui_init(void)
{
	cout << "[+] Initialize SDL and OpenGL libraries...\n";

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) == -1) {
		fprintf(stderr, "[*] Erreur lors de l'initialisation de SDL: %s\n",
				SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/* Fetch the video info */
	videoInfo = SDL_GetVideoInfo();

	if (!videoInfo) {
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
	}

	/* the flags to pass to SDL_SetVideoMode */
	videoFlags = SDL_OPENGL; /* Enable OpenGL in SDL */
	videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	videoFlags |= SDL_HWPALETTE; /* Store the palette in hardware */
	videoFlags |= SDL_RESIZABLE; /* Enable window resizing */

	/* This checks to see if surfaces can be stored in memory */
	if (videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	/* This checks if hardware blits can be done */
	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	/* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1) == -1)
		cerr << "impossible d'initialiser SDL_GL_MULTISAMPLEBUFFERS à 1";
	else {
		if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 6) == -1)
			cerr << "impossible d'initialiser SDL_GL_MULTISAMPLESAMPLES sur 6 buffers";
	}

	/* get a SDL surface */
	pScreen = SDL_SetVideoMode(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SCREEN_BPP,
			videoFlags);

	atexit(SDL_Quit);
	SDL_WM_SetCaption(PROGRAM_TITLE, NULL);

	/* Anti-aliasing */
	glEnable(GL_POINT_SMOOTH);

	/* Enable smooth shading */
	glShadeModel(GL_SMOOTH);

	/* Enables Depth Testing */
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);	// Active l'éclairage
	//glEnable(GL_LIGHT0);	// Allume la lumière n°1
	//glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

	/* The Type Of Depth Test To Do */
	glDepthFunc(GL_LEQUAL); //

	/* Really Nice Perspective Calculations */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	/* Resize the initial window */
	gui_resize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	//printf("[+] Video mode: %dx%d, bbp: %d\n", pScreen->w, pScreen->h, pScreen->format->BitsPerPixel);
	//printf("[+] Video information: %d x %d pixels\n", videoInfo->current_w, videoInfo->current_h);

	/* Activation du support UNICODE */
	SDL_EnableUNICODE(1);

	/* Font initialization */
	font = new FTPixmapFont("fonts/wendy.ttf");

	// If something went wrong, bail out.
	if (font->Error())
		exit(1);

	//font->CharMap(ft_encoding_unicode);
	font->FaceSize(15);

	//SDL_ShowCursor(SDL_DISABLE);

	/* glew initialization */
	GLenum err = glewInit();
	if(GLEW_OK != err) {
		cerr << "Error initializing GLEW: %s\n" << glewGetErrorString(err);
	}
	if (!GLEW_ARB_vertex_program) {
	  cerr << "ARB_vertex_program is missing!\n";
	}

	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("[+] GL Vendor : %s\n", vendor);
	printf("[+] GL Renderer : %s\n", renderer);
	printf("[+] GL Version : %s\n", version);
	printf("[+] GLSL Version : %s\n", glslVersion);
}

void gui_close(void)
{
	CloseAudio();
	SDL_Quit();

	/* and exit appropriately */
	cout << "[+] Exit application!" << endl;
	exit(0);
}

