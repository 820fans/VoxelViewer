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
//鼠标
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;

//使用相机变换代替模型变换
vec4 upDir = vec4(0, 0, 1, 1);//向上的方向默认为+z
vec4 desPos;
vec4 eyePos;
vec3 modelSize;//记录整个模型的大小，以便确认lookat的点
vector<cube> cubes;//用于保存所有的cube

class cube{
public:
    vec3 cor;
    float size=1.0;
};
//缓存
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

//    // 指定环境光的RGBA强度值
    GLfloat ambientLight[] = { 0.5f, 0.4f, 0.4f, 1.0f };
    // 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
//    initLight();
}

void disableEffects(){
    //在绘制包围体时，越简单越好。关掉纹理，光照等等。
    //不需要往缓冲区中写值。
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
    //测试desPos -> eyePos这条线与z轴的夹角
    vec3 dir = vec3(eyePos[0] - desPos[0], eyePos[1] - desPos[1], eyePos[2] - desPos[2]);
    vec3 zdir = vec3(0, 0, 1);
    //angleX>0 代表用户在向下拖，希望看到模型顶部
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
        //左键一般是一些操作
        mouseX = x;
        mouseY = y;
    }
    if (mouseRightDown)
    {
        int angleZ = (x - mouseX) / 3.0;//向右移动
        int flip = eyePos[1] - desPos[1] > 0 ? 1 : -1;//在移动到模型背后之后，x轴转方向改变
        int angleX = flip*(y - mouseY) / 3.0;
        //仅仅使用右键旋转模型，模仿MagicaVoxel
        mat4 translate = Translate(-desPos[0],-desPos[1],-desPos[2]);//平移到原点
        mat4 rotateZ = RotateZ(-angleZ);//绕z轴旋转一定角度

        //绕x轴旋转需要注意与z轴的夹角
        mat4 rotateX = mat4();
        if (testAngle(angleX))
            rotateX = RotateX(angleX); //绕x轴旋转一定角度

        eyePos = rotateX*rotateZ*translate*eyePos+desPos;//变化矩阵由位移和旋转叠加
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
    //限制观察距离
    if (length(eyePos-desPos+offset) <= 10) return;

    eyePos += offset;
    glutPostRedisplay();
}

void processNormalKeys(unsigned char key,int x,int y)
{
    vec4 offset = normalize(eyePos - desPos);
    if(key=='z'){
        // 移近
        offset *= -10;
    }
    else if(key == 'x'){
        // 移远
        offset *= 10;
    }
    else if(key == 'c'){
        // 超级近
        offset *= -50;
    }
    else if(key == 'v'){
        // 微小近
        offset *= -2;
    }
    //限制观察距离
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

        //绘制包围体
        glTranslatef(v[0],v[1],v[2]);
        //glColor3ub(randRGB(), randRGB(), randRGB());
        //glColor3ub(120, 120, 120);

  //      if(cu.size - 0.25 <= 1e-6){
  //          glColor3ub(70, 187, 28);// 绿色//glColor3ub(180, 11, 22);//yellow 143, 123, 25 //blue 145, 191, 255
  //      }
  //      else if(cu.size - 0.5 <= 1e-6){
  //          glColor3ub(143, 123, 25);  // 深黄
  //      }
  //      else{
  //          glColor3ub(124, 171, 225); //蓝色
		//}
		glColor3ub(124, 171, 225); //蓝色
        glutSolidCube(cu.size);
        //结束遮挡查询
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
        //读入整个模型的大小
        fscanf(fd, "size %d %d %d\n", &sizeX, &sizeY, &sizeZ);

        //设置模型中心点为观测原点
        desPos = vec4(static_cast<GLfloat>(sizeX / 2.0), static_cast<GLfloat>(sizeY / 2.0), static_cast<GLfloat>(sizeZ / 2.0), 1);
        int count=0;
        while (fscanf(fd, "%f %f %f %f\n", &x, &y, &z, &size)>0)
        {
            vec3 v = vec3(x, y, z);
            cube c = cube();
            c.cor = v;
            c.size = size;
            cubes.push_back(c);//读数据到内存中
        }
        fclose(fd);

        //设置观测者的位置
        eyePos = vec4(desPos[0] + sizeX/2.0, -sizeY*2, desPos[2] + sizeZ, 1);
        printf("read complete!");
    }
    else{
        printf("Fail to load File! Check Directory!");
    }
}


void init(){
    //读入文件
    readFile();

    enableEffects();
    //写缓存
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
/* 矩阵
	-1.0, 0.0, 0.0, 1.0f,
	0.0, 1.0, 0.0, 1.0f,
	1.0, 0.0, 0.0, 1.0f,
	0.0, -1.0, 0.0, 1.0f,
	0.0, 0.0, 1.0, 1.0f,
	0.0, 0.0, -1.0, 1.0f,*/
