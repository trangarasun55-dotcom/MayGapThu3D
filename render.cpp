#include <glut.h>
#include <math.h>

#include "render.h"
#include "globals.h"
#include "imageloader.h"

// BIẾN LƯU ID CỦA CÁC TEXTURE ĐƯỢC SỬ DỤNG TRONG CẢNH VẬT
GLuint texture_saosang, texture_osao, texture_saoroi;
GLuint texture_kim;
GLuint texture_longxam, texture_longxanhduong, texture_longxanhla, texture_longhong, texture_longkem;
GLuint texture_may, texture_maysao;

// HÀM TẢI TEXTURE TỪ FILE BMP VÀ TRẢ VỀ ID CỦA TEXTURE ĐỂ SỬ DỤNG TRONG OPENGL
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

// HÀM VẼ KHỐI HỘP CÓ TEXTURE (Thêm tùy chọn skipTop để làm rỗng mặt trên)
void drawTexturedCube(float size, bool skipTop = false) {
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
    // TOP (Cho phép ẩn đi để làm rỗng ruột khối)
    if (!skipTop) {
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0); glVertex3f(-s, s, s); glTexCoord2f(1, 0); glVertex3f(s, s, s);
        glTexCoord2f(1, 1); glVertex3f(s, s, -s); glTexCoord2f(0, 1); glVertex3f(-s, s, -s);
    }
    // BOTTOM
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s); glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, -s, s); glTexCoord2f(0, 1); glVertex3f(-s, -s, s);
    glEnd();
}

// HÀM VẼ HỘP CÓ TEXTURE ĐƯỢC SỬ DỤNG CHO THÂN MÁY
void drawBox(GLuint texture, float tx, float ty, float tz, float sx, float sy, float sz)
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
    glScalef(sx, sy, sz);
    drawTexturedCube(1.0f);
    glPopMatrix();
    if (texture == 0) glEnable(GL_TEXTURE_2D);
}

// HÀM VẼ HỘP CÓ GÓC TRÒN VÀ CÓ TEXTURE (Thêm tính năng skipTop để rỗng lòng nhưng góc vẫn bo)
void drawRoundedBox(GLuint texture, float tx, float ty, float tz, float sx, float sy, float sz, bool skipTop = false)
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

    float r = 0.2f;
    if (sx < 0.8f || sz < 0.8f) r = 0.05f;

    glPushMatrix(); glScalef(sx, sy, sz - 2 * r); drawTexturedCube(1.0f, skipTop); glPopMatrix();
    glPushMatrix(); glScalef(sx - 2 * r, sy, sz); drawTexturedCube(1.0f, skipTop); glPopMatrix();

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
        gluDisk(quad, 0.0f, r, 15, 1); // Đĩa bịt dưới
        glTranslatef(0.0f, 0.0f, sy);
        if (!skipTop) gluDisk(quad, 0.0f, r, 15, 1); // Đĩa bịt trên (ẩn nếu cần làm rỗng)
        glPopMatrix();
    }
    gluDeleteQuadric(quad);
    glPopMatrix();

    if (texture == 0) glEnable(GL_TEXTURE_2D);
}

// HÀM VẼ BÀN ĐIỀU KHIỂN
void drawControlPanel() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    drawRoundedBox(texture_saoroi, 0.0f, -0.65f, 1.9f, 3.5f, 0.3f, 0.6f);

    // VẼ ĐĨA XOAY CHÍNH GIỮA BÀN ĐIỀU KHIỂN
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_kim);
    glPushMatrix();
    glTranslatef(-0.6f, -0.5f, 1.9f);
    glRotatef(-90, 1, 0, 0);
    gluDisk(quad, 0.0f, 0.22f, 25, 1);
    glPopMatrix();

    // VẼ TRỤ TRÒN GIỮA ĐĨA XOAY CHÍNH
    glPushMatrix();
    glTranslatef(-0.6f, -0.5f, 1.9f);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, 0.04f, 0.04f, 0.55f, 20, 20);
    glPopMatrix();

    // VẼ NÚT GẠT
    glBindTexture(GL_TEXTURE_2D, texture_may);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.55f, 0.0f);
    gluSphere(quad, 0.18f, 30, 30);
    glPopMatrix();
    glPopMatrix();

    // VẼ NÚT BẤM 
    glBindTexture(GL_TEXTURE_2D, texture_may);
    glPushMatrix();
    glTranslatef(0.6f, -0.42f, 1.9f);
    glScalef(1.0f, 0.4f, 1.0f);
    gluSphere(quad, 0.2f, 30, 30);
    glPopMatrix();

    glColor3f(1, 1, 1);
    gluDeleteQuadric(quad);
}

// HÀM VẼ THÂN MÁY GẮP
void drawMachineBody() {
    // Thân vỏ bên dưới máy rỗng mặt trên (skipTop = true) để chứa máng trượt
    drawRoundedBox(texture_osao, 0.0f, -2.1f, 0.0f, 3.5f, 3.2f, 3.5f, true);

    // Mái máy che bên trên lồng giữ nguyên khối đặc
    drawRoundedBox(texture_osao, 0.0f, 4.0f, 0.0f, 3.5f, 0.8f, 3.5f, false);

    // Cột trụ đứng 4 góc giữ nguyên
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

    drawControlPanel();

    // HỆ THỐNG CỬA SẬP XẢ QUÀ
    glPushMatrix();
    glTranslatef(0.0f, -2.5f, 1.76f);
    glRotatef(doorOpenAngle, 1.0f, 0.0f, 0.0f);
    drawRoundedBox(texture_saosang, 0.0f, 0.7f, 0.0f, 1.4f, 1.4f, 0.05f);
    drawRoundedBox(texture_may, 0.0f, 1.2f, 0.05f, 0.6f, 0.1f, 0.08f);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

// HÀM VẼ SÀN MÁY KHOÉT LỖ VÀ ỐNG TRỤ RỖNG NỐI TỪ LỖ XUỐNG CỬA XẢ QUÀ
void drawDropHole() {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    float hX = DROP_ZONE.x;
    float hZ = DROP_ZONE.z;

    // Tọa độ bao ngoài của mặt sàn lồng kính (top face của phần thân)
    float minX = -1.75f, maxX = 1.75f;
    float minZ = -1.75f, maxZ = 1.75f;
    float topY = -0.5f; // Chiều cao mặt sàn

    // Tọa độ khu vực bị khoét lỗ
    float holeMinX = hX - 0.45f, holeMaxX = hX + 0.45f;
    float holeMinZ = hZ - 0.45f, holeMaxZ = hZ + 0.45f;

    // 1. VẼ MẶT SÀN CỦA MÁY (GHÉP BỞI 4 HÌNH CHỮ NHẬT XUNG QUANH LỖ)
    glBindTexture(GL_TEXTURE_2D, texture_osao);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    // Mảng Trái
    glTexCoord2f(0, 0); glVertex3f(minX, topY, maxZ);
    glTexCoord2f(1, 0); glVertex3f(holeMinX, topY, maxZ);
    glTexCoord2f(1, 1); glVertex3f(holeMinX, topY, minZ);
    glTexCoord2f(0, 1); glVertex3f(minX, topY, minZ);
    // Mảng Phải
    glTexCoord2f(0, 0); glVertex3f(holeMaxX, topY, maxZ);
    glTexCoord2f(1, 0); glVertex3f(maxX, topY, maxZ);
    glTexCoord2f(1, 1); glVertex3f(maxX, topY, minZ);
    glTexCoord2f(0, 1); glVertex3f(holeMaxX, topY, minZ);
    // Mảng Trước
    glTexCoord2f(0, 0); glVertex3f(holeMinX, topY, maxZ);
    glTexCoord2f(1, 0); glVertex3f(holeMaxX, topY, maxZ);
    glTexCoord2f(1, 1); glVertex3f(holeMaxX, topY, holeMaxZ);
    glTexCoord2f(0, 1); glVertex3f(holeMinX, topY, holeMaxZ);
    // Mảng Sau
    glTexCoord2f(0, 0); glVertex3f(holeMinX, topY, holeMinZ);
    glTexCoord2f(1, 0); glVertex3f(holeMaxX, topY, holeMinZ);
    glTexCoord2f(1, 1); glVertex3f(holeMaxX, topY, minZ);
    glTexCoord2f(0, 1); glVertex3f(holeMinX, topY, minZ);
    glEnd();

    // 2. VẼ ỐNG TRỤ RỖNG NỐI XUỐNG CỬA (Gắn màu may.bmp)
    // Tọa độ mép trong của cửa xả 
    float doorMinX = -0.7f, doorMaxX = 0.7f;
    float doorMinY = -2.5f, doorMaxY = -1.1f;
    float doorZ = 1.75f; // Ngay mép sau cánh cửa

    glBindTexture(GL_TEXTURE_2D, texture_may);
    glBegin(GL_QUADS);

    // Vách sau (Nối viền sau của lỗ với mép trên của cửa)
    glNormal3f(0, 1, -1);
    glTexCoord2f(0, 1); glVertex3f(holeMinX, topY, holeMinZ);
    glTexCoord2f(1, 1); glVertex3f(holeMaxX, topY, holeMinZ);
    glTexCoord2f(1, 0); glVertex3f(doorMaxX, doorMaxY, doorZ);
    glTexCoord2f(0, 0); glVertex3f(doorMinX, doorMaxY, doorZ);

    // Vách trước (Nối viền trước của lỗ với mép dưới của cửa)
    glNormal3f(0, -1, 1);
    glTexCoord2f(0, 1); glVertex3f(holeMaxX, topY, holeMaxZ);
    glTexCoord2f(1, 1); glVertex3f(holeMinX, topY, holeMaxZ);
    glTexCoord2f(1, 0); glVertex3f(doorMinX, doorMinY, doorZ);
    glTexCoord2f(0, 0); glVertex3f(doorMaxX, doorMinY, doorZ);

    // Vách trái (Nối viền trái của lỗ với mép trái của cửa)
    glNormal3f(-1, 0, 1);
    glTexCoord2f(0, 1); glVertex3f(holeMinX, topY, holeMaxZ);
    glTexCoord2f(1, 1); glVertex3f(holeMinX, topY, holeMinZ);
    glTexCoord2f(1, 0); glVertex3f(doorMinX, doorMaxY, doorZ);
    glTexCoord2f(0, 0); glVertex3f(doorMinX, doorMinY, doorZ);

    // Vách phải (Nối viền phải của lỗ với mép phải của cửa)
    glNormal3f(1, 0, 1);
    glTexCoord2f(0, 1); glVertex3f(holeMaxX, topY, holeMinZ);
    glTexCoord2f(1, 1); glVertex3f(holeMaxX, topY, holeMaxZ);
    glTexCoord2f(1, 0); glVertex3f(doorMaxX, doorMinY, doorZ);
    glTexCoord2f(0, 0); glVertex3f(doorMaxX, doorMaxY, doorZ);

    glEnd();
}

// HÀM VẼ CÀNG GẮP
void drawClaw() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glPushMatrix();
    glTranslatef(clawPosition.x, clawPosition.y, clawPosition.z);

    GLfloat mat_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 128.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_kim);
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    glPushMatrix();
    glTranslatef(0.0f, 3.6f - clawPosition.y, 0.0f);
    glRotatef(90, 1, 0, 0);
    gluCylinder(quad, 0.015f, 0.015f, 3.6f - clawPosition.y, 16, 16);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0.0f, 0.0f, -0.12f);
    gluCylinder(quad, 0.22f, 0.22f, 0.24f, 24, 4);
    gluDisk(quad, 0.0f, 0.22f, 24, 1);
    glTranslatef(0.0f, 0.0f, 0.24f);
    gluDisk(quad, 0.0f, 0.22f, 24, 1);
    glPopMatrix();

    float upperArmAngle = -20.0f - (clawOpenAngle * 1.4f);
    float midArmAngle = 50.0f - (clawOpenAngle * 0.25f);
    float tipArmAngle = 60.0f - (clawOpenAngle * 0.05f);

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef(i * 120.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.18f, -0.1f, 0.0f);

        glRotatef(upperArmAngle, 0.0f, 0.0f, 1.0f);
        gluSphere(quad, 0.055f, 16, 16);
        drawBox(texture_kim, 0.0f, -0.15f, 0.0f, 0.05f, 0.3f, 0.05f);
        glTranslatef(0.0f, -0.3f, 0.0f);

        glRotatef(midArmAngle, 0.0f, 0.0f, 1.0f);
        gluSphere(quad, 0.05f, 16, 16);
        drawBox(texture_kim, 0.0f, -0.125f, 0.0f, 0.045f, 0.25f, 0.045f);
        glTranslatef(0.0f, -0.25f, 0.0f);

        glRotatef(tipArmAngle, 0.0f, 0.0f, 1.0f);
        gluSphere(quad, 0.045f, 16, 16);
        drawBox(texture_kim, 0.0f, -0.1f, 0.0f, 0.04f, 0.2f, 0.04f);
        glTranslatef(0.0f, -0.2f, 0.0f);

        gluSphere(quad, 0.045f, 12, 12);
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    GLfloat default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);

    glPopMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    gluDeleteQuadric(quad);
}

// HÀM VẼ TẤM KÍNH BẢO VỆ
void drawGlassPanel(float tx, float ty, float tz, float sx, float sy, float sz) {
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glScalef(sx, sy, sz);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.5f, -0.5f, 0.5f); glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);   glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    glPopMatrix();
}

void drawMachineGlass() {
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

    drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f);
    glPushMatrix(); glRotatef(180, 0, 1, 0); drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f); glPopMatrix();
    glPushMatrix(); glRotatef(-90, 0, 1, 0); drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f); glPopMatrix();
    glPushMatrix(); glRotatef(90, 0, 1, 0);  drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f); glPopMatrix();

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

// HÀM VẼ GẤU BÔNG
void drawLabubu(float x, float y, float z, GLuint furTexture, float rotateY, bool isExiting = false) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotated(rotateY, 0, 1, 0);

    if (isExiting) {
        GLdouble clipEquation[] = { 0.0, 0.0, -1.0, 1.76 };
        glClipPlane(GL_CLIP_PLANE0, clipEquation);
        glEnable(GL_CLIP_PLANE0);
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, furTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glScalef(0.7f, 0.65f, 0.7f);
    gluSphere(quad, 0.5f, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.45f, 0.0f);
    gluSphere(quad, 0.45f, 30, 30);
    glPopMatrix();

    glPushMatrix(); glTranslatef(-0.23f, 0.8f, 0.0f); gluSphere(quad, 0.18f, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(0.23f, 0.8f, 0.0f); gluSphere(quad, 0.18f, 20, 20); glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glColor3f(0.05f, 0.05f, 0.05f);
    glPushMatrix(); glTranslatef(-0.13f, 0.48f, 0.40f); glutSolidSphere(0.045f, 15, 15); glPopMatrix();
    glPushMatrix(); glTranslatef(0.13f, 0.48f, 0.40f); glutSolidSphere(0.045f, 15, 15); glPopMatrix();

    glColor3f(0.12f, 0.12f, 0.12f);
    glPushMatrix(); glTranslatef(0.0f, 0.44f, 0.43f); glutSolidSphere(0.035f, 15, 15); glPopMatrix();

    glColor3f(0.95f, 0.25f, 0.2f);
    glPushMatrix(); glTranslatef(0.0f, 0.36f, 0.41f); glutSolidSphere(0.04f, 15, 15); glPopMatrix();

    if (isExiting) {
        glDisable(GL_CLIP_PLANE0);
    }

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

// HÀM VẼ NỀN SAO TRỜI VÀ MẶT ĐẤT
void drawBackground() {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    // CHỈNH SÀN LÊN Y = -3.7f ĐỂ KHỚP VỚI ĐÁY MÁY (Tránh lơ lửng)
    glBindTexture(GL_TEXTURE_2D, texture_may);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -3.7f, 25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -3.7f, 25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, -3.7f, -25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, -3.7f, -25.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture_maysao);
    glBegin(GL_QUADS);
    // TƯỜNG PHÍA SAU
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 35.0f, -25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 35.0f, -25.0f);

    // TƯỜNG BÊN TRÁI
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-25.0f, 35.0f, -25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 35.0f, 25.0f);

    // TƯỜNG BÊN PHẢI
    glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 35.0f, 25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(25.0f, 35.0f, -25.0f);

    // TƯỜNG PHÍA TRƯỚC
    glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-25.0f, 35.0f, 25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(25.0f, 35.0f, 25.0f);
    glEnd();

    glEnable(GL_LIGHTING);
}

void renderScene() {
    drawBackground();
    drawMachineBody();
    drawDropHole();
    drawClaw();

    GLuint furTextures[5] = { texture_longxam, texture_longxanhduong, texture_longxanhla, texture_longkem, texture_longhong };
    extern int exitingToyIndex;

    for (int i = 0; i < TOY_COUNT; i++) {
        if (!listToys[i].isActive) continue;

        bool isExiting = (i == exitingToyIndex);
        float currentRot = (float)(i * 25);
        if (isExiting) {
            currentRot = 0.0f;
        }

        drawLabubu(
            listToys[i].position.x,
            listToys[i].position.y,
            listToys[i].position.z,
            furTextures[i % 5],
            currentRot,
            isExiting
        );
    }
    drawMachineGlass();
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPosition[] = { 3.0f, 8.0f, 6.0f, 1.0f };
    GLfloat ambientLight[] = { 0.75f, 0.75f, 0.8f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
}

void initGraphics() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    texture_longxanhla = loadTexture("data/longxanhla.bmp");
    texture_longxanhduong = loadTexture("data/longxanhduong.bmp");
    texture_longxam = loadTexture("data/longxam.bmp");
    texture_longkem = loadTexture("data/longkem.bmp");
    texture_longhong = loadTexture("data/longhong.bmp");
    texture_kim = loadTexture("data/kim.bmp");
    texture_osao = loadTexture("data/osao.bmp");
    texture_saoroi = loadTexture("data/saoroi.bmp");
    texture_saosang = loadTexture("data/saosang.bmp");
    texture_may = loadTexture("data/may.bmp");
    texture_maysao = loadTexture("data/maysao.bmp");

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