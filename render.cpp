#include <glut.h>
#include <math.h>

#include "render.h"
#include "globals.h"
#include "imageloader.h"

GLuint texture_bac;
GLuint texture_hong_pastel;
GLuint texture_kem;
GLuint texture_xanh_mint;

GLuint texture_kim;
GLuint texture_vang;

GLuint texture_longxam;
GLuint texture_longxanhduong;
GLuint texture_longxanhla;
GLuint texture_longhong;
GLuint texture_longkem;

GLuint texture_ngoclam;
GLuint texture_pink;

GLuint texture_gio;
GLuint texture_nen;

void drawBasket();
void drawBackground();
void drawCube(float x, float y, float z);

GLuint loadTexture(const char* filename)
{
    int width, height;

    unsigned char* pixels =
        loadBMP(filename, width, height);

    if (pixels == nullptr)
        return 0;

    GLuint textureID;

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] pixels;

    return textureID;
}

void drawTexturedCube(float size)
{
    float s = size / 2.0f;

    glBegin(GL_QUADS);

    // FRONT
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);

    // BACK
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(s, s, -s);

    // LEFT
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

    // RIGHT
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(s, s, s);

    // TOP
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, s, s);
    glTexCoord2f(1, 0); glVertex3f(s, s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);

    // BOTTOM
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, -s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, -s, s);

    glEnd();
}

void drawCube(float x, float y, float z)
{
    glPushMatrix();

    glScalef(x, y, z);

    glutSolidCube(1.0f);

    glPopMatrix();
}

void drawMachineBody()
{
    // =========================
    // THÂN DƯỚI
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_ngoclam);

    glPushMatrix();

    glTranslatef(0.0f, -2.0f, 0.0f);

    glScalef(3.5f, 2.8f, 3.5f);

    drawTexturedCube(1.0f);

    glPopMatrix();

    // ==========================================
    // CỬA NHẬN QUÀ ĐỘNG (CÓ MỞ RA / ĐÓNG LẠI)
    // ==========================================

    // Khung viền cửa (giữ cố định trên thân máy)
    glBindTexture(GL_TEXTURE_2D, texture_xanh_mint);
    glPushMatrix();
    glTranslatef(0.0f, -2.2f, 1.76f);
    drawCube(1.2f, 1.2f, 0.06f);
    glPopMatrix();

    // Cánh cửa sập mở hất ra ngoài (Xoay quanh bản lề phía trên đỉnh cửa tại Y = -1.6f)
    glBindTexture(GL_TEXTURE_2D, texture_pink);
    glPushMatrix();

    // Tịnh tiến bản lề lên cạnh trên của cửa để xoay cho đúng thực tế
    glTranslatef(0.0f, -1.6f, 1.79f);

    // Thực hiện xoay cánh cửa ra ngoài dựa vào biến doorOpenAngle cập nhật từ physics
    glRotatef(doorOpenAngle, 1.0f, 0.0f, 0.0f);

    // Dịch tâm khối vẽ xuống dưới để cạnh trên khớp vào vị trí bản lề
    glTranslatef(0.0f, -0.6f, 0.0f);
    drawCube(0.9f, 0.9f, 0.04f);
    glPopMatrix();

    // Núm tay cầm trên cánh cửa (Xoay đồng bộ theo cánh cửa)
    glBindTexture(GL_TEXTURE_2D, texture_kem);
    glPushMatrix();
    glTranslatef(0.0f, -1.6f, 1.79f);
    glRotatef(doorOpenAngle, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -0.25f, 0.03f); // Vị trí núm tay cầm trên mặt cửa
    glutSolidSphere(0.06f, 15, 15);
    glPopMatrix();

    // =========================
    // MÁI
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_ngoclam);

    glPushMatrix();

    glTranslatef(0.0f, 3.9f, 0.0f);

    glScalef(3.55f, 0.8f, 3.55f);

    drawTexturedCube(1.0f);

    glPopMatrix();

    // =========================
    // 4 CỘT GÓC
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_ngoclam);
    float pillarOffset = 1.67f;
    float pillarRadius = 0.1f;

    GLUquadric* pillarQuad = gluNewQuadric();
    gluQuadricTexture(pillarQuad, GL_TRUE);

    // Mảng lưu vị trí x, z của 4 cột
    float pX[] = { -pillarOffset, pillarOffset, -pillarOffset, pillarOffset };
    float pZ[] = { pillarOffset, pillarOffset, -pillarOffset, -pillarOffset };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(pX[i], -0.6f, pZ[i]); // Kéo trụ dài từ đế lên mái
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Xoay trụ đứng thẳng theo trục Y
        gluCylinder(pillarQuad, pillarRadius, pillarRadius, 4.5f, 30, 30);
        glPopMatrix();
    }
    gluDeleteQuadric(pillarQuad);
    
    // =========================
    // KHỐI DI CHUYỂN TRÊN TRẦN
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_kim);

    glPushMatrix();

    glTranslatef(
        clawPosition.x,
        3.7f,
        clawPosition.z);

    glScalef(0.7f, 0.25f, 0.7f);

    drawTexturedCube(1.0f);

    glPopMatrix();

    // =========================
    // BỆ ĐIỀU KHIỂN
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_pink);

    glPushMatrix();

    glTranslatef(0.0f, -0.5f, 1.9f);

    glScalef(1.5f, 0.5f, 0.8f);

    drawTexturedCube(1.0f);

    glPopMatrix();

    // =========================
    // ĐÈN BÁO 
    // =========================

	glBindTexture(GL_TEXTURE_2D, texture_bac); // đế đèn

    glPushMatrix();

    glTranslatef(-0.3f, -0.2f, 2.2f);

    glutSolidSphere(0.12f, 20, 20);

    glPopMatrix();

    GLUquadric* quad = gluNewQuadric();

	glBindTexture(GL_TEXTURE_2D, texture_kim); // chân đèn

    glPushMatrix();

    glTranslatef(-0.3f, -0.45f, 2.2f);

    glRotatef(-90, 1, 0, 0);

    gluCylinder(quad, 0.03f, 0.03f, 0.3f, 20, 20);

    glPopMatrix();

    // =========================
    // NÚT BẤM
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_vang);

    glPushMatrix();

    glTranslatef(0.3f, -0.22f, 2.1f);

    glScalef(1.0f, 0.35f, 1.0f);

    glutSolidSphere(0.15f, 20, 20);

    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawMachineGlass() // Mặt kính của máy gắp
{
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_FALSE);

    glColor4f(0.8f, 1.0f, 1.0f, 0.12f);

    glPushMatrix();

    glTranslatef(0.0f, 1.4f, 0.0f);

    drawCube(3.5f, 4.3f, 3.5f);

    glPopMatrix();

    glDepthMask(GL_TRUE);

    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
}

void drawClaw() // Phần gắp đồ chơi, bao gồm dây treo, đầu gắp và 3 càng gắp
{
    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();

    glTranslatef(
        clawPosition.x,
        clawPosition.y,
        clawPosition.z);

    // =========================
    // DÂY TREO
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_bac);

    glPushMatrix();

    glTranslatef(
        0.0f,
        3.4f - clawPosition.y,
        0.0f);

    glRotatef(90, 1, 0, 0);

    gluCylinder(
        quad,
        0.02f,
        0.02f,
        3.4f - clawPosition.y,
        20,
        20);

    glPopMatrix();

    // =========================
    // ĐẦU GẮP
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_kim);
    gluQuadricTexture(quad, GL_TRUE);
    gluSphere(quad, 0.18f, 20, 20);

    // =========================
    // 3 CÀNG GẮP
    // =========================

    for (int i = 0; i < 3; i++)
    {
        glBindTexture(GL_TEXTURE_2D, texture_kim);

        glPushMatrix();

        glRotatef(i * 120.0f, 0, 1, 0);

        glRotatef(-clawOpenAngle, 1, 0, 0);

        glTranslatef(0.0f, -0.35f, 0.15f);

        glScalef(0.06f, 0.55f, 0.06f);

        gluSphere(quad, 1.0f, 20, 20);

        glPopMatrix();
    }

    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawLabubu(float x, float y, float z, GLuint furTexture, float rotateY)
{
    GLUquadric* quad = gluNewQuadric();

    gluQuadricTexture(quad, GL_TRUE);

    glEnable(GL_TEXTURE_2D);

    glPushMatrix();

    glTranslatef(x, y, z);

    glRotatef(rotateY, 0, 1, 0);

    // THÂN
    glBindTexture(GL_TEXTURE_2D, furTexture);

    glPushMatrix();
    glScalef(0.7f, 0.65f, 0.7f);
    gluSphere(quad, 0.5f, 40, 40);
    glPopMatrix();

    // ĐẦU
    glPushMatrix();
    glTranslatef(0.0f, 0.45f, 0.0f);
    gluSphere(quad, 0.45f, 40, 40);
    glPopMatrix();

    // TAI TRÁI
    glPushMatrix();
    glTranslatef(-0.23f, 0.8f, 0.0f);
    gluSphere(quad, 0.18f, 30, 30);
    glPopMatrix();

    // TAI PHẢI
    glPushMatrix();
    glTranslatef(0.23f, 0.8f, 0.0f);
    gluSphere(quad, 0.18f, 30, 30);
    glPopMatrix();

    // MẮT
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(-0.12f, 0.48f, 0.38f);
    glutSolidSphere(0.04f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.12f, 0.48f, 0.38f);
    glutSolidSphere(0.04f, 20, 20);
    glPopMatrix();

    // KẾT THÚC VẼ gấu, BẬT LẠI TEXTURE CHO CÁC PHẦN KHÁC
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void setupLighting()
{
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPosition[] =
    {
        5.0f,
        8.0f,
        5.0f,
        1.0f
    };

    GLfloat ambientLight[] =
    {
        0.4f,
        0.4f,
        0.4f,
        1.0f
    };

    GLfloat diffuseLight[] =
    {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    };

    glLightfv(
        GL_LIGHT0,
        GL_POSITION,
        lightPosition);

    glLightfv(
        GL_LIGHT0,
        GL_AMBIENT,
        ambientLight);

    glLightfv(
        GL_LIGHT0,
        GL_DIFFUSE,
        diffuseLight);
}

void drawBasket()
{
    glBindTexture(GL_TEXTURE_2D, texture_gio);
    glPushMatrix();
    // Đặt giỏ ngay dưới cửa xả
    glTranslatef(0.0f, -3.4f, 2.6f);

    // Vẽ thân giỏ (Dùng khối rỗng hoặc nắp hở)
    // Mặt đáy
    glPushMatrix(); 
    glScalef(1.2f, 0.05f, 1.2f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    // 4 Vách gi
    glPushMatrix(); 
    glTranslatef(0.0f, 0.3f, 0.6f); 
    glScalef(1.2f, 0.6f, 0.05f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(0.0f, 0.3f, -0.6f); 
    glScalef(1.2f, 0.6f, 0.05f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(0.6f, 0.3f, 0.0f); 
    glScalef(0.05f, 0.6f, 1.2f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(-0.6f, 0.3f, 0.0f); 
    glScalef(0.05f, 0.6f, 1.2f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glPopMatrix();
}

void drawBackground()
{
    glDisable(GL_LIGHTING); // Background không cần đổ bóng
    glBindTexture(GL_TEXTURE_2D, texture_nen);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -15.0f); // Đẩy ra xa sau máy gắp
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); 
    glVertex3f(-20.0f, -10.0f, 0.0f);

    glTexCoord2f(1, 0); 
    glVertex3f(20.0f, -10.0f, 0.0f);

    glTexCoord2f(1, 1); 
    glVertex3f(20.0f, 15.0f, 0.0f);

    glTexCoord2f(0, 1); 
    glVertex3f(-20.0f, 15.0f, 0.0f);
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void renderScene()
{
    drawBackground();
    drawMachineBody();
    drawClaw();
    drawBasket();

	// VẼ GẤU BÔNG TRONG MÁY GẮP
    GLuint furTextures[5] =
    {
        texture_longxam,
        texture_longxanhduong,
        texture_longxanhla,
        texture_longkem,
        texture_longhong
    };

    for (int i = 0; i < TOY_COUNT; i++)
    {
        if (!listToys[i].isActive)
            continue;

        float rotateAngle =
            static_cast<float>(i * 25);

        drawLabubu(
            listToys[i].position.x,
            listToys[i].position.y,
            listToys[i].position.z,

            furTextures[i % 5],

            rotateAngle);
    }

    // ==========================================
        // VẼ LỖ RƠI CÓ CHIỀU SÂU (Chuyển sang góc trái trước)
        // ==========================================
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.05f, 0.05f, 0.05f); // Màu rất tối tạo cảm giác sâu
    glPushMatrix();
    glTranslatef(-1.2f, -0.5f, 1.2f);

    // Đáy lỗ sâu bên dưới
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    glScalef(0.9f, 0.01f, 0.9f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 4 thành ống của lỗ rơi
    glColor3f(0.15f, 0.15f, 0.15f);
    glPushMatrix(); glTranslatef(0.0f, -0.5f, -0.45f); glScalef(0.9f, 1.0f, 0.01f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, -0.5f, 0.45f);  glScalef(0.9f, 1.0f, 0.01f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.45f, -0.5f, 0.0f); glScalef(0.01f, 1.0f, 0.9f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.45f, -0.5f, 0.0f);  glScalef(0.01f, 1.0f, 0.9f); glutSolidCube(1.0f); glPopMatrix();

    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    drawMachineGlass();
}

void initGraphics()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);

    glEnable(GL_LINE_SMOOTH);

    glHint(
        GL_LINE_SMOOTH_HINT,
        GL_NICEST);

    glTexEnvf(
        GL_TEXTURE_ENV,
        GL_TEXTURE_ENV_MODE,
        GL_MODULATE
    );

    texture_bac =loadTexture("data/bac.bmp");
    texture_hong_pastel = loadTexture("data/hong_pastel.bmp");
    texture_kem = loadTexture("data/kem.bmp");
    texture_xanh_mint = loadTexture("data/xanh_mint.bmp");
    
    texture_longxanhla = loadTexture("data/longxanhla.bmp");
	texture_longxanhduong = loadTexture("data/longxanhduong.bmp");
	texture_longxam = loadTexture("data/longxam.bmp");
    texture_longkem = loadTexture("data/longkem.bmp");
    texture_longhong = loadTexture("data/longhong.bmp");

    texture_ngoclam = loadTexture("data/ngoclam.bmp");
    texture_pink = loadTexture("data/pink.bmp");
    texture_kim = loadTexture("data/kim.bmp");
    texture_vang = loadTexture("data/vang.bmp");

    texture_nen =  loadTexture("data/nen.bmp");
    texture_gio = loadTexture("data/gio.bmp");

	// Thiết lập màu nền nhẹ nhàng
    glClearColor(
        0.98f, // R (Đỏ - giữ độ sáng cao)
        0.96f, // G (Xanh lá - giữ độ sáng cao)
        0.89f, // B (Xanh dương - hạ thấp xuống một chút để tạo sắc vàng kem)
        1.0f);

    setupLighting();

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(
        45.0,
        800.0 / 600.0,
        1.0,
        100.0);

    glMatrixMode(GL_MODELVIEW);
}