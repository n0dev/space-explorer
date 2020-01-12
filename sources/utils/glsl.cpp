#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <GL/glew.h>

#include "../include/glsl.h"

std::string loadFromFile(const std::string &file) {
    std::ifstream ifile(file);
    std::string filetext;

    while (ifile.good()) {
        std::string line;
        std::getline(ifile, line);
        filetext.append(line + "\n");
    }

    return filetext;
}

GLShader::GLShader(const std::string &vertex_file, const std::string &fragment_file) {
    // Create both shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        throw std::runtime_error("Unable to create a vertex shader");
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (vertexShader == 0) {
        throw std::runtime_error("Unable to create a fragment shader");
    }

    // Load the vertex shader
    std::string vertexString = loadFromFile(vertex_file);
    GLchar const *vertexSource = vertexString.c_str();
    GLint const vertexLength = vertexString.size();
    glShaderSource(vertexShader, 1, &vertexSource, &vertexLength);

    //Compile the vertex shader
    GLint isCompiled = 0;
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar *log_string = new char[maxLength + 1];
        glGetShaderInfoLog(vertexShader, maxLength, 0, log_string);
        std::cout << "Cannot build vertexShader: " << log_string << std::endl;

        delete log_string;
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    // Load the vertex shader
    std::string fragmentString = loadFromFile(fragment_file);
    GLchar const *fragmentSource = fragmentString.c_str();
    GLint const fragmentLength = fragmentString.size();
    glShaderSource(fragmentShader, 1, &fragmentSource, &fragmentLength);

    //Compile the fragment shader
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar *log_string = new char[maxLength + 1];
        glGetShaderInfoLog(fragmentShader, maxLength, 0, log_string);
        std::cout << "Cannot build fragmentShader: " << log_string << std::endl;

        delete log_string;
        return;
    }

    //Vertex and fragment shaders are successfully compiled.
    //Now time to link them together into a program.
    //Get a program object.
    program = glCreateProgram();

    //Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    //Link our program
    glLinkProgram(program);

    //Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *) &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        //We don't need the program anymore.
        glDeleteProgram(program);
        //Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        //In this simple program, we'll just leave
        return;
    }

    //Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}

void GLShader::begin() {
    glUseProgram(program);
}

// Unuse the program
void GLShader::end() {
    glUseProgram(0);
}
