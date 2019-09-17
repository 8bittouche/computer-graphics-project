
//#pragma comment(lib, "legacy_stdio_definitions.lib")
#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <gl/Glaux.h>

void init();       // ����� �ʱ�ȭ �Լ�
void draw();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void sub_menu_function(int option);
void sub_menu_function2(int option);
void sub_menu_function3(int option);
void sub_menu_function4(int option);
void sub_menu_function5(int option);
void main_menu_function(int option);

void resize(int width, int height);
void draw_axis();
void idle();
void specialkeyboard(int key, int x, int y);

void back_Mode_func();
void handle_func();
void mode_Angle_idle();
void canopy_Angle_idle();

#define PI 3.141592  // ������ PI

// �� ��ǥ�� ��ǥ 
double distance = 120;
double azimuth = 0;
double elevation = 0;

// ī�޶� ������ ��ǥ
double eyex = 0;
double eyey = 0;
double eyez = 120;

// ī�޶� up���� ��� �� �ʿ��� y���� ��
double upy = 0;


// GLUQuadic ��ü ����
GLUquadricObj* m_pQuadric;

// clipping plane
float cx = 0;
float cy = 1;
float cz = 0;
GLdouble eq[4] = { cx, cy, cz, 0 };   // clipping plane�� normal ���� = (cx, cy, cz)


// Texture Mapping
AUX_RGBImageRec * pRGBImage;
GLuint wheel[2];
GLuint bodyColor;
GLuint mesh;
GLuint frame;
GLuint handle_texture;




/*** variables for interactive function ***/
// ���� ȸ��
boolean wheelRotation_On = false;
int wheelRotation_Angle = 0;

// ��ü ȸ��
boolean bodyRotation_On = false;
int bodyRotation_Angle = 0;

// ����� ���� ����. 0�ܰ� ~ 4�ܰ�
boolean back_Mode[5] = { true, false, false, false, false };
int back_Angle = 0;

// ������ ���� ����. 1�� ~ 3��
#define HANDLE_SIZE  5 
boolean handle[3] = { true, false, false };
int handle_2 = 0;   // 2�� 
int handle_3 = 0;   // 3��.   �⺻�� 1��.

// ������ ���� ����
int handle_Angle = 0;

// ĳ���� ����. 0�ܰ� ~ 3�ܰ�
boolean canopy[4] = { true, false, false, false };
int canopy_Angle0 = 0;
int canopy_Angle1 = 0;
int canopy_Angle2 = 0;

// ĳ���� ���� ����.
int canopy_Angle3 = 0;

// ���� ���� ������ ���
boolean cradle_Mode = true;    // ���� ���
boolean stoller_Mode = false;  // ������ ���
int mode_Angle = 0;

int main(int argc, char** argv)
{
	int submenu1;     // sub menu identifier
	int submenu2;
	int submenu3;
	int submenu4;
	int submenu5;

	// Window �ʱ�ȭ
	glutInit(&argc, argv);    // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  // ���÷��� ��� ����
	glutInitWindowSize(500, 500);                 // â ũ�� �ʱ�ȭ 
	glutInitWindowPosition(300, 300);             // â ��ġ �ʱ�ȭ
	glutCreateWindow("12121696 ������ �̴�������Ʈ");      // â ����. â ���� ����.
	init();     // �ʱ�ȭ �Լ�

	// Popup menu ���� �� �߰�

	submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("0�ܰ�", 0);
	glutAddMenuEntry("1�ܰ�", 1);
	glutAddMenuEntry("2�ܰ�", 2);
	glutAddMenuEntry("3�ܰ�", 3);
	glutAddMenuEntry("4�ܰ�", 4);

	submenu2 = glutCreateMenu(sub_menu_function2);
	glutAddMenuEntry("1�ܰ�", 1);
	glutAddMenuEntry("2�ܰ�", 2);
	glutAddMenuEntry("3�ܰ�", 3);

	submenu3 = glutCreateMenu(sub_menu_function3);
	glutAddMenuEntry("0�ܰ�", 0);
	glutAddMenuEntry("1�ܰ�", 1);
	glutAddMenuEntry("2�ܰ�", 2);
	glutAddMenuEntry("3�ܰ�", 3);


	submenu4 = glutCreateMenu(sub_menu_function4);
	glutAddMenuEntry("Cyan", 1);
	glutAddMenuEntry("Dark_blue", 2);
	glutAddMenuEntry("Purple", 3);
	glutAddMenuEntry("Gold", 4);

	submenu5 = glutCreateMenu(sub_menu_function5);
	glutAddMenuEntry("���� ���(cradle)", 1);
	glutAddMenuEntry("������ ���(stoller)", 2);



	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("���� ȸ��", 1);
	glutAddMenuEntry("��ü ȸ��", 2);
	glutAddSubMenu("����� ���� ����", submenu1);
	glutAddSubMenu("������ ���� ����", submenu2);
	glutAddSubMenu("ĳ���� ����", submenu3);
	glutAddSubMenu("���� ����", submenu4);
	glutAddSubMenu("��� ����", submenu5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	// Callback �Լ� ����
	glutDisplayFunc(draw);       // ȭ�� ����Լ��� draw ����  
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	glutIdleFunc(idle);

	// Looping ����
	glutMainLoop();     // event processing ����


	return 0;
}


void init()
{
	// ȭ�� �⺻�� ����
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	
	// texture mapping
	glGenTextures(2, wheel);
	glGenTextures(1, &bodyColor);
	glGenTextures(1, &mesh);
	glGenTextures(1, &frame);
	glGenTextures(1, &handle_texture);


	glBindTexture(GL_TEXTURE_2D, wheel[0]);
	pRGBImage = auxDIBImageLoadA("wheel.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
		GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, wheel[1]);
	pRGBImage = auxDIBImageLoadA("wheel_side.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
	GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, frame);
	pRGBImage = auxDIBImageLoadA("frame.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
		GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, mesh);
	pRGBImage = auxDIBImageLoadA("mesh.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
		GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, bodyColor);
	pRGBImage = auxDIBImageLoadA("cyan.bmp");  // �ʱ� color
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
		GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, handle_texture);
	pRGBImage = auxDIBImageLoadA("handle.bmp");  // �ʱ� color
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
		GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   // Polygon�� ���� ������ �����ϰ� texture�� ����
	glEnable(GL_TEXTURE_2D);


	// LIGHT0 ����
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING);  // LIGHTING MODE Ȱ��ȭ
	glEnable(GL_LIGHT0);    // LIGHT0 Ȱ��ȭ

	GLfloat ambient1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


	m_pQuadric = gluNewQuadric();  // GLUQuadric ��ü ����
	gluQuadricTexture(m_pQuadric, GL_TRUE);
	glutReshapeFunc(resize);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // color ���۸� ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);


	upy = distance / cos(elevation * PI / 180);
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, -eyex, upy - eyey, -eyez);


	gluQuadricDrawStyle(m_pQuadric, GLU_FILL);

	//draw_axis();
	//glColor3f(1.0, 1.0, 1.0);



	glPushMatrix();  // ��ü ȸ���� ���� glPushMatrix()
	glRotatef(-bodyRotation_Angle, 0, 1, 0);   /*** ��ü ȸ�� ***/
   // ************ ��ü �� ������ ************
   // ������ (x�� ����)
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, frame);
		gluCylinder(m_pQuadric, 0.9, 0.9, 3, 100, 100);  // �߾� ����
		glTranslatef(0, 0, 3);
		glTranslatef(0, 0, -3);
		glBindTexture(GL_TEXTURE_2D, frame);
		gluCylinder(m_pQuadric, 0.7, 0.7, 15, 100, 100);  // ��ü ��� �� ����
		glTranslatef(0, 0, 14.9);
		glRotatef(10, -1, 0, 0);
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		glTranslatef(0, 0, 0.8);
		glRotatef(20, -1, 0, 0);
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		glTranslatef(0, 0, 0.75);
		glRotatef(30, -1, 0, 0);
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		glTranslatef(0, 0, 0.75);
		glRotatef(30, -1, 0, 0);    // ��ƴ �κ� ���� ���� �ʿ�
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		glTranslatef(0, 0, 0.7);
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		// ��¦ ū ����
		glTranslatef(0, 0, 0.9);
		gluDisk(m_pQuadric, 0, 0.8, 100, 100);
		gluCylinder(m_pQuadric, 0.8, 0.8, 3, 100, 100);
		glTranslatef(0, 0, 3);
		gluDisk(m_pQuadric, 0, 0.8, 100, 100);
		// ������ ȸ�� ���� ��ħ��
		glTranslatef(0, 0, 0.9);
		glRotatef(90, 1, 0, 0);
		glTranslatef(0, 0, -0.5);
		glPushMatrix();  // ��ü ������ ����
			glRotatef(180, 0, 1, 0);  // ��ü ���� ��ħ ��� �Ʒ��� ��ũ
			glRotatef(-back_Angle, 0, 0, 1);  /*** ����� ���� ���� ***/
			gluCylinder(m_pQuadric, 1.0, 1.0, 0.8, 100, 100);
			glTranslatef(0, 0, 0.8);
			gluDisk(m_pQuadric, 0, 1.0, 100, 100);
			glPushMatrix();  // ��ü ���� ��ħ ���
				glTranslatef(0, 0, -0.4);
				glRotatef(-90, 1, 0, 0);
				glTranslatef(0, 0, 0.9);
				gluCylinder(m_pQuadric, 0.4, 0.4, 2, 100, 100);
			glPopMatrix();
			glTranslatef(0, 3, 0);  // ��ü ���� ��ħ ��� ���� ��ũ
			gluDisk(m_pQuadric, 0, 1.0, 100, 100);
			glTranslatef(0, 0, -0.8);
			gluCylinder(m_pQuadric, 1.0, 1.0, 0.8, 100, 100);
			gluDisk(m_pQuadric, 0, 1.0, 100, 100);

			glPushMatrix();   // ĳ����
				glTranslatef(0, 0, 16.25);  // ĳ���Ǹ� �׸��� ���� �߾����� �̵�
				glRotatef(-90, 0, 1, 0);
				glRotatef(-5 + canopy_Angle3, 1, 0, 0);   /*** ĳ���� ���� ���� ***/				
				for (int i = 0; i<3; i++) {
					glPushMatrix();
						glTranslatef(0, 0, 0.7*i);
						glRotatef(10 * i + canopy_Angle0, 1, 0, 0);
						glBindTexture(GL_TEXTURE_2D, bodyColor);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(-16.25, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(-16.25, 0, 7);
							glTexCoord2f(1, 1); glVertex3f(-16.25, 14.5, 7);
							glTexCoord2f(1, 0); glVertex3f(-16.25, 14.5, 0);
						glEnd();
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(16.25, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(16.25, 0, 7);
							glTexCoord2f(1, 1); glVertex3f(16.25, 14.5, 7);
							glTexCoord2f(1, 0); glVertex3f(16.25, 14.5, 0);
						glEnd();
						glTranslatef(0, 14.5, 0);
						glClipPlane(GL_CLIP_PLANE0, eq);
						glEnable(GL_CLIP_PLANE0);
						gluCylinder(m_pQuadric, 16.25, 16.25, 7, 100, 100);
						glDisable(GL_CLIP_PLANE0);
					glPopMatrix();
				}
				for (int i = 3; i<6; i++) {
					glPushMatrix();
						glTranslatef(0, 0, 0.7*i);
						glRotatef(10 * i + canopy_Angle1, 1, 0, 0);
						glBindTexture(GL_TEXTURE_2D, bodyColor);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(-16.25, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(-16.25, 14.5, 0);
							glTexCoord2f(1, 1); glVertex3f(-16.25, 14.5, 7);
							glTexCoord2f(1, 0); glVertex3f(-16.25, 0, 7);
						glEnd();
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(16.25, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(16.25, 14.5, 0);
							glTexCoord2f(1, 1); glVertex3f(16.25, 14.5, 7);
							glTexCoord2f(1, 0); glVertex3f(16.25, 0, 7);
						glEnd();
						glTranslatef(0, 14.5, 0);
						glClipPlane(GL_CLIP_PLANE0, eq);
						glEnable(GL_CLIP_PLANE0);
						gluCylinder(m_pQuadric, 16.25, 16.25, 7, 100, 100);
						glDisable(GL_CLIP_PLANE0);
					glPopMatrix();
				}
				for (int i = 6; i<9; i++) {
					glPushMatrix();
						glTranslatef(0, 0, 0.7*i);
						glRotatef(10 * i + canopy_Angle2, 1, 0, 0);
						glBindTexture(GL_TEXTURE_2D, bodyColor);
						glBegin(GL_QUADS);
								glTexCoord2f(0, 0); glVertex3f(-16.25, 0, 0);
								glTexCoord2f(0, 1); glVertex3f(-16.25, 14.5, 0);
								glTexCoord2f(1, 1); glVertex3f(-16.25, 14.5, 7);
								glTexCoord2f(1, 0); glVertex3f(-16.25, 0, 7);
						glEnd();
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(16.25, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(16.25, 14.5, 0);
							glTexCoord2f(1, 1); glVertex3f(16.25, 14.5, 7);
							glTexCoord2f(1, 0); glVertex3f(16.25, 0, 7);
						glEnd();
						glTranslatef(0, 14.5, 0);
						glClipPlane(GL_CLIP_PLANE0, eq);
						glEnable(GL_CLIP_PLANE0);
						gluCylinder(m_pQuadric, 16.25, 16.25, 7, 100, 100);
						glDisable(GL_CLIP_PLANE0);
					glPopMatrix();
				}
			glPopMatrix();   // ĳ���� ��
			
			glTranslatef(0, 0, 0.8);  // ��ü ���븦 �׸��� ���� �� �̵�
			glBindTexture(GL_TEXTURE_2D, bodyColor);
			glPushMatrix();  // ��ü ������ ����, �Ʒ���
				glRotatef(180, 0, 1, 0);
				glTranslatef(0, 0, -0.6);
				glTranslatef(0, -0.4, 0);
				glBegin(GL_QUADS);  // ������ ����
					glTexCoord2f(0, 0); glVertex3f(-10, 0, 0);
					glTexCoord2f(0, 1); glVertex3f(-10, -8, 0);
					glTexCoord2f(1, 1); glVertex3f(10, -8, 0);
					glTexCoord2f(1, 0); glVertex3f(10, 0, 0);
				glEnd();
				glTranslatef(0, 0, -14.85);
				glBegin(GL_QUADS);  // �Ʒ���
					glTexCoord2f(0, 0); glVertex3f(-10, -8, 14.85);
					glTexCoord2f(0, 1); glVertex3f(-10, -8, -14.85);
					glTexCoord2f(1, 1); glVertex3f(10, -8, -14.85);
					glTexCoord2f(1, 0); glVertex3f(10, -8, 14.85);
				glEnd();
				glTranslatef(10, 0, -14.85);
				gluDisk(m_pQuadric, 0, 8, 100, 100);
				gluCylinder(m_pQuadric, 8, 8, 29.7, 100, 100);
				glTranslatef(0, 0, 29.7);
				gluDisk(m_pQuadric, 0, 1.0, 100, 100);
			glPopMatrix();   // ��ü ������ ����, �Ʒ��� ��


			glBindTexture(GL_TEXTURE_2D, frame);
			glPushMatrix();  // ��ü ������ ���� �޹���
				glRotatef(-90, 0, 1, 0);
				glTranslatef(0.6, 0, 0);
				gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
				glTranslatef(0, 0, 10);
				gluSphere(m_pQuadric, 0.6, 100, 100);

				glPushMatrix(); /*** ������ ��� : ������ �� ***/
					glRotatef(-mode_Angle, 1, 0, 0);
					glBindTexture(GL_TEXTURE_2D, bodyColor);
					glRotatef(90, 0, 1, 0);
					gluPartialDisk(m_pQuadric, 0, 8, 100, 100, 90, 135);
					glRotatef(-90, 0, 1, 0);
					glBindTexture(GL_TEXTURE_2D, frame);
					gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
					glPushMatrix();  // ������ ���� �Ʒ���
						glTranslatef(14.85, 0, 0);
						glBindTexture(GL_TEXTURE_2D, bodyColor);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(-14.85, -8, 10);
							glTexCoord2f(0, 1); glVertex3f(-14.85, -8, -0.5);
							glTexCoord2f(1, 1); glVertex3f(14.85, -8, -0.5);
							glTexCoord2f(1, 0); glVertex3f(14.85, -8, 10);
						glEnd();
						glTranslatef(0, 0, 10);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(-10.85, -8, 5);
							glTexCoord2f(0, 1); glVertex3f(-10.85, -8, -0.5);
							glTexCoord2f(1, 1); glVertex3f(10.85, -8, -0.5);
							glTexCoord2f(1, 0); glVertex3f(10.85, -8, 5);
						glEnd();
						glTranslatef(-9.75, -8, 0);
						glRotatef(-90, 1, 0, 0);
						gluDisk(m_pQuadric, 0, 5, 100, 100);
						glTranslatef(19.5, 0, 0);
						gluDisk(m_pQuadric, 0, 5, 100, 100);
					glPopMatrix();
					glPushMatrix();  // ������ ���� ��� ����
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
							glTexCoord2f(1, 1); glVertex3f(0, -8, 10);
							glTexCoord2f(1, 0); glVertex3f(0, 0, 10);
						glEnd();
						glTranslatef(0, 0, 9.9);
						glBindTexture(GL_TEXTURE_2D, frame);
						glPushMatrix();
							for (int i = 0; i<9; i++) {   // � �κ�.  90�� ������ ���� 9�� ����
								glRotatef(10, 0, 1, 0);
								gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
								glTranslatef(0, 0, 0.9);
							}
							gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
						glPopMatrix();
						glBindTexture(GL_TEXTURE_2D, bodyColor);
						for (int i = 0; i<9; i++) {   // ������ ���� ���� ��� �κ�
							glRotatef(10, 0, 1, 0);
							glBegin(GL_QUADS);
								glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
								glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
								glTexCoord2f(1, 1); glVertex3f(0, -8, 1);
								glTexCoord2f(1, 0); glVertex3f(0, 0, 1);
							glEnd();
							glTranslatef(0, 0, 1);
						}
						glBegin(GL_QUADS);  // ������ ���� ��
							glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
							glTexCoord2f(1, 1); glVertex3f(0, -8, 10);
							glTexCoord2f(1, 0); glVertex3f(0, 0, 10);
						glEnd();
					glPopMatrix();  // ������ ���� ��� ��
				glPopMatrix();
			glPopMatrix();   // ��ü ������ ���� �޹��� ��
			glPushMatrix();  // ��ü ������ ���� �չ���
				glRotatef(90, 0, 1, 0);
				glTranslatef(-0.6, 0, 0);
				glBindTexture(GL_TEXTURE_2D, frame);
				gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
				glTranslatef(0, 0, 10);
				gluSphere(m_pQuadric, 0.6, 100, 100);
				glPushMatrix(); /*** ������ ��� : ������ �� ***/
					glRotatef(mode_Angle, 1, 0, 0);
					gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
					glPushMatrix();  // ������ �� ���� � �κ�.  90�� ������ ���� 9�� ����
						glTranslatef(0, 0, 9.9);
						for (int i = 0; i<9; i++) {
							glRotatef(-10, 0, 1, 0);
							gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
							glTranslatef(0, 0, 0.9);
						}
						gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
					glPopMatrix();
					glPushMatrix();  // ������ �� �Ʒ���
						glTranslatef(-14.85, 0, 0);
						glBindTexture(GL_TEXTURE_2D, bodyColor);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(14.85, -8, -0.5);
							glTexCoord2f(0, 1); glVertex3f(14.85, -8, 10);
							glTexCoord2f(1, 1); glVertex3f(-14.85, -8, 10);
							glTexCoord2f(1, 0); glVertex3f(-14.85, -8, -0.5);
						glEnd();
						glTranslatef(0, 0, 10);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(10.85, -8, -0.5);
							glTexCoord2f(0, 1); glVertex3f(10.85, -8, 5);
							glTexCoord2f(1, 1); glVertex3f(-10.85, -8, 5);
							glTexCoord2f(1, 0); glVertex3f(-10.85, -8, -0.5);
						glEnd();
						glTranslatef(-9.75, -8, 0);
						glRotatef(-90, 1, 0, 0);
						gluDisk(m_pQuadric, 0, 5, 100, 100);
						glTranslatef(19.5, 0, 0);
						gluDisk(m_pQuadric, 0, 5, 100, 100);
					glPopMatrix();
					glPushMatrix();  // ������ ���� ����	
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
							glTexCoord2f(1, 1); glVertex3f(0, -8, 10);
							glTexCoord2f(1, 0); glVertex3f(0, 0, 10);
						glEnd();
						glTranslatef(0, 0, 9.9);
						for (int i = 0; i<9; i++) {   // ������ ���� ���� ��� �κ�
							glRotatef(-10, 0, 1, 0);
							glBegin(GL_QUADS);
								glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
								glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
								glTexCoord2f(1, 1); glVertex3f(0, -8, 1);
								glTexCoord2f(1, 0); glVertex3f(0, 0, 1);
							glEnd();
							glTranslatef(0, 0, 1);
						}
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
							glTexCoord2f(1, 1); glVertex3f(0, -8, 10);
							glTexCoord2f(1, 0); glVertex3f(0, 0, 10);
						glEnd();
					glPopMatrix();  // ������ ���� ��� ��
				glPopMatrix();
			glPopMatrix();  // ��ü ���� ������ �չ��� ��
		glPopMatrix(); // ��ü ������ ���� ��
		glBindTexture(GL_TEXTURE_2D, frame);
		gluDisk(m_pQuadric, 0.5, 1.3, 100, 100);
		gluCylinder(m_pQuadric, 1.3, 1.3, 1, 100, 100);
		glTranslatef(0, 0, 1);
		gluDisk(m_pQuadric, 0.5, 1.3, 100, 100);
		// ������ ȸ�� ���� �κ�
		glPushMatrix();
			glRotatef(-handle_Angle, 0, 0, 1);   /*** ������ ���� ���� ***/
			gluDisk(m_pQuadric, 0.5, 1.3, 100, 100);
			gluCylinder(m_pQuadric, 1.3, 1.3, 1, 100, 100);
			glTranslatef(0, 0, 1);
			gluDisk(m_pQuadric, 0.5, 1.3, 100, 100);
			// ������ ���
			glTranslatef(0, 0, -0.6);
			glRotatef(90, -1, 0, 0);
			glTranslatef(0, 0, 0.8);
			gluCylinder(m_pQuadric, 0.6, 0.6, 35, 100, 100);   // ������ ���1
			glTranslatef(0, 0, 35 - HANDLE_SIZE);
			glTranslatef(0, 0, handle_2);
			gluCylinder(m_pQuadric, 0.61, 0.61, HANDLE_SIZE, 100, 100);   // ������ ���2
			glTranslatef(0, 0, handle_3);
			gluCylinder(m_pQuadric, 0.62, 0.62, HANDLE_SIZE, 100, 100);   // ������ ���3
																  // ������
			glTranslatef(0, 0, HANDLE_SIZE - 0.5);
			glRotatef(10, -1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, handle_texture);
			gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
			glTranslatef(0, 0, 0.8);
			glRotatef(20, -1, 0, 0);
			gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
			glTranslatef(0, 0, 0.75);
			glRotatef(30, -1, 0, 0);
			gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
			glTranslatef(0, 0, 0.75);
			glRotatef(30, -1, 0, 0);    // ��ƴ �κ� ���� ���� �ʿ�
			gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
			gluCylinder(m_pQuadric, 0.6, 0.6, 20, 100, 100);
		glPopMatrix();
	glPopMatrix();

	// ����(-x�� ����)
	glBindTexture(GL_TEXTURE_2D, frame);
	glPushMatrix();
		glRotatef(-90, 0, 1, 0);
		gluDisk(m_pQuadric, 0.7, 0.9, 100, 100);
		gluCylinder(m_pQuadric, 0.9, 0.9, 3, 100, 100);  // �߾� ����
		glTranslatef(0, 0, 3);
		gluDisk(m_pQuadric, 0.7, 0.9, 100, 100);
		glTranslatef(0, 0, -3);
		gluCylinder(m_pQuadric, 0.7, 0.7, 15, 100, 100);   // ��ü ��� �� ����
		glTranslatef(0, 0, 14.9);
		glRotatef(10, -1, 0, 0);
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		glTranslatef(0, 0, 0.8);
		glRotatef(20, -1, 0, 0);
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		glTranslatef(0, 0, 0.75);
		glRotatef(30, -1, 0, 0);
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		glTranslatef(0, 0, 0.75);
		glRotatef(30, -1, 0, 0);
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		glTranslatef(0, 0, 0.7);
		gluCylinder(m_pQuadric, 0.7, 0.7, 1, 100, 100);
		// ��¦ ū ����
		glTranslatef(0, 0, 0.9);
		gluDisk(m_pQuadric, 0, 0.8, 100, 100);
		gluCylinder(m_pQuadric, 0.8, 0.8, 3, 100, 100);
		glTranslatef(0, 0, 3);
		gluDisk(m_pQuadric, 0, 0.8, 100, 100);
		// ������ ȸ�� ���� ��ħ��
		glTranslatef(0, 0, 0.9);
		glRotatef(90, 1, 0, 0);
		glTranslatef(0, 0, -0.5);
		glPushMatrix();  // ��ü ���� ����
			glRotatef(180, 0, 1, 0);  // ��ü ���� ��ħ ��� �Ʒ��� ��ũ
			glRotatef(back_Angle, 0, 0, 1);  /*** ����� ���� ���� ***/
			gluCylinder(m_pQuadric, 1.0, 1.0, 0.8, 100, 100);
			glTranslatef(0, 0, 0.8);
			gluDisk(m_pQuadric, 0, 1.0, 100, 100);
			glPushMatrix();  // ��ü ���� ��ħ ���
				glTranslatef(0, 0, -0.4);
				glRotatef(-90, 1, 0, 0);
				glTranslatef(0, 0, 0.9);
				gluCylinder(m_pQuadric, 0.4, 0.4, 2, 100, 100);
			glPopMatrix();
			glTranslatef(0, 3, 0);  // ��ü ���� ��ħ ��� ���� ��ũ
			gluDisk(m_pQuadric, 0, 1.0, 100, 100);
			glTranslatef(0, 0, -0.8);
			gluCylinder(m_pQuadric, 1.0, 1.0, 0.8, 100, 100);
			gluDisk(m_pQuadric, 0, 1.0, 100, 100);
			glTranslatef(0, 0, 0.8);  // ��ü ���븦 �׸��� ���� �� �̵�
			glBindTexture(GL_TEXTURE_2D, bodyColor);
			glPushMatrix();  // ��ü ���� ����
				glRotatef(180, 0, 1, 0);
				glTranslatef(0, 0, -0.6);
				glTranslatef(0, -0.4, 0);
				glBindTexture(GL_TEXTURE_2D, bodyColor);
				glRotatef(-180, 0, 1, 0);
				glBegin(GL_QUADS);
					glTexCoord2f(0, 0); glVertex3f(-10, 0, 0);
					glTexCoord2f(0, 1); glVertex3f(10, 0, 0);
					glTexCoord2f(1, 1); glVertex3f(10, -8, 0);
					glTexCoord2f(1, 0); glVertex3f(-10, -8, 0);
				glEnd();
				glRotatef(180, 0, 1, 0);
			glPopMatrix();   // ��ü ���� ���� ��
			glBindTexture(GL_TEXTURE_2D, frame);
			glPushMatrix();  // ��ü ���� ���� �չ���
				glRotatef(-90, 0, 1, 0);
				glTranslatef(0.6, 0, 0);
				gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
				glPushMatrix();  // /*** ������ ��� : ���� �� ***/
					glTranslatef(0, 0, 10);
					gluSphere(m_pQuadric, 0.6, 100, 100);
					glRotatef(mode_Angle, 1, 0, 0);
					gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
					glPushMatrix();
						glTranslatef(0, 0, 9.9);
						for (int i = 0; i<9; i++) {   // � �κ�.  90�� ������ ���� 9�� ����
							glRotatef(10, 0, 1, 0);
							gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
							glTranslatef(0, 0, 0.9);
						}
						gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
					glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, bodyColor);
					glBegin(GL_QUADS);
						glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
						glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
						glTexCoord2f(1, 1); glVertex3f(0, -8, 10);
						glTexCoord2f(1, 0); glVertex3f(0, 0, 10);
					glEnd();
					glTranslatef(0, 0, 9.9);
					for (int i = 0; i<9; i++) {   // ���� ���� ���� ��� �κ�
						glRotatef(10, 0, 1, 0);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
							glTexCoord2f(1, 1); glVertex3f(0, -8, 1);
							glTexCoord2f(1, 0); glVertex3f(0, 0, 1);
						glEnd();
						glTranslatef(0, 0, 1);
					}
					glBegin(GL_QUADS);
						glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
						glTexCoord2f(0, 1); glVertex3f(0, -8, 0);	
						glTexCoord2f(1, 1); glVertex3f(0, -8, 10);
						glTexCoord2f(1, 0); glVertex3f(0, 0, 10);
					glEnd();
				glPopMatrix();
			glPopMatrix();   // ��ü ���� ���� �չ��� ��
			glPushMatrix();  // ��ü ���� ���� �޹���	
				glRotatef(90, 0, 1, 0);
				glTranslatef(-0.6, 0, 0);
				glBindTexture(GL_TEXTURE_2D, frame);
				gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
				glTranslatef(0, 0, 10);
				gluSphere(m_pQuadric, 0.6, 100, 100);
				glPushMatrix();  // /*** ������ ��� : ���� �� ***/
					glRotatef(-mode_Angle, 1, 0, 0);
					gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
					glPushMatrix();
						glTranslatef(0, 0, 9.9);
						for (int i = 0; i<9; i++) {   // � �κ�.  90�� ������ ���� 9�� ����
							glRotatef(-10, 0, 1, 0);
							gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
							glTranslatef(0, 0, 0.9);
						}
						gluCylinder(m_pQuadric, 0.6, 0.6, 10, 100, 100);
					glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, bodyColor);
					glBegin(GL_QUADS);
						glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
						glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
						glTexCoord2f(1, 1); glVertex3f(0, -8, 10);
						glTexCoord2f(1, 0); glVertex3f(0, 0, 10);
					glEnd();
					glRotatef(-90, 0, 1, 0);
					gluPartialDisk(m_pQuadric, 0, 8, 100, 100, 90, 135);
					glRotatef(90, 0, 1, 0);
					glTranslatef(0, 0, 9.9);
					for (int i = 0; i<9; i++) {   // ���� ���� ���� ��� �κ�
						glRotatef(-10, 0, 1, 0);
						glBegin(GL_QUADS);
							glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
							glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
							glTexCoord2f(1, 1); glVertex3f(0, -8, 1);
							glTexCoord2f(1, 0); glVertex3f(0, 0, 1);
						glEnd();
						glTranslatef(0, 0, 1);
					}
					glBegin(GL_QUADS);
						glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
						glTexCoord2f(0, 1); glVertex3f(0, -8, 0);
						glTexCoord2f(1, 1); glVertex3f(0, -8, 10);
						glTexCoord2f(1, 0); glVertex3f(0, 0, 10);
					glEnd();
				glPopMatrix();
			glPopMatrix();  // ��ü ���� ���� �޹��� ��
		glPopMatrix();  // ��ü ���� ���� ��
		glBindTexture(GL_TEXTURE_2D, frame);
		gluDisk(m_pQuadric, 0.5, 1.3, 100, 100);
		gluCylinder(m_pQuadric, 1.3, 1.3, 1, 100, 100);
		glTranslatef(0, 0, 1);
		gluDisk(m_pQuadric, 0.5, 1.3, 100, 100);
		// ������ ȸ�� ���� �κ�
		glPushMatrix();
			glRotatef(handle_Angle, 0, 0, 1);   /*** ������ ���� ���� ***/
			gluDisk(m_pQuadric, 0.5, 1.3, 100, 100);
			gluCylinder(m_pQuadric, 1.3, 1.3, 1, 100, 100);
			glTranslatef(0, 0, 1);
			gluDisk(m_pQuadric, 0.5, 1.3, 100, 100);
			// ������ ���
			glTranslatef(0, 0, -0.6);
			glRotatef(90, -1, 0, 0);
			glTranslatef(0, 0, 0.8);
			gluCylinder(m_pQuadric, 0.6, 0.6, 35, 100, 100);   // ������ ���1
			glTranslatef(0, 0, 35 - HANDLE_SIZE);
			glTranslatef(0, 0, handle_2);
			gluCylinder(m_pQuadric, 0.61, 0.61, HANDLE_SIZE, 100, 100);   // ������ ���2
			glTranslatef(0, 0, handle_3);
			gluCylinder(m_pQuadric, 0.62, 0.62, HANDLE_SIZE, 100, 100);   // ������ ���3
																  // ������
			glTranslatef(0, 0, HANDLE_SIZE - 0.5);
			glRotatef(10, -1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, handle_texture);
			gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
			glTranslatef(0, 0, 0.8);
			glRotatef(20, -1, 0, 0);
			gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
			glTranslatef(0, 0, 0.75);
			glRotatef(30, -1, 0, 0);
			gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
			glTranslatef(0, 0, 0.75);
			glRotatef(30, -1, 0, 0);    // ��ƴ �κ� ���� ���� �ʿ�
			gluCylinder(m_pQuadric, 0.6, 0.6, 1, 100, 100);
			gluCylinder(m_pQuadric, 0.6, 0.6, 20, 100, 100);
		glPopMatrix();
	glPopMatrix();   // ************ ��ü �� ������ �κ� �� ************
	glPopMatrix();  // ��ü ȸ���� ���� glPopMatrix();  draw()�� ���� �κп� glPushMatrix()�� ����.

// ***************** ��ü �κ� *****************

	// �ٱ���
	glPushMatrix();
		glPushMatrix();   // �ٱ��� �׸���
			glTranslatef(0, -20, 0);
			glRotatef(-90, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, mesh);
			glBegin(GL_QUADS);   // �ظ�
				glTexCoord2f(0.0, 0.0); glVertex3f(7, -7, 0);
				glTexCoord2f(1.0, 0.0); glVertex3f(-7, -7, 0);
				glTexCoord2f(1.0, 1.0); glVertex3f(-7, 7, 0);
				glTexCoord2f(0.0, 1.0); glVertex3f(7, 7, 0);
			glEnd();
			glBegin(GL_QUADS);   // ����1
				glTexCoord2f(0.0, 0.0); glVertex3f(10, -10, 10);
				glTexCoord2f(1.0, 0.0); glVertex3f(10, 10, 10);
				glTexCoord2f(1.0, 1.0); glVertex3f(7, 7, 0);
				glTexCoord2f(0.0, 1.0); glVertex3f(7, -7, 0);
			glEnd();
			glBegin(GL_QUADS);   // ����2
				glTexCoord2f(0.0, 0.0); glVertex3f(10, 10, 10);
				glTexCoord2f(0.0, 1.0); glVertex3f(-10, 10, 10);
				glTexCoord2f(1.0, 1.0); glVertex3f(-7, 7, 0);
				glTexCoord2f(1.0, 0.0); glVertex3f(7, 7, 0);
			glEnd();
			glBegin(GL_QUADS);   // ����3
				glTexCoord2f(0.0, 0.0); glVertex3f(-10, 10, 10);
				glTexCoord2f(0.0, 1.0); glVertex3f(-10, -10, 10);
				glTexCoord2f(1.0, 1.0); glVertex3f(-7, -7, 0);
				glTexCoord2f(1.0, 0.0); glVertex3f(-7, 7, 0);
			glEnd();
			glBegin(GL_QUADS);   // ����4
				glTexCoord2f(0.0, 0.0); glVertex3f(-10, -10, 10);
				glTexCoord2f(0.0, 1.0); glVertex3f(10, -10, 10);
				glTexCoord2f(1.0, 1.0); glVertex3f(7, -7, 0);
				glTexCoord2f(1.0, 0.0); glVertex3f(-7, -7, 0);
			glEnd();
		glPopMatrix();
		// �ٱ��� �����
		glTranslatef(10, -10, 10);
		glRotatef(180, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, frame);
		gluSphere(m_pQuadric, 0.8, 100, 100);
		gluCylinder(m_pQuadric, 0.8, 0.8, 20, 100, 100);
		glRotatef(-90, 0, 1, 0);
		gluCylinder(m_pQuadric, 0.8, 0.8, 20, 100, 100);
		glTranslatef(0, 0, 20);
		gluSphere(m_pQuadric, 0.8, 100, 100);
	
		glTranslatef(20, 0, 0);
		glRotatef(180, 1, 0, 0);
		gluSphere(m_pQuadric, 0.8, 100, 100);
		gluCylinder(m_pQuadric, 0.8, 0.8, 20, 100, 100);
		glTranslatef(0, 0, 20);
		gluSphere(m_pQuadric, 0.8, 100, 100);
		glTranslatef(0, 0, -20);
		glRotatef(-90, 0, 1, 0);
		gluCylinder(m_pQuadric, 0.8, 0.8, 20, 100, 100);
	
	glPopMatrix();
	// *************** ��ü ��� �� ���� �κ� ***************
	glPushMatrix();
	// ��ü �� ������ �κа� ��ü�� �����ϴ� �κ�
		glRotatef(90, 1, 0, 0);
		glTranslatef(0, 0, 0.5);
		gluDisk(m_pQuadric, 0, 1.3, 100, 100);
		gluCylinder(m_pQuadric, 1.3, 1.3, 1, 100, 100);
		glTranslatef(0, 0, 1);
		gluDisk(m_pQuadric, 0, 1.3, 100, 100);
	
		// ���� ��� ������, ���� polygon���� ä��� �� �ʿ�
		glPushMatrix();  // ���� ������
			glRotatef(90, 0, 1, 0);
			glTranslatef(-1.3, 0, 0);
			glTranslatef(0, 0, -1.5);
			glTranslatef(0, 1.2, 0);   // �������� �� �̵�
			glPushMatrix();
				gluDisk(m_pQuadric, 1.1, 1.3, 100, 100);
				gluCylinder(m_pQuadric, 1.3, 1.3, 8, 100, 100);
				glTranslatef(0, 0, 8);
				gluDisk(m_pQuadric, 1.1, 1.3, 100, 100);
				// ������ ���� ���
				for (int i = 0; i<9; i++) {
					gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
					glTranslatef(0, 0, 0.8);
					glRotatef(10, 0, -1, 0);
				}
				for (int i = 0; i<4; i++) {
					glRotatef(-10, 1, 0, 0);  // ������ ���η����� �κ�
					gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
					glTranslatef(0, 0, 0.8);
				}
				glRotatef(-5, 1, 0, 0);  // ������ ���η����� �κ�
				gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
				glTranslatef(0, 0, 0.8);

				gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
				gluCylinder(m_pQuadric, 1.2, 1.2, 14, 100, 100);  // ���� ��� ����
				glTranslatef(0, 0, 13.7);
				gluSphere(m_pQuadric, 1.2, 100, 100);  // ���� ��� ������ ��

			   // ������ ��� ���� ����
				glRotatef(45, 1, 0, 0);
				glPushMatrix();
				// ���� ȸ�� ���
					glRotatef(wheelRotation_Angle, 0, 0, 1);
					gluDisk(m_pQuadric, 0, 1.8, 100, 100);
					gluCylinder(m_pQuadric, 1.8, 1.8, 0.8, 100, 100);  // ���� ��ħ��    ����...
					glTranslatef(0, 0, 0.8);
					gluDisk(m_pQuadric, 0, 1.8, 100, 100);
					gluCylinder(m_pQuadric, 1, 1, 1, 100, 100);  // ���� �������1...
					glTranslatef(0, 0, 2.6);
					glRotatef(90, 0, 1, 0);
					glTranslatef(0, 0, -0.5);
					gluDisk(m_pQuadric, 0.5, 1.8, 100, 100);
					gluCylinder(m_pQuadric, 1.8, 1.8, 1, 100, 100);  // ���� �������2...
					glTranslatef(0, 0, 1);
					gluDisk(m_pQuadric, 0.5, 1.8, 100, 100);
					glTranslatef(0, 0, -4.5);
					gluDisk(m_pQuadric, 0, 0.5, 100, 100);
					gluCylinder(m_pQuadric, 0.5, 0.5, 6, 100, 100); // ���� ���� ����  
					glTranslatef(0, 0, 6);
					glPushMatrix();  // ������ ����
						glBindTexture(GL_TEXTURE_2D, wheel[0]);
						gluDisk(m_pQuadric, 0, 4, 100, 100);
						glBindTexture(GL_TEXTURE_2D, wheel[1]);
						gluCylinder(m_pQuadric, 4, 4, 2, 100, 100);
						glTranslatef(0, 0, 2);
						glBindTexture(GL_TEXTURE_2D, wheel[0]);
						gluDisk(m_pQuadric, 0, 4, 100, 100);
					glPopMatrix();
					glTranslatef(0, 0, -6);  // ���� ����
					glPushMatrix();
						glBindTexture(GL_TEXTURE_2D, wheel[0]);
						gluDisk(m_pQuadric, 0, 4, 100, 100);
						glBindTexture(GL_TEXTURE_2D, wheel[1]);
						gluCylinder(m_pQuadric, 4, 4, 2, 100, 100);
						glTranslatef(0, 0, 2);
						glBindTexture(GL_TEXTURE_2D, wheel[0]);
						gluDisk(m_pQuadric, 0, 4, 100, 100);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();

			// ���� ���� ���
			glRotatef(180, 0, 1, 0);
			gluDisk(m_pQuadric, 1.1, 1.3, 100, 100);
			glBindTexture(GL_TEXTURE_2D, frame);
			gluCylinder(m_pQuadric, 1.3, 1.3, 5.4, 100, 100);
			glTranslatef(0, 0, 5.4);
			gluDisk(m_pQuadric, 1.1, 1.3, 100, 100);
			for (int i = 0; i<9; i++) {
				gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
				glTranslatef(0, 0, 0.8);
				glRotatef(10, 0, 1, 0);
			}
			for (int i = 0; i<4; i++) {
				glRotatef(-10, 1, 0, 0);  // ������ ���η����� �κ�
				gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
				glTranslatef(0, 0, 0.8);
			}
			glRotatef(-5, 1, 0, 0);  // ������ ���η����� �κ�
			gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
			glTranslatef(0, 0, 0.8);

			gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
			gluCylinder(m_pQuadric, 1.2, 1.2, 14, 100, 100);  // ���� ��� ����
			glTranslatef(0, 0, 13.7);
			gluSphere(m_pQuadric, 1.2, 100, 100);  // ���� ��� ������ ��.

			 // ������ ��� ���� ����
			glRotatef(45, 1, 0, 0);
			glPushMatrix();
			// ���� ȸ�� ���
				glRotatef(wheelRotation_Angle, 0, 0, 1);
				gluDisk(m_pQuadric, 0, 1.8, 100, 100);
				gluCylinder(m_pQuadric, 1.8, 1.8, 0.8, 100, 100);  // ���� ��ħ�� ����...
				glTranslatef(0, 0, 0.8);
				gluDisk(m_pQuadric, 0, 1.8, 100, 100);
				gluCylinder(m_pQuadric, 1, 1, 1, 100, 100);  // ���� �������1...
				glTranslatef(0, 0, 2.6);
				glRotatef(90, 0, 1, 0);
				glTranslatef(0, 0, -0.5);
				gluDisk(m_pQuadric, 0.5, 1.8, 100, 100);
				gluCylinder(m_pQuadric, 1.8, 1.8, 1, 100, 100);  // ���� �������2...
				glTranslatef(0, 0, 1);
				gluDisk(m_pQuadric, 0.5, 1.8, 100, 100);
				glTranslatef(0, 0, -4.5);
				gluDisk(m_pQuadric, 0, 0.5, 100, 100);
				gluCylinder(m_pQuadric, 0.5, 0.5, 6, 100, 100); // ���� ���� ����  
				glTranslatef(0, 0, 6);
				glPushMatrix();  // ������ ����
					glBindTexture(GL_TEXTURE_2D, wheel[0]);
					gluDisk(m_pQuadric, 0, 4, 100, 100);
					glBindTexture(GL_TEXTURE_2D, wheel[1]);
					gluCylinder(m_pQuadric, 4, 4, 2, 100, 100);
					glTranslatef(0, 0, 2);
					glBindTexture(GL_TEXTURE_2D, wheel[0]);
					gluDisk(m_pQuadric, 0, 4, 100, 100);
				glPopMatrix();
				glTranslatef(0, 0, -6);  // ���� ����
				glPushMatrix();
					glBindTexture(GL_TEXTURE_2D, wheel[0]);
					gluDisk(m_pQuadric, 0, 4, 100, 100);
					glBindTexture(GL_TEXTURE_2D, wheel[1]);
					gluCylinder(m_pQuadric, 4, 4, 2, 100, 100);
					glTranslatef(0, 0, 2);
					glBindTexture(GL_TEXTURE_2D, wheel[0]);
					gluDisk(m_pQuadric, 0, 4, 100, 100);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();  // ���� ������
			glRotatef(90, 0, 1, 0);
			glTranslatef(-1.3, 0, 0);
			glTranslatef(0, 0, -1.5);
			glTranslatef(0, -1.2, 0);  // �������� �� �̵�
			glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, frame);
				gluDisk(m_pQuadric, 1.1, 1.3, 100, 100);
				gluCylinder(m_pQuadric, 1.3, 1.3, 8, 100, 100);
				glTranslatef(0, 0, 8);
				gluDisk(m_pQuadric, 1.1, 1.3, 100, 100);

				// ������ ���� ���
				glRotatef(180, 0, 0, 1);
				for (int i = 0; i<9; i++) {
					gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
					glTranslatef(0, 0, 0.8);
					glRotatef(10, 0, 1, 0);
				}
				for (int i = 0; i<4; i++) {
					glRotatef(-10, 1, 0, 0);  // ������ ���η����� �κ�
					gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
					glTranslatef(0, 0, 0.8);
				}
				glRotatef(-5, 1, 0, 0);  // ������ ���η����� �κ�
				gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
				glTranslatef(0, 0, 0.8);

				gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
				gluCylinder(m_pQuadric, 1.2, 1.2, 14, 100, 100);  // ���� ��� ����
				glTranslatef(0, 0, 13.7);
				gluSphere(m_pQuadric, 1.2, 100, 100);  // ���� ��� ������ ��

			   // ������ ��� ���� ����
				glRotatef(45, 1, 0, 0);
				gluDisk(m_pQuadric, 0, 1.8, 100, 100);
				gluCylinder(m_pQuadric, 1.8, 1.8, 0.8, 100, 100);  // ���� ��ħ�� ����...
				glTranslatef(0, 0, 0.8);
				gluDisk(m_pQuadric, 0, 1.8, 100, 100);
				gluCylinder(m_pQuadric, 1, 1, 1, 100, 100);  // ���� �������1...
				glTranslatef(0, 0, 2.6);
				glRotatef(90, 0, 1, 0);
				glTranslatef(0, 0, -0.5);
				gluDisk(m_pQuadric, 0.5, 1.8, 100, 100);
				gluCylinder(m_pQuadric, 1.8, 1.8, 1, 100, 100);  // ���� �������2...
				glTranslatef(0, 0, 1);
				gluDisk(m_pQuadric, 0.5, 1.8, 100, 100);
				glTranslatef(0, 0, -4.5);
				gluDisk(m_pQuadric, 0, 0.5, 100, 100);
				gluCylinder(m_pQuadric, 0.5, 0.5, 6, 100, 100); // ���� ���� ����  
				glTranslatef(0, 0, 6);
				glPushMatrix();  // ������ ����
					glPushMatrix();  // ������ ���� Ÿ�̾�
						glBindTexture(GL_TEXTURE_2D, wheel[0]);
						gluDisk(m_pQuadric, 0, 4, 100, 100);
						glBindTexture(GL_TEXTURE_2D, wheel[1]);
						gluCylinder(m_pQuadric, 4, 4, 2, 100, 100);
						glTranslatef(0, 0, 2);
						glBindTexture(GL_TEXTURE_2D, wheel[0]);
						gluDisk(m_pQuadric, 0, 4, 100, 100);
					glPopMatrix();
				glPopMatrix();
				glTranslatef(0, 0, -6);  // ���� ����
				glPushMatrix();
					glBindTexture(GL_TEXTURE_2D, wheel[0]);
					gluDisk(m_pQuadric, 0, 4, 100, 100);
					glBindTexture(GL_TEXTURE_2D, wheel[1]);
					gluCylinder(m_pQuadric, 4, 4, 2, 100, 100);
					glTranslatef(0, 0, 2);
					glBindTexture(GL_TEXTURE_2D, wheel[0]);
					gluDisk(m_pQuadric, 0, 4, 100, 100);
				glPopMatrix();
			glPopMatrix();
			// ���� ���� ���
			glRotatef(180, 0, 1, 0);
			glBindTexture(GL_TEXTURE_2D, frame);
			gluDisk(m_pQuadric, 1.1, 1.3, 100, 100);
			gluCylinder(m_pQuadric, 1.3, 1.3, 5.4, 100, 100);
			glTranslatef(0, 0, 5.4);
			gluDisk(m_pQuadric, 1.1, 1.3, 100, 100);
			for (int i = 0; i<9; i++) {
				gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
				glTranslatef(0, 0, 0.8);
				glRotatef(10, 0, 1, 0);
			}
			for (int i = 0; i<4; i++) {
				glRotatef(10, 1, 0, 0);  // ������ ���η����� �κ�
				gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
				glTranslatef(0, 0, 0.8);
			}
			glRotatef(5, 1, 0, 0);  // ������ ���η����� �κ�
			gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
			glTranslatef(0, 0, 0.8);

			gluCylinder(m_pQuadric, 1.2, 1.2, 1, 100, 100);
			gluCylinder(m_pQuadric, 1.2, 1.2, 14, 100, 100);  // ���� ��� ����
			glTranslatef(0, 0, 13.7);
			gluSphere(m_pQuadric, 1.2, 100, 100);  // ���� ��� ������ ��.

		   // ������ ��� ���� ����
			glRotatef(-45, 1, 0, 0);
			gluDisk(m_pQuadric, 0, 1.8, 100, 100);
			gluCylinder(m_pQuadric, 1.8, 1.8, 0.8, 100, 100);  // ���� ��ħ�� ����...
			glTranslatef(0, 0, 0.8);
			gluDisk(m_pQuadric, 0, 1.8, 100, 100);
			gluCylinder(m_pQuadric, 1, 1, 1, 100, 100);  // ���� �������1...
			glTranslatef(0, 0, 2.6);
			glRotatef(90, 0, 1, 0);
			glTranslatef(0, 0, -0.5);
			gluDisk(m_pQuadric, 0.5, 1.8, 100, 100);
			gluCylinder(m_pQuadric, 1.8, 1.8, 1, 100, 100);  // ���� �������2...
			glTranslatef(0, 0, 1);
			gluDisk(m_pQuadric, 0.5, 1.8, 100, 100);
			glTranslatef(0, 0, -4.5);
			gluDisk(m_pQuadric, 0, 0.5, 100, 100);
			gluCylinder(m_pQuadric, 0.5, 0.5, 6, 100, 100); // ���� ���� ����  
			glTranslatef(0, 0, 6);
			glPushMatrix();  // ������ ����
				glPushMatrix();  // ������ ���� Ÿ�̾�
					glBindTexture(GL_TEXTURE_2D, wheel[0]);
					gluDisk(m_pQuadric, 0, 4, 100, 100);
					glBindTexture(GL_TEXTURE_2D, wheel[1]);
					gluCylinder(m_pQuadric, 4, 4, 2, 100, 100);
					glTranslatef(0, 0, 2);
					glBindTexture(GL_TEXTURE_2D, wheel[0]);
					gluDisk(m_pQuadric, 0, 4, 100, 100);
				glPopMatrix();
			glPopMatrix();
			glTranslatef(0, 0, -6);  // ���� ����
			glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, wheel[0]);
				gluDisk(m_pQuadric, 0, 4, 100, 100);
				glBindTexture(GL_TEXTURE_2D, wheel[1]);
				gluCylinder(m_pQuadric, 4, 4, 2, 100, 100);
				glTranslatef(0, 0, 2);
				glBindTexture(GL_TEXTURE_2D, wheel[0]);
				gluDisk(m_pQuadric, 0, 4, 100, 100);
			glPopMatrix();
		glPopMatrix(); // *************** ��ü ��� �� ���� �κ� �� ***************

	glPopMatrix();  // ************ ��ü �κ� �� ************

	glFlush();
	glutSwapBuffers();
}


void mouse(int button, int state, int x, int y)
{
	printf("Mouse button is clicked! (%d, %d, %d, %d) \n",
		button, state, x, y);
}


void motion(int x, int y)
{
	printf("Mouse is moving! (%d, %d) \n", x, y);
}



void keyboard(unsigned char key, int x, int y)
{
	printf("You pressed %c \n", key);

	if (key == 'z') {   // Zoom-in
		if (distance >= 0)
			distance -= 3;
	}
	if (key == 'x')   // Zoom-out
		distance += 3;

	if (key == 'w') {   // handle up
		if (handle_Angle == -90) {}
		else { handle_Angle -= 1; }
	}
	if (key == 's') {   // handle down
		if (handle_Angle == 90) {}
		else { handle_Angle += 1; }
	}

	if (key == 'e') {   // canopy up
		if (canopy_Angle3 == -30) {}
		else { canopy_Angle3 -= 1; }
	}
	if (key == 'd') {   // canopy down
		if (canopy_Angle3 == 30) {}
		else { canopy_Angle3 += 1; }
	}

	if (elevation >= 360)         // �ֱ� 360
		elevation -= 360;
	else if (elevation <= -360)
		elevation += 360;

	if (azimuth >= 360)         // �ֱ� 360
		azimuth -= 360;
	else if (azimuth <= -360)
		azimuth += 360;


	eyex = distance * cos(elevation * PI / 180) * sin(azimuth * PI / 180);
	eyey = distance * sin(elevation * PI / 180);
	eyez = distance * cos(elevation * PI / 180) * cos(azimuth * PI / 180);
}



void sub_menu_function(int option)
{
	printf("submenu %d has been selected \n", option);

	switch (option) {
	case 0:	back_Mode[0] = true;
		back_Mode[1] = false;
		back_Mode[2] = false;
		back_Mode[3] = false;
		back_Mode[4] = false;
		break;
	case 1: back_Mode[0] = false;
		back_Mode[1] = true;
		back_Mode[2] = false;
		back_Mode[3] = false;
		back_Mode[4] = false;
		break;
	case 2: back_Mode[0] = false;
		back_Mode[1] = false;
		back_Mode[2] = true;
		back_Mode[3] = false;
		back_Mode[4] = false;
		break;
	case 3: back_Mode[0] = false;
		back_Mode[1] = false;
		back_Mode[2] = false;
		back_Mode[3] = true;
		back_Mode[4] = false;
		break;
	case 4: back_Mode[0] = false;
		back_Mode[1] = false;
		back_Mode[2] = false;
		back_Mode[3] = false;
		back_Mode[4] = true;
		break;
	}
}

void sub_menu_function2(int option)
{
	printf("submenu2 %d has been selected \n", option);

	switch (option) {
	case 1: handle[0] = true;
		handle[1] = false;
		handle[2] = false;
		break;
	case 2: handle[0] = false;
		handle[1] = true;
		handle[2] = false;
		break;
	case 3: handle[0] = false;
		handle[1] = false;
		handle[2] = true;
		break;
	};
}

void sub_menu_function3(int option)
{
	printf("submenu3 %d has been selected \n", option);

	switch (option) {
	case 0: canopy[0] = true;
		canopy[1] = false;
		canopy[2] = false;
		canopy[3] = false;
		break;
	case 1: canopy[0] = false;
		canopy[1] = true;
		canopy[2] = false;
		canopy[3] = false;
		break;
	case 2: canopy[0] = false;
		canopy[1] = false;
		canopy[2] = true;
		canopy[3] = false;
		break;
	case 3: canopy[0] = false;
		canopy[1] = false;
		canopy[2] = false;
		canopy[3] = true;
		break;
	};
}

void sub_menu_function4(int option)
{
	printf("submenu4 %d has been selected \n", option);

	switch (option) {
	case 1: glBindTexture(GL_TEXTURE_2D, bodyColor);
		pRGBImage = auxDIBImageLoadA("cyan.bmp");
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, pRGBImage->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case 2: glBindTexture(GL_TEXTURE_2D, bodyColor);
		pRGBImage = auxDIBImageLoadA("dark_blue.bmp");
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, pRGBImage->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case 3: glBindTexture(GL_TEXTURE_2D, bodyColor);
		pRGBImage = auxDIBImageLoadA("purple.bmp");
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, pRGBImage->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case 4: glBindTexture(GL_TEXTURE_2D, bodyColor);
		pRGBImage = auxDIBImageLoadA("gold.bmp");
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, pRGBImage->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;

	};
}


void sub_menu_function5(int option)
{
	printf("submenu5 %d has been selected \n", option);

	switch (option) {
	case 1: cradle_Mode = true;
		stoller_Mode = false;
		break;
	case 2: cradle_Mode = false;
		stoller_Mode = true;
		break;
	};
}


void main_menu_function(int option)
{
	printf("Main menu %d has been selected \n", option);
	if (option == 999)
		exit(0);

	switch (option) {
		// ���� ȸ��
	case 1: if (!(wheelRotation_On))
		wheelRotation_On = true;
			else
				wheelRotation_On = false;
		break;
		// ��ü ȸ��
	case 2: if (!(bodyRotation_On))
		bodyRotation_On = true;
			else
				bodyRotation_On = false;
		break;
	}
}


void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void draw_axis()
{
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 40, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1);
}


void idle()
{
	if (wheelRotation_On)
		wheelRotation_Angle += 10;
	if (wheelRotation_Angle >= 360)
		wheelRotation_Angle = 0;

	if (bodyRotation_On)
		bodyRotation_Angle += 2;
	if (bodyRotation_Angle >= 360)
		bodyRotation_Angle = 0;

	back_Mode_func();
	handle_func();
	mode_Angle_idle();
	canopy_Angle_idle();
	glutPostRedisplay();
}


void specialkeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		azimuth += 4;
		break;

	case GLUT_KEY_LEFT:
		azimuth -= 4;
		break;

	case GLUT_KEY_UP:
		elevation += 4;
		break;

	case GLUT_KEY_DOWN:
		elevation -= 4;
		break;
	}

	if (elevation >= 360)         // �ֱ� 360
		elevation -= 360;
	else if (elevation <= -360)
		elevation += 360;

	if (azimuth >= 360)         // �ֱ� 360
		azimuth -= 360;
	else if (azimuth <= -360)
		azimuth += 360;


	eyex = distance * cos(elevation * PI / 180) * sin(azimuth * PI / 180);
	eyey = distance * sin(elevation * PI / 180);
	eyez = distance * cos(elevation * PI / 180) * cos(azimuth * PI / 180);
}

void back_Mode_func()
{
	for (int i = 0; i<5; i++) {
		if (back_Mode[i]) {
			switch (i) {
			case 0:	if (back_Angle == 0) {}
					else { back_Angle -= 1; }
					break;
			case 1:	if (back_Angle < 5) { back_Angle += 1; }
					else if (back_Angle == 5) {}
					else if (back_Angle > 5) { back_Angle -= 1; }
					break;
			case 2:	if (back_Angle < 10) { back_Angle += 1; }
					else if (back_Angle == 10) {}
					else if (back_Angle > 10) { back_Angle -= 1; }
					break;
			case 3:	if (back_Angle < 15) { back_Angle += 1; }
					else if (back_Angle == 15) {}
					else if (back_Angle > 15) { back_Angle -= 1; }
					break;
			case 4:	if (back_Angle < 20) { back_Angle += 1; }
					else if (back_Angle == 20) {}
					else if (back_Angle > 20) { back_Angle -= 1; }
					break;
			}
		}
	}
}

void handle_func()
{
	for (int i = 0; i<3; i++) {
		if (handle[i]) {
			switch (i + 1)
			{
			case 1:	if (0 < handle_3 && handle_3 <= HANDLE_SIZE) { handle_3 -= 1; }
					else if (handle_3 == 0)
					{
						if (0 < handle_2 && handle_2 <= HANDLE_SIZE) { handle_2 -= 1; }
						else if (handle_2 == 0) {}
					}
					break;
			case 2: if (0 < handle_3 && handle_3 <= HANDLE_SIZE) { handle_3 -= 1; }
					else if (handle_3 == 0)
					{
						if (0 <= handle_2 && handle_2 < HANDLE_SIZE) { handle_2 += 1; }
						else if (handle_2 == HANDLE_SIZE) {}
					}
					break;
			case 3: if (0 <= handle_2 && handle_2 < HANDLE_SIZE) { handle_2 += 1; }
					else if (handle_2 == HANDLE_SIZE)
					{
						if (0 <= handle_3 && handle_3 < HANDLE_SIZE) { handle_3 += 1; }
						else if (handle_3 == HANDLE_SIZE) {}
					}
					break;
			};
		}
	}
}

void mode_Angle_idle()
{
	if (cradle_Mode)
	{
		if (0 < mode_Angle && mode_Angle <= 40)
			mode_Angle -= 1;
		else if (mode_Angle == 0) {}
	}
	else if (stoller_Mode)
	{
		if (0 <= mode_Angle && mode_Angle < 40)
			mode_Angle += 1;
		else if (mode_Angle == 40) {}
	}
}

void canopy_Angle_idle()
{
	for (int i = 0; i<4; i++) {
		if (canopy[i]) {
			switch (i)
			{
			case 0:	if (canopy_Angle0 == 0) {}
				else if (0 < canopy_Angle0) { canopy_Angle0 -= 1; }

				if (canopy_Angle1 == 0) {}
				else if (0 < canopy_Angle1) { canopy_Angle1 -= 1; }

				if (canopy_Angle2 == 0) {}
				else if (0 < canopy_Angle2 && canopy_Angle2 <= 30) { canopy_Angle2 -= 1; }
				break;

			case 1:	if (canopy_Angle0 == 30) {}
				else if (0 <= canopy_Angle0 && canopy_Angle0 < 30) { canopy_Angle0 += 1; }
				else if (30 < canopy_Angle0 && canopy_Angle0 <= 90) { canopy_Angle0 -= 1; }

				if (canopy_Angle1 == 0) {}
				else if (0 < canopy_Angle1 && canopy_Angle1 <= 60) { canopy_Angle1 -= 1; }

				if (canopy_Angle2 == 0) {}
				else if (0 < canopy_Angle2 && canopy_Angle2 <= 30) { canopy_Angle2 -= 1; }
				break;

			case 2:	if (canopy_Angle0 == 60) {}
				else if (canopy_Angle0 < 60) { canopy_Angle0 += 1; }
				else if (canopy_Angle0 > 60) { canopy_Angle0 -= 1; }

				if (canopy_Angle1 == 30) {}
				else if (canopy_Angle1 < 30) { canopy_Angle1 += 1; }
				else if (canopy_Angle1 > 30) { canopy_Angle1 -= 1; }

				if (canopy_Angle2 == 0) {}
				else if (canopy_Angle2 > 0) { canopy_Angle2 -= 1; }
				break;

			case 3:	if (canopy_Angle0 == 90) {}
				else if (canopy_Angle0 < 90) { canopy_Angle0 += 1; }

				if (canopy_Angle1 == 60) {}
				else if (canopy_Angle1 < 60) { canopy_Angle1 += 1; }

				if (canopy_Angle2 == 30) {}
				else if (canopy_Angle2 < 30) { canopy_Angle2 += 1; }
				break;
			};
		}
	}
}

