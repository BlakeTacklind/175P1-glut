#include <GL/glut.h>
#include "OpenGLhandler.h"

float* OpenGLhandler::PixelBuffer;
void OpenGLhandler::init(int* argc, char** argv)
{
	PixelBuffer = new float[200 * 200 * 3];

	MakePix(10,10);
	MakePix(1,1);

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);

	glutInitWindowSize(200, 200);

	glutInitWindowPosition(100, 100);

	int MainWindow = glutCreateWindow("Hello Graphics!!");
	glClearColor(0, 0, 0, 0);
	glutDisplayFunc(display);

	glutKeyboardFunc(Keystroke);
	
	glutMainLoop();


	return;
}

void OpenGLhandler::Keystroke(unsigned char key, int x, int y){
	glutReshapeWindow(400,400);

	glutSwapBuffers();
	MakePix(0,0);

	glutPostRedisplay();
}

void OpenGLhandler::MakePix(int x, int y){
	PixelBuffer[(y*200 + x) * 3 ]    = 1;
	PixelBuffer[(y*200 + x) * 3 + 1] = 1;
	PixelBuffer[(y*200 + x) * 3 + 2] = 1;
}

void OpenGLhandler::display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, PixelBuffer);

	glEnd();
	glFlush();

	return;
}
