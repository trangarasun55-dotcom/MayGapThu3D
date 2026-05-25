#include "input.h"
#include "camera.h"
#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include "../glut/glut.h"
#endif

// Con trỏ lưu trữ để tương tác với mảng đồ chơi của Nhung quản lý
static Toy* g_toys = nullptr;
static int g_toyCount = 0;

void InitInputSystem(Toy* toysArray, int count) {
    g_toys = toysArray;
    g_toyCount = count;
}

void KeyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
    case 'c':
    case 'C':
        // Gọi hàm chuyển đổi góc nhìn camera (Trang làm)
        ToggleCamera();
        break;

    case 'r':
    case 'R':
        // Gọi hàm reset hệ thống vật lý và vị trí gấu của Nhung
        if (g_toys != nullptr) {
            resetPhysics(g_toys, g_toyCount);
        }
        break;

    case 32: // Phím Spacebar
        // Kích hoạt chu kỳ gắp tự động (Hàm của Nhung xử lý chuyển đổi sang STATE_LOWERING)
        startGrabCycle();
        break;

    default:
        break;
    }
    glutPostRedisplay(); // Yêu cầu vẽ lại màn hình sau khi tương tác
}

void SpecialKeyHandler(int key, int x, int y) {
    // Tốc độ di chuyển thủ công khi nhấn phím mũi tên
    const float STEP = 0.15f;

    // Nhung đã chặn trong `moveClawManual` (Chỉ thực thi khi STATE_IDLE)
    // Trang chỉ cần truyền độ dời deltaX, deltaZ tương ứng vào hàm của Nhung
    switch (key) {
    case GLUT_KEY_UP:
        moveClawManual(0.0f, -STEP); // Di chuyển lùi vào trong (giảm Z)
        break;
    case GLUT_KEY_DOWN:
        moveClawManual(0.0f, STEP);  // Di chuyển ra ngoài (tăng Z)
        break;
    case GLUT_KEY_LEFT:
        moveClawManual(-STEP, 0.0f); // Di chuyển sang trái (giảm X)
        break;
    case GLUT_KEY_RIGHT:
        moveClawManual(STEP, 0.0f);  // Di chuyển sang phải (tăng X)
        break;
    default:
        break;
    }
    glutPostRedisplay();
}