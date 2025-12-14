#include <GL/gl.h>
#include <GLES2/gl2.h>
#include "stb_image.h"

typedef struct Object
{
    GLuint VBO;
} Object;


Object *create_cube(GLubyte width, GLubyte height, GLubyte depth)
{
    GLbyte vertices[40] = {
        -1, -1, -1, 0, 0,
         1, -1, -1, 1, 0,
        -1, 1, -1, 0, 1,
         1, 1, -1, 1, 1,
        -1, -1, 1, 0, 0,
        1, -1, 1, 1, 0,
        -1, 1, 1, 0, 1,
        1, 1, 1, 1, 1
    };

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 40, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 40, vertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    Object *reference = (Object *) malloc(sizeof(Object));

    reference->VBO = vertexBuffer;

    return reference;
}

Object *create_triangle()
{
    GLuint VBO;
}