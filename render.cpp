#include <glut.h>
#include <math.h>

#include "render.h"
#include "globals.h"
#include "imageloader.h"

// BIẾN LƯU ID CỦA CÁC TEXTURE ĐƯỢC SỬ DỤNG TRONG CẢNH VẬT
GLuint texture_osao, texture_may, texture_suoisao;
GLuint texture_kim;
GLuint texture_longxam, texture_longxanhduong, texture_longxanhla, texture_longhong, texture_longkem;
GLuint texture_saosang, texture_saoroi, texture_nensao;

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

// HÀM VẼ KHỐI HỘP CÓ TEXTURE, ĐƯỢC SỬ DỤNG LÀM NỀN CHO THÂN MÁY GẮP
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

// HÀM VẼ HỘP ĐƠN GIẢN KHÔNG CÓ GÓC TRÒN VÀ KHÔNG CÓ TEXTURE
void drawSolidBox(float tx, float ty, float tz, float sx, float sy, float sz) {
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glScalef(sx, sy, sz);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// HÀM VẼ HỘP CÓ GÓC TRÒN VÀ CÓ TEXTURE, ĐƯỢC SỬ DỤNG CHO THÂN MÁY VÀ BÀN ĐIỀU KHIỂN
void drawRoundedBox(GLuint texture, float tx, float ty, float tz, float sx, float sy, float sz) 
{ 
    if (texture > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
    // Vẽ phần thân hộp chính giữa, đã được cắt bớt 2 * r ở mỗi chiều để tạo chỗ cho các góc tròn
    glPushMatrix();
    glTranslatef(tx, ty, tz);

    float r = 0.2f;
    if (sx < 0.8f || sz < 0.8f) r = 0.05f;

    glPushMatrix(); glScalef(sx, sy, sz - 2 * r); drawTexturedCube(1.0f); glPopMatrix();
    glPushMatrix(); glScalef(sx - 2 * r, sy, sz); drawTexturedCube(1.0f); glPopMatrix();
	// Vẽ phần góc tròn của hộp bằng cách sử dụng gluCylinder và gluDisk
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    float cx = sx / 2.0f - r;
    float cz = sz / 2.0f - r;
    float posX[] = { cx, cx, -cx, -cx };
    float posZ[] = { cz, -cz, cz, -cz };
    // Vẽ 4 góc tròn của hộp
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(posX[i], -sy / 2.0f, posZ[i]);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        gluCylinder(quad, r, r, sy, 15, 1);
        gluDisk(quad, 0.0f, r, 15, 1);
        glTranslatef(0.0f, 0.0f, sy);
        gluDisk(quad, 0.0f, r, 15, 1);
        glPopMatrix();
    }
	// Khôi phục trạng thái render cho các phần tử khác trong cảnh vật
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
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.45f, 0.9f);
    glPushMatrix();
    glTranslatef(0.0f, 0.55f, 0.0f);
    glutSolidSphere(0.18f, 30, 30);
    glPopMatrix();
    glPopMatrix();

    // VẼ NÚT BẤM
    glColor3f(0.1f, 0.5f, 1.0f);
    glPushMatrix();
    glTranslatef(0.6f, -0.42f, 1.9f);
    glScalef(1.0f, 0.4f, 1.0f);
    glutSolidSphere(0.2f, 30, 30);
    glPopMatrix();

	// Khôi phục trạng thái render cho các phần tử khác trong cảnh vật
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    gluDeleteQuadric(quad);
}

// HÀM VẼ THÂN MÁY GẮP, BAO GỒM THÂN VỎ CHÍNH, MÁI CHE, CỘT TRỤ ĐỨNG 4 GÓC, BÀN ĐIỀU KHIỂN VÀ HỆ THỐNG CỬA SẬP XẢ QUÀ CHÍNH GIỮA MẶT TRƯỚC THÂN MÁY
void drawMachineBody() {
    // Thân vỏ bên dưới máy
    drawRoundedBox(texture_osao, 0.0f, -2.1f, 0.0f, 3.5f, 3.2f, 3.5f);

    // Mái máy che bên trên lồng
    drawRoundedBox(texture_osao, 0.0f, 4.0f, 0.0f, 3.5f, 0.8f, 3.5f);

	// Cột trụ đứng 4 góc xung quanh thân máy
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

    drawControlPanel(); // VẼ BÀN ĐIỀU KHIỂN 

    // =========================================================================
    // HỆ THỐNG CỬA SẬP XẢ QUÀ CHÍNH GIỮA MẶT TRƯỚC THÂN MÁY (BẢN LỀ ĐÁY THỰC TẾ)
    // =========================================================================
    glPushMatrix();
    // 1. Tịnh tiến đến vị trí KHỚP BẢN LỀ Ở CẠNH ĐÁY CỬA (Y = -2.5f, Z = 1.76f)
    glTranslatef(0.0f, -2.5f, 1.76f);

    // 2. Thực hiện xoay lật ngửa cửa ra phía trước quanh trục X
    glRotatef(doorOpenAngle, 1.0f, 0.0f, 0.0f);

    // 3. Tịnh tiến ngược lên một nửa chiều cao tấm cửa để định tâm vẽ khối hộp chuẩn
    // Vì cửa cao 1.4f, tâm hình học của nó sẽ cách bản lề đáy một khoảng Y = +0.7f
    drawRoundedBox(texture_saosang, 0.0f, 0.7f, 0.0f, 1.4f, 1.4f, 0.05f);

    // Vẽ tay nắm cửa xả ở phía trên đầu tấm cửa (cách đáy bản lề Y = 1.2f)
    drawRoundedBox(texture_kim, 0.0f, 1.2f, 0.05f, 0.6f, 0.1f, 0.08f);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

// HÀM VẼ LỖ RƠI DUY NHẤT ĐƯỢC TỐI ƯU TOẠ ĐỘ ĐỘNG THEO DROP_ZONE
void drawDropHole() {
    // TẮT TUYỆT ĐỐI TEXTURE VÀ LIÊN KẾT BẰNG 0 ĐỂ XÓA BỘ NHỚ ĐỆM TEXTURE TRƯỚC ĐÓ
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_LIGHTING); // Tắt đèn để chủ động phối màu tạo độ sâu giả lập

    float hX = DROP_ZONE.x;
    float hZ = DROP_ZONE.z;
    float hY = -0.40f; // Bắt đầu lõm sâu xuống từ mặt sàn kính bảo vệ

    // Vẽ khối hộp chìm lõm sâu xuống kết nối lòng thân dưới của máy gắp
    glBegin(GL_QUADS);

    // 1. ĐÁY LỖ (Màu xám tối hơn để tạo cảm giác sâu hút)
    glColor3f(0.6f, 0.6f, 0.6f);
    glVertex3f(hX - 0.45f, -2.0f, hZ - 0.45f);
    glVertex3f(hX + 0.45f, -2.0f, hZ - 0.45f);
    glVertex3f(hX + 0.45f, -2.0f, hZ + 0.45f);
    glVertex3f(hX - 0.45f, -2.0f, hZ + 0.45f);

    // 2. VÁCH TRƯỚC (Hiệu ứng gradient: Trên trắng, dưới xám)
    glColor3f(0.6f, 0.6f, 0.6f); glVertex3f(hX - 0.45f, -2.0f, hZ - 0.45f);
    glColor3f(0.6f, 0.6f, 0.6f); glVertex3f(hX + 0.45f, -2.0f, hZ - 0.45f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(hX + 0.45f, hY, hZ - 0.45f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(hX - 0.45f, hY, hZ - 0.45f);

    // 3. VÁCH SAU
    glColor3f(0.6f, 0.6f, 0.6f); glVertex3f(hX - 0.45f, -2.0f, hZ + 0.45f);
    glColor3f(0.6f, 0.6f, 0.6f); glVertex3f(hX + 0.45f, -2.0f, hZ + 0.45f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(hX + 0.45f, hY, hZ + 0.45f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(hX - 0.45f, hY, hZ + 0.45f);

    // 4. VÁCH TRÁI
    glColor3f(0.6f, 0.6f, 0.6f); glVertex3f(hX - 0.45f, -2.0f, hZ - 0.45f);
    glColor3f(0.6f, 0.6f, 0.6f); glVertex3f(hX - 0.45f, -2.0f, hZ + 0.45f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(hX - 0.45f, hY, hZ + 0.45f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(hX - 0.45f, hY, hZ - 0.45f);

    // 5. VÁCH PHẢI
    glColor3f(0.6f, 0.6f, 0.6f); glVertex3f(hX + 0.45f, -2.0f, hZ - 0.45f);
    glColor3f(0.6f, 0.6f, 0.6f); glVertex3f(hX + 0.45f, -2.0f, hZ + 0.45f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(hX + 0.45f, hY, hZ + 0.45f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(hX + 0.45f, hY, hZ - 0.45f);

    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); // Trả lại màu gốc
}

// HÀM VẼ CÀNG GẮP
void drawClaw() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE); // Bật toạ độ texture cho các hàm GLU

    glPushMatrix();
    glTranslatef(clawPosition.x, clawPosition.y, clawPosition.z);

    // =====================================================================
    // 1. TĂNG CƯỜNG VẬT LIỆU KIM LOẠI (Tăng độ tương phản ánh sáng)
    // =====================================================================
    GLfloat mat_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 128.0f }; // Đẩy độ bóng lên mức tối đa của OpenGL (128)

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_kim);
    glColor3f(1.0f, 1.0f, 1.0f);

    // =====================================================================
    // 2. [QUAN TRỌNG] BẬT SPHERE MAPPING ĐỂ TẠO HIỆU ỨNG PHẢN GƯƠNG
    // =====================================================================
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    // Vẽ trụ tròn chính của càng gắp
    glPushMatrix();
    glTranslatef(0.0f, 3.6f - clawPosition.y, 0.0f);
    glRotatef(90, 1, 0, 0);
    gluCylinder(quad, 0.015f, 0.015f, 3.6f - clawPosition.y, 16, 16);
    glPopMatrix();

    // Vẽ phần đầu trụ tròn của càng gắp
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0.0f, 0.0f, -0.12f);
    gluCylinder(quad, 0.22f, 0.22f, 0.24f, 24, 4);
    gluDisk(quad, 0.0f, 0.22f, 24, 1);
    glTranslatef(0.0f, 0.0f, 0.24f);
    gluDisk(quad, 0.0f, 0.22f, 24, 1);
    glPopMatrix();

    // Tính toán góc xoay cho từng khớp cánh càng gắp
    float upperArmAngle = -20.0f - (clawOpenAngle * 1.4f);
    float midArmAngle = 50.0f - (clawOpenAngle * 0.25f);
    float tipArmAngle = 60.0f - (clawOpenAngle * 0.05f);

    // VẼ 3 CÁNH CÀNG GẮP XUNG QUANH TRỤC GIỮA
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef(i * 120.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.18f, -0.1f, 0.0f);

        // VẼ CÁNH CÀNG 1
        glRotatef(upperArmAngle, 0.0f, 0.0f, 1.0f);
        gluSphere(quad, 0.055f, 16, 16);
        drawBox(texture_kim, 0.0f, -0.15f, 0.0f, 0.05f, 0.3f, 0.05f);
        glTranslatef(0.0f, -0.3f, 0.0f);

        // VẼ CÁNH CÀNG 2
        glRotatef(midArmAngle, 0.0f, 0.0f, 1.0f);
        gluSphere(quad, 0.05f, 16, 16);
        drawBox(texture_kim, 0.0f, -0.125f, 0.0f, 0.045f, 0.25f, 0.045f);
        glTranslatef(0.0f, -0.25f, 0.0f);

        // VẼ CÁNH CÀNG 3
        glRotatef(tipArmAngle, 0.0f, 0.0f, 1.0f);
        gluSphere(quad, 0.045f, 16, 16);
        drawBox(texture_kim, 0.0f, -0.1f, 0.0f, 0.04f, 0.2f, 0.04f);
        glTranslatef(0.0f, -0.2f, 0.0f);

        // VẼ ĐẦU MÓNG CÀNG GẮP
        gluSphere(quad, 0.045f, 12, 12);

        glPopMatrix();
    }

    // =====================================================================
    // 3. TẮT SPHERE MAPPING VÀ RESET VẬT LIỆU
    // =====================================================================
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    // Trả lại vật liệu mặc định cho các object khác khỏi bị vạ lây
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

// HÀM VẼ TẤM KÍNH BẢO VỆ TRONG SUỐT XUNG QUANH MÁY GẮP, ĐƯỢC THIẾT KẾ ĐỂ TẠO RA HIỆU ỨNG TRONG SUỐT VÀ CHỊU TÁC ĐỘNG CỦA ÁNH SÁNG, GIÚP NHÌN RÕ RÀNG GẤU BÔNG BÊN TRONG MÁY GẮP
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

	// Màu kính bảo vệ trong suốt để nhìn rõ gấu bông bên trong
    glColor4f(0.78f, 0.93f, 1.0f, 0.22f);
    float glassWidth = 3.44f;
    float glassDepth = 1.72f;
    float glassY = 1.55f;
    float glassH = 4.1f;

    // Vẽ 4 tấm kính bảo vệ xung quanh máy gắp
    drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f);
    glPushMatrix(); glRotatef(180, 0, 1, 0); drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f); glPopMatrix();
    glPushMatrix(); glRotatef(-90, 0, 1, 0); drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f); glPopMatrix();
    glPushMatrix(); glRotatef(90, 0, 1, 0);  drawGlassPanel(0.0f, glassY, glassDepth, glassWidth, glassH, 0.01f); glPopMatrix();

    // Vẽ khung dây viền xung quanh tấm kính bảo vệ
    glLineWidth(2.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    glPushMatrix();
    glTranslatef(0.0f, glassY, 0.0f);
    glScalef(glassWidth, glassH, glassWidth);
    glutWireCube(1.0f);
    glPopMatrix();

	// KHÔI PHỤC TRẠNG THÁI RENDER CHO CÁC PHẦN TỬ KHÁC TRONG CẢNH VẬT
    glEnable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
}

// HÀM VẼ GẤU BÔNG (hỗ trợ clipping khi chui qua cửa)
void drawLabubu(float x, float y, float z, GLuint furTexture, float rotateY, bool isExiting = false) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotated(rotateY, 0, 1, 0);

    // Nếu gấu đang chui ra cửa, thiết lập mặt phẳng cắt để ẩn phần gấu bên ngoài vỏ máy khi chưa mở hẳn
    if (isExiting) {
        // Phương trình mặt phẳng trùng với mặt trước vỏ máy (Z = 1.76). 
        // Chỉ vẽ những gì có Z > 1.76 (phía trước máy) hoặc điều chỉnh theo góc mở của cửa
        // Để đơn giản và trực quan, ta giới hạn không cho gấu hiển thị nếu vượt quá mặt phẳng cửa khi góc mở chưa đủ
        GLdouble clipEquation[] = { 0.0, 0.0, -1.0, 1.76 };
        glClipPlane(GL_CLIP_PLANE0, clipEquation);
        glEnable(GL_CLIP_PLANE0);
    }

    // =========================================================================
    // 1. VẼ THÂN VÀ ĐẦU GẤU BÔNG (CÓ TEXTURE LÔNG)
    // =========================================================================
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, furTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Thân dưới
    glPushMatrix();
    glScalef(0.7f, 0.65f, 0.7f);
    gluSphere(quad, 0.5f, 30, 30);
    glPopMatrix();

    // Đầu gấu
    glPushMatrix();
    glTranslatef(0.0f, 0.45f, 0.0f);
    gluSphere(quad, 0.45f, 30, 30);
    glPopMatrix();

    // Tai trái & Tai phải
    glPushMatrix(); glTranslatef(-0.23f, 0.8f, 0.0f); gluSphere(quad, 0.18f, 20, 20); glPopMatrix();
    glPushMatrix(); glTranslatef(0.23f, 0.8f, 0.0f); gluSphere(quad, 0.18f, 20, 20); glPopMatrix();

    // =========================================================================
    // 2. VẼ NGŨ QUAN
    // =========================================================================
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Mắt trái
    glColor3f(0.05f, 0.05f, 0.05f);
    glPushMatrix(); glTranslatef(-0.13f, 0.48f, 0.40f); glutSolidSphere(0.045f, 15, 15); glPopMatrix();

    // Mắt phải
    glPushMatrix(); glTranslatef(0.13f, 0.48f, 0.40f); glutSolidSphere(0.045f, 15, 15); glPopMatrix();

    // Mũi
    glColor3f(0.12f, 0.12f, 0.12f);
    glPushMatrix(); glTranslatef(0.0f, 0.44f, 0.43f); glutSolidSphere(0.035f, 15, 15); glPopMatrix();

    // Miệng
    glColor3f(0.95f, 0.25f, 0.2f);
    glPushMatrix(); glTranslatef(0.0f, 0.36f, 0.41f); glutSolidSphere(0.04f, 15, 15); glPopMatrix();

    // =========================================================================
    // 3. KHÔI PHỤC LẠI TRẠNG THÁI MẶC ĐỊNH
    // =========================================================================
    if (isExiting) {
        glDisable(GL_CLIP_PLANE0);
    }

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPopMatrix();
    gluDeleteQuadric(quad);
}

// HÀM VẼ NỀN SAO TRỜI VÀ CÁC BỨC TƯỜNG XUNG QUANH MÁY GẮP, TẠO KHÔNG GIAN SÂU VÀ TRỰC QUAN NHÌN RÕ RÀNG CHO TOÀN BỘ CẢNH VẬT
void drawBackground() {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f); // Reset màu sắc dán texture

    // =========================================================================
    // 1. MẶT NỀN ĐẤT DƯỚI CHÂN (Trục -Y) - Sử dụng texture_may
    // =========================================================================
    glBindTexture(GL_TEXTURE_2D, texture_may);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -4.9f, 25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -4.9f, 25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, -4.9f, -25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, -4.9f, -25.0f);
    glEnd();

    // =========================================================================
    // 4 MẶT TƯỜNG XUNG QUANH MÁY - Đều sử dụng texture_suoisao
    // =========================================================================
    glBindTexture(GL_TEXTURE_2D, texture_suoisao);
    glBegin(GL_QUADS);

    // MẶT 1: TƯỜNG PHÍA SAU MÁY (Trục -Z)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 35.0f, -25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 35.0f, -25.0f);

    // MẶT 2: TƯỜNG BÊN TRÁI MÁY (Trục -X)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-25.0f, 35.0f, -25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 35.0f, 25.0f);

    // MẶT 3: TƯỜNG BÊN PHẢI MÁY (Trục +X)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, -15.0f, -25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 35.0f, 25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(25.0f, 35.0f, -25.0f);

    // MẶT 4: TƯỜNG PHÍA TRƯỚC MÁY (Trục +Z)
    // Tường này bao bọc phía sau lưng góc nhìn chính diện để khi xoay camera không bị trống không gian
    glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-25.0f, -15.0f, 25.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-25.0f, 35.0f, 25.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(25.0f, 35.0f, 25.0f);

    glEnd();

    glEnable(GL_LIGHTING);
}

// HÀM CHÍNH THỰC HIỆN VẼ (thay đổi thứ tự và logic vẽ gấu chui ra cửa)
void renderScene() {
    drawBackground();   // 1. Vẽ bầu trời và mặt đất

    drawMachineBody();  // 2. Vẽ thân máy trước (để làm cấu trúc nền chứa hố và cửa)

    drawDropHole();     // 3. Vẽ lỗ rơi đè lên thân máy

    drawClaw();         // 4. Vẽ càng gắp gấu

    // 5. Vẽ danh sách gấu bông Labubu
    GLuint furTextures[5] = { texture_longxam, texture_longxanhduong, texture_longxanhla, texture_longkem, texture_longhong };

    // Giả định extern biến quản lý gấu đang ra từ file physics cấu trúc của bạn
    extern int exitingToyIndex;

    for (int i = 0; i < TOY_COUNT; i++) {
        if (!listToys[i].isActive) continue;

        // Kiểm tra xem chú gấu này có phải là con đang lăn ra ngoài cửa hay không
        bool isExiting = (i == exitingToyIndex);

        // Nếu gấu đang chui ra cửa, chúng ta có thể chủ động điều chỉnh nhẹ cao độ Y 
        // hoặc hướng xoay để nó chui đầu ra trước một cách mượt mà
        float currentRot = (float)(i * 25);
        if (isExiting) {
            currentRot = 0.0f; // Cho gấu quay mặt ra hướng chính diện khi chui ra cửa
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

    drawMachineGlass(); // 6. Vẽ kính trong suốt luôn luôn ở cuối cùng để xử lý Alpha nén chuẩn
}

// HÀM THIẾT LẬP HỆ THỐNG ÁNH SÁNG CHO CẢ CẢNH VÀ MÁY GẮP
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPosition[] = { 3.0f, 8.0f, 6.0f, 1.0f };
    GLfloat ambientLight[] = { 0.75f, 0.75f, 0.8f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // [BỔ SUNG]: Thêm ánh sáng phản quang (Specular) để chiếu lên kim loại
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

	// [BỔ SUNG]: Thiết lập ánh sáng phản quang để tạo hiệu ứng sáng bóng trên các bề mặt kim loại của máy gắp
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
}

// HÀM KHỞI TẠO CÁC THAM SỐ ĐỒ HỌA VÀ TẢI TEXTURE CHO MÁY GẮP VÀ NỀN SAO TRỜI
void initGraphics() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Tải tất cả texture cần thiết cho máy gắp và nền sao trời
    texture_longxanhla = loadTexture("data/longxanhla.bmp");
    texture_longxanhduong = loadTexture("data/longxanhduong.bmp");
    texture_longxam = loadTexture("data/longxam.bmp");
    texture_longkem = loadTexture("data/longkem.bmp");
    texture_longhong = loadTexture("data/longhong.bmp");

    texture_kim = loadTexture("data/kim.bmp");

    texture_saosang = loadTexture("data/saosang.bmp");
    texture_saoroi = loadTexture("data/saoroi.bmp");
    texture_osao = loadTexture("data/osao.bmp");
    texture_may = loadTexture("data/may.bmp");
    texture_suoisao = loadTexture("data/suoisao.bmp");
    texture_nensao = loadTexture("data/nensao.bmp");

    glClearColor(0.72f, 0.72f, 0.82f, 1.0f);
    
    setupLighting();

	// Thiết lập ma trận chiếu để có góc nhìn rộng và tỷ lệ khung hình phù hợp
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

	// Bật chế độ tự động chuẩn hóa vector pháp tuyến để đảm bảo ánh sáng chính xác khi có phép biến đổi tỷ lệ
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}