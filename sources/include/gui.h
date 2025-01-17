#ifndef GUI_H_
#define GUI_H_

#include <SDL/SDL.h>
#include <FTGL/ftgl.h>

#define DEFAULT_SCREEN_WIDTH 1024
#define DEFAULT_SCREEN_HEIGHT 600

#define PROGRAM_TITLE "Space Simulator"
#define PROGRAM_AUTHOR "Nicolas Hess"
#define SCREEN_BPP 32

extern SDL_Surface *pScreen;
extern Uint32 videoFlags;
extern const SDL_VideoInfo *videoInfo;

// Resize the actual window with these dimensions in pixels
void gui_resize(int width, int height);

// GLWindow Constructor
void gui_init(void);

// GLWindow Destructor
void gui_close(void);

// Font FTGL pointer
extern FTGLPixmapFont *font;

void display_actual_speed(char *text, double speed, int range);

void display_actual_distance(char *text, double distance);

#endif /* GUI_H_ */
