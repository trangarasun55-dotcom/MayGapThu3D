#ifndef RENDER_H
#define RENDER_H

#include <glut.h>

// Khởi tạo các thông số đồ họa, ánh sáng và load textures
void initGraphics();

// Hàm vẽ chính (gọi trong display callback)
void renderScene();

// Các hàm vẽ thành phần (có thể gọi từ ngoài nếu cần test riêng)
void drawMachineBody();
void drawClaw();
void drawBasket();
void drawBackground();
void drawMachineGlass();

#endif // RENDER_H