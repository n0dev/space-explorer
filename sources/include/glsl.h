#ifndef GLSHADER_H
#define GLSHADER_H

#include <iostream>

#include <GL/gl.h>

class GLShader
{
public:

    // Load and compile vertex and fragment shader file
    GLShader(const std::string& vertex_file, const std::string& fragment_file);

    // Use the program
    void begin();

    // Terminate the program
    void end();

    // Access to the program
    GLuint program;

private:

    // Store the shaders
    GLuint vertexShader;
    GLuint fragmentShader;

};

#endif
