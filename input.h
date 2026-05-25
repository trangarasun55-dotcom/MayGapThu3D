#ifndef INPUT_H
#define INPUT_H

#include "physics.h"

// Cần truyền mảng toys và số lượng toy từ main vào để phục vụ tính năng Reset (Phím R)
void InitInputSystem(Toy* toysArray, int count);

// Hàm xử lý phím thường (Space, C, R)
void KeyboardHandler(unsigned char key, int x, int y);

// Hàm xử lý phím đặc biệt (Các phím mũi tên)
void SpecialKeyHandler(int key, int x, int y);

#endif
