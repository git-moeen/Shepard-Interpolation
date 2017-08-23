/*
* SeyedMoein Mirhosseini 20165447
* Assignment 1, Data Visualization Course
*/

#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <fstream>
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;


unsigned char* board;

struct vertex {
	int x; int y; int r;  int g;  int b;
};

vector <vertex> verti;

void getdata() {
	verti.clear();
	ifstream infile("../file.txt");
	int x, y, r, g, b;
	while (infile >> x >> y >> r >> g >> b)
	{
		vertex temp_v = { x,y,r,g,b };
		verti.push_back(temp_v);
	}
}
double cal_dist(double x1, double y1, double x2, double y2) {
	double x = x1 - x2;
	double y = y1 - y2;
	return (sqrt(pow(x, 2) + pow(y, 2)));
}
//Fall off basis function
double basis1(double d) {
	return (1.0 / (1.0 + d*d));
}
void shepard() {
	board = new unsigned char[512 * 512 * 3];
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			double sum_down = 0, rsum = 0, gsum = 0, bsum = 0, basis = 0;
			for (int u = 0; u < verti.size(); u++) {
				basis = basis1(cal_dist(i, j, verti[u].x, verti[u].y));
				sum_down += basis;
				rsum += (verti[u].r*basis);
				gsum += (verti[u].g*basis);
				bsum += (verti[u].b*basis);
			}
			int index = ((511 - j) * 512 + i) * 3;
			board[index] = int(rsum / sum_down);
			board[index + 1] = int(gsum / sum_down);
			board[index + 2] = int(bsum / sum_down);
		}
	}
}



void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shepard();
	glDrawPixels(512, 512, GL_RGB, GL_UNSIGNED_BYTE, board);
	glutSwapBuffers();
}

int main(int argc, char **argv) {

	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("ECE522 - Assignment 1");
	getdata();
	// register callbacks
	glutDisplayFunc(renderScene);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}