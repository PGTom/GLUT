#include<gl/GLUT.H>
#include<iostream>
#include<cmath>
//d點,l線,p多邊,o圓,c清除,r恢復,q結束
using namespace std;
int mousex[50], mousey[50];
int i = 1, j = 1;
void Draw_Line(double x0, double x1, double y0, double y1) {
	cout << "Draw_Line:" << x0 << " " << y0 << endl;
	cout << "next Draw_Line:" << x1 << " " << y1 << endl << endl << endl;
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
	//for (double x = x0; x <= x1; x = x + 0.0001) {
	//	glBegin(GL_POINTS);
	//	glVertex2f(x, y0 + (slope *(x - x0)));
	//	glEnd();
		//if ((y0 + (slope *(x - x0))) > (y + 1/2) ) { // y實際點 > y像素中點
		//	y++;
		//}
}
void onMouseClick(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		mousex[i] = x;
		mousey[i] = 480 - y;
		i++;
	}
	glBegin(GL_POINTS);
	if (i - 2 == 0 || i % 2 == 0) { // 原點
		glEnd();
	}
	else {
		Draw_Line(mousex[i - 2], mousex[i - 1], mousey[i - 2], mousey[i - 1]);
	}
	glEnd();
	glFlush();
}
void Initial(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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
	glutMouseFunc(onMouseClick);
	glutMainLoop();//show windows
	return 0;
}
