#ifndef CAMERA_H
#define CAMERA_H

#include "physics.h"

enum CameraMode {
    CAMERA_PANORAMA,  // Góc nhìn toàn cảnh máy gắp thú
    CAMERA_CLOSEUP    // Góc nhìn cận cảnh bám theo đầu cần gắp
};

extern CameraMode currentCameraMode;

// Hàm thiết lập ma trận góc nhìn, truyền cấu trúc Vector3D vị trí cần gắp của Nhung vào
void SetupCamera(const Vector3D& clawPos);

// Hàm chuyển đổi qua lại giữa 2 chế độ góc nhìn
void ToggleCamera();

#endif