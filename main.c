#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#undef PI
#define PI 3.141592657

GLenum doubleBuffer;
GLint windW = 300, windH = 300;

static void Init(void);
void Reshape(int, int);
static void Key(unsigned char, int, int);
void Idle(void);
void Display(void);
void Visible(int);
static void Args(int, char **);

int main(int argc, char **argv) {
  GLenum type;

  glutInitWindowSize(windW, windH);
  glutInit(&argc, argv);
  Args(argc, argv);

  type = GLUT_RGB;
  type |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;
  glutInitDisplayMode(type);
  glutCreateWindow("Fractals");

  Init();

  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutVisibilityFunc(Visible);
  glutDisplayFunc(Display);
  glutMainLoop();
  return 0;
}

static void Init(void) {
    float angle;

    srand((unsigned int) time(NULL));
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glDisable(GL_DITHER);
}

void Reshape(int width, int height) {
    windW = width;
    windH = height;

    glViewport(0, 0, windW, windH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-0.5, windW + 0.5, -0.5, windH + 0.5);
    glMatrixMode(GL_MODELVIEW);
}

static void Key(unsigned char key, int x, int y) {
    switch (key) {
        case 27: exit(0);
   }
}

void Idle(void) {
    glutPostRedisplay();
}

void Display(void) {
    if (doubleBuffer) {
        glutSwapBuffers();
    } else {
        glFlush();
    }
}

void Visible(int state) {
    if (state == GLUT_VISIBLE) {
        glutIdleFunc(Idle);
    } else {
        glutIdleFunc(NULL);
    }
}

static void Args(int argc, char **argv) {
    doubleBuffer = GL_TRUE;
    
    for(GLint i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-sb") == 0) {
            doubleBuffer = GL_FALSE;
        } else if (strcmp(argv[i], "-db") == 0) {
            doubleBuffer = GL_TRUE;
        }
    }
}
