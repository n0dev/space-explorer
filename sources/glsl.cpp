#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glew.h>

#include <iostream>
#include <fstream>

using namespace std;

const char* loadShaderAsString(const char* file)
{
    std::ifstream shader_file(file, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
    return str.c_str();
}

GLuint LoadShader(GLenum type, const char *filename)
{
    GLuint shader = 0;
    const char *src = NULL;

    /* creation d'un shader de sommet */
    shader = glCreateShader(type);
    if(shader == 0)
    {
        fprintf(stderr, "impossible de creer le shader\n");
        return 0;
    }

    /* chargement du code source */
    src = loadShaderAsString(filename);
    if(src == NULL)
    {
        /* theoriquement, la fonction LoadSource a deja affiche un message
           d'erreur, nous nous contenterons de supprimer notre shader
           et de retourner 0 */

        glDeleteShader(shader);
        return 0;
    }

    /* assignation du code source */
    glShaderSource(shader, 1, (const GLchar**)&src, NULL);

    /* compilation du shader */
    glCompileShader(shader);

    /* liberation de la memoire du code source */
    //free(src);
    //src = NULL;
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if ( GL_FALSE == result)
	{
		fprintf( stderr, "Vertex shader compilation failed!\n");
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char * log = (char *) malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);

			fprintf(stderr, "Shader log:\n%s", log);
			free(log);
		}
		return 0;
	}
    return shader;
}

void LinkShader(GLuint program)
{
	glLinkProgram(program);
	//Verify the link status.
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		cerr << "Failed to link shader program!" << endl;
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char * log = (char *) malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(program, logLen, &written, log);
			fprintf(stderr, "Program log: \n%s", log);
			free(log);
		}
	}
}
