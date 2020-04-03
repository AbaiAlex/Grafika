#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>

GLsizei winWidth = 1000, winHeight = 600;
float s = 5;
mat4 w2v, projection, rotation;
vec3 C = { 0, 0, s };
float Rx, Ry;
vec3 eltol = { 0.0, 0.0, 0.0 };
GLint keyStates[256];
vec3 l(-20, 0, 0);
vec3 le = normalize(l);
int felosztas = 16;
bool jobb ;
bool bal;
vec2 kozepvonal[2] = { {float (winWidth / 2),float(winHeight)},{float (winWidth / 2), 0.0} };

struct Oldalak
{
	vec3 P0, P1, P2, P3; //oldalpontok
	float I; //az adott oldal árnyalata
	vec3 n; //az oldal lap normálvektora
	vec3 v; //
	vec3 ne; //egység normál vektor
	bool haromszog; 
	vec3 lapkozep;
}; std::vector<Oldalak> oldal;

void initMatrices() 
{
	projection = perspective(s);

	vec2 windowSize = { 2, 2 };
	vec2 windowPosition = { -1, -1 };
	vec2 viewportSize = { 400, 400};
	vec2 viewportPosition = { winWidth / 2 - viewportSize.x / 2, winHeight / 2 - viewportSize.y / 2 };
	w2v = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
	
}

void init()
{
	glClearColor(0.0, 0.5, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(2.0);
	glPointSize(5.0);
	initMatrices();
}

void Gömb() {
	
	float deltatheta = pi() / felosztas;
	float deltaphi = (pi() / felosztas) -0.00001;
	int i = 0;
	float phi;
	for (float theta = 0; theta < 2 * pi(); theta += deltatheta)
	{
		for (phi = 0; phi  < pi() ; phi += deltaphi)
		{
			oldal.push_back(Oldalak());
			oldal[i].P0 =
			{
				cos(theta) * sin(phi),
				sin(theta) * sin(phi),
				cos(phi)
			};
			oldal[i].P1 =
			{
				cos(theta) * sin(phi + deltaphi),
				sin(theta) * sin(phi + deltaphi),
				cos(phi + deltaphi)
			};
			oldal[i].P2 =
			{
				cos(theta + deltatheta) * sin(phi + deltaphi),
				sin(theta + deltatheta) * sin(phi + deltaphi),
				cos(phi + deltaphi)
			};
			oldal[i].P3 =
			{
				cos(theta + deltatheta) * sin(phi),
				sin(theta + deltatheta) * sin(phi),
				cos(phi)
			};
			i++;
		}
		
		oldal.push_back(Oldalak());
		oldal[i].P0 =
		{
			cos(theta) * sin(phi),
			sin(theta) * sin(phi),
			cos(phi)
		};
		oldal[i].P1 =
		{
			cos(theta) * sin(phi + deltaphi),
			sin(theta) * sin(phi + deltaphi),
			cos(phi + deltaphi)
		};
		oldal[i].P3 =
		{
			cos(theta + deltatheta) * sin(phi),
			sin(theta + deltatheta) * sin(phi),
			cos(phi)
		};
		oldal[i].haromszog = true;
		i++;

	}
}

void transform1()
{
	mat4 M = translate(eltol) * rotateX(Rx) * rotateY(Ry);
	vec4 PH[4];
	vec4 Tp[4];
	vec4 Pproj;
	for (int i = 0; i < oldal.size(); i++)
	{	
		PH[0] = ihToH(oldal[i].P0);
		Tp[0] = M * PH[0];
		if (Tp[0].w != 0)
		{
			oldal[i].P0 = hToIh(Tp[0]);
		}

		PH[1] = ihToH(oldal[i].P1);
		Tp[1] = M * PH[1];
		if (Tp[1].w != 0)
		{
			oldal[i].P1 = hToIh(Tp[1]);
		}

		PH[2] = ihToH(oldal[i].P2);
		Tp[2] = M * PH[2];
		if (Tp[2].w != 0)
		{
			oldal[i].P2 = hToIh(Tp[2]);
		}

		PH[3] = ihToH(oldal[i].P3);
		Tp[3] = M * PH[3];
		if (Tp[3].w != 0)
		{
			oldal[i].P3 = hToIh(Tp[3]);
		}
		if (oldal[i].haromszog == true) {
			oldal[i].n = cross(oldal[i].P1 - oldal[i].P0, oldal[i].P3 - oldal[i].P0);
			oldal[i].lapkozep = (oldal[i].P0 + oldal[i].P1 + oldal[i].P3) / 3;
		}
		else {
			oldal[i].n = cross(oldal[i].P1 - oldal[i].P0, oldal[i].P2 - oldal[i].P0);
			oldal[i].lapkozep = (oldal[i].P0 + oldal[i].P1 + oldal[i].P2 + oldal[i].P3) / 4;
		}
		oldal[i].v = dist(oldal[i].lapkozep, C);
		
		
		oldal[i].ne = normalize(oldal[i].n);
		
		
		
		if (dot(C - oldal[i].P0, oldal[i].n) >= 0)
		{
			oldal[i].I = (dot(oldal[i].ne, le) + 1) / 2;
		}else
		{
			oldal.erase(oldal.begin() + i);
			i--;
		}
		
	}
	
}

bool sorting(Oldalak& a, Oldalak& b){
	bool tavolsag = a.v.z > b.v.z;
	return tavolsag;
}

void transform2()
{

	std::sort(oldal.begin(), oldal.end(), sorting);
	mat4 M = w2v * projection;
	
	vec4 PH[4];
	vec4 Tp[4];
	vec4 Pproj;
	for (int i = 0; i < oldal.size(); i++)
	{
		PH[0] = ihToH(oldal[i].P0);
		Tp[0] = M * PH[0];
		if (Tp[0].w != 0)
		{
			oldal[i].P0 = hToIh(Tp[0]);
		}

		PH[1] = ihToH(oldal[i].P1);
		Tp[1] = M * PH[1];
		if (Tp[1].w != 0)
		{
			oldal[i].P1 = hToIh(Tp[1]);
		}

		PH[2] = ihToH(oldal[i].P2);
		Tp[2] = M * PH[2];
		if (Tp[2].w != 0)
		{
			oldal[i].P2 = hToIh(Tp[2]);
		}

		PH[3] = ihToH(oldal[i].P3);
		Tp[3] = M * PH[3];
		if (Tp[3].w != 0)
		{
			oldal[i].P3 = hToIh(Tp[3]);
		}
		vec4 P = ihToH(oldal[i].lapkozep);
		vec4 T = M * P;
		if (T.w != 0)
		{
			oldal[i].lapkozep = hToIh(T);
		}
	}
}

void transform()
{
	transform1();
	transform2();
	glutPostRedisplay();
}

void színez()
{
	for (int i = 0; i < oldal.size(); i++) {
		// jobb oldal, fény
		glColor3f(oldal[i].I, oldal[i].I, oldal[i].I);
		if (((kozepvonal[0].y - kozepvonal[1].y) * oldal[i].lapkozep.x) + ((kozepvonal[1].x - kozepvonal[0].x) * oldal[i].lapkozep.y) -
			((kozepvonal[0].y - kozepvonal[1].y) * kozepvonal[0].x) - ((kozepvonal[1].x - kozepvonal[0].x) * kozepvonal[0].y) > 0)
		{
			if (oldal[i].haromszog == true) {
				glBegin(GL_POLYGON);
				glVertex2f(oldal[i].P0.x, oldal[i].P0.y);
				glVertex2f(oldal[i].P1.x, oldal[i].P1.y);
				glVertex2f(oldal[i].P3.x, oldal[i].P3.y);
				glEnd();
			}
			else {
				glBegin(GL_POLYGON);
				glVertex2f(oldal[i].P0.x, oldal[i].P0.y);
				glVertex2f(oldal[i].P1.x, oldal[i].P1.y);
				glVertex2f(oldal[i].P2.x, oldal[i].P2.y);
				glVertex2f(oldal[i].P3.x, oldal[i].P3.y);
				glEnd();
			}
			//középpontok
			if (jobb == true) {
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_POINTS);
				glVertex2f(oldal[i].lapkozep.x, oldal[i].lapkozep.y);
				glEnd();
			}
		}
		//bal oldal
		else{  
			glColor3f(0.5, 0.5, 0.5);
			if (oldal[i].haromszog == true) {
				glBegin(GL_POLYGON);
				glVertex2f(oldal[i].P0.x, oldal[i].P0.y);
				glVertex2f(oldal[i].P1.x, oldal[i].P1.y);
				glVertex2f(oldal[i].P3.x, oldal[i].P3.y);
				glEnd();
			}
			else {
				glBegin(GL_POLYGON);
				glVertex2f(oldal[i].P0.x, oldal[i].P0.y);
				glVertex2f(oldal[i].P1.x, oldal[i].P1.y);
				glVertex2f(oldal[i].P2.x, oldal[i].P2.y);
				glVertex2f(oldal[i].P3.x, oldal[i].P3.y);
				glEnd();
			}
			//rács
			if (bal == true) {
				glColor3f(1.0, 0.6, 0.0);
				if (oldal[i].haromszog == true) {
					glBegin(GL_LINE_LOOP);
					glVertex2f(oldal[i].P0.x, oldal[i].P0.y);
					glVertex2f(oldal[i].P1.x, oldal[i].P1.y);
					glVertex2f(oldal[i].P3.x, oldal[i].P3.y);
					glEnd();
				}
				else {
					glBegin(GL_LINE_LOOP);
					glVertex2f(oldal[i].P0.x, oldal[i].P0.y);
					glVertex2f(oldal[i].P1.x, oldal[i].P1.y);
					glVertex2f(oldal[i].P2.x, oldal[i].P2.y);
					glVertex2f(oldal[i].P3.x, oldal[i].P3.y);
					glEnd();
				}
			}

		}
	}
	//közép vonal
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(kozepvonal[0].x, kozepvonal[0].y);
	glVertex2f(kozepvonal[1].x, kozepvonal[1].y);
	glEnd();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	oldal.erase(oldal.begin(), oldal.end());
	Gömb();
	transform();
	színez();
	glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = 1;
}

void keyUp(unsigned char key, int x, int y)
{
	keyStates[key] = 0;
}

void keyOperations(int value)
{
	if (keyStates['w']) { Rx -= 0.05; }
	if (keyStates['s']) { Rx += 0.05; }
	if (keyStates['e']) { Ry += 0.05; }
	if (keyStates['q']) { Ry -= 0.05; }
	if (keyStates['a']) { eltol.x -= 0.05; }
	if (keyStates['d']) { eltol.x += 0.05; }
	if (keyStates['r']) { eltol.z -= 0.05; }
	if (keyStates['f']) { eltol.z += 0.05; }
	if (keyStates['-']) { 
		if (felosztas > 6) {
			felosztas -= 1; 
		}
	}
	if (keyStates['+']) { 
		if (felosztas < 32) {
			felosztas += 1;
			
		}
	}
	if (keyStates['c']) {jobb = true;}
	if (keyStates['v']) {jobb = false; }
	if (keyStates['y']) { bal = true; }
	if (keyStates['x']) { bal = false; }
	glutPostRedisplay();
	glutTimerFunc(5, keyOperations, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Harmadik házi");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(5, keyOperations, 0);
	glutMainLoop();
	return 0;
}