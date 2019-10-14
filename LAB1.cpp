#include<gl/GLUT.H>
#include<iostream>
#include<cmath>
unsigned char keyPressed;
//d點,l線,p多邊,o圓,c清除,r恢復,q結束
using namespace std;
unsigned char status[1000] = {0};	//record what action done at when (e.g. status[23] = 'o' means that at 23'th arrays we are drawing a circle)
int mousex[1000], mousey[1000];		//position of mouseclick
int circlex[1000], circley[1000];   //for circle
int polyx[1000], polyy[1000];		//for poly
int polysx, polysy;
bool initPoly = false;
int i = 1;
int rePolys, polycount;
void Draw_Dot(double x0, double y0) {
	glVertex2i(x0, y0);
}
void Draw_Line(double x0, double x1, double y0, double y1) {
	bool downward = false;
	double m = abs(y1 - y0) / abs(x1 - x0); //slope
	if (x0 > x1) {	// if the line's going leftside, let (x1, y1) be left point
		swap(x0, x1);  
		swap(y0, y1);
	}
	glVertex2i(x0, y0);
	if (y0 > y1) { // if the line's going downward
		downward = true;
		y1 = y1 + (2 * (y0 - y1)); // mirror with respect to y = y0
	}
	if (m > 1) {  //if slope > 1, mirror with respect to y = x
		swap(x0, y0);
		swap(x1, y1);
	}
	int x = x0; //initial x value
	int y = y0; //initial y value
	int deltax = x1 - x0;
	int deltay = y1 - y0;
	int a = deltay;
	int b = -deltax;
	double d = a + b / 2;
	while (x <= x1) {		// draw from left to right
		if (d <= 0) {		// E
			x++;
			d += a;
			if (m > 1) {
				if (downward) { // mirror back while drawing with respect to y = x & y = y1 (y = x first, y = y1 second)
					glVertex2i(y, 2 * x0 - x); //y1 = y1 + (2 * (y0 - y1))
				}
				else {
					glVertex2i(y,x);
				}
			}
			else {
				if (downward) {
					glVertex2i(x, 2 * y0 - y); //y1 = y1 + (2 * (y0 - y1))
				}
				else {
					glVertex2i(x,y);
				}
			}
		}
		else {				// NE
			x++;
			y++;
			d += (a + b);
			if (m > 1) {
				if (downward) {
					glVertex2i(y, 2 * x0 - x); //y1 = y1 + (2 * (y0 - y1))
				}
				else {
					glVertex2i(y, x);
				}
			}
			else { 
				if (downward) {
					glVertex2i(x, 2 * y0 - y); //y1 = y1 + (2 * (y0 - y1))
				}
				else {
					glVertex2i(x, y);
				}
			}
		}
	}
}
void Draw_Circle(double x0, double x1, double y0, double y1) {
	int R{ (int)sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2)) };
	int x = 0;
	int y = R;
	int d = 1 - R;
	int incE = 3;
	int incSE = -2 * R + 5;
	glVertex2i(0 + x0, R + y0);
	glVertex2i(R + x0, 0 + y0);
	glVertex2i(-R + x0, 0 + y0);
	glVertex2i(0 + x0, -R + y0);
	while (x < y) {
		if (d < 0) {
			d += incE;
			incE += 2;
			incSE += 2;
			++x;
		}
		else {
			d += incSE;
			incE += 2;
			incSE += 4;
			++x;
			--y;
		}
		glVertex2i(x + x0, y + y0);	// shift the circle from (0, 0) to (x1, y1)
		glVertex2i(y + x0, x + y0);
		glVertex2i(x + x0, -y + y0);
		glVertex2i(y + x0, -x + y0);
		glVertex2i(-x + x0, y + y0);
		glVertex2i(-y + x0, x + y0);
		glVertex2i(-x + x0, -y + y0);
		glVertex2i(-y + x0, -x + y0);
	}
}
void Restore_Poly(int first, int last) {
		Draw_Line(mousex[first], mousex[last], mousey[first], mousey[last]);
	while (first < last) {
		if (mousex[first] != 0) {
			Draw_Line(mousex[first], mousex[first + 1], mousey[first], mousey[first + 1]);
		}
		first++;
	}
}
void restore() {
	glBegin(GL_POINTS);
	for (int j = 2; j <= 1000 ; j++) {
		switch (status[j - 1]) {
			case 'd':
				Draw_Dot(mousex[j - 1], mousey[j - 1]);
				break;
			case 'l':
				if (j - 2 == 0 || j % 2 == 0) { //don't draw original point / odd sets of point
					break;
				}
				Draw_Line(mousex[j - 2], mousex[j - 1], mousey[j - 2], mousey[j - 1]);
				break;
			case 'p':
				if (status[j - 2] != 'p') {
					rePolys = j - 1;
				}
				else if (status[j] != 'p') {
					Restore_Poly(rePolys, j - 1);
				}
				break;
			case 'o':
				if (j - 2 == 0 || j % 2 == 0) { //don't draw original point / odd sets of point
					break;
				}
				Draw_Circle(circlex[j - 1], circlex[j], circley[j - 1], circley[j]);
				break;
			case '0':
				break;
		}
	}
	glEnd();
	glFlush();
}
void MouseClick(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		mousex[i] = x;
		mousey[i] = 480 - y;
		i++;
		glBegin(GL_POINTS);
		switch (keyPressed) {
			case 'd':
				status[i - 1] = 'd';
				Draw_Dot(mousex[i - 1], mousey[i - 1]);
				break;
			case 'l':
				status[i - 1] = 'l';
				if (i - 2 == 0 || i % 2 == 0) { //don't draw original point / odd sets of point
					break;
				}
				else {
					Draw_Line(mousex[i - 2], mousex[i - 1], mousey[i - 2], mousey[i - 1]);
				}
				break;
			case 'p':
				status[i - 1] = 'p';
				polyx[i] = mousex[i-1];
				polyy[i] = mousey[i-1];
				if (polyx[i] == 0 && polyy[i] == 0) {   //not starting from (0,0)
					break;
				}
				if (!initPoly) {						//draw new set of edges for new polygon after last polygon closed/are the first polygon
					polysx = polyx[i];
					polysy = polyy[i];
					initPoly = true;
				}
				else {
					Draw_Line(polyx[i - 1], polyx[i], polyy[i - 1], polyy[i]);
				}
				break;
			case 'o':
				status[i - 1] = 'o';
				circlex[i] = mousex[i - 1];
				circley[i] = mousey[i - 1];
				if (i - 2 == 0 || i % 2 == 0) {			//don't draw original point / odd sets of point
					break;
				}
				Draw_Circle(circlex[i - 1], circlex[i], circley[i - 1], circley[i]);
				break;
		}
		glEnd();
		glFlush();
	}
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) {
		glBegin(GL_POINTS);
		switch (keyPressed) {
			case 'p':
				Draw_Line(polyx[i], polysx, polyy[i], polysy);
				for (int k = 0 ; k <= 1000; k++) {
					polyx[k] = 0;
					polyy[k] = 0;
				}
				initPoly = false;
				status[i] = 'P';
				i++;
				break;
		}
		glEnd();
		glFlush();
	}
}
void KeyBoard(unsigned char key, int x, int y) {
	keyPressed = key;
	switch (key) {
		case 'q':
			exit(0);
		case 'c':
			glClear(GL_COLOR_BUFFER_BIT);
			glFlush();
			break;
		case 'r':
			restore();
	}
}
void Initial(void) {
	glColor3f(1.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640, 0.0, 480);
	glFlush();
}
int main(int argc, char * argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Line Phase");
	glutDisplayFunc(Initial);
	glutMouseFunc(MouseClick);
	glutKeyboardFunc(KeyBoard);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glutMainLoop();//show windows
	return 0;
}
