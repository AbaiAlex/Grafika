#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>

GLsizei winWidth = 800, winHeight = 600;
GLint dragged = -1;
vec2 pontok[20];
GLint szamlalo = 0;
bool f = false;
double binomialCoefficient(int n, int k)
{
	if (k > n - k)
		k = n - k;

	double c = 1.0;
	for (int i = 0; i < k; i++)
	{
		c = c * (n - i);
		c = c / (i + 1);
	}
	return c;
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glPointSize(10.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (f == true)
	{
			//Hermite
			float t1 = -1;
			float t2 = 0;
			float t3 = 1;
			float t4 = 2;
			mat24 G = { pontok[szamlalo - 4].x, pontok[szamlalo - 3].x, pontok[szamlalo - 2].x, pontok[szamlalo - 1].x, pontok[szamlalo - 4].y, pontok[szamlalo - 3].y, pontok[szamlalo - 2].y, pontok[szamlalo - 1].y };
			vec4 T1 = { t1 * t1 * t1, t1 * t1, t1, 1 };
			vec4 T2 = { t2 * t2 * t2, t2 * t2, t2, 1 };
			vec4 T3 = { t3 * t3 * t3, t3 * t3, t3, 1 };
			vec4 T4 = { t4 * t4 * t4, t4 * t4, t4, 1 };
			mat4 M = inverse({ T1, T2, T3, T4, true });
			mat24 C = G * M;

			glLineWidth(5.0);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINE_STRIP);
			for (float t = t1; t < t4; t += 0.01)
			{
				vec4 T = { t * t * t, t * t, t, 1 };
				vec2 Q = C * T;
				glVertex2f(Q.x, Q.y);
			}
			glEnd();

			//C1
			vec4 T = { 3 * t1 * t1, 2 * t1, 1, 0 };
			vec2 Qn = pontok[szamlalo - 4];
			vec2 Qn_1 = Qn - (C * T) / (szamlalo - 4);
			pontok[szamlalo - 5] = Qn_1;

			//Bezier
			int n = szamlalo - 4;
			glLineWidth(5.0);
			glColor3f(0.0, 0.0, 1.0);
			glBegin(GL_LINE_STRIP);
			for (float t = 0.0; t < 1.0; t += 0.01)
			{
				vec2 Q = { 0, 0 };
				for (int i = 0; i <= n; i++)
				{
					float binom = binomialCoefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
					Q += (pontok[i] * binom);
				}
				glVertex2f(Q.x, Q.y);
			}
			glVertex2f(pontok[n].x, pontok[n].y);
			glEnd();

			glLineWidth(1.0);
			glColor3f(0.8, 0.8, 0.8);
			glBegin(GL_LINE_STRIP);
			glLineWidth(0.0);
			for (int i = 0; i < szamlalo - 3; i++)
			{
				glVertex2f(pontok[i].x, pontok[i].y);
			}
			glEnd();
	}
	//pontok
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < szamlalo; i++)
	{
		glVertex2f(pontok[i].x, pontok[i].y);
	}
	glEnd();

	glutSwapBuffers();
}

GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y)
{
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };
	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	vec2 points = vec2(xMouse, winHeight - yMouse);
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && szamlalo < 20 && f==false)
	{
			pontok[szamlalo] = points;
			szamlalo++;
	}
	if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN && szamlalo > 7)
	{
		f = true;
	}

	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && f==true )
		if ((i = getActivePoint1(pontok, szamlalo, 10, xMouse, winHeight - yMouse)) != -1)
			dragged = i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		dragged = -1;
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0) {
		pontok[dragged].x = xMouse;
		pontok[dragged].y = winHeight - yMouse;
	
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("második házi");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();
	return 0;
}

