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

// Hằng số giới hạn di chuyển lồng kính
extern const float LIMIT_MIN_X;
extern const float LIMIT_MAX_X;
extern const float LIMIT_MIN_Z;
extern const float LIMIT_MAX_Z;
extern const float LIMIT_MIN_Y;
extern const float LIMIT_MAX_Y;

// Tọa độ tâm hố rơi gấu cố định
extern const Vec3 DROP_ZONE;

// Biến quản lý trạng thái kéo thả chuột
extern int lastMouseX;
extern int lastMouseY;
extern bool isDragging;

// Quản lý cửa xả quà và tiến trình lăn gấu ra ngoài
extern float doorOpenAngle;
extern bool isDoorOpening;
extern int exitingToyIndex;
extern float toyExitProgress;
extern int caughtToysCount; // Số lượng gấu đã gắp thành công để xếp hàng

#endif // GLOBALS_H
