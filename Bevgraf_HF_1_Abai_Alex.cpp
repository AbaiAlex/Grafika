#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>
GLsizei winWidth = 600, winHeight = 600;
GLint margin = 20;
GLint hours = 2;
GLint minutes = 59;
GLint seconds = 50;
GLint distance = 20;
vec2 middle[60][4];
vec2 hour[6];
vec2 min[8];
vec2 sec[10];
GLint i;
GLint j;

void init() 
{
	glClearColor(0.25, 0.5, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(13.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
		
		
		//mutatók
		//másodperc

		glColor3f(0.8, 0.4, 0.25);
		glBegin(GL_POINTS);
		for (j = 0; j < 10; j++) {
			sec[j].x = winWidth / 2 + j * distance * cos(pi() / 2 - seconds * (2 * pi() / 60));
			sec[j].y = winHeight / 2 + j * distance * sin(pi() / 2 - seconds * (2 * pi() / 60));
			glVertex2f(sec[j].x, sec[j].y);
		}
		glEnd();

		//óra
	
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		for (j = 0; j < 6; j++) {
			hour[j].x = winWidth / 2 + j * distance * cos(pi() / 2 - hours * (2 * pi() / 12));
			hour[j].y = winHeight / 2 + j * distance * sin(pi() / 2 - hours * (2 * pi() / 12));
				glVertex2f(hour[j].x, hour[j].y);
		}
		glEnd();
		
		//perc
		
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		for (j = 0; j < 8; j++) {
			min[j].x = winWidth / 2 + j * distance * cos(pi() / 2 - minutes * (2 * pi() / 60));
			min[j].y = winHeight / 2 + j * distance * sin(pi() / 2 - minutes * (2 * pi() / 60));
			glVertex2f(min[j].x, min[j].y);
		}
		glEnd();
		
		//számlap
		
		glBegin(GL_POINTS);
		for (i = 0; i < 60; i++) {
			for (j = 0; j < 4; j++) {
					middle[i][j].x = winWidth / 2 + (winWidth / 2 - margin - j * distance) * cos(i * (2 * pi() / 60));
					middle[i][j].y = winHeight / 2 + (winHeight / 2 - margin - j * distance) * sin(i * (2 * pi() / 60));
				
				float A = (sec[0].y - sec[1].y);
				float B = (sec[1].x - sec[0].x);
				float C = (-1 * (A * sec[0].x + B * sec[0].y));
				
				if (A * middle[i][j].x + B * middle[i][j].y + C > 0){
					glColor3f(1.0, 0.0, 0.0);
				}
				else { glColor3f(0.0, 1.0, 0.0); }
				if (j <= 1 || i % 5 == 0) {
					glVertex2f(middle[i][j].x, middle[i][j].y);
				}
			}
		}
		glEnd();
		
		glutSwapBuffers();
		glutPostRedisplay();
		
}
void update(int n)
{
	
	seconds++;

	if (seconds == 60) { 
		seconds = 0;
		minutes++;
	}
	if (minutes == 60) { 
		minutes = 0;
		hours++; }
	if (hours == 12) {
		hours = 0;
	}
	glutPostRedisplay();

	glutTimerFunc(1000, update, 0);

}

void input(GLint* windowSize, GLint* keyStates, GLint* mouseStates, GLint* mousePosition)
{
	if (keyStates[27] == -1)
	{
		exit(0);
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Óra");
	init();
	glutDisplayFunc(display);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}