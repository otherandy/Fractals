#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/freeglut.h>
  #include <GL/freeglut_ext.h>
#endif

typedef struct {
    double real, imag;
} Complex;

//arawing area
double  real_min  = -2.4, //left border
        real_max  =  1.8, //right border
        imag_min  = -1.2, //top border
        imag_max; //bottom border

GLsizei w = 1280, h = 720;
int max_iterations = 100, step = 10, color_profile = 1, fractal = 1, window_id;
int debug = 0;

//color values
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

//prototypes
static void init();
void resize(int, int);
static void keypress(unsigned char, int, int);
static void special(int, int, int);
void display();
void mandelbrot();
void burning_ship();
<<<<<<< HEAD
void tricorn();
void multibrot();
=======
void multibrot();
void tricorn();
>>>>>>> c32c83a11cbf4eafc4e3903da02aef0042a26f94
void color_mapping(int);

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(w, h);

    glutInitDisplayMode(GLUT_SINGLE);
    window_id = glutCreateWindow("Fractals");

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
    
    printf("Width:\t%d\tHeight:\t%d\n", w, h);
}

static void keypress(unsigned char key, int x, int y) {
    double  real_diff = fabs(real_min - real_max) * 0.05,
            imag_diff = fabs(imag_min - imag_max) * 0.05;
    switch (key) {
        case 'W':
        case 'w':
            real_min += real_diff;
            real_max -= real_diff;
            imag_min += imag_diff;
            imag_max -= imag_diff;
            glutPostRedisplay();
            break;
        case 'A':
        case 'a':
            if(max_iterations > step) max_iterations -= step;
            printf("Iterations:\t%d\n", max_iterations);
            glutPostRedisplay();
            break;
        case 'S':
        case 's':
            real_min -= real_diff;
            real_max += real_diff;
            imag_min -= imag_diff;
            imag_max += imag_diff;
            glutPostRedisplay();
            break;
        case 'D':
        case 'd':
            max_iterations += step;
            printf("Iterations:\t%d\n", max_iterations);
            glutPostRedisplay();
            break;
        case 'E':
        case 'e':
            step++;
            printf("Step:\t%d\n", step);
            break;
        case 'Q':
        case 'q':
            if (step > 1) step--;
            printf("Step:\t%d\n", step);
            break;
        case 'C':
        case 'c':
            color_profile++;
            if (color_profile > 7) color_profile = 1;
            printf("Color profile changed:\t%d\n", color_profile);
            glutPostRedisplay();
            break;
        case 'K':
        case 'k':
            fractal--;
            if (fractal < 1) fractal = 4;
            printf("Set changed:\t%d\n", fractal);
            glutPostRedisplay();
            break;
        case 'L':
        case 'l':
            fractal++;
            if (fractal > 4) fractal = 1;
            printf("Set changed:\t%d\n", fractal);
            glutPostRedisplay();
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
            real_min -= realDif;
            real_max -= realDif;
            break;
        case GLUT_KEY_RIGHT:
            real_min += realDif;
            real_max += realDif;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void display(void) {    
    //clear screen buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch(fractal) {
        case 1: mandelbrot(); break;
        case 2: burning_ship(); break;
        case 3: tricorn(); break;
        case 4: multibrot(); break;
        default: break;
    }
    
    //display result
    glFlush();
}

void mandelbrot() {
    if(debug)
        printf("[D] R(min): %lf\tR(max): %lf\tI(min): %lf\tI(max): %lf\n", real_min, real_max, imag_min, imag_max);
    
    Complex c, z, z_sqr;
    
    double real_factor = (real_max - real_min) / (w - 1),
           imag_factor = (imag_max - imag_min) / (h - 1);

    int isInside;
    unsigned n;

    glBegin(GL_POINTS);
    for(unsigned y = 0; y < h; ++y) {
        c.imag = imag_max - y * imag_factor;
        for(unsigned x = 0; x < w; ++x) {
            c.real = real_min + x * real_factor;
            z.real = c.real, z.imag = c.imag;
            
            isInside = 1;

            for(n = 0; n < max_iterations; ++n) {
                z_sqr.real = z.real * z.real;
                z_sqr.imag = z.imag * z.imag;
                if(z_sqr.real + z_sqr.imag > 4) {
                    isInside = 0;
                    break;
                }
                z.imag = 2 * z.real * z.imag + c.imag;
                z.real = z_sqr.real - z_sqr.imag + c.real;
            }
            if(isInside)
                glColor3f(0.0, 0.0, 0.0);
            else
                color_mapping(n * 100 / max_iterations % 17);
            glVertex2d(x, y);
        }
    }
    glEnd();
}

void burning_ship() {
    Complex c, z, z_sqr;
    
    double real_factor = (real_max - real_min) / (w - 1),
           imag_factor = (imag_max - imag_min) / (h - 1);

    unsigned n;

    glBegin(GL_POINTS);
    for(unsigned y = 0; y < h; ++y) {
        c.imag = imag_max - y * imag_factor;
        for(unsigned x = 0; x < w; ++x) {
            c.real = real_min + x * real_factor;
            z.real = c.real, z.imag = c.imag;

            for(n = 0; n < max_iterations; ++n) {
                z_sqr.real = z.real * z.real;
                z_sqr.imag = z.imag * z.imag;
                if(z_sqr.real + z_sqr.imag > 4)
                    break;
                z.imag = fabs(2 * z.real * z.imag) + c.imag;
                z.real = fabs(z_sqr.real - z_sqr.imag + c.real);
            }
            if(n == max_iterations)
                glColor3f(0.0, 0.0, 0.0);
            else
                color_mapping(n);
            glVertex2d(x, y);
        }
    }
    glEnd();
}

void tricorn() {
    Complex c, z, z_sqr;
    
    double real_factor = (real_max - real_min) / (w - 1),
           imag_factor = (imag_max - imag_min) / (h - 1);

    int isInside;
    unsigned n;

    glBegin(GL_POINTS);
    for(unsigned y = 0; y < h; ++y) {
        c.imag = imag_max - y * imag_factor;
        for(unsigned x = 0; x < w; ++x) {
            c.real = real_min + x * real_factor;
            z.real = c.real, z.imag = c.imag;
            
            isInside = 1;

            for(n = 0; n < max_iterations; ++n) {
                z_sqr.real = z.real * z.real;
                z_sqr.imag = z.imag * z.imag;
                if(z_sqr.real + z_sqr.imag > 4) {
                    isInside = 0;
                    break;
                }
                z.imag = 2 * z.real * z.imag + c.imag;
                z.real = z_sqr.real - z_sqr.imag + c.real;
                if (z.imag < 0) {
                    z.imag = -1 * z.imag;
                }
            }
            if(isInside)
                glColor3f(0.0, 0.0, 0.0);
            else
                color_mapping(n * 100 / max_iterations % 17);
            glVertex2d(x, y);
        }
    }
    glEnd();
}

void multibrot() {
    Complex c, z, z_sqr;
    
    double real_factor = (real_max - real_min) / (w - 1),
           imag_factor = (imag_max - imag_min) / (h - 1);

    int isInside;
    unsigned n;

    glBegin(GL_POINTS);
    for(unsigned y = 0; y < h; ++y) {
        c.imag = imag_max - y * imag_factor;
        for(unsigned x = 0; x < w; ++x) {
            c.real = real_min + x * real_factor;
            z.real = c.real, z.imag = c.imag;
            
            isInside = 1;

            for(n = 0; n < max_iterations; ++n) {
                z_sqr.real = z.real * z.real * z.real;
                z_sqr.imag = z.imag * z.imag * z.imag;
                if(z_sqr.real + z_sqr.imag > 4) {
                    isInside = 0;
                    break;
                }
                z.imag = 2 * z.real * z.imag + c.imag;
                z.real = z_sqr.real - z_sqr.imag + c.real;
                if (z.imag < 0) {
                    z.imag = -1 * z.imag;
                }
            }
            if(isInside)
                glColor3f(0.0, 0.0, 0.0);
            else
                color_mapping(n * 100 / max_iterations % 17);
            glVertex2d(x, y);
        }
    }
    glEnd();
}

void color_mapping(int index) {
    int a, b, c;
    switch(color_profile) {
        case 1: a = 2; b = 1; c = 0; break;
        case 2: a = 2; b = 0; c = 0; break;
        case 3: a = 1; b = 2; c = 0; break;
        case 4: a = 1; b = 0; c = 2; break;
        case 5: a = 0; b = 1; c = 2; break;
        case 6: a = 0; b = 2; c = 1; break;
        case 7: a = 0; b = 0; c = 0; break;
    }
    glColor3f(color_map[index][a], color_map[index][b], color_map[index][c]);
}
