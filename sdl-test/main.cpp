#include <SDL.h>
#include <SDL_opengles2.h>
#include <GLES2/gl2.h>


// Shader sources
const GLchar* vertexSource =
    "attribute vec4 position;                    \n"
    "void main()                                 \n"
    "{                                           \n"
    "   gl_Position = vec4(position.xyz, 1.0);   \n"
    "}                                           \n";

const GLchar* fragmentSource =
    "void main()                                 \n"
    "{                                           \n"
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);  \n"
    "}                                           \n";

const unsigned int DISP_WIDTH = 640;
const unsigned int DISP_HEIGHT = 480;

// Creates the Vertex Buffer Object (VBO) containing
// the given vertices.
GLuint vboCreate(const GLfloat *vertices, GLuint verticesSize) {
    // Create the Vertex Buffer Object
    GLuint vbo;
    int nBuffers = 1;
    glGenBuffers(nBuffers, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Copy the vertex data in, and deactivate
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices,
    GL_STATIC_DRAW);

    // Check for problems
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        // Failed
        glDeleteBuffers(nBuffers, &vbo);
        SDL_Log("Creating VBO failed, code %u\n", err);
        vbo = 0;
    }
    return vbo;
}

// Loads and compiles a shader.
GLuint shaderLoad(const GLchar* source, GLenum shaderType) {
    // Create the shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    
    // Compile the shader
    glCompileShader(shader);
    GLint compileSucceeded = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSucceeded);

    // Check shader compilation result
    if (!compileSucceeded) {
        // Compilation failed. Print error info
        SDL_Log("Compilation of shader failed.\n");

        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *errLog = (GLchar*)malloc(logLength);
        if (errLog) {
            glGetShaderInfoLog(shader, logLength, &logLength, errLog);
            SDL_Log("%s\n", errLog);
            free(errLog);
        }
        else {
            SDL_Log("Couldn't get shader log; out of memory\n");
        }
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}


int main(int argc, char** argv)
{
    // The window
    SDL_Window *window = NULL;
    // The OpenGL context
    SDL_GLContext context = NULL;
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Setup the exit hook
    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("GLES2+SDL2 Window", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, DISP_WIDTH, DISP_HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    
    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                "Couldn't create the main window.", NULL);
        return EXIT_FAILURE;
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                "Couldn't create an OpenGL context.", NULL);
        return EXIT_FAILURE;
    }

    SDL_GL_SetSwapInterval(0);

    // Triangle params
    const GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};
    const GLsizei vertSize = sizeof(vertices);
    
    // Creates the Vertex Buffer Object (VBO)
    GLuint vbo = vboCreate(vertices, vertSize);
    if (!vbo) {
        return EXIT_FAILURE;
    }

    // Create vertex shader
    GLuint vertexShader = shaderLoad(vertexSource, GL_VERTEX_SHADER);
    if (!vertexShader) {
        return EXIT_FAILURE;
    }


    // Create fragment shader
    GLuint fragmentShader = shaderLoad(fragmentSource, GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        return EXIT_FAILURE;
    }

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProg = glCreateProgram();

    if (shaderProg) {
        glAttachShader(shaderProg, vertexShader);
        glAttachShader(shaderProg, fragmentShader);
        glLinkProgram(shaderProg);
        GLint linkingSucceeded = GL_FALSE;
        glGetProgramiv(shaderProg, GL_LINK_STATUS, &linkingSucceeded);
        
        if (!linkingSucceeded) {
            SDL_Log("Linking shader failed\n");
            GLint logLength = 0;
            glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &logLength);
            GLchar *errLog = (GLchar*)malloc(logLength);
           
            if (errLog) {
                glGetProgramInfoLog(shaderProg, logLength, &logLength, errLog);

                SDL_Log("%s\n", errLog);
                free(errLog);
            }
            else {
                SDL_Log("Couldn't get shader link log; out of memory\n");
            }
            glDeleteProgram(shaderProg);
            shaderProg = 0;
        }
    }
    else {
        SDL_Log("Couldn't create shader program\n");
    }

    if (!shaderProg) {
        return EXIT_FAILURE;
    }

    glUseProgram(shaderProg);

    // Specify the layout of the vertex data
    GLuint positionIdx = 0;
    glVertexAttribPointer(positionIdx, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
    glEnableVertexAttribArray(positionIdx);

    while(1)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT) {
                SDL_Log("SDL_QUIT event occurs\n");
                glDeleteBuffers(1, &vbo);
                vbo = 0;
                glDeleteProgram(shaderProg);
                shaderProg = 0;
                return 0;
            }
        }


        // Clear to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    };
    return 0;
}