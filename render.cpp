#include <glut.h>
#include <math.h>

//=====================================================
// VẼ KHỐI HỘP
//=====================================================
void drawCube(float x, float y, float z)
{
    glPushMatrix();
    glScalef(x, y, z);
    glutSolidCube(1.0);
    glPopMatrix();
}

//=====================================================
// THÂN MÁY GẮP THÚ (Đã thêm chi tiết Pop Mart)
//=====================================================
void drawMachineBody()
{
    //=====================
    // THÂN DƯỚI
    //=====================
    glColor3f(1.0f, 0.75f, 0.82f);
    glPushMatrix();
    glTranslatef(0.0f, -2.0f, 0.0f);
    drawCube(3.5f, 2.8f, 3.5f);
    glPopMatrix();

    //=================================================
    // [THÊM MỚI] CỬA NHẬN QUÀ Ở MẶT TRƯỚC THÂN DƯỚI
    //=================================================
    // Khung viền cửa màu xanh mint nhạt
    glColor3f(0.75f, 0.9f, 0.85f);
    glPushMatrix();
    glTranslatef(0.0f, -2.2f, 1.76f); // Nhô ra một chút so với mặt thân (1.75f) để tránh trùng mặt
    drawCube(1.2f, 1.2f, 0.06f);
    glPopMatrix();

    // Lòng cửa sập màu hồng nhạt
    glColor3f(1.0f, 0.85f, 0.88f);
    glPushMatrix();
    glTranslatef(0.0f, -2.2f, 1.79f);
    drawCube(0.9f, 0.9f, 0.04f);
    glPopMatrix();

    // Núm tay cầm nhỏ trên cửa quà
    glColor3f(0.75f, 0.9f, 0.85f);
    glPushMatrix();
    glTranslatef(0.0f, -1.85f, 1.82f);
    glutSolidSphere(0.06f, 15, 15);
    glPopMatrix();


    //=====================
    // MÁI
    //=====================
    glColor3f(1.0f, 0.85f, 0.9f);
    glPushMatrix();
    glTranslatef(0.0f, 3.9f, 0.0f);
    drawCube(3.55f, 0.8f, 3.55f);
    glPopMatrix();

    //=================================================
    // [THÊM MỚI] 4 CỘT TRỤ GÓC LỒNG KÍNH (MÀU XANH MINT)
    //=================================================
    glColor3f(0.75f, 0.9f, 0.85f);
    float pOffset = 1.7f; // Tọa độ đặt cột sát các góc kính
    float pSize = 0.16f;  // Độ dày của cột trụ

    // Cột Trước - Trái
    glPushMatrix(); 
    glTranslatef(-pOffset, 1.4f, pOffset); 
    drawCube(pSize, 4.3f, pSize); 
    glPopMatrix();
    
    // Cột Trước - Phải
    glPushMatrix(); 
    glTranslatef(pOffset, 1.4f, pOffset); 
    drawCube(pSize, 4.3f, pSize); 
    glPopMatrix();
    
    // Cột Sau - Trái
    glPushMatrix(); 
    glTranslatef(-pOffset, 1.4f, -pOffset); 
    drawCube(pSize, 4.3f, pSize); 
    glPopMatrix();
    
    // Cột Sau - Phải
    glPushMatrix(); 
    glTranslatef(pOffset, 1.4f, -pOffset); 
    drawCube(pSize, 4.3f, pSize); 
    glPopMatrix();

    //=================================================
    // [THÊM MỚI] TRỤC BỘ DI CHUYỂN CẦN GẮP (TRÊN TRẦN MÁY)
    //=================================================
    glColor3f(0.75f, 0.9f, 0.85f);
    glPushMatrix();
    glTranslatef(0.0f, 3.42f, 0.0f); // Nằm ngay dưới mái, ôm lấy đầu dây treo
    drawCube(0.7f, 0.25f, 0.7f);
    glPopMatrix();


    //=====================// BỆ ĐIỀU KHIỂN
    //=====================
    glColor3f(1.0f, 0.85f, 0.85f);
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 1.9f);
    drawCube(1.5f, 0.5f, 0.8f);
    glPopMatrix();

    //=====================
    // JOYSTICK
    //=====================
    glColor3f(0.7f, 1.0f, 0.8f);
    glPushMatrix();
    glTranslatef(-0.3f, -0.2f, 2.2f);
    glutSolidSphere(0.12f, 20, 20);
    glPopMatrix();

    glColor3f(0.8f, 0.8f, 0.8f);
    GLUquadric* q = gluNewQuadric();
    glPushMatrix();
    glTranslatef(-0.3f, -0.45f, 2.2f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 0.03, 0.03, 0.3, 20, 20);
    glPopMatrix();

    //=================================================
    // [THÊM MỚI] NÚT BẤM LỚN (ACTION BUTTON) BÊN PHẢI JOYSTICK
    //=================================================
    glColor3f(0.7f, 1.0f, 0.8f);
    glPushMatrix();
    glTranslatef(0.3f, -0.22f, 2.1f); // Đặt đối xứng với cần gạt qua tâm bệ
    glScalef(1.0f, 0.35f, 1.0f);      // Làm dẹt khối cầu thành hình nút bấm tap
    glutSolidSphere(0.15f, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(q);
}

//=====================================================
// KHUNG KÍNH TRONG SUỐT
//=====================================================
void drawMachineGlass()
{
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
}

//=====================================================
// CẦN GẮP
//=====================================================
void drawClaw()
{
    GLUquadric* q = gluNewQuadric();

    //=====================
    // DÂY TREO
    //=====================
    glColor3f(0.85f, 0.85f, 0.85f);
    glPushMatrix();
    glTranslatef(0.0f, 3.55f, 0.0f);
    glRotatef(90, 1, 0, 0);
    gluCylinder(q, 0.025, 0.025, 2.4, 20, 20);
    glPopMatrix();

    //=====================
    // ĐẦU GẮP
    //=====================
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.15f, 0.0f);
    glutSolidSphere(0.18f, 20, 20);
    glPopMatrix();

    //=====================
    // 3 CÀNG GẮP
    //=====================
    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        glTranslatef(0.0f, 1.15f, 0.0f);
        glRotatef(i * 120, 0, 1, 0);
        glRotatef(-35, 1, 0, 0);
        glTranslatef(0.0f, -0.35f, 0.15f);

        glScalef(0.06f, 0.55f, 0.06f);
        glutSolidSphere(1.0, 20, 20);
        glPopMatrix();
    }

    gluDeleteQuadric(q);
}

//=====================================================
// GẤU BÔNG
//=====================================================
void drawLabubu(
    float x, float y, float z,
    float r, float g, float b,
    float rotY = 0.0f)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotY, 0, 1, 0);

    // BODY
    glColor3f(r, g, b);
    glPushMatrix();
    glScalef(0.7f, 0.65f, 0.7f);
    glutSolidSphere(0.5, 40, 40);
    glPopMatrix();

    // HEAD
    glPushMatrix();
    glTranslatef(0.0f, 0.45f, 0.0f);
    glutSolidSphere(0.45, 40, 40);
    glPopMatrix();

    // LEFT EAR
    glPushMatrix();
    glTranslatef(-0.23f, 0.8f, 0.0f);
    glutSolidSphere(0.18, 30, 30);
    glPopMatrix();

    // RIGHT EAR
    glPushMatrix();
    glTranslatef(0.23f, 0.8f, 0.0f);
    glutSolidSphere(0.18, 30, 30);
    glPopMatrix();

    // EYES
    glColor3f(0.2f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(-0.12f, 0.48f, 0.38f);
    glutSolidSphere(0.04, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.12f, 0.48f, 0.38f);
    glutSolidSphere(0.04, 20, 20);
    glPopMatrix();

    // MOUTH
    glColor3f(0.5f, 0.2f, 0.2f);
    glBegin(GL_LINE_STRIP);
    for (float i = -0.5f; i <= 0.5f; i += 0.05f)
    {
        glVertex3f(i * 0.12f, 0.35f - (i * i * 0.05f), 0.4f);
    }
    glEnd();

    // BLUSH
    glColor3f(1.0f, 0.6f, 0.7f);
    glPushMatrix();
    glTranslatef(-0.22f, 0.38f, 0.36f);
    glutSolidSphere(0.05, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.22f, 0.38f, 0.36f);
    glutSolidSphere(0.05, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

//=====================================================
// ÁNH SÁNG
//=====================================================
void setupLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPos[] = { 5, 8, 5, 1 };
    GLfloat ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
}

//=====================================================
// DISPLAY
//=====================================================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        7, 6, 12,
        0, 1, 0,
        0, 1, 0
    );

    // 1. Vẽ thân máy + các chi tiết khung, nút, cửa sập mới
    drawMachineBody();

    // 2. Cần gắp
    drawClaw();

    // 3. Gấu bông
    drawLabubu(-0.8f, -0.3f, 0.8f, 1.0f, 0.9f, 0.4f, 35.0f);
    drawLabubu(0.8f, -0.3f, -0.3f, 1.0f, 0.7f, 0.8f, -25.0f);
    drawLabubu(0.0f, -0.3f, 0.0f, 0.7f, 1.0f, 0.8f, 0.0f);
    drawLabubu(-0.2f, -0.3f, -0.8f, 0.7f, 0.85f, 1.0f, 15.0f);

    // 4. Khung kính trong suốt đè lên trên cùng
    drawMachineGlass();

    glutSwapBuffers();
}

//=====================================================
// INIT
//=====================================================
void init()
{
    glClearColor(0.97f, 0.92f, 0.92f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    setupLighting();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.33, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}