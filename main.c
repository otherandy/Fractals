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

//drawing area
double  real_min  = -2.4, //left border
        real_max  =  1.8, //right border
        imag_min  = -1.2, //top border
        imag_max; //bottom border

GLsizei w = 1280, h = 720;
int max_iterations = 100, step = 10, color_profile = 1, fractal = 0, showInfo = 1, window_id;
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
void text(char*);
void menu();
void info();
void hud();
void mandelbrot();
void multibrot();
void tricorn();
void burning_ship();
void color_mapping(int);

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(w, h);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(3);
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
            if (showInfo || fractal == 0) break;
            real_min += real_diff;
            real_max -= real_diff;
            imag_min += imag_diff;
            imag_max -= imag_diff;
            glutPostRedisplay();
            break;
        case 'A':
        case 'a':
            if (showInfo || fractal == 0) break;
            if(max_iterations > step) max_iterations -= step;
            printf("Iterations:\t%d\n", max_iterations);
            glutPostRedisplay();
            break;
        case 'S':
        case 's':
            if (showInfo || fractal == 0) break;
            real_min -= real_diff;
            real_max += real_diff;
            imag_min -= imag_diff;
            imag_max += imag_diff;
            glutPostRedisplay();
            break;
        case 'D':
        case 'd':
            if (showInfo || fractal == 0) break;
            max_iterations += step;
            printf("Iterations:\t%d\n", max_iterations);
            glutPostRedisplay();
            break;
        case 'E':
        case 'e':
            if (showInfo || fractal == 0) break;
            step++;
            printf("Step:\t%d\n", step);
            glutPostRedisplay();
            break;
        case 'Q':
        case 'q':
            if (showInfo || fractal == 0) break;
            if (step > 1) step--;
            printf("Step:\t%d\n", step);
            glutPostRedisplay();
            break;
        case 'C':
        case 'c':
            if (showInfo || fractal == 0) break;
            color_profile++;
            if (color_profile > 7) color_profile = 1;
            printf("Color profile changed:\t%d\n", color_profile);
            glutPostRedisplay();
            break;
        case '1':
        case '!':
            fractal = 1;
            showInfo = 1;
            printf("Set changed:\t%d\n", fractal);
            glutPostRedisplay();
            break;
        case '2':
        case '@':
            fractal = 2;
            showInfo = 1;
            printf("Set changed:\t%d\n", fractal);
            glutPostRedisplay();
            break;
        case '3':
        case '#':
            fractal = 3;
            showInfo = 1;
            printf("Set changed:\t%d\n", fractal);
            glutPostRedisplay();
            break;
        case '4':
        case '$':
            fractal = 4;
            showInfo = 1;
            printf("Set changed:\t%d\n", fractal);
            glutPostRedisplay();
            break;
        case 'M':
        case 'm':
        case '0':
        case ')':
            fractal = 0;
            printf("Menu loaded.");
            glutPostRedisplay();
            break;
        case 32:
            if(fractal > 0) {
                showInfo = 0;
                glutPostRedisplay();
            }
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
    glLoadIdentity();

    if(debug)
        printf("[D] R(min): %lf\tR(max): %lf\tI(min): %lf\tI(max): %lf\n", real_min, real_max, imag_min, imag_max);

    if(fractal != 0) {
        if(showInfo) {
            glScalef(0.2, 0.2, 0.2);
            info();
        }
        else {
            switch(fractal) {
            	case 1: mandelbrot(); break;
            	case 2: multibrot(); break;
            	case 3: tricorn(); break;
            	case 4: burning_ship(); break;
			}
            glScalef(0.2, 0.2, 0.2);
            hud();
        }	
    } else {
        glScalef(0.5, 0.5, 0.5);
        menu();
        
    }

    //display result
    glutSwapBuffers();
}

void text(char *str) {
    int len = (int) strlen(str);
    glPushMatrix();
    for (unsigned i = 0; i < len; i++) {
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void menu() {
    glTranslatef(40.0, (h * 2) - 140, 0.0);
    text("Fractales");
    glTranslatef(0.0, -200, 0.0);
    text("1.- Conjunto de Mandelbrot.");
    glTranslatef(0.0, -200, 0.0);
    text("2.- Multibrot.");
    glTranslatef(0.0, -200, 0.0);
    text("3.- Conjunto de Tricorn.");
    glTranslatef(0.0, -200, 0.0);
    text("4.- 'Burning Ship.'");
}

void info() {
    glTranslatef(50.0, (h * 5) - 150, 0.0);
    switch(fractal) {
        case 1:
            text("El conjunto de Mandelbrot es un subconjunto de los numeros");
            glTranslatef(0.0, -200, 0.0);
            text("complejos cuyos elementos c cumplen que la funcion");
            glTranslatef(0.0, -200, 0.0);
            text("f_c(z) = z^2 + c no diverge.");
               glTranslatef(0.0, -400, 0.0);
            text("Los puntos de este conjunto tienen un modulo menos a 2,");
             glTranslatef(0.0, -200, 0.0);
            text("es decir, todos los puntos estan a una distancia menor a");
            glTranslatef(0.0, -200, 0.0);
            text("dos del punto (0 + 0i).");
               glTranslatef(0.0, -400, 0.0);
            text("En la orilla de la figura resultante incorpora versiones");
            glTranslatef(0.0, -200, 0.0);
            text("pequennas de la figura original, lo que es muestra de la");
            glTranslatef(0.0, -200, 0.0);
            text("propiedad de autosimilaridad propia de los fractales, como");
            glTranslatef(0.0, -200, 0.0);
            text("el propio Conjunto de Mandelbrot.");
               glTranslatef(0.0, -400, 0.0);
            text("La graficacion de este conjunto se lleva a cabo utilizando");
            glTranslatef(0.0, -200, 0.0);
            text("los valores reales e imaginarios de c como coordenadas del");
            glTranslatef(0.0, -200, 0.0);
            text("plano, y se pintan de color negro los puntos que pertenecen");
            glTranslatef(0.0, -200, 0.0);
            text("al conjunto; en caso contrario, se pinta el pixel seg�n la");
            glTranslatef(0.0, -200, 0.0);
            text("rapidez con que la serie diverge.");
            glTranslatef((w * 3) + 425, 0.0, 0.0);
            text("Presione espacio para continuar.");
            break;
        case 2:
            text("Los conjuntos de Multibrot son una generalizacion del conjunto");
            glTranslatef(0.0, -200, 0.0);
            text("de Mandelbrot para los cuales el exponente al que se eleva el");
            glTranslatef(0.0, -200, 0.0);
            text("n�mero complejo z de la funci�n f(z) = z^d + c, es cualquier numero mayor a 2.");
            glTranslatef((w * 3) + 425, -3000, 0.0);
            text("Presione espacio para continuar.");
            break;
        case 3:
            text("El conjunto 'Tricorn' es una variacion del conjunto de Mandelbrot");
            glTranslatef(0.0, -200, 0.0);
            text("en el cual se toma el complemento de cada numero complejo z que");
            glTranslatef(0.0, -200, 0.0);
            text("se desea utilizar en la funcion.");
            glTranslatef((w * 3) + 425, -3000, 0.0);
            text("Presione espacio para continuar.");
            break;
        case 4:
            text("El 'Burning Ship Fractal' es un subconjunto de los numeros");
            glTranslatef(0.0, -200, 0.0);
            text("complejos cuyos elementos c cumplen con que la funcion");
            glTranslatef(0.0, -200, 0.0);
            text("f(z_n+1) = (|Re(z_n)| + i|Im(z_n)|)^2 + c, con z_0 = 0, no diverge.");
            glTranslatef(0.0, -400, 0.0);
            text("La diferencia principal de este conjunto y el de Mandelbrot es que");
            glTranslatef(0.0, -200, 0.0);
            text("se toman los valores absolutos de los componentes reales e imaginarios");
            glTranslatef(0.0, -200, 0.0);
            text("de cada numero complejo c.");
            glTranslatef((w * 3) + 425, -2200, 0.0);
            text("Presione espacio para continuar.");
            break;
    }

}

void hud() {
    char str[100], *str1;
    switch(fractal) {
        case 1: str1 = "Mandelbrot:"; break;
        case 2: str1 = "Multibrot:";  break;
        case 3: str1 = "Tricorn:"; break;
        case 4: str1 = "Burning Ship:"; break;
        default: break;
        
    }
    sprintf(str, "%s %d %s %d %s", str1, max_iterations, "| Iteraciones -/+ [A/D] Paso: ", step, "[Q/E] | Zoom [A/S] Moverse [Flechas]");
    
    glTranslatef(30.0, 50.0, 0.0);
    text(str);
}

void mandelbrot() {
    Complex c, z, z_sqr;
    
    double real_factor = (real_max - real_min) / (w - 1),
           imag_factor = (imag_max - imag_min) / (h - 1);

    int isInside;
    unsigned n;

    glBegin(GL_POINTS);
    for(unsigned y = 40; y < h; ++y) {
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

void multibrot() {
    Complex c, z, z_sqr, z_cub;
    
    double real_factor = (real_max - real_min) / (w - 1),
           imag_factor = (imag_max - imag_min) / (h - 1);

    int isInside;
    unsigned n;

    glBegin(GL_POINTS);
    for(unsigned y = 40; y < h; ++y) {
        c.imag = imag_max - y * imag_factor;
        for(unsigned x = 0; x < w; ++x) {
            c.real = real_min + x * real_factor;
            z.real = c.real, z.imag = c.imag;
            
            isInside = 1;

            for(n = 0; n < max_iterations; ++n) {
                z_sqr.real = z.real * z.real;
                z_sqr.imag = z.imag * z.imag;
                z_cub.real = z.real * z.real * z.real;
                z_cub.imag = z.imag * z.imag * z.imag;
                if(z_sqr.real + z_sqr.imag > 4) {
                    isInside = 0;
                    break;
                }
                z.imag = 3 * z_sqr.real * z.imag - z_cub.imag + c.imag;
                z.real = z_cub.real - 3 * z.real * z_sqr.imag + c.real;
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

void tricorn() {
    Complex c, z, z_sqr;
    
    double real_factor = (real_max - real_min) / (w - 1),
           imag_factor = (imag_max - imag_min) / (h - 1);

    int isInside;
    unsigned n;

    glBegin(GL_POINTS);
    for(unsigned y = 40; y < h; ++y) {
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
                z.imag = -2 * z.real * z.imag + c.imag;
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
    for(unsigned y = 40; y < h; ++y) {
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
