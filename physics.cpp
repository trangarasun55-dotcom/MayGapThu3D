#include "physics.h"
#include <math.h>
#include <cstdlib>

const float LIMIT_MIN_X = -1.4f;
const float LIMIT_MAX_X = 1.4f;
const float LIMIT_MIN_Z = -1.4f;
const float LIMIT_MAX_Z = 1.4f;

const float LIMIT_MIN_Y = -0.5f;
const float LIMIT_MAX_Y = 3.2f;

// Tọa độ lỗ rơi: Góc bên trái phía sau của vùng kẹp hợp lệ
const hmtoan::Vec3 DROP_ZONE = { -1.2f, 3.2f, 1.2f };

void initPhysics()
{
    clawPosition = { 0.0f, LIMIT_MAX_Y, 0.0f };
    clawOpenAngle = 45.0f;
    currentClawState = STATE_IDLE;
    grabbedToyIndex = -1;

    // Reset trạng thái cửa xả và đẩy quà
    doorOpenAngle = 0.0f;
    isDoorOpening = false;
    exitingToyIndex = -1;
    toyExitProgress = 0.0f;

    for (int i = 0; i < TOY_COUNT; i++)
    {
        listToys[i].id = i;
        listToys[i].isGrabbed = false;
        listToys[i].isActive = true;

        bool inHole;
        do {
            // Rải gấu ngẫu nhiên trong lồng
            listToys[i].position.x = -1.4f + static_cast<float>(rand()) / (RAND_MAX / 2.8f);
            listToys[i].position.z = -1.4f + static_cast<float>(rand()) / (RAND_MAX / 2.8f);

            // Tính khoảng cách đến lỗ rơi để né
            float distToHole = sqrt(pow(listToys[i].position.x - DROP_ZONE.x, 2) + pow(listToys[i].position.z - DROP_ZONE.z, 2));
            inHole = (distToHole < 0.7f); // Bán kính 0.7 để gấu không lọt hố
        } while (inHole);

        // Tạo hiệu ứng xếp chồng nhẹ nhàng
        listToys[i].position.y = -0.3f + (i % 3) * 0.2f;
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

            // Thả gấu rơi xuống lỗ bên trái
            if (grabbedToyIndex != -1) {
                exitingToyIndex = grabbedToyIndex;
                listToys[exitingToyIndex].isGrabbed = false;

                // Tọa độ khởi tạo bên trong hốc cửa xả (gần với vị trí render cánh cửa y=-2.2, z=1.76)
                listToys[exitingToyIndex].position = { 0.0f, -2.6f, 1.2f };

                grabbedToyIndex = -1;
                isDoorOpening = true;   // Kích hoạt mở cửa xả
                toyExitProgress = 0.0f; // Reset tiến trình đẩy gấu
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
    // =================================================================
    // LOGIC ĐẨY GẤU RƠI VÀO GIỎ
    // =================================================================
    if (isDoorOpening && exitingToyIndex != -1) {
        if (doorOpenAngle > -50.0f) {
            doorOpenAngle -= 120.0f * deltaTime;
            if (doorOpenAngle < -50.0f) doorOpenAngle = -50.0f;
        }

        // CHỈNH CHẬM TỐC ĐỘ RƠI (Giảm từ 0.6f xuống 0.25f)
        toyExitProgress += deltaTime * 0.25f;

        // Gấu trượt theo đường cong Parabol để rơi vào giỏ
        listToys[exitingToyIndex].position.z = 1.2f + (toyExitProgress * 1.5f); // Tiến ra xa

        if (listToys[exitingToyIndex].position.z > 1.76f) {
            // Rơi tự do xuống giỏ (giỏ ở y = -3.2f)
            listToys[exitingToyIndex].position.y = -2.6f - pow((listToys[exitingToyIndex].position.z - 1.76f), 2) * 1.5f;
        }

        if (toyExitProgress >= 1.0f) {
            // KHÔNG tắt isActive để gấu vẫn nằm trong giỏ
            // Đặt cố định vị trí gấu trong giỏ
            listToys[exitingToyIndex].position.y = -3.2f + (rand() % 10) * 0.05f; // Đáy giỏ
            listToys[exitingToyIndex].position.z = 2.6f + (rand() % 10 - 5) * 0.02f;
            exitingToyIndex = -1;
        }
    }
    else {
        if (doorOpenAngle < 0.0f) {
            doorOpenAngle += 80.0f * deltaTime;
            if (doorOpenAngle > 0.0f) doorOpenAngle = 0.0f;
            isDoorOpening = false;
        }
    }

}