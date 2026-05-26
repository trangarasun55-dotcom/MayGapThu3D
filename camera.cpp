#include "camera.h"
#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include "../glut/glut.h"   
#endif

CameraMode currentCameraMode = CAMERA_PANORAMA;

void ToggleCamera() {
    if (currentCameraMode == CAMERA_PANORAMA) {
        currentCameraMode = CAMERA_CLOSEUP;
    }
    else {
        currentCameraMode = CAMERA_PANORAMA;
    }
}

void SetupCamera(const Vector3D& clawPos) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (currentCameraMode == CAMERA_PANORAMA) {
        // Góc nhìn toàn cảnh: Đặt camera ở ngoài bao quát toàn bộ khung kính
        gluLookAt(0.0f, 12.0f, 16.0f,  // Vị trí mắt người xem (Cao và lùi về sau)
            0.0f, 4.0f, 0.0f,    // Điểm nhìn tập trung vào tâm máy gắp
            0.0f, 1.0f, 0.0f);   // Vector hướng lên trời
    }
    else if (currentCameraMode == CAMERA_CLOSEUP) {
        // Góc nhìn cận cảnh: Camera đặt hơi check phía trên cần gắp một chút và nhìn thẳng vào cần gắp
        // Vị trí camera tịnh tiến liên tục dựa trên tọa độ thực clawPos của cần gắp
        gluLookAt(clawPos.x, clawPos.y + 2.5f, clawPos.z + 3.5f,  // Vị trí mắt di chuyển theo cần
            clawPos.x, clawPos.y, clawPos.z,                // Nhìn thẳng vào tâm hộp va chạm cần gắp
            0.0f, 1.0f, 0.0f);                              // Vector hướng lên trời
    }
}