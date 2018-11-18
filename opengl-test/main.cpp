#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "Opengl32.lib")

using namespace std;
using namespace glm;



glm::mat4 transform(glm::vec2 const& Orientation, glm::vec3 const& Translate, glm::vec3 const& Up)
{
	glm::mat4 Proj = glm::perspective(glm::radians(45.f), 1.33f, 0.1f, 10.f);
	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.f), Translate);
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Orientation.y, Up);
	glm::mat4 View = glm::rotate(ViewRotateX, Orientation.x, Up);
	glm::mat4 Model = glm::mat4(1.0f);
	return Proj * View * Model;
}

void renderBitmapCharacter(double x, double y, void *font, const char *string)
{
	const char *c;
	glRasterPos2d(x, y);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}


/*
float angl = 0.0f;

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	glRotatef(angl, 0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLES);
	glVertex3f(-2.0f, -2.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0);
	glVertex3f(0.0f, 2.0f, 0.0);
	glEnd();

	angl += 0.1f;

	glutSwapBuffers();
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 0.1, 100);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

*/

typedef struct rect {
	double x;
	double y;
	double width;
	double height;
} rect;
rect rectangle = { 50,50,10,20 };

int cnt = 0;

int ww = 10;
int hh = 10;
void reshape1(int w, int h) {
	cout << "reshapefunc1" << endl;

	/*
	glMatrixMode(GL_PROJECTION); //matrix mode switchnot needed in 2d
	glLoadIdentity();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/

	//ordering essential
	ww = w;
	hh = h;
	glViewport(0, 0, w, h);
	
}

void display1() {
	//cout << "displayfunc1" << endl;

	glutSetWindow(1);
	glClear(GL_COLOR_BUFFER_BIT);

	
	glLoadIdentity(); //essential for reload
	gluOrtho2D(0.0, 100.0, 0.0, 100.0); //coordinate in virtual world
	

	glColor3d(0, 1, 1);
	glRectd(50, 0, 75, 100);
	glColor3d(1, 1, 0);
	glRectd(75, 0, 100, 100);

	glColor3d(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(rectangle.x, rectangle.y);
	glVertex2d(rectangle.x, rectangle.y + rectangle.height);
	glVertex2d(rectangle.x + rectangle.width, rectangle.y + rectangle.height);
	glVertex2d(rectangle.x + rectangle.width, rectangle.y);
	glEnd();

	renderBitmapCharacter(50, 50, GLUT_BITMAP_HELVETICA_18, "PRESS ANY KEY");

	glPushMatrix();
	glTranslated(0, 20, 0);
	glScaled(0.1, 0.1, 0);
	glLineWidth(60);
	glColor3d(1, 0, 0);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"HELLO WORLD");
	glLineWidth(1);
	glPopMatrix();


	glLoadIdentity(); //essential for reload
	gluOrtho2D(0.0, ww, 0.0, hh); //absolute

	glColor3d(0, 0.5, 1);
	glRectd(00, 0, 75, 100);

	
	glutSwapBuffers();
}


//multiple viewport (screen split)

void reshape2(int w, int h) {
	cout << "reshapefunc2" << endl;
}

void display2() {
	cout << "displayfunc2" << endl;

	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, ww / 2, hh / 2);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
	glColor3ub(0, 0, 255);
	glRectd(0, 0, 33.3, 100);
	glColor3ub(255, 255, 255);
	glRectd(33.3, 0, 66.6, 100);
	glColor3ub(255, 0, 0);
	glRectd(66.6, 0, 99.9, 100);

	glViewport(ww / 2, 0, ww / 2, hh / 2);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
	glColor3ub(255, 204, 0);
	glRectd(0, 0, 100, 33.3);
	glColor3ub(255, 0, 0);
	glRectd(0, 33.3, 100, 66.6);
	glColor3ub(0, 0, 0);
	glRectd(0, 66.6, 100, 99.9);

	glViewport(0, hh / 2, ww / 2, hh / 2);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
	glColor3ub(0, 255, 0);
	glRectd(0, 0, 33.3, 100);
	glColor3ub(255, 255, 255);
	glRectd(33.3, 0, 66.6, 100);
	glColor3ub(255, 0, 0);
	glRectd(66.6, 0, 99.9, 100);

	glViewport(ww / 2, hh / 2, ww / 2, hh / 2);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
	glColor3ub(255, 255, 255);
	glRectd(0, 0, 100, 100);
	glColor3ub(255, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 50; i++) {
		double theta = 2.0f * 3.1415926f * double(i) / double(50);//get the current angle 	
		double x = 30 * cosl(theta);//calculate the x component 	
		double y = 30 * sinl(theta);//calculate the y component 	
		glVertex2d(x + 50, y + 50);//output vertex 	
	}
	glEnd();

	glViewport(0, 0, ww, hh);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(0, 50);
	glVertex2d(100, 50);
	glVertex2d(50, 0);
	glVertex2d(50, 100);
	glEnd();

	renderBitmapCharacter(50, 50, GLUT_BITMAP_HELVETICA_18, "HELLO WORLD");

	glutSwapBuffers();
}






void idle1() {
	
	cnt++;
	//rectangle.x += 0.1;
	//rectangle.y += 0.1;
	//glutSetWindow(1);
	//glutPostRedisplay();
	//glutSetWindow(2);
	//glutPostRedisplay();
}


void timer1(int value) { //auxiliary value, pass when registering callback
	cout << "5 sec elapsed, aux value : " << value << ", idlecnt : " << cnt << endl;
}

void mouse1(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON)
		cout << "left mouse ";
	else if (button == GLUT_RIGHT_BUTTON)
		cout << "right mouse ";
	else if (button == GLUT_MIDDLE_BUTTON)
		cout << "middle mouse ";

	if (state == GLUT_UP)
		cout << "UP";
	else if (state == GLUT_DOWN)
		cout << "DOWN";

	cout << "(" << x << "," << y << ")" << endl;

}

void key1(unsigned char key, int x, int y)
{
	cout << "key : " << key << " | mousepos : (" << x << "," << y << ")" << endl;

}
void skey1(int key, int x, int y)
{
	cout << "specialkey : " << key << " | mousepos : (" << x << "," << y << ")" << endl;
}


void display_sub() {
	cout << "display sub func" << endl;
	glutSetWindow(2);
	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(0, rectangle.y);
	glVertex2d(100, rectangle.y);
	glVertex2d(rectangle.x, 0);
	glVertex2d(rectangle.x, 100);
	glEnd();
	glutSwapBuffers();
}



int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //GLUT_DEPTH for 3D
	glutInitWindowPosition(400, 400);
	glutInitWindowSize(800, 800);
	cout << glutCreateWindow("Hello OpenGL") << endl;
	glClearColor(1, 1, 1, 1); //only needed once actually if you're going to clear with the same color
	glShadeModel(GL_FLAT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glutReshapeFunc(reshape1);
	glutDisplayFunc(display1);
	glutIdleFunc(idle1);
	glutKeyboardFunc(key1);
	glutSpecialFunc(skey1);
	glutMouseFunc(mouse1);
	glutTimerFunc(5000, timer1, 444);

	glewInit();

	//cout << glutCreateSubWindow(1, 100, 100, 100, 100) << endl;
	//glutDisplayFunc(display_sub);

	//glewInit();

	glutMainLoop();



	return 0;
}