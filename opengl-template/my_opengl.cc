#include "my_opengl.h"
#include <math.h>
#include <GL/glh_glut.h>
#include "load_obj.h"
using namespace glh;
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat zRot = 0;

int myState = 0;
int scalState = 0;
int oldX = 0;
int oldY = 0;
float rate = 1;
double x_trans, y_trans, z_trans = 0;
double x_old_theta, y_old_theta, z_old_theta = 0;
float scale=1;

void OpenglInit(int argc, char** argv)
{
	//objs1.Load("./dataset/Scene03/VeachMIS.obj");
	objs1.Load("./dataset/Scene01/cup.obj");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowSize(512, 512);			//set window size
	glutInitWindowPosition(100, 100);		//set window position
	glutCreateWindow("Display");		//window name
	
	// scale
	/*scale = 0.9f / (2.0f*obj1.radius_);
	glScalef(scale, scale, scale);*/

	// message
	glutDisplayFunc(DisplayFunc);//屏幕显示的回调函数
	glutReshapeFunc(ReshapeFunc);
	glutIdleFunc(IdleFunc);//闲置时回调函数（当没有消息时调用）
	glutKeyboardFunc(KeyboardFunc);//数字、字母键的按键检测的回调函数
	glutSpecialFunc(SpecialFunc);//特殊按键检测（F1~F12，控制键）
	glutMouseFunc(MouseFunc);//鼠标检测
	glutMotionFunc(MotionFunc);//鼠标按着拖动检测
	
	glutPassiveMotionFunc(PassiveMotionFunc);//鼠标移动检测

	// main loop
	glutMainLoop();
}

void SetBackground1()
{
	// exmaple: shape1
	float color = 0.0f;
	glBegin(GL_POINTS);
	for (float posY = -1.0f; posY <= 1.0f; posY += 0.001f)
	{
		color += 0.0005f;
		//设置点颜色
		glColor3f(pow(color, 2), color, color); //设置点颜色
		for (float posX = 1.0f; posX >= -1.0f; posX -= 0.001f)
		{
			//绘制点
			glVertex3f(posX, posY, 0);
		}
	}
	glEnd();
}



// insert to DisplayFunc
void AddText()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST); //打开深度测试

	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(-0.9/scale,0.9/scale);
	DrawString("[Page Up] Zoom in");
	glRasterPos2f(-0.9 / scale, 0.8 / scale);
	DrawString("[Page Down] Zoom out");
}
void AddLight()
{
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	//light
	GLfloat ambient_light[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat difuss_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat specular_light[] = { 0.1,0.1,0.1,1.0 };
	GLfloat emission_light[] = { 50,50,40,1};
	GLfloat LightPos[] = { 0.0, 1.589, -1.274, 1.0 };
	
	/*glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difuss_light);*/
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
	glLightfv(GL_LIGHT0, GL_EMISSION, emission_light);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glEnable(GL_LIGHT0);	// enable light0
	glEnable(GL_LIGHTING);	// open light
}
void AddLight(Material& dat)
{
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	


	//light
	GLfloat ambient_light[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat difuss_light[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular_light[] = { 0.0,0.0,0.0,1.0 };
	GLfloat emission_light[] = { 40,40,40,1 };
	GLfloat LightPos[] = { -2.758771896,1.5246,0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difuss_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
	glLightfv(GL_LIGHT0, GL_EMISSION, emission_light);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glEnable(GL_LIGHT0);	// enable light0	
	glEnable(GL_LIGHTING);	// open light
}
void AddMaterial(Material& dat)
{
	// material
	GLfloat ambient_material[] = { 0.2, 0.01, 0.01, 0.5 };
	GLfloat diffuse_material[] = { 1.0, 0, 0, 1.0 };
	GLfloat specular_material[] = { 1.0,1,1, 1.0 };
	GLfloat shininess_material[] = {10};
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_material);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess_material);
	glEnable(GL_COLOR_MATERIAL);
}

void DisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear color buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);	//can set the background
	
	// 保存矩阵状态并旋转
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);	
	
	glColor3f(0.5, 0.5, 0.5);
	
	for (int i = 0; i < objs1.groups_.size(); i++)
	{
		int id_temp = objs1.groups_[i].mtl_id_;
		AddMaterial(objs1.mtls_.materials_[id_temp]);
		AddLight();
		for (auto& f : objs1.groups_[i].f_)
		{			
			glBegin(GL_POLYGON);
			glNormal3f(f.normal_.x, f.normal_.y, f.normal_.z);
			for (int j = 0; j < f.v_id_.size(); j++)
			{
				Vec3f& v_temp = objs1.v_[f.v_id_[j]];
				Vec3f& vt_temp = objs1.vt_[f.vt_id_[j]];
				//glTexCoord2f(vt_temp.x, vt_temp.y);
				glVertex3f(v_temp.x, v_temp.y, v_temp.z);
			}			
			glEnd();
		}
	}
	glPopMatrix();	
	glutSwapBuffers();	//swap buffer
}

void ReshapeFunc(GLsizei width, GLsizei height)
{
	//if (height == 0) height = 1;

	//glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	//glMatrixMode(GL_MODELVIEW);
	////glLoadIdentity();
}

void Plot(vector<float>& x, vector<float>& y)
{
	glColor3f(1, 0, 0);
	if (x.size() == y.size())
	{
		vector<float>::iterator it = max_element(y.begin(), y.end());
		float y_max = *it;
		it = min_element(y.begin(), y.end());
		float y_min = *it;

		glBegin(GL_LINES);
		for (int i = 0; i < x.size() - 1; i++)
		{
			glVertex3f(TOSCREEN(x[i], x[0], x[x.size() - 1]), TOSCREEN(y[i], y_min, y_max), 0);
			glVertex3f(TOSCREEN(x[i + 1], x[0], x[x.size() - 1]), TOSCREEN(y[i + 1], y_min, y_max), 0);
		}
		glEnd();
	}
	else
	{
		cout << "Error: The Dimension of x and y not matched!" << endl;
	}
}

void Mesh(vector<vector<Vec3f>>& dat)
{
	for (int i = 0; i < dat.size() - 1; i++)
	{
		for (int j = 0; j < dat[0].size() - 1; j++)
		{
			float colorx = 0.6*(sin(10 * dat[i][j].x) + 1);
			float colory = 0.6*(sin(10 * dat[i][j].y) + 1);
			float colorz = 0.6*(sin(10 * dat[i][j].x + 1) + 1);

			glColor3f(colorx, colory, colorz);
			glBegin(GL_LINE_LOOP);
			glVertex3f(dat[i][j].x, dat[i][j].y, dat[i][j].z);
			glVertex3f(dat[i][j + 1].x, dat[i][j + 1].y, dat[i][j + 1].z);
			glVertex3f(dat[i + 1][j + 1].x, dat[i + 1][j + 1].y, dat[i + 1][j + 1].z);
			glVertex3f(dat[i + 1][j].x, dat[i + 1][j].y, dat[i + 1][j].z);
			glVertex3f(dat[i][j].x, dat[i][j].y, dat[i][j].z);
			glEnd();
		}
	}
}

void DrawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) {
		isFirstCall = 0;
		lists = glGenLists(128);
		wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}



//闲置时调用的函数
void IdleFunc()
{
	glutPostRedisplay();//重新绘制（即调用DisplayFunc()函数）
}
//数字、字母键按键检测
void KeyboardFunc(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		
			
	}

	if (key == 's' || key == 'S')
	{
		
	}

	if (key == 'a' || key == 'A')
	{
	
	}

	if (key == 'd' || key == 'D')
	{
		
	}

}
//F1~F12、控制键检测
void SpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		xRot -= 5.0f;
		break;
	case GLUT_KEY_DOWN:
		xRot += 5.0f;
		break;
	case GLUT_KEY_LEFT:
		yRot -= 5.0f;
		break;
	case GLUT_KEY_RIGHT:
		yRot += 5.0f;
		break;
	case GLUT_KEY_F1:
		zRot += 5.0f;
		break;
	case GLUT_KEY_F2:
		zRot -= 5.0f;
		break;
	case GLUT_KEY_PAGE_DOWN:
		glScalef(0.9, 0.9, 0.9);
		break;
	case GLUT_KEY_PAGE_UP:
		glScalef(1.1, 1.1, 1.1);
		break;


	}

	// 使用新的坐标重新绘制场景
	glutPostRedisplay();
}
//鼠标检测
void MouseFunc(int button, int state, int x, int y)
{
	//button  鼠标按键 0 -- 左键    1 -- 中键   2 -- 右键
	//state  鼠标状态  0 -- 按下  1 -- 抬起
	//x,y  鼠标的像素点坐标（以窗口的左上角为原点的坐标系）
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		oldX = x;
		oldY = y;
		myState = 1;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		myState = 0;
		oldX = x;
		oldY = y;
		x_old_theta = xRot;
		y_old_theta = yRot;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		scalState = 1;
		oldX = x;
		oldY = y;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		scalState = 0;

	glutPostRedisplay();
}

//鼠标按着拖动
void MotionFunc(int x, int y)
{
	//x,y  鼠标的像素点坐标（以窗口的左上角为原点的坐标系）
	
	if (myState == 1)
	{
		yRot = (x - oldX) / 3 + y_old_theta;
		xRot = (y - oldY) / 3 + x_old_theta;
	}
	if (scalState == 1)
	{
		if (y > oldY)
			rate += 0.02;
		else
		{
			rate -= 0.02;
			if (rate < 0.1)
				rate = 0.1;
		}
		oldX = x;
		oldY = y;
	}
	glutPostRedisplay();
}



//鼠标移动
void PassiveMotionFunc(int x, int y)
{
	//x,y  鼠标的像素点坐标（以窗口的左上角为原点的坐标系）
	
}