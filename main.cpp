#include <glut.h>
#include "physics.h"

const int TOY_COUNT = 5;
Toy listToys[TOY_COUNT];

// --- HÀM KHỞI TẠO ---
void init() {
    // Thiết lập ánh sáng, màu nền của Huyền ở đây...
    glClearColor(0.8f, 0.9f, 1.0f, 1.0f);

    // ĐƯA VÀO ĐÂY: Khởi tạo vị trí gấu và cần gắp khi vừa mở game
    initPhysics(listToys, TOY_COUNT);
}

// --- HÀM UPDATE LOGIC KHUNG HÌNH ---
void idle() {
    // ĐƯA VÀO ĐÂY: Cập nhật tọa độ va chạm liên tục
    updatePhysics(0.016f, listToys, TOY_COUNT);

    glutPostRedisplay(); // Yêu cầu vẽ lại màn hình sau khi cập nhật vật lý
}

// --- HÀM VẼ CHÍNH ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Gọi các hàm vẽ khung máy, vẽ gấu từ render.cpp ...

    glutSwapBuffers();
}

// --- HÀM MAIN CỦA CHƯƠNG TRÌNH ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("May Gap Thu 3D - Nhom 15");

    init(); // Gọi hàm khởi tạo phía trên

    glutDisplayFunc(display);
    glutIdleFunc(idle); // Đăng ký hàm idle để chạy logic vật lý liên tục

    glutMainLoop();
    return 0;
}