#ifndef PHYSICS_H
#define PHYSICS_H

// Cấu trúc tọa độ 3D đơn giản
struct Vector3D {
    float x, y, z;
};

// Thuật toán va chạm AABB sử dụng Điểm cực tiểu và Điểm cực đại
struct AABB {
    Vector3D min;
    Vector3D max;
};

// Trạng thái của vật thể gấu bông
struct Toy {
    int id;
    Vector3D position;
    AABB bbox;
    float width, height, depth; // Kích thước vật thể để tính toán AABB
    bool isGrabbed;             // Đang bị gắp hay không
    bool isActive;              // Còn ở trong bể kính hay đã rơi xuống lỗ
};

// State Machine quản lý Chu kỳ tự động khi bấm SPACE
enum ClawState {
    STATE_IDLE,        // Đang đứng yên (Cho phép người chơi di chuyển bằng mũi tên)
    STATE_LOWERING,    // Cần gắp đang hạ xuống (Trục Y giảm)
    STATE_CLAMPING,    // Đang đóng càng gắp lại
    STATE_LIFTING,     // Cần gắp đang kéo lên (Trục Y tăng)
    STATE_RETURNING,   // Đang tự động chạy về ô trúng thưởng (Drop Zone)
    STATE_DROPPING     // Mở càng, thả gấu bông rơi xuống lỗ
};

// --- CÁC BIẾN TOÀN CỤC (Chia sẻ với render.cpp và input.cpp) ---
extern ClawState currentClawState;
extern Vector3D clawPosition;
extern AABB clawBBox;
extern float clawOpenAngle; // Góc mở của càng (ví dụ: 0 = đóng hoàn toàn, 45 = mở hoàn toàn)
extern int grabbedToyIndex; // Chỉ số của con gấu đang bị gắp trong mảng (-1 nếu không gắp trúng)

// --- GIỚI HẠN KHÔNG GIAN KHUNG MÁY (Boundary Constants) ---
extern const float LIMIT_MIN_X, LIMIT_MAX_X;
extern const float LIMIT_MIN_Z, LIMIT_MAX_Z;
extern const float LIMIT_MIN_Y, LIMIT_MAX_Y;
extern const Vector3D DROP_ZONE; // Tọa độ vị trí lỗ thoát thưởng

// --- CÁC HÀM XỬ LÝ CHỨC NĂNG ---
void initPhysics(Toy* toys, int toyCount);
bool checkAABBCollision(const AABB& a, const AABB& b);
void updateAABB(AABB& bbox, const Vector3D& pos, float w, float h, float d);
void moveClawManual(float deltaX, float deltaZ); // Dùng cho Trang gọi khi bấm phím mũi tên
void startGrabCycle();                           // Dùng cho Trang gọi khi bấm SPACE
void updatePhysics(float deltaTime, Toy* toys, int toyCount); // Gọi liên tục trong hàm Idle/Timer của main
void resetPhysics(Toy* toys, int toyCount);      // Dùng cho Trang gọi khi bấm phím R

#endif