#ifndef GLOBALS_H
#define GLOBALS_H

#include "virtualreality.h"

using namespace hmtoan;

enum ClawState
{
    STATE_IDLE,
    STATE_LOWERING,
    STATE_CLAMPING,
    STATE_LIFTING,
    STATE_RETURNING,
    STATE_DROPPING
};

enum CameraMode
{
    CAMERA_PANORAMA,
    CAMERA_CLOSEUP
};

struct Toy
{
    int id;

    Vec3 position;

    bool isGrabbed;
    bool isActive;
};

extern const int TOY_COUNT;

extern Toy listToys[];

extern Vec3 clawPosition;

extern float clawOpenAngle;

extern int grabbedToyIndex;

extern ClawState currentClawState;

extern CameraMode currentCameraMode;

// Thêm các biến quản lý Camera
extern float cameraAngleX;
extern float cameraAngleY;
extern float cameraDistance;

// Thêm các hằng số cấu hình lồng kính và lỗ rơi
extern const float MIN_X;
extern const float MAX_X;
extern const float MIN_Z;
extern const float MAX_Z;
extern const float DROP_HOLE_X;
extern const float DROP_HOLE_Z;

// Biến quản lý trạng thái kéo thả chuột
extern int lastMouseX;
extern int lastMouseY;
extern bool isDragging;

// --- QUẢN LÝ CỬA MỞ VÀ ĐẨY GẤU ---
extern float doorOpenAngle;   // Góc mở cánh cửa nhận quà
extern bool isDoorOpening;    // Trạng thái cửa đang mở hay đóng
extern int exitingToyIndex;   // Chỉ số con gấu đang được đẩy ra khay thưởng
extern float toyExitProgress; // Tiến trình đẩy gấu từ trong ra ngoài (0.0f -> 1.0f)

#endif
