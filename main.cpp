#include <GL/glut.h>

float *PixelBuffer;
void display();
void MakePix(int x, int y);
int main(int argc, char *argv[])
{
	PixelBuffer = new float[200 * 200 * 3];

	MakePix(10,10);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);

	glutInitWindowSize(200, 200);

	glutInitWindowPosition(100, 100);

	int MainWindow = glutCreateWindow("Hello Graphics!!");
	glClearColor(0, 0, 0, 0);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}

void MakePix(int x, int y){
	PixelBuffer[x*200 + y]     = 1;
	PixelBuffer[x*200 + y + 1] = 1;
	PixelBuffer[x*200 + y + 2] = 1;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, PixelBuffer);

	glEnd();
	glFlush();

	return;
}
