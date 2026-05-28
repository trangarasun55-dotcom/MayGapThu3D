#include <glut.h>
#include <math.h>

#include "render.h"
#include "globals.h"
#include "imageloader.h"

// Biến lưu trữ Textures
GLuint texture_osao, texture_may, texture_suoisao;
GLuint texture_kim;
GLuint texture_longxam, texture_longxanhduong, texture_longxanhla, texture_longhong, texture_longkem;
GLuint texture_saosang, texture_saoroi, texture_nensao;

// Hàm tải texture
GLuint loadTexture(const char* filename) {
    int width, height;
    unsigned char* pixels = loadBMP(filename, width, height);
    if (pixels == nullptr) return 0;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    delete[] pixels;
    return textureID;
}

// Hàm vẽ khối lập phương có dán texture
void drawTexturedCube(float size) {
    float s = size / 2.0f;
    glBegin(GL_QUADS);
    // FRONT
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, s); glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s); glTexCoord2f(0, 1); glVertex3f(-s, s, s);
    // BACK
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(s, -s, -s); glTexCoord2f(1, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, -s); glTexCoord2f(0, 1); glVertex3f(s, s, -s);
    // LEFT
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s); glTexCoord2f(1, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, s); glTexCoord2f(0, 1); glVertex3f(-s, s, -s);
    // RIGHT
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(s, -s, s); glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s); glTexCoord2f(0, 1); glVertex3f(s, s, s);
    // TOP
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, s, s); glTexCoord2f(1, 0); glVertex3f(s, s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s); glTexCoord2f(0, 1); glVertex3f(-s, s, -s);
    // BOTTOM
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s); glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, -s, s); glTexCoord2f(0, 1); glVertex3f(-s, -s, s);
    glEnd();
}

// Hàm vẽ hình hộp biến đổi hình khối
void drawBox(GLuint texture, float tx, float ty, float tz, float sx, float sy, float sz) {
    if (texture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glScalef(sx, sy, sz);
    drawTexturedCube(1.0f);
    glPopMatrix();
    if (texture == 0) glEnable(GL_TEXTURE_2D);
}

// Hàm vẽ hình hộp đặc với màu sắc đơn sắc, không dán texture
void drawSolidBox(float tx, float ty, float tz, float sx, float sy, float sz) {
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glScalef(sx, sy, sz);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Hàm vẽ hình hộp CÓ BO TRÒN GÓC (sử dụng thuật toán ghép cylinder để làm mềm 4 cạnh đứng)
void drawRoundedBox(GLuint texture, float tx, float ty, float tz, float sx, float sy, float sz)
{
    if (texture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    glPushMatrix();
    glTranslatef(tx, ty, tz);

    // Tính toán bán kính bo góc phù hợp
    float r = 0.2f;
    if (sx < 0.8f || sz < 0.8f) r = 0.05f; // Bán kính nhỏ cho các vật thể nhỏ (tay nắm cửa...)

    // Vẽ 2 khối hộp chữ thập bên trong để lấp đầy phần lõi
    glPushMatrix(); glScalef(sx, sy, sz - 2 * r); drawTexturedCube(1.0f); glPopMatrix();
    glPushMatrix(); glScalef(sx - 2 * r, sy, sz); drawTexturedCube(1.0f); glPopMatrix();

    // Vẽ 4 cột trụ (cylinder) ở 4 góc để tạo độ cong mềm mại
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    float cx = sx / 2.0f - r;
    float cz = sz / 2.0f - r;
    float posX[] = { cx, cx, -cx, -cx };
    float posZ[] = { cz, -cz, cz, -cz };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(posX[i], -sy / 2.0f, posZ[i]);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        gluCylinder(quad, r, r, sy, 15, 1);
        // Bịt nắp trên và dưới của góc bo
        gluDisk(quad, 0.0f, r, 15, 1);
        glTranslatef(0.0f, 0.0f, sy);
        gluDisk(quad, 0.0f, r, 15, 1);
        glPopMatrix();
    }

    gluDeleteQuadric(quad);
    glPopMatrix();

    if (texture == 0) glEnable(GL_TEXTURE_2D);
}

// ===== HỆ THỐNG BỆ ĐIỀU KHIỂN =====
void drawControlPanel() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    // 1. Bệ gỗ điều khiển
    drawRoundedBox(texture_saoroi, 0.0f, -0.65f, 1.9f, 3.5f, 0.3f, 0.6f);

    // 2. Chân cần gạt (Texture kim loại)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_kim);
    glPushMatrix();
    glTranslatef(-0.6f, -0.5f, 1.9f);
    glRotatef(-90, 1, 0, 0);
    gluDisk(quad, 0.0f, 0.22f, 25, 1);
    glPopMatrix();

    // 3. CẦN GẠT JOYSTICK
    glPushMatrix();
    glTranslatef(-0.6f, -0.5f, 1.9f);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, 0.04f, 0.04f, 0.55f, 20, 20);
    glPopMatrix();

    // Đầu bi Joystick
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.45f, 0.9f);
    glPushMatrix();
    glTranslatef(0.0f, 0.55f, 0.0f);
    glutSolidSphere(0.18f, 30, 30);
    glPopMatrix();
    glPopMatrix();

    // 4. NÚT BẤM (Nâng cao và làm dày mặt nút)
    glColor3f(0.1f, 0.5f, 1.0f);
    glPushMatrix();
    glTranslatef(0.6f, -0.42f, 1.9f); // Y=-0.42 để nổi rõ trên mặt bệ (Y=-0.5)
    glScalef(1.0f, 0.4f, 1.0f);
    glutSolidSphere(0.2f, 30, 30);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    gluDeleteQuadric(quad);
}

// Hàm vẽ thân máy
void drawMachineBody() {
    // 1. Thân dưới
    drawRoundedBox(texture_osao, 0.0f, -2.1f, 0.0f, 3.5f, 3.2f, 3.5f);


    // 2. Mái máy
    drawRoundedBox(texture_osao, 0.0f, 4.0f, 0.0f, 3.5f, 0.8f, 3.5f);

    // 3. Bốn cột góc (Bù khe hở 0.1 để chạm khít mái)
    float pO = 1.6f;
    float pX[] = { -pO, pO, -pO, pO };
    float pZ[] = { pO, pO, -pO, -pO };
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_saoroi);

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(pX[i], -0.5f, pZ[i]);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        gluCylinder(quad, 0.1f, 0.1f, 4.1f, 20, 20);
        glPopMatrix();
    }

    // Gọi hàm vẽ hệ thống điều khiển
    drawControlPanel();

    // 4. Cửa sập và Tay nắm
    glPushMatrix();
    glTranslatef(0.0f, -1.8f, 1.76f); // Vị trí mặt trước thân máy
    glRotatef(doorOpenAngle, 1.0f, 0.0f, 0.0f);

    // Cửa sập
    drawRoundedBox(texture_saosang, 0.0f, -0.6f, 0.0f, 1.4f, 1.4f, 0.05f);

    // Tay nắm cửa (Hạ thấp Y xuống -1.1f để nằm gần cạnh dưới cửa)
    drawRoundedBox(texture_kim, 0.0f, -1.1f, 0.05f, 0.6f, 0.1f, 0.08f);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawDropHole() {
    // Bật dán texture ngôi sao sáng cho lỗ rơi [cite: 495, 527]
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_saosang); 
    [cite:495, 528] ;
    glColor3f(1.0f, 1.0f, 1.0f);

    // DROP_ZONE đặt ở đâu thì vẽ lỗ chuẩn tại đó (Ví dụ: góc trái phía trước máy) [cite: 334]
    float hX = DROP_ZONE.x; 
    [cite:334] ;
        float hZ = DROP_ZONE.z; [cite:334]
        float hY = -0.5f; // Bằng mặt sàn bệ kính

    // Vẽ 5 mặt của hộp lõm làm lỗ rơi (để khi nhìn từ trên xuống thấy chiều sâu)
    glBegin(GL_QUADS);
    // Đáy lỗ rơi
    glTexCoord2f(0.0f, 0.0f); glVertex3f(hX - 0.35f, hY - 0.4f, hZ - 0.35f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(hX + 0.35f, hY - 0.4f, hZ - 0.35f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(hX + 0.35f, hY - 0.4f, hZ + 0.35f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(hX - 0.35f, hY - 0.4f, hZ + 0.35f);

    // Thành sau lỗ
    glTexCoord2f(0.0f, 0.0f); glVertex3f(hX - 0.35f, hY - 0.4f, hZ - 0.35f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(hX + 0.35f, hY - 0.4f, hZ - 0.35f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(hX + 0.35f, hY, hZ - 0.35f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(hX - 0.35f, hY, hZ - 0.35f);

    // Thành trước lỗ
    glTexCoord2f(0.0f, 0.0f); glVertex3f(hX - 0.35f, hY - 0.4f, hZ + 0.35f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(hX + 0.35f, hY - 0.4f, hZ + 0.35f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(hX + 0.35f, hY, hZ + 0.35f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(hX - 0.35f, hY, hZ + 0.35f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// ===== HỆ THỐNG CÀNG GẮP =====
void drawClaw() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glPushMatrix();
    glTranslatef(clawPosition.x, clawPosition.y, clawPosition.z);

    // 1. DÂY TREO (Bám sát trần máy ở Y = 3.6)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_kim);
    glPushMatrix();
    glTranslatef(0.0f, 3.6f - clawPosition.y, 0.0f);
    glRotatef(90, 1, 0, 0);
    gluCylinder(quad, 0.015f, 0.015f, 3.6f - clawPosition.y, 16, 16);
    glPopMatrix();

    // 2. CỦ GẮP TRUNG TÂM
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0.0f, 0.0f, -0.12f);
    gluCylinder(quad, 0.22f, 0.22f, 0.24f, 24, 4);
    gluDisk(quad, 0.0f, 0.22f, 24, 1);
    glTranslatef(0.0f, 0.0f, 0.24f);
    gluDisk(quad, 0.0f, 0.22f, 24, 1);
    glPopMatrix();

    // 3. BA CÀNG GẮP ĐA KHỚP 
    // Tinh chỉnh công thức động học: Khớp trên xòe rộng tối đa, các khớp dưới giữ phom móc ôm gấu
    float upperArmAngle = -20.0f - (clawOpenAngle * 1.4f); // Tăng hệ số mở (1.4f) để ngàm giang rộng ra hơn hẳn
    float midArmAngle = 50.0f - (clawOpenAngle * 0.25f); // Khớp giữa mở nhẹ, phom vẫn chúi vào trong tâm
    float tipArmAngle = 60.0f - (clawOpenAngle * 0.05f); // Móng vuốt giữ nguyên góc cụp sâu tạo phom "móc câu" bám gấu

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef(i * 120.0f, 0.0f, 1.0f, 0.0f);

        glTranslatef(0.18f, -0.1f, 0.0f);

        // --- ĐOẠN 1: Cánh tay trên ---
        glRotatef(upperArmAngle, 0.0f, 0.0f, 1.0f);

        // Quả cầu Khớp nối 1 (che khe hở giữa củ gắp và cánh tay)
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.5f, 0.5f, 0.5f);
        glutSolidSphere(0.055f, 16, 16);

        glColor3f(0.85f, 0.85f, 0.85f);
        drawSolidBox(0.0f, -0.15f, 0.0f, 0.05f, 0.3f, 0.05f);
        glTranslatef(0.0f, -0.3f, 0.0f);

        // --- ĐOẠN 2: Khớp giữa ---
        glRotatef(midArmAngle, 0.0f, 0.0f, 1.0f);

        // Quả cầu Khớp nối 2 (che khe hở khi gập tay)
        glColor3f(0.55f, 0.55f, 0.55f);
        glutSolidSphere(0.05f, 16, 16);

        glColor3f(0.75f, 0.75f, 0.75f);
        drawSolidBox(0.0f, -0.125f, 0.0f, 0.045f, 0.25f, 0.045f);
        glTranslatef(0.0f, -0.25f, 0.0f);

        // --- ĐOẠN 3: Móng vuốt bám ---
        glRotatef(tipArmAngle, 0.0f, 0.0f, 1.0f);

        // Quả cầu Khớp nối 3
        glColor3f(0.6f, 0.6f, 0.6f);
        glutSolidSphere(0.045f, 16, 16);

        glColor3f(0.65f, 0.65f, 0.65f);
        drawSolidBox(0.0f, -0.1f, 0.0f, 0.04f, 0.2f, 0.04f);
        glTranslatef(0.0f, -0.2f, 0.0f);

        // Đầu mút bọc silicon bảo vệ
        glColor3f(0.0f, 0.55f, 1.0f);
        glutSolidSphere(0.045f, 12, 12);

        glEnable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    glPopMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    gluDeleteQuadric(quad);
}

// Hàm vẽ tấm kính phẳng với hiệu ứng trong suốt
void drawGlassPanel(float tx, float ty, float tz, float sx, float sy, float sz)
{
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glScalef(sx, sy, sz);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    glPopMatrix();
}

// Hàm vẽ lớp kính bảo vệ bao quanh máy gắp
void drawMachineGlass()
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    glColor4f(0.78f, 0.93f, 1.0f, 0.22f);
    float glassWidth = 3.44f;
    float glassDepth = 1.72f;
    float glassY = 1.55f;
    float glassH = 4.1f;

    // FRONT
    drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f);
    // BACK
    glPushMatrix(); glRotatef(180, 0, 1, 0); drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f); glPopMatrix();
    // LEFT
    glPushMatrix(); glRotatef(-90, 0, 1, 0); drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f); glPopMatrix();
    // RIGHT
    glPushMatrix(); glRotatef(90, 0, 1, 0); drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f); glPopMatrix();

    // Khung viền phản sáng mảnh mai
    glLineWidth(2.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    glPushMatrix();
    glTranslatef(0.0f, glassY, 0.0f);
    glScalef(glassWidth, glassH, glassWidth);
    glutWireCube(1.0f);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
}

// Hàm vẽ mô hình gấu bông Labubu
void drawLabubu(float x, float y, float z, GLuint furTexture, float rotateY) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotateY, 0, 1, 0);

    // Tạo khối chất liệu lông đặc trưng
    glBindTexture(GL_TEXTURE_2D, furTexture);
    glPushMatrix(); glScalef(0.7f, 0.65f, 0.7f); gluSphere(quad, 0.5f, 30, 30); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 0.45f, 0.0f); gluSphere(quad, 0.45f, 30, 30); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.23f, 0.8f, 0.0f); gluSphere(quad, 0.18f, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(0.23f, 0.8f, 0.0f); gluSphere(quad, 0.18f, 20, 20); glPopMatrix();

    // Đôi mắt đen láy tương phản dễ thương
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(0.08f, 0.08f, 0.08f);
    glPushMatrix(); glTranslatef(-0.12f, 0.48f, 0.38f); glutSolidSphere(0.04f, 15, 15); glPopMatrix();
    glPushMatrix(); glTranslatef(0.12f, 0.48f, 0.38f); glutSolidSphere(0.04f, 15, 15); glPopMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

// Thiết kế không gian nền phòng giả lập
void drawBackground() {
    glDisable(GL_LIGHTING);

    // 1. Tường chính diện phía sau
    glBindTexture(GL_TEXTURE_2D, texture_suoisao);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 35.0f, -25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 35.0f, -25.0f);
    glEnd();

    // 2. Sàn phòng (Mây)
    glBindTexture(GL_TEXTURE_2D, texture_may);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -4.9f, 25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -4.9f, 25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, -4.9f, -25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, -4.9f, -25.0f);
    glEnd();

    // 3. Tường bên trái
    glBindTexture(GL_TEXTURE_2D, texture_suoisao);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-25.0f, 35.0f, -25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 35.0f, 25.0f);
    glEnd();

    // 4. Tường bên phải
    glBindTexture(GL_TEXTURE_2D, texture_suoisao);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 35.0f, 25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(25.0f, 35.0f, -25.0f);
    glEnd();

    glEnable(GL_LIGHTING);
}

// Hàm render scene chính điều phối thứ tự vẽ
void renderScene() {
    drawBackground();
    drawMachineBody();
    drawClaw();
    drawDropHole();

    // Vẽ gấu
    GLuint furTextures[5] = { texture_longxam, texture_longxanhduong, texture_longxanhla, texture_longkem, texture_longhong };
    for (int i = 0; i < TOY_COUNT; i++) {
        if (!listToys[i].isActive) continue;
        drawLabubu(listToys[i].position.x, listToys[i].position.y, listToys[i].position.z, furTextures[i % 5], (float)(i * 25));
    }

    // ===== PHẦN LỖ RƠI GẤU NỔI BẬT NỐI LIỀN CỬA SẬP =====
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_saosang);
    glPushMatrix();
    glTranslatef(-1.1f, 0.0f, 1.1f); // Trục trung tâm của hố

    // Đáy hố sâu
    drawBox(texture_saosang, 0.0f, -1.0f, 0.0f, 1.0f, 0.01f, 1.0f);

    // Thành hố đi thẳng từ mép bệ (Y=-0.5) xuống sát đáy (Y=-1.0) => Tâm tại Y=-0.75, cao 0.5
    drawBox(texture_saosang, 0.0f, -0.75f, -0.5f, 1.0f, 0.5f, 0.02f);
    drawBox(texture_saosang, 0.0f, -0.75f, 0.5f, 1.0f, 0.5f, 0.02f);
    drawBox(texture_saosang, -0.5f, -0.75f, 0.0f, 0.02f, 0.5f, 1.0f);
    drawBox(texture_saosang, 0.5f, -0.75f, 0.0f, 0.02f, 0.5f, 1.0f);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);

    // Kính bảo vệ lớp ngoài cùng tối ưu độ trong suốt khi trộn tầng màu blending depth mask
    drawMachineGlass();
}

// Thiết lập nguồn sáng
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPosition[] = { 3.0f, 8.0f, 6.0f, 1.0f };
    GLfloat ambientLight[] = { 0.75f, 0.75f, 0.8f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
}

// Khởi tạo đồ họa ban đầu và nạp tài nguyên hệ thống
void initGraphics() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Nạp cấu trúc lông gấu bông Labubu
    texture_longxanhla = loadTexture("data/longxanhla.bmp");
    texture_longxanhduong = loadTexture("data/longxanhduong.bmp");
    texture_longxam = loadTexture("data/longxam.bmp");
    texture_longkem = loadTexture("data/longkem.bmp");
    texture_longhong = loadTexture("data/longhong.bmp");

    // Thành phần vỏ máy cơ học và phụ kiện cơ bản
    texture_kim = loadTexture("data/kim.bmp");

    // Bộ tài nguyên nền vũ trụ tinh vân chọn lọc sạch sẽ
    texture_saosang = loadTexture("data/saosang.bmp");
    texture_saoroi = loadTexture("data/saoroi.bmp");
    texture_osao = loadTexture("data/osao.bmp");
    texture_may = loadTexture("data/may.bmp");
    texture_suoisao = loadTexture("data/suoisao.bmp");
    texture_nensao = loadTexture("data/nensao.bmp");

    glClearColor(0.72f, 0.72f, 0.82f, 1.0f);

    setupLighting();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}