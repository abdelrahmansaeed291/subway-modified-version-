//  Includes
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <iostream>
using namespace std;
#include <Windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <math.h>
#include <random>
#include <string.h>
#include <glut.h>
//-----------------

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

int WIDTH = 1920;
int HEIGHT = 1080;

bool start = false;
bool firstScene = true;
bool secondScene = false;
bool thirdScene = false;
bool isFirstPerson = false;
bool firstRail = false;
bool thirdRail = false;
bool down = true;
bool jump = false;
bool coinTaken[34];
bool arrowTaken[9];
bool keyTaken[5];
bool kill = false;
bool open = false;
bool KillWitharrow = false;
bool endScene1 = false;
bool endScene2 = false;
bool endScene3 = false;
bool done = false;
bool isWin = false;
bool isGameOver = false;
bool fov = false;
bool paused = false;
float posPlayerX ;
float posPlayerZ;
float playerY = 0;
float yDirection = 0;
float objectRot = 0;
float objectRotWheel = 0;
float fovy = 70;
float gravity = 1;
float treePos[8] = {-1,-3,-5,-7,-9,-11,-13,-15};
float arrowRot = 0;
float arrowPosX = 0;
float deathscale = 1;


float scene1Intencity = -0.1;
float scene2Intencity = 0.2f;
float scene3Intencity = 1;
float speed = 5;
int counter1st = 0;
int counter3rd = 0;
int hearts = 3;
int coins1 = 0;
int coins2 = 0;
int arrows = 0;
int keys = 0;
int runPlayer = 0;
int cameraZoom = 0;

GLuint tex_1;
GLuint tex_2;

// Model Variables
Model_3DS model_death;
Model_3DS model_archer;
Model_3DS model_bow;
Model_3DS model_wheel;
Model_3DS model_treasure;
Model_3DS model_arrow;
Model_3DS model_yacht;
Model_3DS model_yacht2;
Model_3DS model_tree;
Model_3DS model_barrier;
Model_3DS model_barrier2;
Model_3DS model_coins;
Model_3DS model_castle;
Model_3DS model_weapon;
Model_3DS model_key;

// Textures
GLTexture tex_rails1;
GLTexture tex_mine;
GLTexture tex_ground1;
GLTexture tex_rails2;
GLTexture tex_ground2;
GLTexture tex_trousers;
GLTexture tex_tshirt;
GLTexture tex_lovetrousers;
GLTexture tex_dress;
GLTexture tex_playerface;
GLTexture tex_loveface;

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0.996158f, float eyeY = 0.310991f, float eyeZ = 0.5f, float centerX = 0.0582472f, float centerY = -0.0358866f, float centerZ = 0.5f, float upX = -0.346878f, float upY = 0.937911f, float upZ = 0.0f) /*Camera(float eyeX = 0.467848f, float eyeY = 0.211558f, float eyeZ = 0.5f, float centerX = -0.470065f, float centerY = -0.13532f, float centerZ = 0.5f, float upX = -0.346878f, float upY = 0.937911f, float upZ = 0.0f)*/ {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;

	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;

	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;

	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitLightSource()
{
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);

	GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	GLfloat light_position[] = { -0.9f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void InitMaterial()
{
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	GLfloat shininess[] = { 10.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void Launch(int value)
{
	sndPlaySound(TEXT("Launch"), SND_ASYNC | SND_FILENAME);
}

void Reset1(int value)
{
	camera.eye.x = 0.996158f;
	camera.eye.y = 0.310991f;
	camera.eye.z = 0.5f;
	camera.center.x = 0.0582472f;
	camera.center.y = -0.0358866f;
	camera.center.z = 0.5f;
	camera.up.x = -0.346878f;
	camera.up.y = 0.937911f;
	camera.up.z = 0.0f;
	posPlayerX = 0;
	posPlayerZ = 0;
	firstScene = true;
	secondScene = false;
	thirdScene = false;
	isFirstPerson = false;
	firstRail = false;
	thirdRail = false;
	down = true;
	jump = false;
	for (int i = 0; i < 17; i++)
	{
		coinTaken[i] = false;
	}
	for (int i = 0; i < 9; i++)
	{
		arrowTaken[i] = false;
	}
	kill = false;
	KillWitharrow = false;
	endScene1 = false;
	done = false;
	counter1st = 0;
	counter3rd = 0;
	playerY = 0;
	yDirection = 0;
	objectRot = 0;
	objectRotWheel = 0;
	fovy = 70;
	gravity = 1;
	deathscale = 1;
	arrowRot = 0;
	arrowPosX = 0;
	scene1Intencity = -0.05f;
	hearts = 3;
	coins1 = 0;
	arrows = 0;
	runPlayer = 0;
	glutPostRedisplay();
}
void Reset2(int value)
{
	camera.eye.x = 0.996158f;
	camera.eye.y = 0.310991f;
	camera.eye.z = 0.5f;
	camera.center.x = 0.0582472f;
	camera.center.y = -0.0358866f;
	camera.center.z = 0.5f;
	camera.up.x = -0.346878f;
	camera.up.y = 0.937911f;
	camera.up.z = 0.0f;
	posPlayerX = 0;
	posPlayerZ = 0;
	firstScene = false;
	secondScene = true;
	thirdScene = false;
	isFirstPerson = false;
	firstRail = false;
	thirdRail = false;
	down = true;
	jump = false;
	for (int i = 17; i < 34; i++)
	{
		coinTaken[i] = false;
	}
	for (int i = 0; i < 5; i++)
	{
		keyTaken[i] = false;
	}
	endScene2 = false;
	open = false;
	done = false;
	counter1st = 0;
	counter3rd = 0;
	playerY = 0;
	yDirection = 0;
	objectRot = 0;
	objectRotWheel = 0;
	fovy = 70;
	gravity = 1;
	scene2Intencity = 0.2f;
	hearts = 3;
	coins2 = 0;
	keys = 0;
	runPlayer = 0;
	glutPostRedisplay();
}
void Reset3(int value)
{
	camera.eye.x = 0.996158f;
	camera.eye.y = 0.310991f;
	camera.eye.z = 0.5f;
	camera.center.x = 0.0582472f;
	camera.center.y = -0.0358866f;
	camera.center.z = 0.5f;
	camera.up.x = -0.346878f;
	camera.up.y = 0.937911f;
	camera.up.z = 0.0f;
	posPlayerX = 0;
	posPlayerZ = 0;
	firstScene = false;
	secondScene = false;
	thirdScene = true;
	isFirstPerson = false;
	firstRail = false;
	thirdRail = false;
	down = true;
	jump = false;
	done = false;
	counter1st = 0;
	counter3rd = 0;
	playerY = 0;
	yDirection = 0;
	objectRot = 0;
	objectRotWheel = 0;
	fovy = 70;
	gravity = 1;
	runPlayer = 0;
	sndPlaySound(TEXT("Ending"), SND_ASYNC | SND_FILENAME | SND_LOOP);
	glutPostRedisplay();
}

void EndScene1(int value)
{
	endScene1 = true;
	if (arrows >= 5)
	{
		sndPlaySound(TEXT("Growl"), SND_ASYNC | SND_FILENAME);
		glutTimerFunc(10000, Reset2, 0);
		isWin = true;
	}
	else if(arrows < 5 || isGameOver)
	{
		glutTimerFunc(1000, Reset1, 0);
	}
}
void EndScene2(int value)
{
	endScene2 = true;
	if (keys >= 3 )
	{
		sndPlaySound(TEXT("OpenGate"), SND_ASYNC | SND_FILENAME);
		glutTimerFunc(10000, Reset3, 0);
	}
	else if (keys != 5 || isGameOver)
	{
		glutTimerFunc(1000, Reset2, 0);
	}
}
void EndScene3(int value)
{
	//glutTimerFunc(10000, Reset3, 0);
}

void drawPlayer()
{
	glPushMatrix();
	glTranslated(0.6, -0.05, 0.5);
	glRotated(90, 0, 1, 0);
	glScalef(0.0015, 0.002, 0.0015);
	model_archer.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.632, 0.13, 0.5);
	glRotated(90, 0, 1, 0);
	glRotated(90, 1, 0, 0);
	glRotated(-30, 0, 1, 0);
	glScalef(0.003, 0.006, 0.003);
	model_bow.Draw();
	glPopMatrix();

	//front
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_mine.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);
	glVertex3f( 0.53, 0.13, 0.58);
	glTexCoord2f(0, 1);
	glVertex3f( 0.53, 0.13, 0.42);
	glTexCoord2f(1, 1);
	glVertex3f( 0.56, -0.07, 0.45);
	glTexCoord2f(1, 0);
	glVertex3f( 0.56, -0.07,  0.54);
	glEnd();
	glPopMatrix();

	//back
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_mine.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);
	glVertex3f(0.70, 0.13, 0.58);
	glTexCoord2f(0, 1);
	glVertex3f(0.70, 0.13, 0.42);
	glTexCoord2f(1, 1);
	glVertex3f(0.67, -0.07, 0.45);
	glTexCoord2f(1, 0);
	glVertex3f(0.67, -0.07, 0.54);
	glEnd();
	glPopMatrix();


	//left
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_mine.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);
	glVertex3f(0.53, 0.13, 0.58);
	glTexCoord2f(0, 1);
	glVertex3f(0.70, 0.13, 0.58);
	glTexCoord2f(1, 1);
	glVertex3f(0.67, -0.07, 0.54);
	glTexCoord2f(1, 0);
	glVertex3f(0.56, -0.07, 0.54);
	glEnd();
	glPopMatrix();

	//right
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_mine.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);
	glVertex3f(0.53, 0.13, 0.42);
	glTexCoord2f(0, 1);
	glVertex3f(0.70, 0.13, 0.42);
	glTexCoord2f(1, 1);
	glVertex3f(0.67, -0.07, 0.45);
	glTexCoord2f(1, 0);
	glVertex3f(0.56, -0.07, 0.45);
	glEnd();
	glPopMatrix();

	//bottom
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_mine.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);
	glVertex3f(0.67, -0.07, 0.54);
	glTexCoord2f(1, 0);
	glVertex3f(0.56, -0.07, 0.54);
	glTexCoord2f(1, 1);
	glVertex3f(0.56, -0.07, 0.45);
	glTexCoord2f(1, 0);
	glVertex3f(0.67, -0.07, 0.45);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6, -0.1, 0.568);
	glRotated(90, 0, 1, 0);
	glRotated(-objectRotWheel, 1, 0, 0);
	glScalef(0.00009, 0.00009, 0.00012);
	model_wheel.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6, -0.1, 0.422);
	glRotated(90, 0, 1, 0);
	glRotated(-objectRotWheel, 1, 0, 0);
	glScalef(0.00009, 0.00009, 0.00012);
	model_wheel.Draw();
	glPopMatrix();

}

void setupCamera() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, 640 / 480, 0.001, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
	InitLightSource();
	InitMaterial();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void drawBarrier()
{
	glPushMatrix();
	glTranslated(0, -0.18, 0.115);
	glScalef(0.36, 0.25, 0.14);
	model_barrier.Draw();
	glPopMatrix();
}

void drawBarriers()
{
	glPushMatrix();
	glTranslated(-2.5, 0, 0.08);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.5, 0, 0.38);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5, 0, 0.08);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5, 0, 0.68);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-7.5, 0, 0.38);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-7.5, 0, 0.68);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-10, 0, 0.08);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-10, 0, 0.38);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-12.5, 0, 0.08);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-12.5, 0, 0.68);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-15, 0, 0.08);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-15, 0, 0.38);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-17.5, 0, 0.08);
	drawBarrier();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-17.5, 0, 0.68);
	drawBarrier();
	glPopMatrix();
}

void drawcoin()
{
	if (firstScene)
	{
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glTranslated(0, -0.1, 0);
		glRotated(objectRot, 0, 1, 0);
		glRotated(90, 1, 0, 0);
		glScalef(0.01, 0.01, 0.01);
		model_coins.Draw();
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}

	else if (secondScene)
	{
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glTranslated(0, -0.1, -0.01);
		glRotated(objectRot, 0, 1, 0);
		glRotated(90, 1, 0, 0);
		glScalef(0.01, 0.01, 0.01);
		model_coins.Draw();
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}
}

void drawcoins()
{
	if (firstScene)
	{
		if (!coinTaken[0])
		{
			glPushMatrix();
			glTranslated(-1, 0, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[1])
		{
			glPushMatrix();
			glTranslated(-2, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[2])
		{
			glPushMatrix();
			glTranslated(-3, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[3])
		{
			glPushMatrix();
			glTranslated(-4, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[4])
		{
			glPushMatrix();
			glTranslated(-5, 0.2, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[5])
		{
			glPushMatrix();
			glTranslated(-6, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[6])
		{
			glPushMatrix();
			glTranslated(-7, 0, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[7])
		{
			glPushMatrix();
			glTranslated(-8, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[8])
		{
			glPushMatrix();
			glTranslated(-9, 0, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[9])
		{
			glPushMatrix();
			glTranslated(-10, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[10])
		{
			glPushMatrix();
			glTranslated(-11, 0, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[11])
		{
			glPushMatrix();
			glTranslated(-12, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[12])
		{
			glPushMatrix();
			glTranslated(-13, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[13])
		{
			glPushMatrix();
			glTranslated(-14, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[14])
		{
			glPushMatrix();
			glTranslated(-15, 0.2, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[15])
		{
			glPushMatrix();
			glTranslated(-16, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[16])
		{
			glPushMatrix();
			glTranslated(-17.5, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}
	}
	
	else if (secondScene)
	{
		if (!coinTaken[17])
		{
			glPushMatrix();
			glTranslated(-1, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[18])
		{
			glPushMatrix();
			glTranslated(-2.5, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[19])
		{
			glPushMatrix();
			glTranslated(-3, 0, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[20])
		{
			glPushMatrix();
			glTranslated(-4, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[21])
		{
			glPushMatrix();
			glTranslated(-5, 0.2, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[22])
		{
			glPushMatrix();
			glTranslated(-6, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[23])
		{
			glPushMatrix();
			glTranslated(-7, 0, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[24])
		{
			glPushMatrix();
			glTranslated(-8, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[25])
		{
			glPushMatrix();
			glTranslated(-9, 0, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[26])
		{
			glPushMatrix();
			glTranslated(-10, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[27])
		{
			glPushMatrix();
			glTranslated(-11, 0, 0.2);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[28])
		{
			glPushMatrix();
			glTranslated(-12, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[29])
		{
			glPushMatrix();
			glTranslated(-13, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[30])
		{
			glPushMatrix();
			glTranslated(-14, 0, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[31])
		{
			glPushMatrix();
			glTranslated(-15, 0.2, 0.5);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[32])
		{
			glPushMatrix();
			glTranslated(-16, 0, 0.8);
			drawcoin();
			glPopMatrix();
		}if (!coinTaken[33])
		{
			glPushMatrix();
			glTranslated(-17, 0, 0.2);
			drawcoin();
			glPopMatrix();
		}
	}
}

void drawarrow()
{
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslated(0, -0.1, 0);
	glRotated(objectRot, 0, 1, 0);
	glScalef(0.005, 0.02, 0.005);
	model_arrow.Draw();
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void drawarrows()
{
	if (!arrowTaken[0])
	{
		glPushMatrix();
		glTranslated(-1.5, 0, 0.2);
		drawarrow();
		glPopMatrix();
	}if (!arrowTaken[1])
	{
		glPushMatrix();
		glTranslated(-3.5, 0, 0.2);
		drawarrow();
		glPopMatrix();
	}if (!arrowTaken[2])
	{
		glPushMatrix();
		glTranslated(-5, 0.2, 0.8);
		drawarrow();
		glPopMatrix();
	}if (!arrowTaken[3])
	{
		glPushMatrix();
		glTranslated(-7.5, 0.2, 0.5);
		drawarrow();
		glPopMatrix();
	}if (!arrowTaken[4])
	{
		glPushMatrix();
		glTranslated(-9.5, 0, 0.5);
		drawarrow();
		glPopMatrix();
	}if (!arrowTaken[5])
	{
		glPushMatrix();
		glTranslated(-10.5, 0, 0.2);
		drawarrow();
		glPopMatrix();
	}if (!arrowTaken[6])
	{
		glPushMatrix();
		glTranslated(-12.5, 0.2, 0.8);
		drawarrow();
		glPopMatrix();
	}if (!arrowTaken[7])
	{
		glPushMatrix();
		glTranslated(-15.5, 0, 0.2);
		drawarrow();
		glPopMatrix();
	}if (!arrowTaken[8])
	{
		glPushMatrix();
		glTranslated(-17, 0, 0.8);
		drawarrow();
		glPopMatrix();
	}
}

void drawKey()
{
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslated(0, -0.09, -0.01);
	glRotated(objectRot, 0, 1, 0);
	glRotated(-90, 0, 0, 1);
	glScalef(0.02, 0.02, 0.02);
	model_key.Draw();
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void drawKeys()
{
	if (!keyTaken[0])
	{
		glPushMatrix();
		glTranslated(-1.5, 0, 0.5);
		drawKey();
		glPopMatrix();
	}if (!keyTaken[1])
	{
		glPushMatrix();
		glTranslated(-4.5, 0, 0.8);
		drawKey();
		glPopMatrix();
	}if (!keyTaken[2])
	{
		glPushMatrix();
		glTranslated(-7.5, 0.2, 0.8);
		drawKey();
		glPopMatrix();
	}if (!keyTaken[3])
	{
		glPushMatrix();
		glTranslated(-10.5, 0, 0.5);
		drawKey();
		glPopMatrix();
	}if (!keyTaken[4])
	{
		glPushMatrix();
		glTranslated(-15, 0.2, 0.2);
		drawKey();
		glPopMatrix();
	}
}

void drawbarrier2()
{
	glPushMatrix();
	glTranslated(0, -0.17, 0.11);
	glRotated(22, 0, 1, 0);
	glScalef(0.0005, 0.00055, 0.00035);
	model_barrier2.Draw();
	glPopMatrix();
}

void drawbarrier2s()
{
	glPushMatrix();
	glTranslated(-2.5, 0, 0.08);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.5, 0, 0.38);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5, 0, 0.08);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5, 0, 0.68);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-7.5, 0, 0.38);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-7.5, 0, 0.68);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-10, 0, 0.08);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-10, 0, 0.38);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-12.5, 0, 0.08);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-12.5, 0, 0.68);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-15, 0, 0.08);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-15, 0, 0.38);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-17.5, 0, 0.08);
	drawbarrier2();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-17.5, 0, 0.68);
	drawbarrier2();
	glPopMatrix();
}

void drawGround(GLuint tex1, GLuint tex2, GLfloat s, GLfloat t, GLfloat q)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(11, 0, 0.03);
	glTexCoord2f(s, 0);
	glVertex3f(q, 0, 0.03);
	glTexCoord2f(s, t);
	glVertex3f(q, 0, 0.97);
	glTexCoord2f(0, t);
	glVertex3f(11, 0, 0.97);
	glEnd();
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D, tex2);
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(11, -0.001, -19);
	glTexCoord2f(6.2, 0);
	glVertex3f(-29, -0.001, -19);
	glTexCoord2f(6.2, 4.8);
	glVertex3f(-29, -0.001, 21);
	glTexCoord2f(0, 4.8);
	glVertex3f(11, -0.001, 21);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

void drawEnvironment()
{
	if (firstScene)
	{
		glPushMatrix();
		glTranslated(0, -0.18 + 0.0005, 0);
		drawGround(tex_rails1.texture[0], tex_ground1.texture[0], 15, 3.06f, -29);
		glPopMatrix();

		glPushMatrix();
		drawBarriers();
		glPopMatrix();

		glPushMatrix();
		drawcoins();
		glPopMatrix();

		glPushMatrix();
		drawarrows();
		glPopMatrix();
	}

	else if (secondScene)
	{
		glPushMatrix();
		glTranslated(0, -0.18 + 0.0005, 0);
		drawGround(tex_rails1.texture[0], tex_ground1.texture[0], 15, 3.06f, -29);
		glPopMatrix();

		glPushMatrix();
		drawbarrier2s();
		glPopMatrix();

		glPushMatrix();
		drawcoins();
		glPopMatrix();

		glPushMatrix();
		drawKeys();
		glPopMatrix();
	}

	else if(thirdScene)
	{
		glPushMatrix();
		glTranslated(0, -0.18 + 0.0005, 0);
		drawGround(tex_rails2.texture[0], tex_ground2.texture[0], 22, 1.0f, -1);
		glPopMatrix();
	}
}

void drawTrees()
{
	for (int i = 0; i < 8; i++)
	{
		glPushMatrix();
		glColor3d(1, 1, 1);
		glTranslated(treePos[i], -0.18, -0.2);
		glRotated(90, 0, 1, 0);
		glScalef(0.2, 0.2, 0.2);
		model_tree.Draw();
		glPopMatrix();

		glPushMatrix();
		glColor3d(1, 1, 1);
		glTranslated(treePos[i], -0.18, 1.2);
		glRotated(90, 0, 1, 0);
		glScalef(0.2, 0.2, 0.2);
		model_tree.Draw();
		glPopMatrix();
	}
	
}


void print(int x, int y, char* string)
{
	int len, i;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 0, 480, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2f(x, y);

	len = (int)strlen(string);

	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
}

void Display() {
	setupCamera();

	glEnable(GL_TEXTURE_2D);

	if (firstScene)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat lightIntensity[] = { scene1Intencity, scene1Intencity, scene1Intencity, 1.0f };
		GLfloat lightPosition[] = { -1 * (posPlayerX / 30), -0.17f, 0.5f, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

		glPushMatrix();
		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(-9, 0, 0);
		glRotated(180, 1, 0, 0);
		glRotated(119, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, tex_1);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 20, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();

		glPushMatrix();
		drawEnvironment();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-9, -0.17, 0.5);
		glRotated(-90, 0, 1, 0);
		glScalef(1.7, 1.7, 1.7);
		model_castle.Draw();
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(-18.5, -0.17, 0.5);
		glRotated(90, 0, 1, 0);		
		glScalef(deathscale, deathscale, deathscale);
		glScalef(0.0025, 0.0025, 0.0025);
		model_death.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-18.5, -0.17, 0.75);
		glRotated(90, 0, 1, 0);
		glScalef(deathscale, deathscale, deathscale);
		glScalef(0.0025, 0.0025, 0.0025);
		model_death.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-18.5, -0.17, 0.25);
		glRotated(90, 0, 1, 0);
		glScalef(deathscale, deathscale, deathscale);
		glScalef(0.0025, 0.0025, 0.0025);
		model_death.Draw();
		glPopMatrix();
	
		glPushMatrix();
		drawTrees();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, playerY, 0);
		glTranslated(posPlayerX, 0, posPlayerZ);
		drawPlayer();
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);
		char* p0s[20];
		sprintf((char*)p0s, "COINS: %d", coins1);
		print(50, 450, (char*)p0s);
		glColor3d(1, 1, 1);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);
		char* p1s[20];
		sprintf((char*)p1s, "HEARTS: %d", hearts);
		print(50, 420, (char*)p1s);
		glColor3d(1, 1, 1);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0, 1, 0);
		char* p2s[20];
		sprintf((char*)p2s, "ARROWS: %d", arrows);
		print(50, 390, (char*)p2s);
		glColor3d(1, 1, 1);
		glPopMatrix();
		//glEnable(GL_LIGHTING);

		if (!start)
		{
			glDisable(GL_LIGHTING);
			glPushMatrix();
			glColor3f(1, 0, 0);
			char* p3s[20];
			sprintf((char*)p3s, "Press J To Start The Game");
			print(275, 50, (char*)p3s);
			glColor3d(1, 1, 1);
			glPopMatrix();
			glEnable(GL_LIGHTING);
		}
		if (paused)
		{
			glDisable(GL_LIGHTING);
			glPushMatrix();
			glColor3f(1, 0, 0);
			char* p3s[20];
			sprintf((char*)p3s, "Paused");
			print(308, 120, (char*)p3s);
			glColor3d(1, 1, 1);
			glPopMatrix();
			glEnable(GL_LIGHTING);
			
		}
	}

	else if (secondScene)
	{ 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat lightIntensity[] = { scene2Intencity, scene2Intencity, scene2Intencity, 1.0f };
		GLfloat lightPosition[] = { -1 * (posPlayerX / 30), -0.17f, 0.5f, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

		glPushMatrix();
		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(-9, 0, 0);
		glRotated(180, 1, 0, 0);
		glRotated(119, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, tex_1);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 20, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();

		glPushMatrix();
		drawEnvironment();
		glPopMatrix();

		glPushMatrix();
		glTranslated(13.7, -0.17, 0.5);
		glRotated(-90, 0, 1, 0);
		glScalef(1.7, 1.7, 1.7);
		model_castle.Draw();
		glPopMatrix();

		glPushMatrix();
		drawTrees();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-18.5, -0.17, 0.47);
		glRotated(90, 0, 1, 0);
		glScalef(0.2, 0.3, 0.2);
		model_treasure.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, playerY, 0);
		glTranslated(posPlayerX, 0, posPlayerZ);
		drawPlayer();
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 0, 0);
		char* p0s[20];
		sprintf((char*)p0s, "COINS: %d", coins1+coins2);
		print(50, 450, (char*)p0s);
		glColor3d(1, 1, 1);
		glPopMatrix();
	
		glPushMatrix();
		glColor3f(1, 0, 0);
		char* p1s[20];
		sprintf((char*)p1s, "HEARTS: %d", hearts);
		print(50, 420, (char*)p1s);
		glColor3d(1, 1, 1);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 0, 0);
		char* p2s[20];
		sprintf((char*)p2s, "KEYS: %d", keys);
		print(50, 390, (char*)p2s);
		glColor3d(1, 1, 1);
		glPopMatrix();

		if (paused)
		{
			glDisable(GL_LIGHTING);
			glPushMatrix();
			glColor3f(1, 0, 0);
			char* p3s[20];
			sprintf((char*)p3s, "Paused");
			print(308, 50, (char*)p3s);
			glColor3d(1, 1, 1);
			glPopMatrix();
			glEnable(GL_LIGHTING);
		 }
	}

	else if (thirdScene)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat lightIntensity[] = { scene3Intencity, scene3Intencity, scene3Intencity, 1.0f };
		GLfloat lightPosition[] = { -9, -0.17f, 0.5f, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

		glPushMatrix();
		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(-9, 0, 0);
		glRotated(180, 0, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex_2);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 20, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();

		glPushMatrix();
		drawEnvironment();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-1.8, 0.45, 1.4);
		glScalef(0.0018 , 0.0024, 0.0018);
		model_yacht.Draw();
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(-1, -0.15, -0.5);
		glRotated(90, 1, 0, 0);
		glRotated(55, 0, 0, 1);
		glScalef(0.007, 0.011, 0.007);
		model_yacht2.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.5, -0.17, 0.50);
		glRotated(90, 0, 1, 0);
		glScalef(0.1, 0.2, 0.15);
		model_treasure.Draw();
		glPopMatrix();



		glPushMatrix();
		glTranslated(0.6, -0.05, 0.5);
		glRotated(90, 0, 1, 0);
		glScalef(0.0015, 0.002, 0.0015);
		model_archer.Draw();
		glPopMatrix();
	}

	glutSwapBuffers();
}

void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0 && !thirdScene)
	{
		if (fov)
		{
			fovy--;
		}
		else
		{
			camera.moveZ(0.01);
		}
	}
	else if (cameraZoom - y <= 0 && !thirdScene)
	{
		if (fov)
		{
			fovy++;
		}
		else
		{
			camera.moveZ(-0.01);
		}
	}

	cameraZoom = y;

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	std::cout << x + " ";
	y = HEIGHT - y;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && !thirdScene)
	{
		fov = true;
		cameraZoom = y;
	}else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !thirdScene)
	{
		fov = false;
		cameraZoom = y;
	}
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;
	float a = 1.0;

	switch (key) {
	case 'p':
		if (!thirdScene && start)
		{
			paused = !paused;
		}
		break;
	case 'j':
		if (!start && firstScene)
		{
			start = true;
			scene1Intencity = -0.05;
		}
		break;
	
	case 'c':
		speed -= 1;
		break;
	case 'x':
		speed += 1;
		break;
	case 'e':
		if (!thirdScene)
			camera.moveY(d);
		break;
	case 'q':
		if (!thirdScene)
			camera.moveY(-d);
		break;
	case 'a':
		if (!thirdScene)
			camera.moveX(d);
		break;
	case 'd':
		if (!thirdScene)
			camera.moveX(-d);
		break;
	case 'w':
		if (!thirdScene)
			camera.moveZ(d);
		break;
	case 's':
		if (!thirdScene)
			camera.moveZ(-d);
		break;
	case 'z':
		if (!isFirstPerson && !thirdScene)
		{
			fovy = 70;
	
			camera.eye.x = 0.467848f;
			camera.eye.y = 0.211558f;
			camera.eye.z = 0.5f;
			camera.center.x = -0.470065f;
			camera.center.y = -0.13532f;
			camera.center.z = 0.5f;
			camera.up.x = -0.346878f;
			camera.up.y = 0.937911f;
			camera.up.z = 0.0f;
			if (firstRail)
			{
				camera.moveX(-0.3);
			}
			if (thirdRail)
			{
				camera.moveX(0.3);
			}
		
			camera.moveZ(0.0008065 * speed * (counter1st + counter3rd));
			camera.moveY(0.0002985 * speed * (counter1st + counter3rd));
			isFirstPerson = true;
		}

		else if (isFirstPerson && !thirdScene)
		{
			fovy = 70;
			camera.eye.x = 0.996158f;
			camera.eye.y = 0.310991f;
			camera.eye.z = 0.5f;
			camera.center.x = 0.0582472f;
			camera.center.y = -0.0358866f;
			camera.center.z = 0.5f;
			camera.up.x = -0.346878f;
			camera.up.y = 0.937911f;
			camera.up.z = 0.0f;
			if (firstRail)
			{
				camera.moveX(-0.3);
			}
			if (thirdRail)
			{
				camera.moveX(0.3);
			}
			//////////////////////////////////////////////////////////////////////////////////////////////
			camera.moveZ(0.0008 * speed * (counter1st + counter3rd));
			camera.moveY(0.0003 * speed * (counter1st + counter3rd));
			isFirstPerson = false;
		}
		break;
	case 't':
		if (!thirdScene)
			camera.rotateX(a);
		break;
	case 'g':
		if (!thirdScene)
			camera.rotateX(-a);
		break;
	case 'f':
		if (!thirdScene)
			camera.rotateY(a);
		break;
	case 'h':
		if (!thirdScene)
			camera.rotateY(-a);
		break;
	case ' ':
		if (!jump && !thirdScene)
		{
			jump = true;
			sndPlaySound(TEXT("Jump"), SND_ASYNC | SND_FILENAME);
			glutTimerFunc(1000, Launch, 0);
		}
		break;
	case 'r':
		if (!thirdScene)
			if (0.5 + posPlayerX < -17.8)
		{
			if (firstScene)
			{
				kill = true;
			}
			else if (secondScene)
			{
				open = true;
			}
		}
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void Special(int key, int x, int y) {
	float a = 1.0;
	switch (key) {
	case GLUT_KEY_LEFT:
		if (0.5 + posPlayerX >= -17.8 && !thirdScene)
		{
			if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
			{
				posPlayerZ += 0.3;
				camera.moveX(0.3);
				thirdRail = true;
				firstRail = false;
			}
			else if (0.5 + posPlayerZ <= 0.35 && 0.5 + posPlayerZ >= 0.05)
			{
				posPlayerZ += 0.3;
				camera.moveX(0.3);
				thirdRail = false;
				firstRail = false;
			}
		}
		break;
	case GLUT_KEY_RIGHT:
		if (0.5 + posPlayerX >= -17.8 && !thirdScene)
		{
			if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
			{
				posPlayerZ -= 0.3;
				camera.moveX(-0.3);
				firstRail = true;
				thirdRail = false;
			}
			else if (0.5 + posPlayerZ <= 0.95 && 0.5 + posPlayerZ >= 0.65)
			{
				posPlayerZ -= 0.3;
				camera.moveX(-0.3);
				thirdRail = false;
				firstRail = false;
			}
		}
		break;
	}
	glutPostRedisplay();
}

void Update()
{
	if (hearts == 0)
	{
		if (firstScene)
		{
			glutTimerFunc(0, Reset1, 0);
		}
		else if (secondScene)
		{
			glutTimerFunc(0, Reset2, 0);
		}
	}

	if (firstScene && start && !paused)
	{
		if (!endScene1)
		{
			objectRot += 5 * (speed / 20);
			if (posPlayerX > -17.8) {
				objectRotWheel += 5 * (speed / 20);
			}
			scene1Intencity += 0.00001;
			if ((0.5 + posPlayerX < -1.45 && 0.5 + posPlayerX > -1.55) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !arrowTaken[0])
			{
				arrowTaken[0] = true;
				arrows++;
				sndPlaySound(TEXT("Takearrow"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -3.45 && 0.5 + posPlayerX > -3.55) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !arrowTaken[1])
			{
				arrowTaken[1] = true;
				arrows++;
				sndPlaySound(TEXT("Takearrow"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -4.95 && 0.5 + posPlayerX > -5.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY > 0.1) && !arrowTaken[2])
			{
				arrowTaken[2] = true;
				arrows++;
				sndPlaySound(TEXT("Takearrow"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -7.45 && 0.5 + posPlayerX > -7.55) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY > 0.1) && !arrowTaken[3])
			{
				arrowTaken[3] = true;
				arrows++;
				sndPlaySound(TEXT("Takearrow"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -9.45 && 0.5 + posPlayerX > -9.55) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0.1) && !arrowTaken[4])
			{
				arrowTaken[4] = true;
				arrows++;
				sndPlaySound(TEXT("Takearrow"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -10.45 && 0.5 + posPlayerX > -10.55) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !arrowTaken[5])
			{
				arrowTaken[5] = true;
				arrows++;
				sndPlaySound(TEXT("Takearrow"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -12.45 && 0.5 + posPlayerX > -12.55) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY > 0.1) && !arrowTaken[6])
			{
				arrowTaken[6] = true;
				arrows++;
				sndPlaySound(TEXT("Takearrow"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -15.45 && 0.5 + posPlayerX > -15.55) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !arrowTaken[7])
			{
				arrowTaken[7] = true;
				arrows++;
				sndPlaySound(TEXT("Takearrow"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -16.95 && 0.5 + posPlayerX > -17.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !arrowTaken[8])
			{
				arrowTaken[8] = true;
				arrows++;
				sndPlaySound(TEXT("Takearrow"), SND_ASYNC | SND_FILENAME);
			}
			//=======================================================================================================//
			//=======================================================================================================//
			if ((0.5 + posPlayerX < -0.95 && 0.5 + posPlayerX > -1.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !coinTaken[0])
			{
				coinTaken[0] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -1.95 && 0.5 + posPlayerX > -2.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[1])
			{
				coinTaken[1] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -2.95 && 0.5 + posPlayerX > -3.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[2])
			{
				coinTaken[2] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -3.95 && 0.5 + posPlayerX > -4.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[3])
			{
				coinTaken[3] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -4.95 && 0.5 + posPlayerX > -5.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY > 0.1) && !coinTaken[4])
			{
				coinTaken[4] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -5.95 && 0.5 + posPlayerX > -6.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[5])
			{
				coinTaken[5] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -6.95 && 0.5 + posPlayerX > -7.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !coinTaken[6])
			{
				coinTaken[6] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -7.95 && 0.5 + posPlayerX > -8.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[7])
			{
				coinTaken[7] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -8.95 && 0.5 + posPlayerX > -9.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !coinTaken[8])
			{
				coinTaken[8] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -9.95 && 0.5 + posPlayerX > -10.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[9])
			{
				coinTaken[9] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -10.95 && 0.5 + posPlayerX > -11.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !coinTaken[10])
			{
				coinTaken[10] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -11.95 && 0.5 + posPlayerX > -12.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[11])
			{
				coinTaken[11] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -12.95 && 0.5 + posPlayerX > -13.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[12])
			{
				coinTaken[12] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -13.95 && 0.5 + posPlayerX > -14.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[13])
			{
				coinTaken[13] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -14.95 && 0.5 + posPlayerX > -15.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY > 0.1) && !coinTaken[14])
			{
				coinTaken[14] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -15.95 && 0.5 + posPlayerX > -16.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[15])
			{
				coinTaken[15] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -17.45 && 0.5 + posPlayerX > -17.55) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[16])
			{
				coinTaken[16] = true;
				coins1++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			//=======================================================================================================//
			//=======================================================================================================//
			if (0.5 + posPlayerX < -2.35 - 0.025 && 0.5 + posPlayerX > -2.5 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.95 && 0.5 + posPlayerZ >= 0.65))
			{
				sndPlaySound(TEXT("Ugh"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35 && 0.5 + posPlayerZ >= 0.05)
				{
					posPlayerZ += 0.6;
					camera.moveX(0.6);
				}
				else if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				thirdRail = true;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -4.85 - 0.025 && 0.5 + posPlayerX > -5.0 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35))
			{
				sndPlaySound(TEXT("Ugh"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				else
				{
					posPlayerZ -= 0.3;
					camera.moveX(-0.3);
				}
				thirdRail = false;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -7.35 - 0.025 && 0.5 + posPlayerX > -7.5 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.35 && 0.5 + posPlayerZ >= 0.05))
			{
				sndPlaySound(TEXT("Ugh"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.95 && 0.5 + posPlayerZ >= 0.65)
				{
					posPlayerZ -= 0.6;
					camera.moveX(-0.6);
				}
				else if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
				{
					posPlayerZ -= 0.3;
					camera.moveX(-0.3);
				}
				thirdRail = false;
				firstRail = true;
			}
			else if (0.5 + posPlayerX < -9.85 - 0.025 && 0.5 + posPlayerX > -10.0 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.95 && 0.5 + posPlayerZ >= 0.65))
			{
				sndPlaySound(TEXT("Ugh"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35 && 0.5 + posPlayerZ >= 0.05)
				{
					posPlayerZ += 0.6;
					camera.moveX(0.6);
				}
				else if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				thirdRail = true;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -12.35 - 0.025 && 0.5 + posPlayerX > -12.5 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35))
			{
				sndPlaySound(TEXT("Ugh"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				else
				{
					posPlayerZ -= 0.3;
					camera.moveX(-0.3);
				}
				thirdRail = false;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -14.85 - 0.025 && 0.5 + posPlayerX > -15.0 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.95 && 0.5 + posPlayerZ >= 0.65))
			{
				sndPlaySound(TEXT("Ugh"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35 && 0.5 + posPlayerZ >= 0.05)
				{
					posPlayerZ += 0.6;
					camera.moveX(0.6);
				}
				else if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				thirdRail = true;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -17.35 - 0.025 && 0.5 + posPlayerX > -17.5 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35))
			{
				sndPlaySound(TEXT("Ugh"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				else
				{
					posPlayerZ -= 0.3;
					camera.moveX(-0.3);
				}
				thirdRail = false;
				firstRail = false;
			}
			else if (0.5 + posPlayerX >= -17.8)
			{
				runPlayer = 1;
				posPlayerX -= 0.00086 * speed;
				
				if (isFirstPerson)
				{
					camera.moveZ(0.0008065 * speed);
					camera.moveY(0.0002985 * speed);
					camera.moveY(0.01 * yDirection * gravity * (speed / 20));
					counter1st++;
			
				
				}
				else
				{
					camera.moveZ(0.0008 * speed);
					camera.moveY(0.0003 * speed);
					counter3rd++;
				}

				if (jump)
				{
					if (playerY > 0.25)
					{
						down = false;
						yDirection = -1;
					}
					else if (playerY <= 0.25 && down)
					{
						yDirection = 1;
					}
					else if (playerY <= 0 && !down)
					{
						yDirection = 0;
						down = true;
						jump = false;
						gravity = 1;
					}
					gravity += yDirection * 0.01;
					playerY += 0.01 * yDirection * gravity * (speed / 20);
				}
			}
			else if (0.5 + posPlayerX < -17.8)
			{
				thirdRail = false;
				firstRail = false;
				if (0.5 + posPlayerZ > 0.65)
				{
					camera.moveX(-0.3);
				}
				else if (0.5 + posPlayerZ < 0.35)
				{
					camera.moveX(0.3);
				}
				playerY = 0;
				posPlayerZ = 0;
				runPlayer = 0;
				if (kill )
				{
					if (!KillWitharrow)
					{
						KillWitharrow = sndPlaySound(TEXT("KillWitharrow"), SND_ASYNC | SND_FILENAME);
						glutTimerFunc(2800, EndScene1, 0);
					}
					arrowPosX += 0.1 * (speed / 20);
				}
			}
		}
		
		else if (endScene1)
		{
		if (deathscale >= 0 && isWin)
		{
			deathscale -= 0.05 * (speed / 20);
		}
		if (scene1Intencity != -0.55)
		{
			scene1Intencity -= 0.01 * (speed / 20);
		}
		}
	}

	else if (secondScene && !paused)
	{
		if (!endScene2)
		{
			objectRot += 5 * (speed / 20);
			if (posPlayerX > -17.8) {
				objectRotWheel += 5 * (speed / 20);
			}
			if ((0.5 + posPlayerX < -1.45 && 0.5 + posPlayerX > -1.55) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !keyTaken[0])
			{
				sndPlaySound(TEXT("TakeKey"), SND_ASYNC | SND_FILENAME);
				keyTaken[0] = true;
				keys++;
			}
			else if ((0.5 + posPlayerX < -4.45 && 0.5 + posPlayerX > -4.55) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !keyTaken[1])
			{
				sndPlaySound(TEXT("TakeKey"), SND_ASYNC | SND_FILENAME);
				keyTaken[1] = true;
				keys++;
			}
			else if ((0.5 + posPlayerX < -7.45 && 0.5 + posPlayerX > -7.55) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY > 0.1) && !keyTaken[2])
			{
				sndPlaySound(TEXT("TakeKey"), SND_ASYNC | SND_FILENAME);
				keyTaken[2] = true;
				keys++;
			}
			else if ((0.5 + posPlayerX < -10.45 && 0.5 + posPlayerX > -10.55) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !keyTaken[3])
			{
				sndPlaySound(TEXT("TakeKey"), SND_ASYNC | SND_FILENAME);
				keyTaken[3] = true;
				keys++;
			}
			else if ((0.5 + posPlayerX < -14.95 && 0.5 + posPlayerX > -15.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY > 0.1) && !keyTaken[4])
			{
				sndPlaySound(TEXT("TakeKey"), SND_ASYNC | SND_FILENAME);
				keyTaken[4] = true;
				keys++;
			}
			//=======================================================================================================//
			//=======================================================================================================//
			if ((0.5 + posPlayerX < -0.95 && 0.5 + posPlayerX > -1.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[17])
			{
				coinTaken[17] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -2.45 && 0.5 + posPlayerX > -2.55) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[18])
			{
				coinTaken[18] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -2.95 && 0.5 + posPlayerX > -3.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !coinTaken[19])
			{
				coinTaken[19] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -3.95 && 0.5 + posPlayerX > -4.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[20])
			{
				coinTaken[20] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -4.95 && 0.5 + posPlayerX > -5.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY > 0.1) && !coinTaken[21])
			{
				coinTaken[21] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -5.95 && 0.5 + posPlayerX > -6.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[22])
			{
				coinTaken[22] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -6.95 && 0.5 + posPlayerX > -7.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !coinTaken[23])
			{
				coinTaken[23] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -7.95 && 0.5 + posPlayerX > -8.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[24])
			{
				coinTaken[24] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -8.95 && 0.5 + posPlayerX > -9.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !coinTaken[25])
			{
				coinTaken[25] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -9.95 && 0.5 + posPlayerX > -10.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[26])
			{
				coinTaken[26] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -10.95 && 0.5 + posPlayerX > -11.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !coinTaken[27])
			{
				coinTaken[27] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -11.95 && 0.5 + posPlayerX > -12.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[28])
			{
				coinTaken[28] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -12.95 && 0.5 + posPlayerX > -13.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[29])
			{
				coinTaken[29] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -13.95 && 0.5 + posPlayerX > -14.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY <= 0) && !coinTaken[30])
			{
				coinTaken[30] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -14.95 && 0.5 + posPlayerX > -15.05) && (0.5 + posPlayerZ < 0.65 && 0.5 + posPlayerZ > 0.35) && (playerY > 0.1) && !coinTaken[31])
			{
				coinTaken[31] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -15.95 && 0.5 + posPlayerX > -16.05) && (0.5 + posPlayerZ < 0.95 && 0.5 + posPlayerZ > 0.65) && (playerY <= 0) && !coinTaken[32])
			{
				coinTaken[32] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			else if ((0.5 + posPlayerX < -16.95 && 0.5 + posPlayerX > -17.05) && (0.5 + posPlayerZ < 0.35 && 0.5 + posPlayerZ > 0.05) && (playerY <= 0) && !coinTaken[33])
			{
				coinTaken[33] = true;
				coins2++;
				sndPlaySound(TEXT("TakeCoin"), SND_ASYNC | SND_FILENAME);
			}
			//=======================================================================================================//
			//=======================================================================================================//
			if (0.5 + posPlayerX < -2.35 - 0.025 && 0.5 + posPlayerX > -2.5 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.95 && 0.5 + posPlayerZ >= 0.65))
			{
				sndPlaySound(TEXT("Dino"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35 && 0.5 + posPlayerZ >= 0.05)
				{
					posPlayerZ += 0.6;
					camera.moveX(0.6);
				}
				else if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				thirdRail = true;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -4.85 - 0.025 && 0.5 + posPlayerX > -5.0 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35))
			{
				sndPlaySound(TEXT("Dino"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				else
				{
					posPlayerZ -= 0.3;
					camera.moveX(-0.3);
				}
				thirdRail = false;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -7.35 - 0.025 && 0.5 + posPlayerX > -7.5 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.35 && 0.5 + posPlayerZ >= 0.05))
			{
				sndPlaySound(TEXT("Dino"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.95 && 0.5 + posPlayerZ >= 0.65)
				{
					posPlayerZ -= 0.6;
					camera.moveX(-0.6);
				}
				else if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
				{
					posPlayerZ -= 0.3;
					camera.moveX(-0.3);
				}
				thirdRail = false;
				firstRail = true;
			}
			else if (0.5 + posPlayerX < -9.85 - 0.025 && 0.5 + posPlayerX > -10.0 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.95 && 0.5 + posPlayerZ >= 0.65))
			{
				sndPlaySound(TEXT("Dino"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35 && 0.5 + posPlayerZ >= 0.05)
				{
					posPlayerZ += 0.6;
					camera.moveX(0.6);
				}
				else if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				thirdRail = true;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -12.35 - 0.025 && 0.5 + posPlayerX > -12.5 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35))
			{
				sndPlaySound(TEXT("Dino"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				else
				{
					posPlayerZ -= 0.3;
					camera.moveX(-0.3);
				}
				thirdRail = false;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -14.85 - 0.025 && 0.5 + posPlayerX > -15.0 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.95 && 0.5 + posPlayerZ >= 0.65))
			{
				sndPlaySound(TEXT("Dino"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35 && 0.5 + posPlayerZ >= 0.05)
				{
					posPlayerZ += 0.6;
					camera.moveX(0.6);
				}
				else if (0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				thirdRail = true;
				firstRail = false;
			}
			else if (0.5 + posPlayerX < -17.35 - 0.025 && 0.5 + posPlayerX > -17.5 && playerY < 0.1 && !(0.5 + posPlayerZ <= 0.65 && 0.5 + posPlayerZ >= 0.35))
			{
				sndPlaySound(TEXT("Dino"), SND_ASYNC | SND_FILENAME);
				hearts--;
				if (0.5 + posPlayerZ <= 0.35)
				{
					posPlayerZ += 0.3;
					camera.moveX(0.3);
				}
				else
				{
					posPlayerZ -= 0.3;
					camera.moveX(-0.3);
				}
				thirdRail = false;
				firstRail = false;
			}
			else if (0.5 + posPlayerX >= -17.8)
			{
				runPlayer = 1;
				posPlayerX -= 0.00086 * speed;
				
				if (isFirstPerson)
				{
			
					camera.moveZ(0.0008065 * speed);
					camera.moveY(0.0002985 * speed);
					camera.moveY(0.02 * yDirection * gravity * (speed / 20));
					counter1st++;
				}
				else
				{
					camera.moveZ(0.0008 * speed);
					camera.moveY(0.0003 * speed);
					counter3rd++;
				}

				if (jump)
				{
					if (playerY > 0.25)
					{
						down = false;
						yDirection = -1;
					}
					else if (playerY <= 0.25 && down)
					{
						yDirection = 1;
					}
					else if (playerY <= 0 && !down)
					{
						yDirection = 0;
						down = true;
						jump = false;
						gravity = 1;
					}
					gravity += yDirection * 0.01;
					playerY += 0.01 * yDirection * gravity * (speed / 20);
				}
			}
			else if (0.5 + posPlayerX < -17.8)
			{
				thirdRail = false;
				firstRail = false;
				if (0.5 + posPlayerZ > 0.65)
				{
					camera.moveX(-0.3);
				}
				else if (0.5 + posPlayerZ < 0.35)
				{
					camera.moveX(0.3);
				}
				playerY = 0;
				posPlayerZ = 0;
				runPlayer = 0;
				if (open)
				{
					glutTimerFunc(0, EndScene2, 0);
				}
			}
		}

		else if (endScene2)
		{
			if (scene2Intencity != -0.55)
			{
				scene2Intencity -= 0.01 * (speed / 20);
			}
		}
	}

	else if (thirdScene)
	{
		camera.moveZ(-0.00008);
		if (scene3Intencity != -0.55)
		{
			scene3Intencity -= 0.00008;
		}
	}

	if (0.5 + posPlayerX < -17.8 && isFirstPerson && !done)
	{
		done = true;
		camera.rotateX(20);
	}
	glutPostRedisplay();
}

void LoadAssets()
{
	//model_death.Load("Models/death/Katarnbeta Star wars N200713.3ds");
	model_death.Load("Models/death/death.3ds");
	model_archer.Load("Models/archer/Man N060215.3DS");
	model_bow.Load("Models/bow/Bow.3ds");
	model_wheel.Load("Models/wheel/Wheel car tire N260322.3ds");
	model_treasure.Load("Models/treasure/Trunk.3DS");
	model_arrow.Load("Models/arrow/Arrow.3ds");
	model_yacht.Load("Models/yacht/Yacht adastra N270616.3ds");
	model_yacht2.Load("Models/yacht2/Boat.3ds");



	model_coins.Load("Models/Coin/Coin.3ds");
	model_tree.Load("Models/Tree/firtree3.3ds");
	model_barrier.Load("Models/Barrier/cmt_barrier.3ds");
	model_castle.Load("Models/Castle/Castle.3ds");
	model_barrier2.Load("Models/barrier2/Fence N210814.3ds");
	model_key.Load("Models/Key_B_02.3ds/Key_B_02.3ds");

	// Loading texture files
	tex_rails1.Load("Textures/Rails.bmp");
	tex_mine.Load("Textures/mine.bmp");
	tex_ground1.Load("Textures/Ground_1.bmp");
	tex_rails2.Load("Textures/Beach.bmp");
	tex_ground2.Load("Textures/Water_0.bmp");

	loadBMP(&tex_1, "Textures/Sky_1.bmp", true);
	loadBMP(&tex_2, "Textures/Sky_2.bmp", true);
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Treasure Hunt");
	glutDisplayFunc(Display);
	glutMotionFunc(myMotion);
	glutIdleFunc(Update);
	glutMouseFunc(mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}