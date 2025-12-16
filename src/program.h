#ifndef PROGRAM_H
#define PROGRAM_H

struct Program {
    GLuint shaderProgram;
        // VERTEX STUFF
    // uniforms
    GLuint projection_matrix;
    GLuint modelview_matrix;
    GLuint time;

    // Attributes
    GLuint vertex_position;
    GLuint texcoord;
    GLuint vertex_id;

        //FRAGMENT STUFF
    // uniforms
    GLuint sampler0;
} Program;

#endif//PROGRAM_H
