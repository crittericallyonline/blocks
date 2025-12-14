#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GLES2/gl2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *readFile(const char *filePath)
{
    FILE *f = fopen(filePath, "rb");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    char *string = malloc(sizeof(char) * size + 1);
    fseek(f, 0, SEEK_SET);
    for (size_t i = 0; i < size; i++)
    {
        string[i] = fgetc(f);
    }
    string[size + 1] = '\0';
    // printf("%s\n", string);
    
    return string;
}

GLuint load_shader(GLuint type, const char *filePath)
{
    GLuint shader = glCreateShader(type);
    char *source = readFile(filePath);
    int size = strlen(source);
    glShaderSource(shader, sizeof(char), (char const * const *)&source, &size);
    glCompileShader(shader);

    // free(source);
    
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        printf("FUCK\n");
    }
    // hey theres no fuck message? what? did my shader compile correctly?
    // ok now theres fuck messages.

    return shader;
}

GLuint gen_program(const char *vertPath, const char *fragPath)
{
    GLuint program = glCreateProgram();
    GLuint vertex = load_shader(GL_VERTEX_SHADER, vertPath);
    GLuint fragment = load_shader(GL_FRAGMENT_SHADER, fragPath);
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glValidateProgram(program);
    glUseProgram(program);
    return program;
}