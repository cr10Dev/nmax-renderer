#include "kernel.h"
#include "interactions.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

GLuint pbo = 0;
GLuint tex = 0;
struct cudaGraphicsResource* cuda_pbo_resource;

void render() {
	uchar4* d_out = 0;
	cudaGraphicsMapResources(1, &cuda_pbo_resource, 0);
	cudaGraphicsResourceGetMappedPointer((void**)&d_out, NULL, cuda_pbo_resource);

	kernelLauncher(d_out, d_vol, W, H, volumeSize, method, zs, theta, threshold, dist);
	cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0);
	char title[128];
	sprintf(title, "volume visualiser : objId = %d, method = %d, dist = %.1f, theta = %.1f", id, method, dist, theta);
	glutSetWindowTitle(title);
}

void draw_texture() {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1, 1);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void display() {
	render();
	draw_texture();
	glutSwapBuffers();

}


