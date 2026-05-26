#include <glut.h>

#include "globals.h"
#include "physics.h"
#include "input.h"
#include "camera.h"
#include "render.h"

// Khởi tạo thông số camera ban đầu
float cameraAngleX = 0.0f;
float cameraAngleY = 15.0f;
float cameraDistance = 18.0f;

// Cấu hình giới hạn lồng kính
const float MIN_X = -4.0f;
const float MAX_X = 4.0f;
const float MIN_Z = -4.0f;
const float MAX_Z = 4.0f;

// Biến quản lý trạng thái kéo thả chuột

int lastMouseX = 0;
int lastMouseY = 0;

bool isDragging = false;

// Vị trí lỗ rơi gấu
const float DROP_HOLE_X = -3.5f;
const float DROP_HOLE_Z = -3.5f;

const int TOY_COUNT = 5;

Toy listToys[TOY_COUNT];

hmtoan::Vec3 clawPosition = { 0, 3.2f, 0 };

float clawOpenAngle = 45.0f;

int grabbedToyIndex = -1;

ClawState currentClawState = STATE_IDLE;

CameraMode currentCameraMode = CAMERA_PANORAMA;

void init()
{
    glEnable(GL_DEPTH_TEST);

    initGraphics();

    initPhysics();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SetupCamera();

    renderScene();

    glutSwapBuffers();
}

void idle()
{
    updatePhysics(0.016f);

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH);

    glutInitWindowSize(1000, 700);

    glutCreateWindow("May Gap Thu 3D");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    glutMainLoop();

    return 0;
}