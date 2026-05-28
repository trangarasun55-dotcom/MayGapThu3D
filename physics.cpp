#include "physics.h"
#include <math.h>
#include <cstdlib>

// Định nghĩa các hằng số cấu hình lồng kính
const float LIMIT_MIN_X = -1.4f;
const float LIMIT_MAX_X = 1.4f;
const float LIMIT_MIN_Z = -1.4f;
const float LIMIT_MAX_Z = 1.4f;
const float LIMIT_MIN_Y = -0.25f; //gấu đứng đúng trên mặt sàn kính, không bị lún
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
        listToys[i].position.y = LIMIT_MIN_Y; // Đáy gấu nằm khít trên mặt sàn
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

    case STATE_DROPPING: // Giai đoạn thả gấu ra ngoài hố
        clawOpenAngle += rotateSpeed * 3.0f;
        if (clawOpenAngle >= 45.0f)
        {
            clawOpenAngle = 45.0f;

            if (grabbedToyIndex != -1)
            {
                // Đánh dấu chuyển giao gấu sang hàng chờ xử lý rơi tự do trong lòng hố
                exitingToyIndex = grabbedToyIndex;
                listToys[exitingToyIndex].isGrabbed = false;
                grabbedToyIndex = -1; // Càng gắp sạch bóng giải phóng hoàn toàn

                currentClawState = STATE_IDLE; // Đưa càng gắp về vị trí nghỉ sẵn sàng
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

    // Gấu bám dính di chuyển theo càng gắp (Chỉ chạy khi gấu đang bị gắp thực sự trên không)
    if (grabbedToyIndex != -1) {
        listToys[grabbedToyIndex].position.x = clawPosition.x;
        listToys[grabbedToyIndex].position.y = clawPosition.y - 1.2f;
        listToys[grabbedToyIndex].position.z = clawPosition.z;
    }

    // XỬ LÝ QUỸ ĐẠO RƠI TỰ DO XUỐNG HỐ TRƯỚC KHI BIẾN MẤT
    if (exitingToyIndex != -1 && !isDoorOpening) {
        // Cho gấu rơi thẳng đứng dọc theo hố (tốc độ cao)
        listToys[exitingToyIndex].position.y -= 4.0f * deltaTime;

        // Khi gấu rơi chìm sâu hoàn toàn dưới hố đáy sâu (-1.95f)
        if (listToys[exitingToyIndex].position.y <= -1.95f) {
            listToys[exitingToyIndex].isActive = false; // Ẩn gấu hoàn toàn tạo hiệu ứng lọt hố khuất mắt

            // Bắt đầu chu trình mở cửa sập và đẩy gấu ra khay ngoài
            isDoorOpening = true;
            doorOpenAngle = 0.0f;
            toyExitProgress = 0.0f;
        }
    }

    // =================================================================
    // LOGIC CỬA SẬP BẢN LỀ ĐÁY VÀ ĐẨY GẤU CHUI RA MÁNG NHẬN QUÀ
    // =================================================================
    if (isDoorOpening) {
        // 1. Cho cửa sập lật mở ra phía trước từ từ (Góc mở tối đa khoảng 60-70 độ theo thực tế)
        if (doorOpenAngle < 65.0f) {
            doorOpenAngle += 120.0f * deltaTime; // Tốc độ lật mở cửa mượt mà
            if (doorOpenAngle > 65.0f) doorOpenAngle = 65.0f;
        }

        // 2. CHỈ KHI CỬA ĐÃ LẬT MỞ TOÀN BỘ THÌ GẤU MỚI BẮT ĐẦU CHUI RA TỪ TRONG KHE CỬA
        if (doorOpenAngle >= 65.0f && exitingToyIndex != -1) {

            // Frame đầu tiên gấu chuẩn bị trượt ra ngoài
            if (toyExitProgress == 0.0f) {
                listToys[exitingToyIndex].isActive = true; // Hiện gấu trở lại tại máng cửa xả
                // Điểm xuất phát: Nằm khít phía sau tấm cửa xả chuẩn bị trượt
                listToys[exitingToyIndex].position = Vec3(0.0f, -1.8f, 1.76f);
            }

            // Tăng tiến trình trượt ra ngoài của gấu
            toyExitProgress += 1.2f * deltaTime;

            float t = toyExitProgress;
            if (t > 1.0f) t = 1.0f;

            // Xác định phân bổ máng nhận quà bên ngoài sang 2 bên trái/phải cân đối
            int side = caughtToysCount % 2;
            int slotIndex = caughtToysCount / 2;
            float targetX = (side == 0) ? (-1.8f - slotIndex * 0.5f) : (1.8f + slotIndex * 0.5f);
            float targetY = -3.2f;
            float targetZ = 2.8f;

            // Nội suy di chuyển gấu trượt nghiêng từ khe cửa xập lăn ra ngoài khay
            listToys[exitingToyIndex].position.x = 0.0f + t * (targetX - 0.0f);
            listToys[exitingToyIndex].position.z = 1.76f + t * (targetZ - 1.76f);
            // Tạo chuyển động hình sin nảy nhẹ sinh động khi lăn từ dốc xuống khay thưởng
            listToys[exitingToyIndex].position.y = -1.8f + t * (targetY - (-1.8f)) + sin(t * 3.14159f) * 0.3f;

            // 3. Khi gấu đã trượt hẳn ra khay an toàn hoàn tất
            if (toyExitProgress >= 1.0f) {
                listToys[exitingToyIndex].position = Vec3(targetX, targetY, targetZ); // Khóa vị trí cố định

                caughtToysCount++;     // Tăng điểm
                exitingToyIndex = -1;  // Giải phóng biến trạng thái gấu chu trình này
                isDoorOpening = false; // Kích hoạt chu trình khép cửa bảo vệ lại
            }
        }
    }
    else {
        // Đóng khít cửa sập lại bằng cách tự động khép góc lật về 0 khi không có gấu chui ra
        if (doorOpenAngle > 0.0f && exitingToyIndex == -1) {
            doorOpenAngle -= 150.0f * deltaTime;
            if (doorOpenAngle < 0.0f) doorOpenAngle = 0.0f;
        }
    }
}