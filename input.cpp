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

    case '1': //chính diện
        cameraAngleX = 0.0f;
        cameraAngleY = 30.0f;
        cameraDistance = 15.0f;
        break;

    case '2': //phải
        cameraAngleX = 90.0f;
        cameraAngleY = 25.0f;
        cameraDistance = 12.0f;
        break;

	case '3': //phía sau
        cameraAngleX = 180.0f;
        cameraAngleY = 35.0f;
        cameraDistance = 14.0f;
        break;

	case '4': //trái
        cameraAngleX = -90.0f;
        cameraAngleY = 20.0f;
        cameraDistance = 10.0f;
        break;

	case '5': //trên cao
        cameraAngleX = 45.0f;
        cameraAngleY = 70.0f;
        cameraDistance = 18.0f;
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

void mouseButton(int button, int state, int x, int y)
{
    // CLICK CHUỘT TRÁI
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else
        {
            isDragging = false;
        }
    }

    // CUỘN LÊN -> ZOOM IN
    if (button == 3)
    {
        cameraDistance -= 1.0f;

        if (cameraDistance < 5.0f)
            cameraDistance = 5.0f;
    }

    // CUỘN XUỐNG -> ZOOM OUT
    if (button == 4)
    {
        cameraDistance += 1.0f;

        if (cameraDistance > 30.0f)
            cameraDistance = 30.0f;
    }

    glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
    if (isDragging)
    {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        cameraAngleX += dx * 0.4f;
        cameraAngleY += dy * 0.4f;

        if (cameraAngleY > 85.0f)
            cameraAngleY = 85.0f;

        if (cameraAngleY < 5.0f)
            cameraAngleY = 5.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}