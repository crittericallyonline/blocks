#include <stdio.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5_webgl.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>

#include "shader.h"

double width, height;
GLuint global_program;

bool onMouseMove(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    // printf("(%d, %d)\n", mouseEvent->clientX, mouseEvent->clientY);
    return true;
}

bool RESIZE_CALLBACK(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{
    emscripten_get_element_css_size("#canvas", &width, &height);
    glViewport(0, 0, width, height);
    return true;
}

void draw()
{
    double now = emscripten_performance_now();
    glClearColor(sin(now / 500.0) / 2 + 0.5f, cos(now / 500.0) / 2 + 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    glFlush();
}

void renderLoop()
{
    draw();
}

int main()
{
    if(!glfwInit())
    {
        printf("Error with creating the webgl context.\n");
        return -1;
    }

    EmscriptenWebGLContextAttributes attrs;
    
    attrs.majorVersion = 2;
    attrs.minorVersion = 3;

    emscripten_webgl_init_context_attributes(&attrs);
    
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);

    int success = emscripten_webgl_make_context_current(context);

    if(success)
    {
        printf("error setting the webgl context to current\n");
        return -1;
    }
    global_program = gen_program("/shader/vertex.glsl", "/shader/fragment.glsl");
    
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, onMouseMove);
    emscripten_get_element_css_size("#canvas", &width, &height);
    emscripten_set_resize_callback("#canvas", NULL, true, RESIZE_CALLBACK);

    emscripten_set_main_loop(renderLoop, 0, true);

    return 0;
}