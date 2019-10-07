//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>
#include <fstream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include "Timing.h"
#include "Graphics/Car.h"
#include "Graphics/Grid.h"
#include "Graphics/Quad.h"
#include "Graphics/Axis.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Texture.h"
#include "Math/MathUtil.h"

int width = 1024;
int height = 768;

// Mouse position last frame.
float prevMouseX = 0.f;
float prevMouseY = 0.f;
// Difference between the mouse position in this frame and the previous frame.
float mouseXDiff = 0.f;
float mouseYDiff = 0.f;

// Whether to perform a depth pass for shadow mapping.
bool enableShadows = true;

void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void updateInputs(float timestep, GLFWwindow* window, Car* car, Camera* cam);

int main() {
    // Give std::rand the current time as a seed.
    std::srand((unsigned)std::time(NULL));
    
    // Initialize GLFW and OpenGL version.
    glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create Window and rendering context using GLFW.
    GLFWwindow* window = glfwCreateWindow(width, height, "Comp371 - Lab 01", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPos(window, width / 2.f, height / 2.f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    prevMouseX = (float)mouseX;
    prevMouseY = (float)mouseY;

    // Initialize GLEW.
    glewExperimental = true; // Needed for core profile.
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    
    // Fixed time steps.
    Timing* timing = new Timing(60);
    
    // Cameras.
    Camera* cam = new Camera((float)width / height);
    cam->setPosition(Vector3f(0.f, 5.f, -20.f));
    
    Camera* light = new Camera(1.f);
    light->setPosition(Vector3f(0.f, 30.f, 0.f));
    light->addAngle(0.f, MathUtil::PI / -2.f);
    light->setProjectionMatrix(Matrix4x4f::constructOrthographicMat(100.f, 100.f, 0.01f, 50.f));

    // Shaders.
    Shader* defaultShader = new Shader("Shaders/default/");
    defaultShader->addVec3VertexInput("position");
    defaultShader->addVec3VertexInput("normal");
    cam->addShader(defaultShader);
    
    Shader* imageShader = new Shader("Shaders/Image/");
    imageShader->addVec2VertexInput("position");
    imageShader->addVec2VertexInput("uv");
    
    Shader* depthPassShader = new Shader("Shaders/DepthPass/");
    depthPassShader->addVec3VertexInput("position");
    depthPassShader->addVec3VertexInput("normal");
    
    Shader* shadowPassShader = new Shader("Shaders/ShadowPass/");
    shadowPassShader->addVec3VertexInput("position");
    shadowPassShader->addVec3VertexInput("normal");
    cam->addShader(shadowPassShader);
    
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("Failed to create shaders.");
    }

    // Models.
    Car* car = new Car(depthPassShader);
    car->addRotationY(MathUtil::PI / -2.f);
    // 100x100 grid.
    Grid* grid = new Grid(depthPassShader);
    grid->scale = Vector3f(50.f, 1.f, 50.f);
    
//    Quad* quad = new Quad(imageShader);
//    Texture* testTex = new Texture("Textures/test.png");
    
//    Axis* xAxis = new Axis(defaultShader);
//    xAxis->color = Vector4f(1.f, 0.f, 0.f, 1.f);
//    xAxis->rotation = Vector3f(0.f, MathUtil::PI / 2.f, 0.f);
//    Axis* yAxis = new Axis(defaultShader);
//    yAxis->color = Vector4f(0.f, 0.f, 1.f, 1.f);
//    yAxis->rotation = Vector3f(MathUtil::PI / -2.f, 0.f, 0.f);
//    Axis* zAxis = new Axis(defaultShader);
//    zAxis->color = Vector4f(0.f, 0.75f, 0.f, 1.f);
    
    // Shadows.
    GLuint depthMapFrameBuffer;
    glGenFramebuffers(1, &depthMapFrameBuffer);
    
    GLuint SHADOW_DIMENSIONS = 2048;

    GLuint depthMapTextureID;
    glGenTextures(1, &depthMapTextureID);
    glBindTexture(GL_TEXTURE_2D, depthMapTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_DIMENSIONS, SHADOW_DIMENSIONS, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTextureID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    imageShader->getIntUniform("tex0")->setValue(0);
    shadowPassShader->getIntUniform("shadowMap")->setValue(0);

    while (!glfwWindowShouldClose(window)) {
        while (timing->tickReady()) {
            // Detect inputs.
            mouseXDiff = 0.f;
            mouseYDiff = 0.f;
            glfwPollEvents();
            
            updateInputs((float)timing->getTimeStep(), window, car, cam);
            
            timing->subtractTick();
        }

        // Draw code.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cam->update();
        light->update();
        
        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        // render scene from light's point of view
        if (enableShadows) {
            depthPassShader->getMat4Uniform("depthViewMatrix")->setValue(light->getViewMatrix());
            depthPassShader->getMat4Uniform("depthProjectionMatrix")->setValue(light->getProjectionMatrix());

            glViewport(0, 0, SHADOW_DIMENSIONS, SHADOW_DIMENSIONS);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
            glClear(GL_DEPTH_BUFFER_BIT);
            grid->setShader(depthPassShader);
            grid->render();
            car->setShader(depthPassShader);
            car->render();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // reset viewport
            glViewport(0, 0, width * 2, height * 2);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
        // 2. render scene as normal using the generated depth/shadow map
        // --------------------------------------------------------------
        // set light uniforms
        shadowPassShader->getBoolUniform("enableShadows")->setValue(enableShadows);
        shadowPassShader->getVec3fUniform("cameraPosition")->setValue(cam->getPosition());
        shadowPassShader->getVec3fUniform("lightPosition")->setValue(light->getPosition());
        shadowPassShader->getMat4Uniform("lightViewMatrix")->setValue(light->getViewMatrix());
        shadowPassShader->getMat4Uniform("lightProjectionMatrix")->setValue(light->getProjectionMatrix());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMapTextureID);
        grid->setShader(shadowPassShader);
        grid->render();
        car->setShader(shadowPassShader);
        car->render();
        
//        glDisable(GL_DEPTH_TEST);
//        xAxis->render();
//        yAxis->render();
//        zAxis->render();
//        glEnable(GL_DEPTH_TEST);

//        // render Depth map to quad for visual debugging
//        // ---------------------------------------------
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, depthMap);
//
//        glDisable(GL_DEPTH_TEST);
//        quad->render();
//        glEnable(GL_DEPTH_TEST);
        glfwSwapBuffers(window);
        
        // Get elapsed seconds since last run.
        double secondsPassed = timing->getElapsedSeconds();
        timing->addSecondsToAccumulator(secondsPassed);
    }

    delete cam;
    delete light;
    delete car;
    delete grid;
//    delete xAxis;
//    delete yAxis;
//    delete zAxis;
    delete defaultShader;
    delete imageShader;
    delete depthPassShader;
    delete shadowPassShader;
//    delete quad;
//    delete testTex;

    // Shutdown GLFW
    glfwTerminate();

	return 0;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseXDiff = xpos - prevMouseX;
        mouseYDiff = ypos - prevMouseY;
        prevMouseX = xpos;
        prevMouseY = ypos;
        
        float sensitivity = 0.01f;
        mouseXDiff *= sensitivity;
        mouseYDiff *= sensitivity;
    }
}

// Used to determine whether a key was HIT, as opposed to just pressed.
int lastKeySpaceState = GLFW_RELEASE;
int lastKeyXState = GLFW_RELEASE;

void updateInputs(float timestep, GLFWwindow* window, Car* car, Camera* cam) {
    // Cursor position.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    
    // Teleport.
    if (lastKeySpaceState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        car->addPositionXZ(Vector2f((std::rand() % 4) - 1.5f, (std::rand() % 4) - 1.5f));
    }
    lastKeySpaceState = glfwGetKey(window, GLFW_KEY_SPACE);
    
    // Movement.
    float speed = 5.f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed += 15.f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        speed = 2.f;
    }
    
    Car::WalkInput input = Car::WalkInput::None;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        input = input | Car::WalkInput::Forward;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        input = input | Car::WalkInput::Backward;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        input = input | Car::WalkInput::Left;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        input = input | Car::WalkInput::Right;
    }
    car->walk(input, timestep * speed);
    
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        car->addRotationY(timestep * speed);
        input = input | Car::WalkInput::Right;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        car->addRotationY(timestep * -speed);
        input = input | Car::WalkInput::Right;
    }
    
    // Scale.
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        car->addScale(timestep * 2.f);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        car->addScale(timestep * -2.f);
    }
    
    // Change how the car is rendered.
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        car->setRenderingMode(GL_POINT);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        car->setRenderingMode(GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        car->setRenderingMode(GL_FILL);
    }
    
    // Toggle shadow map.
    if (lastKeyXState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        enableShadows = !enableShadows;
    }
    lastKeyXState = glfwGetKey(window, GLFW_KEY_X);
    
    // Camera orientation.
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cam->addAngle(timestep * -5.f, 0.f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cam->addAngle(timestep * 5.f, 0.f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cam->addAngle(0.f, timestep * -5.f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cam->addAngle(0.f, timestep * 5.f);
    }
    
    // Reset camera position and orientation.
    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
        cam->setPosition(Vector3f(0.f, 5.f, -10.f));
        cam->resetAngle();
    }
    
    // Use mouse movement to manipulate the camera.
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        cam->addFov(mouseYDiff * 2.f);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        cam->addAngle(mouseXDiff, 0.f);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        cam->addAngle(0.f, mouseYDiff);
    }
}
