#include "physics.h"
#include <math.h>
#include <cstdlib>

// Định nghĩa các hằng số cấu hình lồng kính
const float LIMIT_MIN_X = -1.2f;
const float LIMIT_MAX_X = 1.2f;
const float LIMIT_MIN_Z = -1.2f;
const float LIMIT_MAX_Z = 1.2f;
const float LIMIT_MIN_Y = -0.25f; //gấu đứng đúng trên mặt sàn kính, không bị lún
const float LIMIT_MAX_Y = 3.2f;

// Tâm của lỗ rơi gấu
const Vec3 DROP_ZONE = { -1.2f, 3.2f, 1.2f };

int caughtToysCount = 0; // Biến đếm số lượng gấu đã gắp thành công

// Hàm khởi tạo trạng thái vật lý ban đầu và phân bổ gấu bông theo ngẫu nhiên từng tầng
void initPhysics()
{
    clawPosition = Vec3(0.0f, LIMIT_MAX_Y, 0.0f);
    clawOpenAngle = 45.0f;
    currentClawState = STATE_IDLE;
    grabbedToyIndex = -1;
    exitingToyIndex = -1;
    caughtToysCount = 0;
    isDoorOpening = false;
    doorOpenAngle = 0.0f;

    float safeMinX = -1.2f, safeMaxX = 1.2f;
    float safeMinZ = -1.2f, safeMaxZ = 1.2f;

    float stepX = (safeMaxX - safeMinX) / 3.0f;
    float stepZ = (safeMaxZ - safeMinZ) / 3.0f;

    // 1. Khai báo mảng 15 ô hợp lệ trong 1 tầng (bỏ qua ô số 12 là lỗ rơi)
    int validSlots[15] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15 };
    int currentTier = 0;
    int toysPlacedInTier = 0;

    // Trộn ngẫu nhiên mảng vị trí cho tầng đầu tiên
    for (int k = 0; k < 15; k++) {
        int swapIdx = rand() % 15;
        int temp = validSlots[k];
        validSlots[k] = validSlots[swapIdx];
        validSlots[swapIdx] = temp;
    }

    // 2. Phân bổ gấu
    for (int i = 0; i < TOY_COUNT; i++) {
        listToys[i].id = i;
        listToys[i].isGrabbed = false;
        listToys[i].isActive = true;

        // Nếu tầng hiện tại đã đầy (15 con), chuyển lên tầng trên và trộn lại vị trí
        if (toysPlacedInTier >= 15) {
            currentTier++;
            toysPlacedInTier = 0;

            for (int k = 0; k < 15; k++) {
                int swapIdx = rand() % 15;
                int temp = validSlots[k];
                validSlots[k] = validSlots[swapIdx];
                validSlots[swapIdx] = temp;
            }
        }

        // Lấy ô ngẫu nhiên từ mảng đã trộn
        int slotInTier = validSlots[toysPlacedInTier];
        int row = slotInTier / 4;
        int col = slotInTier % 4;

        // Tọa độ tâm gốc của ô đó
        float baseX = safeMinX + col * stepX;
        float baseZ = safeMinZ + row * stepZ;

        // 3. THÊM ĐỘ LỆCH NGẪU NHIÊN (JITTER)
        float jitterX = ((rand() % 100) / 100.0f - 0.5f) * 0.02f;
        float jitterZ = ((rand() % 100) / 100.0f - 0.5f) * 0.02f;

        listToys[i].position.x = baseX + jitterX;
        listToys[i].position.z = baseZ + jitterZ;

        // Chiều cao vẫn lấp từ dưới lên trên
        float stackHeightOffset = 1.15f;
        listToys[i].position.y = LIMIT_MIN_Y + (currentTier * stackHeightOffset);

        toysPlacedInTier++; // Tăng biến đếm số gấu trong tầng
    }
}
// Hàm đặt lại trạng thái vật lý về ban đầu khi bắt đầu lại trò chơi hoặc reset
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
    {
        // 1. Quét tìm con gấu CAO NHẤT đang nằm ngay dưới càng gắp
        float minDistance = 0.55f; // Bán kính vợt gắp hiệu dụng
        int highestToyIndex = -1;
        float highestToyY = LIMIT_MIN_Y - 1.0f; // Khởi tạo Y thấp hơn mặt sàn để dễ so sánh

        for (int i = 0; i < TOY_COUNT; i++) {
            if (listToys[i].isActive) {
                float dx = clawPosition.x - listToys[i].position.x;
                float dz = clawPosition.z - listToys[i].position.z;
                float distXZ = sqrt(dx * dx + dz * dz);

                // Nếu gấu nằm trong vùng kẹp VÀ có độ cao lớn hơn gấu đã lưu
                if (distXZ < minDistance && listToys[i].position.y > highestToyY) {
                    highestToyY = listToys[i].position.y;
                    highestToyIndex = i;
                }
            }
        }

        // 2. Tính toán độ cao dừng kẹp động (Dynamic Stop Height)
        // Khoảng cách bù trừ từ tâm gấu lên trục càng gắp là 1.7f (tính từ code cũ: 1.45f - (-0.25f) = 1.7f)
        float targetStopY = (highestToyIndex != -1) ? (highestToyY + 1.7f) : 1.45f;

        // Tiếp tục hạ càng xuống
        clawPosition.y -= moveSpeedY;

        // 3. Dừng hạ càng ngay khi chạm tới đỉnh của con gấu cao nhất (hoặc chạm đáy nếu không có gấu)
        if (clawPosition.y <= targetStopY) {
            clawPosition.y = targetStopY;

            // Chốt hạ mục tiêu là con gấu ở trên cùng
            if (highestToyIndex != -1) {
                grabbedToyIndex = highestToyIndex;
            }
            else {
                grabbedToyIndex = -1; // Cắp trượt, không có gấu
            }

            currentClawState = STATE_CLAMPING;
        }
    }
    break;

    case STATE_CLAMPING: // Giai đoạn kẹp chặt càng gắp
    {
        // Nếu kẹp trúng gấu, góc kẹp dừng 35 độ. Nếu trượt, 22 độ.
        float targetClampAngle = (grabbedToyIndex != -1) ? 35.0f : 22.0f;

        clawOpenAngle -= rotateSpeed * 4.0f;

        if (clawOpenAngle <= targetClampAngle) {
            clawOpenAngle = targetClampAngle;

            // Nếu đã xác định trúng gấu từ bước trước, cập nhật trạng thái của gấu
            if (grabbedToyIndex != -1) {
                listToys[grabbedToyIndex].isGrabbed = true;
            }

            currentClawState = STATE_LIFTING; // Bắt đầu nâng càng gắp lên
        }
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

        // Nếu khoảng cách còn lại nhỏ hơn quãng đường đi được trong frame này
        if (dist <= moveSpeedXZ) {
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
        listToys[grabbedToyIndex].position.y = clawPosition.y - 1.55f;
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

            float spacingX = 0.8f;
            float targetX = (side == 0) ? (-1.8f - slotIndex * spacingX) : (1.8f + slotIndex * spacingX);

            float staggerZ = (slotIndex % 2 == 0) ? 0.2f : -0.2f;
            float targetZ = 2.8f + staggerZ;

            float targetY = -3.2f;

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