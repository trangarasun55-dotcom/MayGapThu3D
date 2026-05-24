#include <glut.h>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);

    glutSolidTeapot(1);

    glutSwapBuffers();
}

void init()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);

    gluPerspective(45, 1, 1, 100);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 600);

    glutCreateWindow("May Gap Thu 3D");

    init();

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}