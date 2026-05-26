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

#endif
