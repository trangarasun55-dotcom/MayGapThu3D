#include <glut.h>
#include <math.h>

#include "render.h"
#include "globals.h"
#include "imageloader.h"

// Biến lưu trữ Textures
GLuint texture_osao; // Texture chủ đạo của thân máy
GLuint texture_kim, texture_vang, texture_pink;
GLuint texture_longxam, texture_longxanhduong, texture_longxanhla, texture_longhong, texture_longkem;
GLuint texture_saosang, texture_saotoi, texture_saoroi, texture_suoisao, texture_nensao;

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

// Hàm tiện ích vẽ hình hộp biến đổi hình khối
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

void drawSolidBox(float tx, float ty, float tz, float sx, float sy, float sz) {
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glScalef(sx, sy, sz);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Hàm vẽ thân máy thiết kế mới
void drawMachineBody() {
    // 1. Thân dưới & Mái máy đồng bộ tinh tế với texture_osao chủ đạo
    drawBox(texture_osao, 0.0f, -2.0f, 0.0f, 3.5f, 2.8f, 3.5f);
    drawBox(texture_osao, 0.0f, 3.9f, 0.0f, 3.55f, 0.8f, 3.55f);

    // 2. Khung viền và cửa nhận quà
    drawBox(texture_saosang, 0.0f, -2.2f, 1.76f, 1.2f, 1.2f, 0.06f);

    // Cánh cửa sập mở nhận gấu
    glPushMatrix();
    glTranslatef(0.0f, -1.6f, 1.79f);
    glRotatef(doorOpenAngle, 1.0f, 0.0f, 0.0f);
    drawBox(texture_saotoi, 0.0f, -0.45f, 0.0f, 0.9f, 0.9f, 0.04f);

    // Núm tay cầm cửa 
    drawBox(texture_vang, 0.0f, -0.45f, 0.03f, 0.1f, 0.1f, 0.1f);
    glPopMatrix();

    // 3. Bốn cột góc 
    float pO = 1.67f;
    float pX[] = { -pO, pO, -pO, pO };
    float pZ[] = { pO, pO, -pO, -pO };
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_nensao);

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(pX[i], -0.6f, pZ[i]);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        gluCylinder(quad, 0.08f, 0.08f, 4.5f, 30, 30);
        glPopMatrix();
    }

    // 4. Khối chạy trên trần & Bệ điều khiển tích hợp đồng bộ màu sắc
    drawBox(texture_saosang, clawPosition.x, 3.7f, clawPosition.z, 0.7f, 0.25f, 0.7f);
    drawBox(texture_saosang, 0.0f, -0.5f, 1.9f, 1.5f, 0.5f, 0.8f);

    // 5. Hệ thống điều khiển (Joystick & Nút bấm)
    drawBox(texture_pink, -0.3f, -0.2f, 2.2f, 0.24f, 0.24f, 0.24f); // Đế điều khiển

    glBindTexture(GL_TEXTURE_2D, texture_pink);
    glPushMatrix(); glTranslatef(-0.3f, -0.45f, 2.2f); glRotatef(-90, 1, 0, 0); gluCylinder(quad, 0.02f, 0.02f, 0.3f, 20, 20); glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_vang); // Cần gạt và nút bấm
    glPushMatrix(); glTranslatef(-0.3f, 0.1f, 2.2f); glutSolidSphere(0.08f, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(0.3f, -0.22f, 2.1f); glScalef(1.0f, 0.35f, 1.0f); glutSolidSphere(0.15f, 20, 20); glPopMatrix();

    gluDeleteQuadric(quad);
}

// Hàm vẽ kính máy gắp bảo vệ
void drawMachineGlass() {
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glColor4f(0.85f, 0.95f, 1.0f, 0.12f); // Kính trong suốt ánh xanh nhẹ tinh khiết hơn
    drawSolidBox(0.0f, 1.4f, 0.0f, 3.45f, 4.3f, 3.45f);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
}

// Hàm vẽ càng gắp gấu bông
void drawClaw() {
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(clawPosition.x, clawPosition.y, clawPosition.z);

    // Trục dây cáp
    glBindTexture(GL_TEXTURE_2D, texture_kim);
    glPushMatrix();
    glTranslatef(0.0f, 3.4f - clawPosition.y, 0.0f);
    glRotatef(90, 1, 0, 0);
    gluCylinder(quad, 0.015f, 0.015f, 3.4f - clawPosition.y, 20, 20);
    glPopMatrix();

    // Củ gắp trung tâm
    gluQuadricTexture(quad, GL_TRUE);
    gluSphere(quad, 0.16f, 20, 20);

    // Hệ thống 3 càng gắp
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef(i * 120.0f, 0, 1, 0);
        glRotatef(-clawOpenAngle, 1, 0, 0);
        glTranslatef(0.0f, -0.35f, 0.15f);
        glScalef(0.05f, 0.5f, 0.05f);
        gluSphere(quad, 1.0f, 20, 20);
        glPopMatrix();
    }
    glPopMatrix();
    gluDeleteQuadric(quad);
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
    glBindTexture(GL_TEXTURE_2D, texture_saoroi);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -5.0f, -15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -5.0f, -15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 22.0f, -15.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 22.0f, -15.0f);
    glEnd();

    // 2. Sàn phòng
    glBindTexture(GL_TEXTURE_2D, texture_suoisao);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -4.9f, 15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -4.9f, 15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, -4.9f, -15.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, -4.9f, -15.0f);
    glEnd();

    // 3. Tường bên trái
    glBindTexture(GL_TEXTURE_2D, texture_saoroi);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -5.0f, 15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-25.0f, -5.0f, -15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-25.0f, 22.0f, -15.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 22.0f, 15.0f);
    glEnd();

    // 4. Tường bên phải
    glBindTexture(GL_TEXTURE_2D, texture_saoroi);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, -5.0f, -15.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -5.0f, 15.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 22.0f, 15.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(25.0f, 22.0f, -15.0f);
    glEnd();

    glEnable(GL_LIGHTING);
}

// Hàm render scene chính điều phối thứ tự vẽ
void renderScene() {
    drawBackground();
    drawMachineBody();
    drawClaw();

    // Tiến hành kết xuất tập hợp các bé gấu Labubu bên trong lòng kính
    GLuint furTextures[5] = { texture_longxam, texture_longxanhduong, texture_longxanhla, texture_longkem, texture_longhong };
    for (int i = 0; i < TOY_COUNT; i++) {
        if (!listToys[i].isActive) continue;
        drawLabubu(listToys[i].position.x, listToys[i].position.y, listToys[i].position.z, furTextures[i % 5], (float)(i * 25));
    }

    // Thiết lập hệ thống hố rơi gấu chìm góc trong đáy máy gắp
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.02f, 0.02f, 0.05f); // Đáy hố sẫm tối sâu thẳm phù hợp không gian
    glPushMatrix();
    glTranslatef(-1.2f, -0.5f, 1.2f);
    drawSolidBox(0.0f, -1.0f, 0.0f, 0.9f, 0.01f, 0.9f);

    glColor3f(0.1f, 0.1f, 0.15f); // Màu thành hố
    drawSolidBox(0.0f, -0.5f, -0.45f, 0.9f, 1.0f, 0.01f);
    drawSolidBox(0.0f, -0.5f, 0.45f, 0.9f, 1.0f, 0.01f);
    drawSolidBox(-0.45f, -0.5f, 0.0f, 0.01f, 1.0f, 0.9f);
    drawSolidBox(0.45f, -0.5f, 0.0f, 0.01f, 1.0f, 0.9f);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
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
    GLfloat ambientLight[] = { 0.45f, 0.45f, 0.5f, 1.0f }; // Tăng nhẹ sắc xanh cho ánh sáng môi trường thêm huyền ảo
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

    // Nạp cấu trúc lông gấu bông
    texture_longxanhla = loadTexture("data/longxanhla.bmp");
    texture_longxanhduong = loadTexture("data/longxanhduong.bmp");
    texture_longxam = loadTexture("data/longxam.bmp");
    texture_longkem = loadTexture("data/longkem.bmp");
    texture_longhong = loadTexture("data/longhong.bmp");

    // Thành phần vỏ máy cơ học và phụ kiện
    texture_pink = loadTexture("data/pink.bmp");
    texture_kim = loadTexture("data/kim.bmp");
    texture_vang = loadTexture("data/vang.bmp");

    // Bộ tài nguyên nền và vỏ máy hệ tinh vân vì sao
    texture_saosang = loadTexture("data/saosang.bmp");
    texture_saotoi = loadTexture("data/saotoi.bmp");
    texture_saoroi = loadTexture("data/saoroi.bmp");
    texture_osao = loadTexture("data/osao.bmp");
    texture_suoisao = loadTexture("data/suoisao.bmp");
    texture_nensao = loadTexture("data/nensao.bmp");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    setupLighting();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}