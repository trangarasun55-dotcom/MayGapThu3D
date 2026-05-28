#ifndef RENDER_H
#define RENDER_H

#include <glut.h>

void initGraphics(); // Hàm khởi tạo đồ họa
void renderScene(); // Hàm vẽ toàn bộ cảnh, bao gồm máy gắp và nền
void drawMachineBody(); // Hàm vẽ thân máy gắp
void drawClaw(); // Hàm vẽ càng gắp
void drawBackground(); // Hàm vẽ nền
void drawMachineGlass(); // Hàm vẽ kính máy gắp
void drawDropHole(); // Hàm vẽ lỗ rơi

#endif // RENDER_H