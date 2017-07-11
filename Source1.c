#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>

GLfloat blue[] = { 0.5, 0.8, 0.8, 0 };
GLfloat red[] = { 0.8, 0.5, 0.5, 0 };
GLfloat brown[] = { 0.8, 0.3, 0.1, 0 };
GLfloat green[] = { 0.0, 0.8, 0.2, 0 };
GLfloat white[] = { 1.0, 1.0, 1.0, 0 };

GLdouble vertex[][3] = {
	{ 0.0, 0.0, 0.0 },
	{ 2.0, 0.0, 0.0 },
	{ 2.0, 2.0, 0.0 },
	{ 0.0, 2.0, 0.0 },
	{ 0.0, 0.0, 0.04 },
	{ 2.0, 0.0, 0.04 },
	{ 2.0, 2.0, 0.04 },
	{ 0.0, 2.0, 0.04 }
};

int face[][4] = {
	{ 0, 1, 2, 3 },
	{ 1, 5, 6, 2 },
	{ 5, 4, 7, 6 },
	{ 4, 0, 3, 7 },
	{ 4, 5, 1, 0 },
	{ 3, 2, 6, 7 }
};

GLdouble normal[][3] = {
	{ 0.0, 0.0, -1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ -1.0, 0.0, 0.0 },
	{ 0.0, -1.0, 0.0 },
	{ 0.0, 1.0, 0.0 }
};

void desk(void)
{
	int i;
	int j;

	glBegin(GL_QUADS);
	for (j = 0; j < 6; j++) {
		glNormal3dv(normal[j]);
		for (i = 0; i < 4; i++) {
			glVertex3dv(vertex[face[j][i]]);
		}
	}
	glEnd();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(0, 0.7, 0);
	glPushMatrix();
	glTranslatef(-0.6, 0, 0);
	glRotatef(-20, 0, 0, 1);
	glMaterialfv(GL_FRONT, GL_AMBIENT, brown);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);
	glutSolidTeapot(0.7);
	glPopMatrix();

	glPushMatrix();
	//glClear(GL_COLOR_BUFFER_BIT);
	glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, blue);
	glTranslatef(0.58, 0, 0);
	double vx = 0.02, vy = 0.02;
	for (size_t i = 0; i < 40; i++)
	{
		glTranslatef(vx, vy, 0);
		vy = vy - 0.004;
		glutSolidSphere(0.04, 30, 30);
	}
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, green);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glTranslatef(1.3, -2.4, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.5, 0.01, 100, 100);
	glRotatef(180, 1, 0, 0);
	for (size_t i = 0; i < 100; i++)
	{
		glutSolidTorus(0.04, 0.5 + 0.002*i, 30, 30);
		glTranslatef(0, 0, -0.01);
	}
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, green);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);
	glTranslatef(0.25, -2.5, -1.0);
	glRotatef(90, 1, 0, 0);
	desk();
	glPopMatrix();
	glutSwapBuffers();
}


void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	GLfloat pos0[] = { 30,40,40,0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//perspective 1
	//gluLookAt(6, 4, 9, 1, 0, 0.0, 0.0, 1.0, 0.0);

	//perspective 2
	gluLookAt(8, 8, 0, 1, 0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("C Dev");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
