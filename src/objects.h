#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <stdlib.h>
#include "program.h"
#include "textures.h"


typedef struct Object
{
    GLuint textureID;
    uint32_t index_count;
    GLuint VAO;
    GLuint buffers[2]; // vert, index
    mat4 transform;
} Object;

Object *create_cube(GLuint id)
{

    GLfloat vertices[] = {
        0, -.5, -.5, 0.5, 0.0, 0.0,
        1, 0.5, -.5, 0.5, 1.0, 0.0,
        2, -.5, 0.5, 0.5, 0.0, 1.0,
        3, 0.5, 0.5, 0.5, 1.0, 1.0,
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 1, 3,
    };

    GLuint array, vertBuffer, indexBuffer;

    //https://open.gl/drawing -> **Since only calls after binding a VAO stick to it**, make sure that you've created and bound the VAO at the start of your program. **Any vertex buffers and element buffers bound before it will be [ignored].**
    glGenVertexArrays(1, &array);
    glBindVertexArray(array);

    // vertex
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // index
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

    // verts
    glEnableVertexAttribArray(Program.vertex_position);
    glVertexAttribPointer(Program.vertex_position, 3, GL_FLOAT, false, 6 * sizeof(GLfloat), (void *)(sizeof(GLfloat)));

    // uvs
    glEnableVertexAttribArray(Program.texcoord);
    glVertexAttribPointer(Program.texcoord, 2, GL_FLOAT, false, 6 * sizeof(GLfloat), (void *)(sizeof(GLfloat)));
    
    // glEnableVertexAttribArray(Program.vertex_id);
    // glVertexAttribPointer(Program.vertex_id, 1, GL_FLOAT, false, 6 * sizeof(GLfloat), NULL);
    
    glBindVertexArray(0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Object *model = (Object *) malloc(sizeof(Object));

    model->VAO = array;
    model->buffers[0] = vertBuffer;
    model->buffers[1] = indexBuffer;
    model->index_count = sizeof(indices) / 4;
    model->textureID = Textures.index[id];

    return model;
}

void drawModel(Object *model)
{
    glBindTexture(GL_TEXTURE_2D, model->textureID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->buffers[1]);
    glBindVertexArray(model->VAO);
    glDrawElements(GL_TRIANGLES, model->index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void freeModel(Object *model)
{
    glActiveTexture(GL_TEXTURE0);
    glDeleteBuffers(3, &model->buffers[0]);
    glDeleteVertexArrays(1, &model->VAO);
}