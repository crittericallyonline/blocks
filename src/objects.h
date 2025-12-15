#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <stdlib.h>
#include "program.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct Object
{
    GLuint textureID;
    uint32_t index_count;
    GLuint VAO;
    GLuint buffers[3]; // vert, 
} Object;

// some of it is from: https://open.gl/textures
GLuint generate_checker()
{
    GLuint image;
    // glActiveTexture(GL_TEXTURE0); // idk what this does so lets disable it and see what happens

    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_2D, image);
    // Black/white checkerboard
    float pixels[] = {
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f
    };

    // now for binding data to the buffer
    // I HATE SAMPLING, pixel art ftw

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return image;
}

GLuint load_image(const char *imagePath)
{
    GLuint image;
    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_2D, image);
    int w, h, c;

    unsigned char *imageData = stbi_load(imagePath, &w, &h, &c, STBI_rgb_alpha);
    if(!imageData)
    {
        glDeleteTextures(1, &image);
        return 0;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(c == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    else if(c == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    
    free(imageData);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    return image;
}

Object *create_cube()
{

    GLfloat vertices[] = {
      //X  Y  Z  U  V
        0.0, 0.0, 0.5, 0.0, 0.0,
        1.0, 0.0, 0.5, 1.0, 0.0,
        0.0, 1.0, 0.5, 0.0, 1.0,
        1.0, 1.0, 0.5, 1.0, 1.0,
        0.0, 0.0, -.5, 0.0, 0.0,
        0.0, 1.0, -.5, 1.0, 0.0,
        1.0, 0.0, -.5, 0.0, 1.0,
        1.0, 1.0, -.5, 1.0, 1.0,
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 1, 3,
        4, 5, 6,
        6, 5, 7
    };

    GLuint array, vertBuffer, indexBuffer, uvBuffer;

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
    glVertexAttribPointer(Program.vertex_position, 3, GL_FLOAT, false, 5 * sizeof(float), NULL);

    // uvs
    glEnableVertexAttribArray(Program.texcoord);
    glVertexAttribPointer(Program.texcoord, 2, GL_FLOAT, false,5 * sizeof(float), NULL);
    
    glBindVertexArray(0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Object *model = (Object *) malloc(sizeof(Object));

    model->VAO = array;
    model->buffers[0] = vertBuffer;
    model->buffers[1] = indexBuffer;
    model->buffers[2] = uvBuffer;
    model->index_count = sizeof(indices) / 4;
    model->textureID = load_image("/tex/blocks.png");

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