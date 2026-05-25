#include "physics.h"
#include <cmath>

// Khởi tạo các hằng số không gian máy gắp
const float LIMIT_MIN_X = -4.0f; const float LIMIT_MAX_X = 4.0f;
const float LIMIT_MIN_Z = -4.0f; const float LIMIT_MAX_Z = 4.0f;
const float LIMIT_MIN_Y = 1.0f; const float LIMIT_MAX_Y = 8.0f;
const Vector3D DROP_ZONE = { -3.5f, 8.0f, -3.5f }; // Góc phòng đặt lỗ nhận quà

// Định nghĩa các biến thực tế
ClawState currentClawState = STATE_IDLE;
Vector3D clawPosition = { 0.0f, 8.0f, 0.0f };
AABB clawBBox;
float clawOpenAngle = 45.0f; // Mặc định mở càng để chuẩn bị gắp
int grabbedToyIndex = -1;

// Kích thước mặc định hộp va chạm của đầu cần gắp
const float CLAW_W = 0.8f, CLAW_H = 0.8f, CLAW_D = 0.8f;

// Cập nhật tọa độ Hộp va chạm dựa trên tâm vật thể
void updateAABB(AABB& bbox, const Vector3D& pos, float w, float h, float d) {
    bbox.min.x = pos.x - w / 2.0f;
    bbox.max.x = pos.x + w / 2.0f;
    bbox.min.y = pos.y - h / 2.0f;
    bbox.max.y = pos.y + h / 2.0f;
    bbox.min.z = pos.z - d / 2.0f;
    bbox.max.z = pos.z + d / 2.0f;
}

// Khởi tạo trạng thái ban đầu của hệ thống và các vật thể
void initPhysics(Toy* toys, int toyCount) {
    clawPosition = { 0.0f, LIMIT_MAX_Y, 0.0f };
    currentClawState = STATE_IDLE;
    clawOpenAngle = 45.0f;
    grabbedToyIndex = -1;
    updateAABB(clawBBox, clawPosition, CLAW_W, CLAW_H, CLAW_D);

    // Đặt vị trí ngẫu nhiên cho các mô hình gấu bông của Huyền bên trong lòng máy
    for (int i = 0; i < toyCount; i++) {
        toys[i].id = i;
        toys[i].width = 1.0f; toys[i].height = 1.0f; toys[i].depth = 1.0f;
        // Rải gấu ngẫu nhiên trên sàn máy (Y sàn = 0.5f)
        toys[i].position.x = -3.0f + static_cast<float>(rand()) / (RAND_MAX / 6.0f);
        toys[i].position.y = 0.5f;
        toys[i].position.z = -3.0f + static_cast<float>(rand()) / (RAND_MAX / 6.0f);
        toys[i].isGrabbed = false;
        toys[i].isActive = true;
        updateAABB(toys[i].bbox, toys[i].position, toys[i].width, toys[i].height, toys[i].depth);
    }
}

// Thuật toán cốt lõi: Kiểm tra va chạm AABB 3D
bool checkAABBCollision(const AABB& a, const AABB& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
        (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
        (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

// Điều khiển thủ công bằng các phím mũi tên (Ràng buộc biên không đâm xuyên kính)
void moveClawManual(float deltaX, float deltaZ) {
    if (currentClawState == STATE_IDLE) {
        clawPosition.x += deltaX;
        clawPosition.z += deltaZ;

        // Giới hạn trong thành máy gắp thú
        if (clawPosition.x < LIMIT_MIN_X) clawPosition.x = LIMIT_MIN_X;
        if (clawPosition.x > LIMIT_MAX_X) clawPosition.x = LIMIT_MAX_X;
        if (clawPosition.z < LIMIT_MIN_Z) clawPosition.z = LIMIT_MIN_Z;
        if (clawPosition.z > LIMIT_MAX_Z) clawPosition.z = LIMIT_MAX_Z;

        updateAABB(clawBBox, clawPosition, CLAW_W, CLAW_H, CLAW_D);
    }
}

// Kích hoạt chu kỳ gắp tự động khi nhấn SPACE
void startGrabCycle() {
    if (currentClawState == STATE_IDLE) {
        currentClawState = STATE_LOWERING;
    }
}

// Vòng lặp cập nhật vật lý (Phần quan trọng nhất của Nhung)
void updatePhysics(float deltaTime, Toy* toys, int toyCount) {
    const float MOVE_SPEED = 3.0f; // Tốc độ di chuyển của cần
    const float CLAMP_SPEED = 60.0f; // Tốc độ đóng/mở càng gắp
    const float GRAVITY = 5.0f; // Tốc độ rơi của gấu

    updateAABB(clawBBox, clawPosition, CLAW_W, CLAW_H, CLAW_D);

    switch (currentClawState) {
    case STATE_IDLE:
        // Không thực hiện hành động tự động nào, chờ tương tác thủ công từ Trang
        break;

    case STATE_LOWERING:
        // Hạ cần gắp xuống theo trục Y
        clawPosition.y -= MOVE_SPEED * deltaTime;

        // Kiểm tra va chạm với bất kỳ gấu bông nào trong lúc hạ xuống
        for (int i = 0; i < toyCount; i++) {
            if (toys[i].isActive && checkAABBCollision(clawBBox, toys[i].bbox)) {
                grabbedToyIndex = i; // Đánh dấu mục tiêu chạm trúng
                currentClawState = STATE_CLAMPING;
                break;
            }
        }
        // Nếu chạm đáy sàn mà không trúng gấu thì cũng phải chuyển sang đóng càng
        if (clawPosition.y <= LIMIT_MIN_Y) {
            clawPosition.y = LIMIT_MIN_Y;
            currentClawState = STATE_CLAMPING;
        }
        break;

    case STATE_CLAMPING:
        // Giảm góc mở để khép các ngón của càng gắp lại
        clawOpenAngle -= CLAMP_SPEED * deltaTime;
        if (clawOpenAngle <= 0.0f) { // Đóng chặt hoàn toàn
            clawOpenAngle = 0.0f;

            // Nếu trước đó chạm trúng gấu, thực hiện cơ chế Liên kết phân cấp (Hierarchical Modeling)
            if (grabbedToyIndex != -1) {
                toys[grabbedToyIndex].isGrabbed = true;
            }
            currentClawState = STATE_LIFTING;
        }
        break;

    case STATE_LIFTING:
        // Kéo cần gắp đi lên lại trục Y
        clawPosition.y += MOVE_SPEED * deltaTime;

        // Nếu đang giữ gấu bông, cập nhật tọa độ gấu theo tọa độ cần gắp
        if (grabbedToyIndex != -1) {
            toys[grabbedToyIndex].position = clawPosition;
            // Có thể căn chỉnh Y dịch xuống một chút để gấu nằm dưới móng vuốt
            toys[grabbedToyIndex].position.y -= 0.5f;
            updateAABB(toys[grabbedToyIndex].bbox, toys[grabbedToyIndex].position, toys[grabbedToyIndex].width, toys[grabbedToyIndex].height, toys[grabbedToyIndex].depth);
        }

        if (clawPosition.y >= LIMIT_MAX_Y) {
            clawPosition.y = LIMIT_MAX_Y;
            currentClawState = STATE_RETURNING;
        }
        break;

    case STATE_RETURNING: {
        // Tự động di chuyển cần gắp về tọa độ Ô trúng thưởng (DROP_ZONE)
        float dx = DROP_ZONE.x - clawPosition.x;
        float dz = DROP_ZONE.z - clawPosition.z;
        float distance = sqrt(dx * dx + dz * dz);

        if (distance > 0.1f) {
            clawPosition.x += (dx / distance) * MOVE_SPEED * deltaTime;
            clawPosition.z += (dz / distance) * MOVE_SPEED * deltaTime;

            // Cập nhật vị trí gấu di chuyển cùng cần gắp theo trục X, Z
            if (grabbedToyIndex != -1) {
                toys[grabbedToyIndex].position.x = clawPosition.x;
                toys[grabbedToyIndex].position.z = clawPosition.z;
                updateAABB(toys[grabbedToyIndex].bbox, toys[grabbedToyIndex].position, toys[grabbedToyIndex].width, toys[grabbedToyIndex].height, toys[grabbedToyIndex].depth);
            }
        }
        else {
            // Đã đến vị trí ô chứa phần thưởng
            clawPosition.x = DROP_ZONE.x;
            clawPosition.z = DROP_ZONE.z;
            currentClawState = STATE_DROPPING;
        }
        break;
    }
    case STATE_DROPPING:
        // Mở càng gắp ra
        clawOpenAngle += CLAMP_SPEED * deltaTime;

        // Khi càng mở ra đủ rộng, hủy liên kết vật thể để gấu rơi tự do
        if (grabbedToyIndex != -1) {
            toys[grabbedToyIndex].isGrabbed = false;
            // Mô phỏng rơi tự do theo trọng lực đơn giản
            toys[grabbedToyIndex].position.y -= GRAVITY * deltaTime;
            updateAABB(toys[grabbedToyIndex].bbox, toys[grabbedToyIndex].position, toys[grabbedToyIndex].width, toys[grabbedToyIndex].height, toys[grabbedToyIndex].depth);

            // Nếu gấu đã rơi sâu xuống lỗ khuất tầm nhìn
            if (toys[grabbedToyIndex].position.y <= -2.0f) {
                toys[grabbedToyIndex].isActive = false; // Biến mất (Người chơi nhận thưởng)
                grabbedToyIndex = -1; // Reset mục tiêu gắp
            }
        }

        // Hoàn thành chu kỳ khi càng gắp mở lại góc 45 độ ban đầu và gấu đã rơi xong
        if (clawOpenAngle >= 45.0f && grabbedToyIndex == -1) {
            clawOpenAngle = 45.0f;
            currentClawState = STATE_IDLE; // Trở lại trạng thái chờ lượt chơi mới
        }
        break;
    }
}

// Chức năng làm mới toàn bộ hệ thống (Khi bấm R)
void resetPhysics(Toy* toys, int toyCount) {
    initPhysics(toys, toyCount);
}