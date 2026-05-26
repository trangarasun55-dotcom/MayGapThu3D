#include "physics.h"
#include <math.h>
#include <cstdlib>

const float LIMIT_MIN_X = -1.4f;
const float LIMIT_MAX_X = 1.4f;
const float LIMIT_MIN_Z = -1.4f;
const float LIMIT_MAX_Z = 1.4f;

const float LIMIT_MIN_Y = -0.5f;
const float LIMIT_MAX_Y = 3.2f;

// Tọa độ lỗ rơi của máy gắp gấu
const hmtoan::Vec3 DROP_ZONE = { -1.2f, 3.2f, 1.2f };

void initPhysics()
{
    clawPosition = { 0.0f, LIMIT_MAX_Y, 0.0f };
    clawOpenAngle = 45.0f;
    currentClawState = STATE_IDLE;
    grabbedToyIndex = -1;

    for (int i = 0; i < TOY_COUNT; i++)
    {
        listToys[i].id = i;
        listToys[i].isGrabbed = false;
        listToys[i].isActive = true;

        // Khởi tạo vị trí ngẫu nhiên cho gấu trong lồng
        listToys[i].position.x = -1.2f + static_cast<float>(rand()) / (RAND_MAX / 2.4f);

		listToys[i].position.y = -0.3f; // Đặt gấu nằm sát đáy lồng, có thể điều chỉnh nếu muốn gấu nằm cao hơn một chút

        listToys[i].position.z = -1.2f + static_cast<float>(rand()) / (RAND_MAX / 2.4f);
    }
}

void resetPhysics()
{
    initPhysics();
}

// CHỈ GIỮ LẠI MỘT HÀM ĐIỀU KHIỂN CÀNG NÀY
void moveClawManual(float dx, float dz)
{
    if (currentClawState != STATE_IDLE)
        return;

    clawPosition.x += dx;
    clawPosition.z += dz;

    // Chặn không cho di chuyển lố ra ngoài thành kính
    if (clawPosition.x < LIMIT_MIN_X) clawPosition.x = LIMIT_MIN_X;
    if (clawPosition.x > LIMIT_MAX_X) clawPosition.x = LIMIT_MAX_X;
    if (clawPosition.z < LIMIT_MIN_Z) clawPosition.z = LIMIT_MIN_Z;
    if (clawPosition.z > LIMIT_MAX_Z) clawPosition.z = LIMIT_MAX_Z;
}

void startGrabCycle()
{
    if (currentClawState == STATE_IDLE)
    {
        currentClawState = STATE_LOWERING;
    }
}

void updatePhysics(float deltaTime)
{
    float moveSpeedY = 1.2f * deltaTime;
    float moveSpeedXZ = 0.8f * deltaTime;
    float rotateSpeed = 10.0f * deltaTime;

    switch (currentClawState) {
    case STATE_LOWERING:
        clawPosition.y -= moveSpeedY;
        if (clawPosition.y <= 2.0f) { // Chạm đáy lồng
			clawPosition.y = 1.0f; // Đặt thấp hơn một chút để đảm bảo kẹp chạm vào gấu
            currentClawState = STATE_CLAMPING;
        }
        break;

    case STATE_CLAMPING:
        clawOpenAngle -= rotateSpeed;
        if (clawOpenAngle <= 0.0f) {
            clawOpenAngle = 0.0f;

            // --- LOGIC KIỂM TRA KHOẢNG CÁCH THỰC TẾ ---
            if (grabbedToyIndex == -1) {
                float minDistance = 0.6f; // Bán kính vùng gắp (gấu nằm trong khoảng này mới gắp được)
                int targetIndex = -1;

                for (int i = 0; i < TOY_COUNT; i++) {
                    if (listToys[i].isActive) {
                        // Tính khoảng cách Euclidean giữa càng gắp và gấu trên mặt phẳng ngang XZ
                        float dx = clawPosition.x - listToys[i].position.x;
                        float dz = clawPosition.z - listToys[i].position.z;
                        float distXZ = sqrt(dx * dx + dz * dz);

                        // Tìm con gấu nằm gần tâm càng gắp nhất
                        if (distXZ < minDistance) {
                            minDistance = distXZ;
                            targetIndex = i;
                        }
                    }
                }

                // Nếu tìm thấy con gấu đủ gần, thực hiện gắp nó lên
                if (targetIndex != -1) {
                    grabbedToyIndex = targetIndex;
                    listToys[targetIndex].isGrabbed = true;
                }
            }

            currentClawState = STATE_LIFTING;
        }
        break;

    case STATE_LIFTING:
        clawPosition.y += moveSpeedY;
        if (clawPosition.y >= LIMIT_MAX_Y) { // Thu cần về độ cao cao nhất
            clawPosition.y = LIMIT_MAX_Y;
            currentClawState = STATE_RETURNING;
        }
        break;

    case STATE_RETURNING: {
        // Đã sửa từ DROP_HOLE sang DROP_ZONE cho đồng bộ hằng số của bạn
        float dirX = DROP_ZONE.x - clawPosition.x;
        float dirZ = DROP_ZONE.z - clawPosition.z;
        float dist = sqrt(dirX * dirX + dirZ * dirZ);

        if (dist < 0.2f) { // Đã về tới vị trí lỗ thả gấu
            currentClawState = STATE_DROPPING;
        }
        else {
            clawPosition.x += (dirX / dist) * moveSpeedXZ;
            clawPosition.z += (dirZ / dist) * moveSpeedXZ;
        }
        break;
    }

    case STATE_DROPPING:
        clawOpenAngle += rotateSpeed;
        if (clawOpenAngle >= 45.0f) { // Mở kẹp ra góc ban đầu
            clawOpenAngle = 45.0f;

            // Thả gấu rơi xuống lỗ -> Ẩn gấu đi
            if (grabbedToyIndex != -1) {
                listToys[grabbedToyIndex].isActive = false;
                listToys[grabbedToyIndex].isGrabbed = false;
                grabbedToyIndex = -1;
            }
            currentClawState = STATE_IDLE; // Kết thúc 1 chu trình tự động
        }
        break;

    case STATE_IDLE:
    default:
        break;
    }

    // Cập nhật vị trí gấu di chuyển theo càng gắp khi đã bám trúng
    if (grabbedToyIndex != -1) {
        listToys[grabbedToyIndex].position.x = clawPosition.x;
        listToys[grabbedToyIndex].position.y = clawPosition.y - 1.5f; // Treo ngay dưới ngàm kẹp
        listToys[grabbedToyIndex].position.z = clawPosition.z;
    }
}