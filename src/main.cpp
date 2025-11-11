#include <windows.h>
#include <GL/freeglut.h>
#include "Game.h"

Game game;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    game.draw();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    game.handleInput(key, x, y);
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    game.handleSpecialInput(key, x, y);
    glutPostRedisplay();
}

void mouseMove(int x, int y) {
    game.handleMouseMove(x, y);
    glutPostRedisplay();
}

void update(int value) {
    game.update();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Arcanoid with Transparency");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    game.init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutPassiveMotionFunc(mouseMove);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}