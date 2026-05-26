#include "input.h"
#include "physics.h"
#include "camera.h"
#include <glut.h>

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case ' ': // Bấm Space để bắt đầu gắp
        if (currentClawState == STATE_IDLE) {
            currentClawState = STATE_LOWERING;
        }
        break;
    case 'c': case 'C':
        ToggleCamera(); // Phím tắt bạn đã có sẵn
        break;
    case 'r': case 'R':
        resetPhysics(); // Reset lại vị trí gấu
        break;

        // Xoay Camera bằng W, A, S, D
    case 'a': case 'A':
        cameraAngleX -= 5.0f;
        break;
    case 'd': case 'D':
        cameraAngleX += 5.0f;
        break;
    case 'w': case 'W':
        cameraAngleY += 5.0f;
        if (cameraAngleY > 89.0f) cameraAngleY = 89.0f; // Tránh lật ngược camera
        break;
    case 's': case 'S':
        cameraAngleY -= 5.0f;
        if (cameraAngleY < 5.0f) cameraAngleY = 5.0f; // Tránh nhìn xuyên gầm máy
        break;
    }
    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
    float moveSpeed = 0.5f;
    // Chỉ cho phép di chuyển càng gắp khi đang ở trạng thái nghỉ
    if (currentClawState == STATE_IDLE) {
        switch (key) {
        case GLUT_KEY_UP: moveClawManual(0.0f, -moveSpeed); break;
        case GLUT_KEY_DOWN: moveClawManual(0.0f, moveSpeed); break;
        case GLUT_KEY_LEFT: moveClawManual(-moveSpeed, 0.0f); break;
        case GLUT_KEY_RIGHT: moveClawManual(moveSpeed, 0.0f); break;
        }
    }
    glutPostRedisplay();
}