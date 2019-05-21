#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

GLsizei w = 1280, h = 720; //width and height
double  real_min  = -2.9,
        real_max  =  1.4,
        imag_min  = -1.2,
		imag_max;
int max_iterations = 100, thread_count = 2, step = 10, color_profile = 1, window_id;

const float color_map[17][3] = {
    {0.41, 0.2,  0.01},
    {0.6,  0.34, 0.0},
    {0.8,  0.5,  0.0},
    {1.0,  0.67, 0.0},
    {0.97, 0.79, 0.37},
    {0.94, 0.91, 0.75},
    {0.82, 0.92, 0.97},
    {0.52, 0.71, 0.9},
    {0.22, 0.49, 0.82},
    {0.09, 0.32, 0.69},
    {0.05, 0.17, 0.54},
    {0.0,  0.03, 0.39},
    {0.02, 0.02, 0.29},
    {0.04, 0.0,  0.18},
    {0.1,  0.03, 0.1},
    {0.26, 0.18, 0.06},
    {0.0,  0.0,  0.0}
};

static void init(void);
void resize(int, int);
static void keypress(unsigned char, int, int);
static void special(int, int, int);
void display(void);
void color(int);

int main(int argc, char *argv[]) {
    glutInitWindowSize(w, h);
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE);
    glutCreateWindow("Fractals");

    init();

    glutReshapeFunc(resize);
    glutKeyboardFunc(keypress);
    glutSpecialFunc(special);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}

static void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1);
    
    imag_max = imag_min + (real_max - real_min) * h / w;
}

void resize(int width, int height) {
    w = width;
    h = height;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

static void keypress(unsigned char key, int x, int y) {
    double  real_diff = fabs(real_min - real_max) * 0.05,
            imag_diff = fabs(imag_min - imag_max) * 0.05;
    switch (key) {
        case 'w':
            real_min += real_diff;
            real_max -= real_diff;
            imag_min += imag_diff;
            imag_max -= imag_diff;
            glutPostRedisplay();
            break;
        case 'a':
            if(max_iterations > step) max_iterations -= step;
            printf("Iterations:\t%d\n", max_iterations);
            glutPostRedisplay();
            break;
        case 's':
            real_min -= real_diff;
            real_max += real_diff;
            imag_min -= imag_diff;
            imag_max += imag_diff;
            glutPostRedisplay();
            break;
        case 'd':
            max_iterations += step;
            printf("Iterations:\t%d\n", max_iterations);
            glutPostRedisplay();
            break;
        case 'e':
            step++;
            printf("Step:\t%d\n", step);
            break;
        case 'q': 
            if (step > 1) step--;
            printf("Step:\t%d\n", step);
            break;
        case 'c':
            color_profile++;
            if (color_profile > 5) color_profile = 1;
            printf("Color profile changed\n");
            break;
        case 27:
            glutDestroyWindow(window_id);
            exit(EXIT_SUCCESS);
		    break;
	    default:
	    	break;
    }
}

static void special(int key, int x, int y) {
    double  realDif = fabs(real_min - real_max) * 0.05,
            imagDif = fabs(imag_min - imag_max) * 0.05;
    switch (key) {
        case GLUT_KEY_UP:
            imag_min -= imagDif;
            imag_max -= imagDif;
            break;
        case GLUT_KEY_DOWN:
            imag_min += imagDif;
            imag_max += imagDif;
            break;
        case GLUT_KEY_LEFT:
            real_min += realDif;
            real_max += realDif;
            break;
        case GLUT_KEY_RIGHT:
            real_min -= realDif;
            real_max -= realDif;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    double  x0, y0, x, y, xtemp,
            real_factor = (real_max - real_min) / (w - 1),
            imag_factor = (imag_max - imag_min) / (h - 1);
    int iteration;
    
    
    glBegin(GL_POINTS);
    for(int py = 0; py < h; py++) {
        y0 = imag_max - py * imag_factor;
        for(int px = 0; px < w; px++) {
            x0 = real_max - px * real_factor;
            x = 0;
            y = 0;
            iteration = 0;
            while ((x * x + y * y <= 2 * 2) && (iteration < max_iterations)) {
                xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iteration++;
            }
            color(iteration);
            glVertex2d(px, py);
        }
    }
    glEnd();

    glFlush();
}

void color(int iteration) {
    int index = iteration * 100 / max_iterations % 17;
    glColor3f(color_map[index][2], color_map[index][1], color_map[index][0]);
}
