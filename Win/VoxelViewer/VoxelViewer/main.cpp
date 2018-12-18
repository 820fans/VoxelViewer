#define GLUT_DISABLE_ATEXIT_HACK 
#define _CRT_SECURE_NO_WARNINGS
#define NDEBUG
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <math/vmath.h>
#include <math/mat.h>

#pragma comment(lib, "glew32.lib")

using namespace std;

class cube;
//���
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;

//ʹ������任����ģ�ͱ任
vec4 upDir = vec4(0, 0, 1, 1);//���ϵķ���Ĭ��Ϊ+z
vec4 desPos;
vec4 eyePos;
vec3 modelSize;//��¼����ģ�͵Ĵ�С���Ա�ȷ��lookat�ĵ�
vector<cube> cubes;//���ڱ������е�cube

class cube{
public:
    vec3 cor;
    float size=1.0;
};
//����
GLuint buffer;

void enableEffects(){
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glDepthMask(GL_TRUE);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

//    // ָ���������RGBAǿ��ֵ
    GLfloat ambientLight[] = { 0.5f, 0.4f, 0.4f, 1.0f };
    // ���ù���ģ�ͣ���ambientLight��ָ����RGBAǿ��ֵӦ�õ�������
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
//    initLight();
}

void disableEffects(){
    //�ڻ��ư�Χ��ʱ��Խ��Խ�á��ص��������յȵȡ�
    //����Ҫ����������дֵ��
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_NORMALIZE);
    glDepthMask(GL_FALSE);
    glColorMask(0, 0, 0, 0);
}

void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if (state == GLUT_UP)
            mouseLeftDown = false;
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if (state == GLUT_UP)
            mouseRightDown = false;
    }
}

bool testAngle(int angleX){
    //����desPos -> eyePos��������z��ļн�
    vec3 dir = vec3(eyePos[0] - desPos[0], eyePos[1] - desPos[1], eyePos[2] - desPos[2]);
    vec3 zdir = vec3(0, 0, 1);
    //angleX>0 �����û��������ϣ�ϣ������ģ�Ͷ���
    double value = dot(dir, zdir) / (length(dir)*length(zdir));
    if (value > 0.95 && angleX > 0){
        return false;
    }
    if (value < -0.95 && angleX < 0){
        return false;
    }
    return true;
}

void mouseMotionCB(int x, int y)
{
    if (mouseLeftDown)
    {
        //���һ����һЩ����
        mouseX = x;
        mouseY = y;
    }
    if (mouseRightDown)
    {
        int angleZ = (x - mouseX) / 3.0;//�����ƶ�
        int flip = eyePos[1] - desPos[1] > 0 ? 1 : -1;//���ƶ���ģ�ͱ���֮��x��ת����ı�
        int angleX = flip*(y - mouseY) / 3.0;
        //����ʹ���Ҽ���תģ�ͣ�ģ��MagicaVoxel
        mat4 translate = Translate(-desPos[0],-desPos[1],-desPos[2]);//ƽ�Ƶ�ԭ��
        mat4 rotateZ = RotateZ(-angleZ);//��z����תһ���Ƕ�

        //��x����ת��Ҫע����z��ļн�
        mat4 rotateX = mat4();
        if (testAngle(angleX))
            rotateX = RotateX(angleX); //��x����תһ���Ƕ�

        eyePos = rotateX*rotateZ*translate*eyePos+desPos;//�仯������λ�ƺ���ת����
        eyePos[3] = 1;
        mouseX = x;
        mouseY = y;
    }

    glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y){
    vec4 offset = normalize(eyePos - desPos);
    if (dir > 0){
        offset *= -10;
    }
    else{
        offset *= 10;
    }
    //���ƹ۲����
    if (length(eyePos-desPos+offset) <= 10) return;

    eyePos += offset;
    glutPostRedisplay();
}

void processNormalKeys(unsigned char key,int x,int y)
{
    vec4 offset = normalize(eyePos - desPos);
    if(key=='z'){
        // �ƽ�
        offset *= -10;
    }
    else if(key == 'x'){
        // ��Զ
        offset *= 10;
    }
    else if(key == 'c'){
        // ������
        offset *= -50;
    }
    else if(key == 'v'){
        // ΢С��
        offset *= -2;
    }
    //���ƹ۲����
    if (length(eyePos-desPos+offset) <= cubes[0].size/4.0) return;

    eyePos += offset;
    glutPostRedisplay();
}


void renderScene(void) {
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eyePos[0], eyePos[1],eyePos[2], desPos[0],desPos[1],desPos[2],upDir[0],upDir[1],upDir[2]);

    int count = 0;
    for (vector<cube>::iterator it = cubes.begin(); it != cubes.end();it++)
    {
        cube cu = *(it);
        vec3 v = cu.cor;

        glPushMatrix();

        //���ư�Χ��
        glTranslatef(v[0],v[1],v[2]);
        //glColor3ub(randRGB(), randRGB(), randRGB());
        //glColor3ub(120, 120, 120);

  //      if(cu.size - 0.25 <= 1e-6){
  //          glColor3ub(70, 187, 28);// ��ɫ//glColor3ub(180, 11, 22);//yellow 143, 123, 25 //blue 145, 191, 255
  //      }
  //      else if(cu.size - 0.5 <= 1e-6){
  //          glColor3ub(143, 123, 25);  // ���
  //      }
  //      else{
  //          glColor3ub(124, 171, 225); //��ɫ
		//}
		glColor3ub(124, 171, 225); //��ɫ
        glutSolidCube(cu.size);
        //�����ڵ���ѯ
        //glEndQuery(GL_SAMPLES_PASSED);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void sizeChanged(int width, int height){
    renderScene();
}

void readFile(){
    // dragon bunny  kawada-hironex bike
    FILE* fd = fopen("zot/bunny_64_2_BFS.zot", "rb");
//    FILE* fd = fopen("zot/triangle_64_1.zot", "rb");
    int sizeX,sizeY,sizeZ;
    float x, y, z, size;
    if (fd != NULL){
        //��������ģ�͵Ĵ�С
        fscanf(fd, "size %d %d %d\n", &sizeX, &sizeY, &sizeZ);

        //����ģ�����ĵ�Ϊ�۲�ԭ��
        desPos = vec4(static_cast<GLfloat>(sizeX / 2.0), static_cast<GLfloat>(sizeY / 2.0), static_cast<GLfloat>(sizeZ / 2.0), 1);
        int count=0;
        while (fscanf(fd, "%f %f %f %f\n", &x, &y, &z, &size)>0)
        {
            vec3 v = vec3(x, y, z);
            cube c = cube();
            c.cor = v;
            c.size = size;
            cubes.push_back(c);//�����ݵ��ڴ���
        }
        fclose(fd);

        //���ù۲��ߵ�λ��
        eyePos = vec4(desPos[0] + sizeX/2.0, -sizeY*2, desPos[2] + sizeZ, 1);
        printf("read complete!");
    }
    else{
        printf("Fail to load File! Check Directory!");
    }
}


void init(){
    //�����ļ�
    readFile();

    enableEffects();
    //д����
    /*glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, cubes.size()*sizeof(vec3), &cubes.front(), GL_STATIC_DRAW);*/

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    double aspect = (double)viewport[2] / (double)viewport[3];
    gluPerspective(60, aspect, 1, 1000);

}

int main(int argc, char **argv)
{
    //  Initialize GLUT and process user parameters
    glutInit(&argc, argv);

    //  Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1200, 900);
    glutCreateWindow("GLDemo");

    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);
    glutMouseWheelFunc(mouseWheel);
    glutKeyboardFunc(processNormalKeys);

    init();
    glutDisplayFunc(renderScene);
    glutReshapeFunc(sizeChanged);

    glutMainLoop();
    return 0;
}
/* ����
	-1.0, 0.0, 0.0, 1.0f,
	0.0, 1.0, 0.0, 1.0f,
	1.0, 0.0, 0.0, 1.0f,
	0.0, -1.0, 0.0, 1.0f,
	0.0, 0.0, 1.0, 1.0f,
	0.0, 0.0, -1.0, 1.0f,*/
