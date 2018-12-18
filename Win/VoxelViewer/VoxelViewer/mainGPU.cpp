//#define GLUT_DISABLE_ATEXIT_HACK 
//#define _CRT_SECURE_NO_WARNINGS
//#define NDEBUG
//#include <cstdlib>
//#include <stdio.h>
//#include <algorithm>
//#include <ctime>
//#include <vector>
////#include <GL/glew.h>
//#include <gluTest.h>
//#include <GL/freeglut.h>
//#include <math.h>
//#include <math/vmath.h>
//#include <math/mat.h>
//
//#pragma comment(lib, "glew32.lib")
//
//using namespace std;
//
//GLuint *ebo, *vao, *vbo;
//// This will identify our vertex buffer
//GLuint vertexbuffer;
//GLuint VertexArrayID;
//
//static const GLfloat g_vertex_buffer_data[] = {
//	-1.0f, -1.0f, 0.0f,
//	1.0f, -1.0f, 0.0f,
//	0.0f, 1.0f, 0.0f,
//};
//
//void enableEffects(){
//	glShadeModel(GL_SMOOTH);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_COLOR_MATERIAL);
//	glEnable(GL_NORMALIZE);
//	glDepthMask(GL_TRUE);
//	// Cull triangles which normal is not towards the camera
//	glEnable(GL_CULL_FACE);
//
//	// 指定环境光的RGBA强度值
//	GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
//	// 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
//}
//
//void renderScene(void)
//{
//	//  Clear screen and Z-buffer
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// 1rst attribute buffer : vertices
//	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//	glVertexAttribPointer(
//		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//		3,                  // size
//		GL_FLOAT,           // type
//		GL_FALSE,           // normalized?
//		0,                  // stride
//		(void*)0            // array buffer offset
//		);
//	// Draw the triangle !
//	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//	glDisableVertexAttribArray(0);
//
//	glFlush();
//}
//
//void init(){
//
//	enableEffects();
//	
//	glGenVertexArrays(1, &VertexArrayID);
//	glBindVertexArray(VertexArrayID);
//
//	// Generate 1 buffer, put the resulting identifier in vertexbuffer
//	glGenBuffers(1, &vertexbuffer);
//
//	// The following commands will talk about our 'vertexbuffer' buffer
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//
//	// Give our vertices to OpenGL.
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//}
//
//void main(int argc, char **argv)
//{
//	//  Initialize GLUT and process user parameters
//	glutInit(&argc, argv);
//
//	//  Request double buffered true color window with Z-buffer
//	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//	glutInitDisplayMode(GLUT_RGB);
//
//	glutInitWindowPosition(0, 0);
//	glutInitWindowSize(800, 600);
//	glutCreateWindow("GLDemo");
//
//	glewInit();
//
//	init();
//	glutDisplayFunc(renderScene);
//
//	glutMainLoop();
//}
