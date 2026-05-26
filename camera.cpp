#include "camera.h"
#include <math.h>
#include <glut.h>

void SetupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (currentCameraMode == CAMERA_PANORAMA) {
        // Tính toán vị trí Camera dựa trên góc xoay cameraAngleX và cameraAngleY
        float radX = cameraAngleX * 3.14159265f / 180.0f;
        float radY = cameraAngleY * 3.14159265f / 180.0f;

        float eyeX = cameraDistance * cos(radY) * sin(radX);
        float eyeY = cameraDistance * sin(radY);
        float eyeZ = cameraDistance * cos(radY) * cos(radX);

        // Nhìn thẳng vào tâm máy gắp thú (tọa độ 0, 3, 0)
        gluLookAt(
            eyeX,
            eyeY + 2.5f,
            eyeZ,

            clawPosition.x,
            1.5f,
            clawPosition.z,

            0.0f,
            1.0f,
            0.0f
        );
    }
    else {
        // Góc nhìn cận cảnh di chuyển bám sát theo vị trí càng gắp
        gluLookAt(clawPosition.x, clawPosition.y + 3.0f, clawPosition.z + 5.0f,
            clawPosition.x, clawPosition.y, clawPosition.z,
            0.0f, 1.0f, 0.0f);
    }
}

void ToggleCamera() {
    if (currentCameraMode == CAMERA_PANORAMA) {
        currentCameraMode = CAMERA_CLOSEUP;
    }
    else {
        currentCameraMode = CAMERA_PANORAMA;
    }
}
// ----------------------------------------------