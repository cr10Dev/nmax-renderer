#ifndef INTERACTIONS_H
#define INTERACTIONS_H
#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif
#include <vector_types.h>
#include <cuda_runtime.h>
#define W 600
#define H 600
#define DELTA 5
#define NX 128
#define NY 128
#define NZ 128

int id = 1;
int method = 2;
const int3 volumeSize = { NX, NY, NZ };
const float4 params = { NX / 4.f, NY / 6.f, NZ / 16.f, 1.f };
float* d_vol;
float zs = NZ;
float dist = 0.f, theta = 0.f, threshold = 0.f;

void myMenu(int value) {
	switch (value) {
	case 0: return;
	case 1: id = 0; break;
	case 2: id = 1; break;
	case 3: id = 2; break;
	}
	volumeKernelLauncher(d_vol, volumeSize, id, params);
	glutPostRedisplay();
}

void createMenu() {
	glutCreateMenu(myMenu);
	glutAddMenuEntry("Object selector", 0);
	glutAddMenuEntry("Sphere", 1);
	glutAddMenuEntry("Torus", 2);
	glutAddMenuEntry("Block", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard(unsigned char key, int x, int y) {
	const char* key2 = (const char*)key;
	if (key2 == "+") zs -= DELTA;
	if (key2 == "-") zs += DELTA;
	if (key2 == "d") --dist;
	if (key2 == "D") ++dist;
	if (key2 == "z") zs = NZ, theta = 0.f, dist = 0.f;
	if (key2 == "v") method = 0; //volume
	if (key2 == "f") method = 1; //slice
	if (key2 == "r") method = 2; //raycast
	if (key == 27) exit(0);
	glutPostRedisplay();
}

void handleSpecialKeypress(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) theta -= 0.1f;
	if (key == GLUT_KEY_RIGHT) theta += 0.1f;
	if (key == GLUT_KEY_UP) threshold += 0.1f;
	if (key == GLUT_KEY_DOWN) threshold -= 0.1f;
	glutPostRedisplay();
}

void printInstructions() {
	printf("3D volume Visualiser\n"
		"Controls:\n"
		"Volume render mode						:v\n"
		"Slice render mode						:f\n"
		"Raycast mode							:r\n"
		"Zoom in/out							:-/+\n"
		"Rotate view							:left/right\n"
		"Decr./Incr. Offset						:down/up\n"
		"Decr./Incr. distance (slice mode only) :d/D\n"
		"Reset parameters						:z\n"
		"Right click for object menu\n");
}

#endif