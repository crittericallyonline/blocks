#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <stdlib.h>
#include "program.h"

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

typedef struct Object
{
    GLuint VAO, VAB, EAB;
    uint32_t vert_count;
    float *vertices;
    uint32_t *indicies;
} Object;

Object *create_triangle()
{

    float vertices[] = {
        0, 0.500, 0,
        0.500, -0.500, 0,
        -0.500, -0.500, 0
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    GLuint buffer1;
    glGenVertexArrays(1, &buffer1);
    glBindVertexArray(buffer1);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

    Object *model = (Object *) malloc(sizeof(Object));

    model->VAB = buffer1;
    model->vert_count = 3;
    model->vertices = vertices;
    model->indicies = indices;

    return model;
}

void drawModel(Object *model)
{
    glEnableVertexAttribArray(Program.vertex_position);
    glVertexAttribPointer(Program.vertex_position, 3, GL_FLOAT, false, 0, model->vertices);
    glBindBuffer(GL_ARRAY_BUFFER, model->VAB);
    glDrawArrays(GL_TRIANGLES, 0, 3 * sizeof(float));
    glDisableVertexAttribArray(Program.vertex_position);
}