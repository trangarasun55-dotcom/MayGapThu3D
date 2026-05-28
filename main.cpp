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
const float DROP_HOLE_X = -1.2f;
const float DROP_HOLE_Z = 1.2f;

const int TOY_COUNT = 15; // Số lượng gấu trong lồng kính
Toy listToys[TOY_COUNT]; 

hmtoan::Vec3 clawPosition = { 0, 3.2f, 0 };
float clawOpenAngle = 45.0f;
int grabbedToyIndex = -1;

// Khởi tạo các biến quản lý trạng thái cửa xả gấu
float doorOpenAngle = 0.0f;
bool isDoorOpening = false;
int exitingToyIndex = -1;
float toyExitProgress = 0.0f;

// Biến đếm số lượng gấu đã gắp thành côn
ClawState currentClawState = STATE_IDLE;

// Biến quản lý chế độ camera hiện tại
CameraMode currentCameraMode = CAMERA_PANORAMA;

// Hàm khởi tạo OpenGL
void init()
{
    glEnable(GL_DEPTH_TEST);

    initGraphics();

    initPhysics();
}

// Hàm vẽ lại toàn bộ cảnh mỗi khi có sự thay đổi
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SetupCamera();

    renderScene();

    glutSwapBuffers();
}

// Hàm cập nhật vật lý và trạng thái máy mỗi frame khi không có sự kiện đầu vào
void idle()
{
    updatePhysics(0.016f);

    glutPostRedisplay();
}

// ===== HÀM MAIN ===== //
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH);

    glutInitWindowSize(1200,780);

    glutCreateWindow("May Gap Thu 3D");

	init(); // Hàm khởi tạo OpenGL và thiết lập trạng thái ban đầu

	glutDisplayFunc(display); // Hàm vẽ lại toàn bộ cảnh mỗi khi có sự thay đổi

	glutIdleFunc(idle); // Hàm cập nhật vật lý và trạng thái máy mỗi frame

    glutKeyboardFunc(keyboard); // Hàm xử lý phím thườn

    glutSpecialFunc(specialKeyboard); // Hàm xử lý phím đặc biệt (các phím mũi tên)

	glutMouseFunc(mouseButton); // Hàm xử lý sự kiện nhấn chuột

	glutMotionFunc(mouseMotion); // Hàm xử lý sự kiện di chuyển chuột khi đang nhấn giữ

	glutMainLoop(); // Bắt đầu vòng lặp chính của GLUT

    return 0;
}