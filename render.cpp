#include <glut.h>
#include <math.h>

#include "render.h"
#include "globals.h"
#include "imageloader.h"

GLuint texture_bac;
GLuint texture_hong;
GLuint texture_kem;
GLuint texture_xanhmint;

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

    glBindTexture(GL_TEXTURE_2D, texture_hong);

    glPushMatrix();

    glTranslatef(0.0f, -2.0f, 0.0f);

    glScalef(3.5f, 2.8f, 3.5f);

    drawTexturedCube(1.0f);

    glPopMatrix();

    // =========================
    // CỬA NHẬN QUÀ
    // =========================

    glColor3f(0.75f, 0.9f, 0.85f);

    glPushMatrix();

    glTranslatef(0.0f, -2.2f, 1.76f);

    drawCube(1.2f, 1.2f, 0.06f);

    glPopMatrix();


    glColor3f(1.0f, 0.85f, 0.88f);

    glPushMatrix();

    glTranslatef(0.0f, -2.2f, 1.79f);

    drawCube(0.9f, 0.9f, 0.04f);

    glPopMatrix();


    glColor3f(0.75f, 0.9f, 0.85f);

    glPushMatrix();

    glTranslatef(0.0f, -1.85f, 1.82f);

    glutSolidSphere(0.06f, 15, 15);

    glPopMatrix();

    // =========================
    // MÁI
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_kem);

    glPushMatrix();

    glTranslatef(0.0f, 3.9f, 0.0f);

    glScalef(3.55f, 0.8f, 3.55f);

    drawTexturedCube(1.0f);

    glPopMatrix();

    // =========================
    // 4 CỘT GÓC
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_xanhmint);

    float pillarOffset = 1.7f;
    float pillarSize = 0.16f;

    // CỘT 1
    glPushMatrix();
    glTranslatef(-pillarOffset, 1.4f, pillarOffset);
    glScalef(pillarSize, 4.3f, pillarSize);
    drawTexturedCube(1.0f);
    glPopMatrix();

    // CỘT 2
    glPushMatrix();
    glTranslatef(pillarOffset, 1.4f, pillarOffset);
    glScalef(pillarSize, 4.3f, pillarSize);
    drawTexturedCube(1.0f);
    glPopMatrix();

    // CỘT 3
    glPushMatrix();
    glTranslatef(-pillarOffset, 1.4f, -pillarOffset);
    glScalef(pillarSize, 4.3f, pillarSize);
    drawTexturedCube(1.0f);
    glPopMatrix();

    // CỘT 4
    glPushMatrix();
    glTranslatef(pillarOffset, 1.4f, -pillarOffset);
    glScalef(pillarSize, 4.3f, pillarSize);
    drawTexturedCube(1.0f);
    glPopMatrix();

    // =========================
    // KHỐI DI CHUYỂN TRÊN TRẦN
    // =========================

    glBindTexture(GL_TEXTURE_2D, texture_xanhmint);

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

    glBindTexture(GL_TEXTURE_2D, texture_hong);

    glPushMatrix();

    glTranslatef(0.0f, -0.5f, 1.9f);

    glScalef(1.5f, 0.5f, 0.8f);

    drawTexturedCube(1.0f);

    glPopMatrix();

    // =========================
    // JOYSTICK
    // =========================

    glColor3f(0.7f, 1.0f, 0.8f);

    glPushMatrix();

    glTranslatef(-0.3f, -0.2f, 2.2f);

    glutSolidSphere(0.12f, 20, 20);

    glPopMatrix();

    GLUquadric* quad = gluNewQuadric();

    glColor3f(0.8f, 0.8f, 0.8f);

    glPushMatrix();

    glTranslatef(-0.3f, -0.45f, 2.2f);

    glRotatef(-90, 1, 0, 0);

    gluCylinder(quad, 0.03f, 0.03f, 0.3f, 20, 20);

    glPopMatrix();

    // =========================
    // NÚT BẤM
    // =========================

    glColor3f(0.7f, 1.0f, 0.8f);

    glPushMatrix();

    glTranslatef(0.3f, -0.22f, 2.1f);

    glScalef(1.0f, 0.35f, 1.0f);

    glutSolidSphere(0.15f, 20, 20);

    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawMachineGlass()
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

void drawClaw()
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

    glColor3f(0.85f, 0.85f, 0.85f);

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

    glColor3f(1.0f, 1.0f, 1.0f);

    glutSolidSphere(0.18f, 20, 20);

    // =========================
    // 3 CÀNG GẮP
    // =========================

    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();

        glRotatef(i * 120.0f, 0, 1, 0);

        glRotatef(-clawOpenAngle, 1, 0, 0);

        glTranslatef(0.0f, -0.35f, 0.15f);

        glScalef(0.06f, 0.55f, 0.06f);

        glutSolidSphere(1.0f, 20, 20);

        glPopMatrix();
    }

    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawLabubu(float x, float y, float z, float r, float g, float b, float rotateY)
{
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();

    glTranslatef(x, y, z);

    glRotatef(rotateY, 0, 1, 0);

    // THÂN
    glColor3f(r, g, b);

    glPushMatrix();
    glScalef(0.7f, 0.65f, 0.7f);
    glutSolidSphere(0.5f, 40, 40);
    glPopMatrix();

    // ĐẦU
    glPushMatrix();
    glTranslatef(0.0f, 0.45f, 0.0f);
    glutSolidSphere(0.45f, 40, 40);
    glPopMatrix();

    // TAI TRÁI
    glPushMatrix();
    glTranslatef(-0.23f, 0.8f, 0.0f);
    glutSolidSphere(0.18f, 30, 30);
    glPopMatrix();

    // TAI PHẢI
    glPushMatrix();
    glTranslatef(0.23f, 0.8f, 0.0f);
    glutSolidSphere(0.18f, 30, 30);
    glPopMatrix();

    // MẮT
    glColor3f(0.2f, 0.1f, 0.1f);

    glPushMatrix();
    glTranslatef(-0.12f, 0.48f, 0.38f);
    glutSolidSphere(0.04f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.12f, 0.48f, 0.38f);
    glutSolidSphere(0.04f, 20, 20);
    glPopMatrix();

    // MIỆNG
    glColor3f(0.5f, 0.2f, 0.2f);

    glBegin(GL_LINE_STRIP);

    for (float i = -0.5f; i <= 0.5f; i += 0.05f)
    {
        glVertex3f(
            i * 0.12f,
            0.35f - (i * i * 0.05f),
            0.4f);
    }

    glEnd();

    // MÁ HỒNG
    glColor3f(1.0f, 0.6f, 0.7f);

    glPushMatrix();
    glTranslatef(-0.22f, 0.38f, 0.36f);
    glutSolidSphere(0.05f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.22f, 0.38f, 0.36f);
    glutSolidSphere(0.05f, 20, 20);
    glPopMatrix();

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
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

void renderScene()
{
    drawMachineBody();

    drawClaw();

    float colors[5][3] =
    {
        {1.0f, 0.9f, 0.4f},
        {1.0f, 0.7f, 0.8f},
        {0.7f, 1.0f, 0.8f},
        {0.7f, 0.85f, 1.0f},
        {0.9f, 0.6f, 0.9f}
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

            colors[i % 5][0],
            colors[i % 5][1],
            colors[i % 5][2],

            rotateAngle);
    }

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

    texture_bac =
        loadTexture("data/bac.bmp");

    texture_hong =
        loadTexture("data/hong_pastel.bmp");

    texture_kem =
        loadTexture("data/kem.bmp");

    texture_xanhmint =
        loadTexture("data/xanh_mint.bmp");

    glClearColor(
        0.95f,
        0.92f,
        0.95f,
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