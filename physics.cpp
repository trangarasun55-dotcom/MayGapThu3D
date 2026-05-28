#include "physics.h"
#include <math.h>
#include <cstdlib>

// Định nghĩa các hằng số cấu hình lồng kính
const float LIMIT_MIN_X = -1.4f;
const float LIMIT_MAX_X = 1.4f;
const float LIMIT_MIN_Z = -1.4f;
const float LIMIT_MAX_Z = 1.4f;
const float LIMIT_MIN_Y = -0.5f;
const float LIMIT_MAX_Y = 3.2f;

// Tâm của lỗ rơi gấu
const Vec3 DROP_ZONE = { -1.1f, 3.2f, 1.1f };

int caughtToysCount = 0; // Biến đếm số lượng gấu đã gắp thành công

void initPhysics() 
{
	// Thiết lập vị trí ban đầu của càng gắp ở chính giữa lồng kính
    clawPosition = Vec3(0.0f, LIMIT_MAX_Y, 0.0f);
    clawOpenAngle = 45.0f; // Mở rộng góc kẹp ban đầu
	currentClawState = STATE_IDLE;
    grabbedToyIndex = -1;
    exitingToyIndex = -1;
	toyExitProgress = 0.0f; // Đặt lại số lượng gấu đã gắp thành công
    caughtToysCount = 0;
    isDoorOpening = false;
    doorOpenAngle = 0.0f;

    // Vùng phân bố gấu an toàn
    float safeMinX = -1.0f, safeMaxX = 1.0f;
    float safeMinZ = -1.0f, safeMaxZ = 1.0f;

    int cols = 4; // Chia lưới gấu phân bổ 4 cột cố định
    float stepX = (safeMaxX - safeMinX) / 3.0f;
    float stepZ = (safeMaxZ - safeMinZ) / 3.0f;

    for (int i = 0; i < TOY_COUNT; i++) {
        listToys[i].id = i;
        listToys[i].isGrabbed = false;
        listToys[i].isActive = true;

        int row = i / cols;
        int col = i % cols;

        // Tạo độ lệch Jitter rất nhỏ để nhìn tự nhiên nhưng không bao giờ đè khuyết vào nhau
        float offsetX = ((rand() % 100) / 100.0f - 0.5f) * 0.1f;
        float offsetZ = ((rand() % 100) / 100.0f - 0.5f) * 0.1f;

        float posX = safeMinX + col * stepX + offsetX;
        float posZ = safeMinZ + row * stepZ + offsetZ;

        // Nếu tọa độ trùng sát vào hố rơi gấu, đẩy nhẹ ra xa để không bị rơi tự do ngay lúc đầu game
        float distToHole = sqrt(pow(posX - DROP_ZONE.x, 2) + pow(posZ - DROP_ZONE.z, 2));
        if (distToHole < 0.5f) {
            posX += 0.4f;
            posZ -= 0.4f;
        }

        listToys[i].position.x = posX;
        listToys[i].position.z = posZ;
        listToys[i].position.y = -0.45f; // Đáy gấu nằm khít trên mặt sàn
    }
}

void resetPhysics()
{
    initPhysics();
}

// HÀM ĐIỀU KHIỂN CÀNG GẮP
void moveClawManual(float dx, float dz) 
{
    if (currentClawState != STATE_IDLE) return;
    clawPosition.x += dx;
    clawPosition.z += dz;

	// Giới hạn vùng di chuyển của càng gắp trong lồng kính
    if (clawPosition.x < LIMIT_MIN_X) clawPosition.x = LIMIT_MIN_X;
    if (clawPosition.x > LIMIT_MAX_X) clawPosition.x = LIMIT_MAX_X;
    if (clawPosition.z < LIMIT_MIN_Z) clawPosition.z = LIMIT_MIN_Z;
    if (clawPosition.z > LIMIT_MAX_Z) clawPosition.z = LIMIT_MAX_Z;
}

// Hàm bắt đầu chu trình gắp tự động khi nhấn nút
void startGrabCycle() 
{
    if (currentClawState == STATE_IDLE) {
        currentClawState = STATE_LOWERING;
    }
}

// Hàm cập nhật vật lý và trạng thái máy theo thời gian mỗi frame
void updatePhysics(float deltaTime) {
    float moveSpeedY = 1.5f * deltaTime;
    float moveSpeedXZ = 1.0f * deltaTime;
    float rotateSpeed = 15.0f * deltaTime;

    switch (currentClawState) {
    case STATE_LOWERING: // Giai đoạn hạ càng gắp xuống
        clawPosition.y -= moveSpeedY;
        if (clawPosition.y <= 1.0f) { // Chạm tới độ cao bám gấu phù hợp
            clawPosition.y = 1.0f;
            currentClawState = STATE_CLAMPING;
        }
        break;

    case STATE_CLAMPING: // Giai đoạn kẹp chặt càng gắp
        clawOpenAngle -= rotateSpeed * 4.0f;
        if (clawOpenAngle <= 0.0f) {
            clawOpenAngle = 0.0f;

            if (grabbedToyIndex == -1) {
                float minDistance = 0.55f; // Bán kính vợt gắp hiệu dụng
                int targetIndex = -1;
                
                for (int i = 0; i < TOY_COUNT; i++) 
                {
                    if (listToys[i].isActive) 
                    {
						// Tính khoảng cách từ càng gắp đến từng gấu để tìm gấu gần nhất trong bán kính hiệu dụng
                        float dx = clawPosition.x - listToys[i].position.x;
                        float dz = clawPosition.z - listToys[i].position.z;
                        float distXZ = sqrt(dx * dx + dz * dz);
                        
                        if (distXZ < minDistance) 
                        {
                            minDistance = distXZ;
                            targetIndex = i;
                        }
                    }
                }
                // Nếu tìm thấy gấu nào trong bán kính hiệu dụng, đánh dấu là đã gắp được
                if (targetIndex != -1) {
                    grabbedToyIndex = targetIndex;
                    listToys[targetIndex].isGrabbed = true;
                }
            }
            currentClawState = STATE_LIFTING; // Bắt đầu nâng càng gắp lên sau khi đã kẹp chặt
        }
        break;

    case STATE_LIFTING: // Giai đoạn nâng càng gắp lên
        clawPosition.y += moveSpeedY;
        if (clawPosition.y >= LIMIT_MAX_Y) 
        {
            clawPosition.y = LIMIT_MAX_Y;
            currentClawState = STATE_RETURNING;
        }
        break;

	case STATE_RETURNING: // Giai đoạn di chuyển càng gắp về vị trí rơi gấu
    {
        float dirX = DROP_ZONE.x - clawPosition.x;
        float dirZ = DROP_ZONE.z - clawPosition.z;
        float dist = sqrt(dirX * dirX + dirZ * dirZ);
		// Nếu đã gần tới vị trí rơi gấu, đặt thẳng vào tâm và chuyển sang trạng thái thả gấu
        if (dist < 0.1f) {
            clawPosition.x = DROP_ZONE.x;
            clawPosition.z = DROP_ZONE.z;
            currentClawState = STATE_DROPPING;
        }
        else {
            clawPosition.x += (dirX / dist) * moveSpeedXZ;
            clawPosition.z += (dirZ / dist) * moveSpeedXZ;
        }
        break;
    }

	case STATE_DROPPING: // Giai đoạn thả gấu ra ngoài
        clawOpenAngle += rotateSpeed * 3.0f;
        if (clawOpenAngle >= 45.0f)
        {
            clawOpenAngle = 45.0f;

            if (grabbedToyIndex != -1) 
            {
                exitingToyIndex = grabbedToyIndex;
                listToys[exitingToyIndex].isGrabbed = false;
                grabbedToyIndex = -1;

                isDoorOpening = true;
                toyExitProgress = 0.0f;
            }
            else {
                currentClawState = STATE_IDLE;
            }
        }
        break;

    case STATE_IDLE: // Trạng thái chờ sẵn sàng
    default:
        break;
    }

    // Gấu bám dính di chuyển theo càng gắp
    if (grabbedToyIndex != -1) {
        listToys[grabbedToyIndex].position.x = clawPosition.x;
        listToys[grabbedToyIndex].position.y = clawPosition.y - 1.2f;
        listToys[grabbedToyIndex].position.z = clawPosition.z;
    }

    // =================================================================
    // LOGIC RƠI GẤU XUYÊN QUA CỬA SẬP VÀ LĂN RA 2 BÊN PHÍA TRƯỚC MÁY
    // =================================================================
    if (isDoorOpening && exitingToyIndex != -1) 
    {
        // Hoạt ảnh mở mượt cửa sập mặt trước máy
        if (doorOpenAngle > -60.0f) 
        {
            doorOpenAngle -= 140.0f * deltaTime;
            if (doorOpenAngle < -60.0f) doorOpenAngle = -60.0f;
        }

        toyExitProgress += deltaTime * 0.45f; // Điều chỉnh tốc độ hành trình gấu rơi lăn

        if (toyExitProgress <= 0.35f) {
            // Giai đoạn 1: Gấu rơi thẳng đứng từ càng gắp luồn sâu xuống qua lỗ sàn kính (Y=-0.5) xuống lòng thân máy (Y=-1.8)
            float t = toyExitProgress / 0.35f;
            listToys[exitingToyIndex].position.x = DROP_ZONE.x;
            listToys[exitingToyIndex].position.z = DROP_ZONE.z;
            listToys[exitingToyIndex].position.y = LIMIT_MAX_Y - t * (LIMIT_MAX_Y - (-1.8f));
        }
        else if (toyExitProgress <= 0.5f) {
            // Giai đoạn 2: Trượt theo máng nghiêng ẩn từ trong hố dồn ra vị trí cửa sập trung tâm mặt trước (X=0.0, Y=-1.8, Z=1.76)
            float t = (toyExitProgress - 0.35f) / 0.15f;
            listToys[exitingToyIndex].position.x = DROP_ZONE.x + t * (0.0f - DROP_ZONE.x);
            listToys[exitingToyIndex].position.z = DROP_ZONE.z + t * (1.76f - DROP_ZONE.z);
            listToys[exitingToyIndex].position.y = -1.8f;
        }
        else {
            // Giai đoạn 3: Rơi qua cửa sập bật ra ngoài, lăn một đoạn dài hướng ra trước máy và tách sang 2 bên
            float t = (toyExitProgress - 0.5f) / 0.5f;

            int side = caughtToysCount % 2;     // 0: xếp sang bên trái máy, 1: xếp sang bên phải máy
            int slotIndex = caughtToysCount / 2; // Số thứ tự hàng xếp trên nhánh đó

            // Tính toán vị trí đích bên ngoài máy một khoảng cách dài về phía trước (Z=2.8) và dạt 2 bên để thoáng ô cửa trung tâm
            float targetX = (side == 0) ? (-1.8f - slotIndex * 0.5f) : (1.8f + slotIndex * 0.5f);
            float targetZ = 2.8f;      // Đẩy xa hẳn máy
            float targetY = -3.2f;     // Điểm đáp tiếp đất phẳng ngoài máy

            // Nội suy quỹ đạo Lerp Parabol có nảy nhẹ hình sin khi gấu lăn ra ngoài
            listToys[exitingToyIndex].position.x = 0.0f + t * (targetX - 0.0f);
            listToys[exitingToyIndex].position.z = 1.76f + t * (targetZ - 1.76f);
            listToys[exitingToyIndex].position.y = -1.8f + t * (targetY - (-1.8f)) + sin(t * 3.14159f) * 0.4f;
        }

        if (toyExitProgress >= 1.0f) {
            // Chốt hạ vị trí cố định của gấu ngoài hàng chờ, hoàn thành chu kỳ để đón con tiếp theo lăn ra
            int side = caughtToysCount % 2;
            int slotIndex = caughtToysCount / 2;
            listToys[exitingToyIndex].position.x = (side == 0) ? (-1.8f - slotIndex * 0.5f) : (1.8f + slotIndex * 0.5f);
            listToys[exitingToyIndex].position.z = 2.8f;
            listToys[exitingToyIndex].position.y = -3.2f;

            caughtToysCount++;
            exitingToyIndex = -1;
            if (currentClawState == STATE_IDLE) {
                // Trả trạng thái máy sẵn sàng
            }
        }
    }
    else {
        // Đóng khít cửa sập lại mượt mà khi không có gấu thoát ra ngoài
        if (doorOpenAngle < 0.0f) {
            doorOpenAngle += 100.0f * deltaTime;
            if (doorOpenAngle > 0.0f) {
                doorOpenAngle = 0.0f;
                isDoorOpening = false;
                if (currentClawState == STATE_RETURNING || currentClawState == STATE_DROPPING) {
                    currentClawState = STATE_IDLE;
                }
            }
        }
    }
}