#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "BmpLoader.h"

#define ESCAPE 27
#define N_MOBIL 4
#define PI 3.14159265358979323846
using namespace std;

char title[] = "EAS CG | Febriyoga BS | 181511013";

unsigned int ID;

int Xsize = 1000, Ysize = 600, count = 1;
int night = 0;
int shoot = 0; 
int view = 0;
int mode = 0; 
int move = 1;
int car	 = 0;  
int th	 = 110;        
int ph	 = 0;     
int fov	 = 55;   
int light= 1;  
int refreshMills = 15;
int r = 1, g = 0, b = 0;

// Transformation values
float xangle = 10.0, yangle = 0.0, zangle = 0.0, angle = 0.0;
float xt = 0.0, yt = 0.0, zt =-21.0;
float xc[N_MOBIL] = {-4.0, 4.0, 10.0, -10.0}, yc[N_MOBIL] = {-1.0, -1.0, -1.0, -1.0}, zc[N_MOBIL] = {15.0, 16.0, 15.0, 15.0}, tc[N_MOBIL] = {50, 50, 50, 50};
float i, sudut;

double asp = 1.333;  
double dim = 8.0;

double fpX = 0;
double fpY = 0.7;
double fpZ =-0.3;

double refX = 5;
double refY = 0;
double refZ = 0;

double tpX = -4;
double tpY = 0;
double tpZ = 1.2;

// Light values
int one       =   1;  // Unit value
int distance  =   25; // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =  30;  // Emission intensity (%)
int ambient   =  80;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  = 13;   // Elevation of light

float Ambient[]   = {0.01*80 ,0.01*80 ,0.01*80 ,1.0};
float Diffuse[]   = {1.0,1.0,1.0,1.0};
float Specular[]  = {0.01*0,0.01*0,0.01*0,1.0};

GLfloat xs = 1.0, ys = 1.0, zs = 1.0;
GLuint 	_textureWall, _textureDoor1, _textureDoor2, _textureDoor3, _textureWindow, _textureWindow2, _textureFence, _textureFence2, _textureStreet, 
		_textureSand, _textureGrass, _textureWater, _textureRoof, _textureSky, _basic_metal, _glass, _textureStandWall, _textureStandSeats;
		
GLUquadricObj *t;

void Init();
void SpecialKeyHandler(int key, int x, int y);
void KeyboardHandler(unsigned char key, int x, int y);
void Reshape(GLsizei width, GLsizei height);
void ReSizeGLScene(GLint Width, GLint Height);
void Display();
void Timer(int value);
void display_string(int x, int y, char *string, int font);
void myMenu(int id);
void reset();

GLuint loadTexture(const char* filename);

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(Xsize, Ysize);
	glutInitWindowPosition(50, 50);
	glutCreateWindow(title);
	glEnable(GL_DEPTH_TEST);
	
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutReshapeFunc(ReSizeGLScene);
	
	glutSpecialFunc(SpecialKeyHandler);
	glutKeyboardFunc(KeyboardHandler);
	
	Init();
	
	glutCreateMenu(myMenu);
	glutAddMenuEntry("Day Mode", 1);
	glutAddMenuEntry("Night Mode", 2);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}

void myMenu(int id)
{
	if(id==1){
		night = 0;
//		glClearColor(1,1,1,1);
//		glDisable(GL_FOG);
//		glutPostRedisplay();
	}

	if(id==2){
		night = 1;
//		glClearColor(0.1,0.1,0.1,0);
//		GLfloat fogcolour[4]={0.0,0.0,0.0,1.0};
//		
//		glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
//		glFogf(GL_FOG_DENSITY,0.5);                   /* How dense */
//		glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
//		                         /* end */
//		glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
//		glEnable(GL_FOG);
	}
}

GLvoid Transform(GLfloat Width, GLfloat Height)
{
	glViewport(0, 0, Width, Height);              /* Set the viewport */
	glMatrixMode(GL_PROJECTION);                  /* Select the projection matrix */
	glLoadIdentity();    /* Reset The Projection Matrix */
	gluPerspective(45.0,Width/Height,0.1,100.0);  /* Calculate The Aspect Ratio Of The Window */
	glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */
}

GLvoid ReSizeGLScene(GLint Width, GLint Height)
{
  if (Height==0)     Height=1;                   /* Sanity checks */
  if (Width==0)      Width=1;
  Transform( Width, Height );                   /* Perform the transformation */
}

void Init(){
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -5.0, 1.0);
	
	glLineWidth(2.0);
	
	t=gluNewQuadric();
        gluQuadricDrawStyle(t, GLU_FILL);
        
//	// Create light components
//	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
//	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
//	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//	GLfloat position[] = { 1.5f, 1.0f, 4.0f, 1.0f };
//	
//	// Assign created components to GL_LIGHT0
//	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
//	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	
	_textureSky = loadTexture("sky.bmp");
	_textureStreet = loadTexture("street.bmp");
	_textureSand = loadTexture("sand.bmp");
	_textureGrass = loadTexture("grass.bmp");
	_textureWater = loadTexture("water.bmp");
	_textureWall = loadTexture("wall.bmp");
	_textureRoof = loadTexture("roof.bmp");
	_textureDoor1 = loadTexture("door1.bmp");
	_textureDoor2 = loadTexture("door2.bmp");
	_textureDoor3 = loadTexture("door3.bmp");
	_textureWindow = loadTexture("window.bmp");
	_textureWindow2 = loadTexture("window2.bmp");
	_textureFence = loadTexture("fence.bmp");
	_textureFence2 = loadTexture("fence2.bmp");
	_basic_metal = loadTexture("basic-metal.bmp");
	_glass = loadTexture("glass.bmp");
	_textureStandWall = loadTexture("stand-wall.bmp");
	_textureStandSeats = loadTexture("stand-seats.bmp");
}

void Arena(GLfloat x, GLfloat y, GLfloat z, GLfloat radius){
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle
	
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureGrass );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glColor3f(0.1,0.8,0.1);
        
		GLfloat twicePi = 2.0f * PI;
        
        glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0, 0, -1);
		//glTexCoord3f(0.5, 0.5);
		//glVertex3f(0, 0, -0.05);
		
		for(i = 0; i <= triangleAmount; i++){ 
			glTexCoord3f(0.5*(i/triangleAmount)+0.5, 0, 0.5*(i/triangleAmount)+0.5);
			glVertex3f(
		        x + (radius * cos(i *  twicePi / triangleAmount)), 
		        y,
			    z + (radius * sin(i * twicePi / triangleAmount))
			);
		}
		glEnd();
    glDisable(GL_TEXTURE_2D);
}

void display_string(int x, int y, char *string, int font)
{
    int len, i;
	
	glClearColor(1, 1, 1, 1);
	glColor3f(1.0,1.0,1.0);
	glRasterPos2f(x, y);
    len = (int) strlen(string);
    
	for (i = 0; i < len; i++) {
		if(font==1)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
		if(font==2)
    		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
 		if(font==3)
        	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,string[i]);
 		if(font==4)
    		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,string[i]);
 	}
}

void InitialScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	display_string(300, 540,"EAS KOMPUTER GRAFIK", 1); 
	display_string(350, 510, "FBS - 181511013", 1);
	display_string(10, 450, "PETUNJUK", 2);
	display_string(10, 420, "W-S UNTUK PERPINDAHAN KAMERA (DEPTH)", 3);
	display_string(10, 390, "A-D UNTUK PERPINDAHAN KAMERA (HORIZONTAL)", 3);
	display_string(10, 360, "Q-E UNTUK PERPINDAHAN KAMERA (VERTIKAL)", 3);
	display_string(10, 330, "R UNTUK MERESET KAMERA", 3);
	display_string(10, 300, "ARROW KEY UNTUK MENGGERAKAN MOBIL", 3);
	display_string(10, 270, ",-. UNTUK MENGELUARKAN TEMBAKAN", 3);
	display_string(10, 240, "ESC UNTUK KELUAR", 3);
	display_string(270, 130, "TEKAN (SPASI) UNTUK BERGANTI SCENE", 2);
	glutPostRedisplay();
	glutSwapBuffers();
}

void init()
{
	count = 1;
    glClearColor(0, 0, 0, 0);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 900.0, 0.0, 600.0, 50.0, -50.0);
	glutPostRedisplay();   // request redisplay
}

static void Bullet(float x, float y, float z){
	//  Save transformation
   glPushMatrix();
   	glTranslated(x, y, z);
   	glRotatef(xangle,1.0,0.0,0.0);
	glRotatef(yangle,0.0,1.0,0.0);
	glRotatef(zangle,0.0,0.0,1.0);
	GLUquadric *quadratico = gluNewQuadric();
		// Head
			glPushMatrix();
			glColor3f(0.0, 1.0, 0.0);
			gluCylinder(quadratico, 0.30, 0.50, 1.0, 10, 10);
			glPopMatrix();

		// Body
			glPushMatrix();
			glColor3f(0.0, 1.0, 0.0);
			gluCylinder(quadratico, 0.50, 0.50, 3.0, 10, 10);
			glPopMatrix();

		// Back
			glPushMatrix();
			glColor3f(1.0, 0.0, 0.0);
			gluSphere(quadratico, 1.0, 10, 10);
			glPopMatrix();
	glPopMatrix();
}

static void Vertex(double th,double ph)
{
   double x = sin(th)*cos(ph);
   double y = cos(th)*cos(ph);
   double z =         sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {1.0,1.0,1.0,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,0.9);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

static void lampPost(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   //glRotated(th,0,1,0);
	glRotatef(xangle,1.0,0.0,0.0);
	glRotatef(yangle,0.0,1.0,0.0);
	glRotatef(zangle,0.0,0.0,1.0);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   float radScale = 0.03;
   float baseRadScale = 0.05;
   float capRadScale = 0.01;
   float outerRadScale = 0.08;

   //Pole
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,_basic_metal);   
   glBegin(GL_QUADS);
   int i;
   for(i=45; i <= 360; i += 45) {
      //Base
      glNormal3f(cos(i-22.5),0,sin(i-22.5));
      glTexCoord2f(cos(i), 0); glVertex3f(baseRadScale*cos(i), 0, baseRadScale*sin(i));
      glTexCoord2f(cos(i), 0); glVertex3f(baseRadScale*cos(i-45), 0, baseRadScale*sin(i-45));
      glTexCoord2f(cos(i), 1); glVertex3f(radScale*cos(i-45), 0.1, radScale*sin(i-45));
      glTexCoord2f(cos(i), 1); glVertex3f(radScale*cos(i), 0.1, radScale*sin(i));
      //Pole Lower
      glNormal3f(cos(i-22.5),0,sin(i-22.5));
      glTexCoord2f(cos(i), 0); glVertex3f(radScale*cos(i), 0.1, radScale*sin(i));
      glTexCoord2f(cos(i), 0); glVertex3f(radScale*cos(i-45), 0.1, radScale*sin(i-45));
      glTexCoord2f(cos(i), 2); glVertex3f(radScale*cos(i-45), 0.5, radScale*sin(i-45));
      glTexCoord2f(cos(i), 2); glVertex3f(radScale*cos(i), 0.5, radScale*sin(i));
      //Pole Upper
      glNormal3f(cos(i-22.5),0,sin(i-22.5));
      glTexCoord2f(cos(i), 0); glVertex3f(radScale*cos(i), 0.5, radScale*sin(i));
      glTexCoord2f(cos(i), 0); glVertex3f(radScale*cos(i-45), 0.5, radScale*sin(i-45));
      glTexCoord2f(cos(i), 2); glVertex3f(radScale*cos(i-45), 1, radScale*sin(i-45));
      glTexCoord2f(cos(i), 2); glVertex3f(radScale*cos(i), 1, radScale*sin(i));
      //Light Cap
      glNormal3f(cos(i-22.5),0,sin(i-22.5));
      glTexCoord2f(cos(i), 0); glVertex3f(radScale*cos(i), 1.15, radScale*sin(i));
      glTexCoord2f(cos(i), 0); glVertex3f(radScale*cos(i-45), 1.15, radScale*sin(i-45));
      glTexCoord2f(cos(i), 1); glVertex3f(capRadScale*cos(i-45), 1.17, capRadScale*sin(i-45));
      glTexCoord2f(cos(i), 1); glVertex3f(capRadScale*cos(i), 1.17, capRadScale*sin(i));
   }
   glEnd();

   //Light
   float em[4] = {0.8, 0.8, 0.1, 1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,0);
   glMaterialfv(GL_FRONT,GL_SPECULAR,em);
   glMaterialfv(GL_FRONT,GL_EMISSION,em);
   glColor3f(0.8, 0.8, 0.1);
   glBindTexture(GL_TEXTURE_2D,_glass);   
   glBegin(GL_QUADS);
   i = 45;
   for(i = 45; i <= 360; i += 45) {
      //Lower
      glNormal3f(cos(i-22.5),0,sin(i-22.5));
      glTexCoord2f(cos(i), 0); glVertex3f(radScale*cos(i), 1, radScale*sin(i));
      glTexCoord2f(cos(i), 0); glVertex3f(radScale*cos(i-45), 1, radScale*sin(i-45));
      glTexCoord2f(cos(i), 1); glVertex3f(outerRadScale*cos(i-45), 1.1, outerRadScale*sin(i-45));
      glTexCoord2f(cos(i), 1); glVertex3f(outerRadScale*cos(i), 1.1, outerRadScale*sin(i));
      //Upper
      glNormal3f(cos(i-22.5),0,sin(i-22.5));
      glTexCoord2f(cos(i), 0); glVertex3f(outerRadScale*cos(i), 1.1, outerRadScale*sin(i));
      glTexCoord2f(cos(i), 0); glVertex3f(outerRadScale*cos(i-45), 1.1, outerRadScale*sin(i-45));
      glTexCoord2f(cos(i), 1); glVertex3f(radScale*cos(i-45), 1.15, radScale*sin(i-45));
      glTexCoord2f(cos(i), 1); glVertex3f(radScale*cos(i), 1.15, radScale*sin(i));
   }
   glEnd();

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Undo transformations
   glPopMatrix();
}

static void stand(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
	//  Save transformation
	glPushMatrix();
	//  Offset, scale and rotate
	glTranslated(x,y,z);
	glRotatef(xangle,1.0,0.0,0.0);
	glRotatef(yangle,0.0,1.0,0.0);
	glRotatef(zangle,0.0,0.0,1.0);
	
	glScaled(dx,dy,dz);
	
	//Texture repitition values
	float texRepX = 1.0;
	float texRepY = 1.0;
	
	glBindTexture(GL_TEXTURE_2D,_textureStandWall);
	float texScale = 0.5;
	glNormal3f( 0, 0, 1);
	//  Cube
	glBegin(GL_QUADS);
	//  Front
	texRepX = dx/texScale;
	texRepY = dy/texScale;
	glColor3f(0.7, 0.7, 0.7);
	
	glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
	glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
	glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
	glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
	//  Back
	texRepX = dx/texScale;
	texRepY = dy/texScale;
	glNormal3f( 0, 0,-1);
	glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
	glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
	glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+10,-1);
	glTexCoord2f(0.0,texRepY); glVertex3f(+1,+10,-1);
	//  Right
	texRepX = dz/texScale;
	texRepY = dy/texScale;
	glNormal3f(+1, 0, 0);
	glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,+1);
	glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
	glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+10,-1);
	glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,+1);
	//  Left
	texRepX = dz/texScale;
	texRepY = dy/texScale;
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
	glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,+1);
	glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,+1);
	glTexCoord2f(0.0,texRepY); glVertex3f(-1,+10,-1);
	
	//  Bottom
	texRepX = dx/texScale;
	texRepY = dz/texScale;
	glNormal3f( 0,-one, 0);
	glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
	glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
	glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
	glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);
	
	//  End
	glEnd();
	
	// Top
	glColor3f(0.7, 0.7, 0.7);
	glBindTexture(GL_TEXTURE_2D,_textureStandSeats);
	texScale = 0.5;
	glBegin(GL_QUADS);
	
	// -- Seats
	texRepX = dx/texScale;
	texRepY = dz/texScale;
	glNormal3f( 0,+1, 0);
	glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
	glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
	glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+10,-1);
	glTexCoord2f(0.0,texRepY); glVertex3f(-1,+10,-1);
	glEnd();
	
	//  Undo transofrmations
	glPopMatrix();
}

static void mobil (float x, float y, float z,
                 float dx, float dy, float dz,
                 float xa, float ya, float za,
                 float cr, float cb, float cg){
	///////////////////////////////////
	/** MOBIL PICK-UP **/
	///////////////////////////////////
	//  Save transformation
    glPushMatrix();
    //  Offset
    glTranslatef(x,y,z);
    //glScalef(dx,dy,dz);
    
    glRotatef(xa,1.0,0.0,0.0);
	glRotatef(ya,0.0,1.0,0.0);
	glRotatef(za,0.0,0.0,1.0);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glPointSize(100.0); 
	
		//BADAN MOBIL 
		glColor3f(cr, cg, cb);              
			
			/* SISI MIRING DEPAN */       
			glBegin(GL_QUADS);  
			glVertex3f(1.6, 0.7, 0.6);
			glVertex3f(1.6, 0.7, 0.2);
			glVertex3f(1.8, 0.45, 0.2);
			glVertex3f(1.8, 0.45, 0.6);
			glEnd();
			
			/* SISI DEPAN */   
			glBegin(GL_QUADS);      
			glVertex3f(1.8, 0.2, 0.6);
			glVertex3f(1.8, 0.2, 0.2);
			glVertex3f(1.8, 0.45, 0.2);
			glVertex3f(1.8, 0.45, 0.6);
			glEnd();
			
			/* SISI ATAS */
			glBegin(GL_QUADS);
			glVertex3f(1.3,0.7,0.6);
			glVertex3f(1.3,0.7,0.2);
			glVertex3f(1.6,0.7,0.2);
			glVertex3f(1.6,0.7,0.6);
			glEnd();
			
			/* SISI BAWAH */
			glBegin(GL_QUADS);
			glVertex3f(1.3,0.2,0.6);
			glVertex3f(1.3,0.2,0.2);
			glVertex3f(1.8,0.2,0.2);
			glVertex3f(1.8,0.2,0.6);
			glEnd();
			
			/* SISI BELAKANG */
			glBegin(GL_QUADS);
			glVertex3f(1.3, 0.7, 0.6);
			glVertex3f(1.3, 0.7, 0.2);
			glVertex3f(1.3, 0.2, 0.2);
			glVertex3f(1.3, 0.2, 0.6);
			glEnd();
			
			/* SISI KIRI BAWAH */
			glBegin(GL_QUADS);
			glVertex3f(1.3,0.2,0.2);
			glVertex3f(1.3,0.45,0.2);
			glVertex3f(1.8,0.45,0.2);
			glVertex3f(1.8,0.2,0.2);
			glEnd();
			
			/* SISI KANAN BAWAH*/
			glBegin(GL_QUADS);
			glVertex3f(1.3,0.2,0.6);
			glVertex3f(1.3,0.45,0.6);
			glVertex3f(1.8,0.45,0.6);
			glVertex3f(1.8,0.2,0.6);
			glEnd();
			
			/* SISI KIRI ATAS */
			glBegin(GL_QUADS);
			glVertex3f(1.6, 0.7, 0.21);
			glVertex3f(1.3, 0.7, 0.21);
			glVertex3f(1.3, 0.45, 0.21);
			glVertex3f(1.8, 0.45, 0.21);      
			glEnd();
			
			/* SISI KANAN ATAS */
			glBegin(GL_QUADS);
			glVertex3f(1.6, 0.7, 0.59);
			glVertex3f(1.3, 0.7, 0.59);
			glVertex3f(1.3, 0.45, 0.59);
			glVertex3f(1.8, 0.45, 0.59);
			glEnd();
			
			///////////////////////////////////
			// BAGIAN BAK MOBIL PICK-UP
			///////////////////////////////////
			
			/* SISI BELAKANG */
			glBegin(GL_QUADS);
			glVertex3f(0.3,0.25,0.6);
			glVertex3f(0.3,0.5,0.6);
			glVertex3f(0.3,0.5,0.2);
			glVertex3f(0.3,0.25,0.2);
			glEnd();
			
			/* SISI KIRI */
			glBegin(GL_QUADS);
			glVertex3f(0.3,0.25,0.6);
			glVertex3f(1.3,0.25,0.6);
			glVertex3f(1.3,0.5,0.6);
			glVertex3f(0.3,0.5,0.6);
			glEnd();
	
			/* SISI KANAN */
			glBegin(GL_QUADS);
			glVertex3f(0.3,0.25,0.2);
			glVertex3f(1.3,0.25,0.2);
			glVertex3f(1.3,0.5,0.2);
			glVertex3f(0.3,0.5,0.2);
			glEnd();
	
			/* SISI BAWAH */
			glBegin(GL_QUADS);
			glVertex3f(0.3,0.25,0.6);
			glVertex3f(0.3,0.25,0.2);
			glVertex3f(1.3,0.25,0.2);
			glVertex3f(1.3,0.25,0.6);
			glEnd();

		// KACA MOBIL
		glBegin(GL_QUADS);
		glColor3f(0.3,0.3,0.3);
		
			/* KIRI */
			glVertex3f(1.55, 0.65, 0.2);
			glVertex3f(1.35, 0.65, 0.2);
			glVertex3f(1.35, 0.5, 0.2);
			glVertex3f(1.65, 0.5, 0.2);
			
			/* KANAN */
			glVertex3f(1.55, 0.65, 0.6);
			glVertex3f(1.35, 0.65, 0.6);
			glVertex3f(1.35, 0.5, 0.6);
			glVertex3f(1.65, 0.5, 0.6);

			/* DEPAN */
			glVertex3f(1.65, 0.65, 0.55);
			glVertex3f(1.65, 0.65, 0.25);
			glVertex3f(1.78, 0.5, 0.25);
			glVertex3f(1.78, 0.5, 0.55);

			/* BELAKANG */
			glVertex3f(1.29, 0.65, 0.55);
			glVertex3f(1.29, 0.65, 0.25);
			glVertex3f(1.29, 0.55, 0.25);
			glVertex3f(1.29, 0.55, 0.55);
		glEnd();
		
		///////////////////////////////////
		// KNALPOT
		///////////////////////////////////
		glPushMatrix();
			glColor3f(0.7, 0.7, 0.7);
			glTranslatef(0.2, 0.25, 0.5);
			glRotatef(90.0, 0, 1, 0);
				gluCylinder(t, 0.02, 0.03, 0.5, 10, 10);
		glPopMatrix();
		
		///////////////////////////////////
		// RODA
		///////////////////////////////////
		glPushMatrix();
		//JARI-JARI
			glColor3f(0.7,0.7,0.7);
			// KANAN BELAKANG
			glBegin(GL_LINE_STRIP);
				for(sudut=0;sudut<360;sudut=sudut+20){
					glVertex3f(0.6,0.2,0.62);
					glVertex3f(0.6+(0.08*(cos(((sudut+angle)*3.14)/180))),0.2+(0.08*(sin(((sudut+angle)*3.14)/180))),0.62);
				}
			glEnd();
	
			// KIRI BELAKANG
			glBegin(GL_LINE_STRIP);
				for(sudut=0;sudut<360;sudut=sudut+20){
					glVertex3f(0.6,0.2,0.18);
					glVertex3f(0.6+(0.08*(cos(((sudut+angle)*3.14)/180))),0.2+(0.08*(sin(((sudut+angle)*3.14)/180))),0.18);
				}
			glEnd();
	
			// KIRI DEPAN
			glBegin(GL_LINE_STRIP);
				for(sudut=0;sudut<360;sudut=sudut+20){
					glVertex3f(1.55,0.2,0.18);
					glVertex3f(1.55+(0.08*(cos(((sudut+angle)*3.14)/180))),0.2+(0.08*(sin(((sudut+angle)*3.14)/180))),0.18);
				}
			glEnd();
	
			// KANAN DEPAN
			glBegin(GL_LINE_STRIP);
				for(sudut=0;sudut<360;sudut=sudut+20){
					glVertex3f(1.55,0.2,0.62);
					glVertex3f(1.55+(0.08*(cos(((sudut+angle)*3.14)/180))),0.2+(0.08*(sin(((sudut+angle)*3.14)/180))),0.62);
				}
			glEnd();
			
		//BAN
			glColor3f(1,1,1);
			// KANAN BELAKANG
			glTranslatef(0.6, 0.2, 0.6);
			glutSolidTorus(0.025, 0.07, 10, 25);
	
			// KIRI BELAKANG
			glTranslatef(0, 0 , -0.4);
			glutSolidTorus(0.025, 0.07, 10, 25);
	
			// KIRI DEPAN
			glTranslatef(0.95, 0, 0);
			glutSolidTorus(0.025, 0.07, 10, 25);
	
			// KANAN DEPAN
			glTranslatef(0, 0, 0.4);
			glutSolidTorus(0.025, 0.07, 10, 25);
		glPopMatrix();
	glPopMatrix();
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(view == 0){
		init();
		glEnable(GL_LIGHTING); 
			InitialScene();
		glDisable(GL_LIGHTING);
	} else {
		if(count == 1){
			Transform(Xsize, Ysize);
			count = 0;
		}
//		if (mode == 1){
//	      	double Ex = -2*dim*sin(th)*cos(ph);
//	      	double Ey = +2*dim        *sin(ph);
//	      	double Ez = +2*dim*cos(th)*cos(ph);
//	      
//	      	gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,cos(ph),0);
//	    } else if(mode == 2){
//	   	  	refX = (dim * sin(th)) + fpX;
//	      	refY = (dim * sin(ph));
//	      	refZ = (dim * -cos(th)) + fpZ;
//	      	if(tpX > 0) {
//	      		gluLookAt(tpX,tpY,tpZ, tpX+2,tpY,tpZ, 0,1,0);
//			} else if(tpX < 0) 
//		  		gluLookAt(tpX,tpY,tpZ, tpX-2,tpY,tpZ, 0,1,0);
//	   }
//	   //  First Person
//	   else{
//	      refX = (dim * sin(th)) + fpX;
//	      refY = (dim * sin(ph));
//	      refZ = (dim * -cos(th)) + fpZ;
//	      gluLookAt(fpX, fpY, fpZ, refX, refY, refZ, 0, 1, 0);
//	   }
		
		// WORLD
	    glPushMatrix();
	    	glTranslatef(xt, yt, zt);
	        glRotatef(xangle,1.0,0.0,0.0);
			glRotatef(yangle,0.0,1.0,0.0);
			glRotatef(zangle,0.0,0.0,1.0);
				Arena(0, -1.4999, 10, 25);
		glPopMatrix();
		
		// CAR
		mobil(xc[0]+xt, yc[0]+yt, zc[0]+zt, 1, 1, 1, xangle, yangle, zangle, 1, 0, 0);
		
		mobil(xc[1]+xt, yc[1]+yt, zc[1]+zt, 1, 1, 1, xangle, yangle+180, zangle, 0, 1, 0);
		
		mobil(xc[2]+xt, yc[2]+yt, zc[2]+zt, 1, 1, 1, xangle, yangle+180, zangle, 0, 0, 1);
		
		mobil(xc[3]+xt, yc[3]+yt, zc[3]+zt, 1, 1, 1, xangle, yangle, zangle, 1, 0, 1);
		
		//BULLET
		if (shoot == 1){
			Bullet(xc[car]-1.5, yc[car], zc[car]);
			Bullet(xc[car]-3.5, yc[car], zc[car]);
			Bullet(xc[car]-5.5, yc[car], zc[car]);
			Bullet(xc[car]-7.5, yc[car], zc[car]);
		} else if (shoot == 2){
			Bullet(xc[car]+1.5, yc[car], zc[car]);
			Bullet(xc[car]+3.5, yc[car], zc[car]);
			Bullet(xc[car]+5.5, yc[car], zc[car]);
			Bullet(xc[car]+7.5, yc[car], zc[car]);
		}
	
	    // SKY
	    glPushMatrix();
	    	glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureSky);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        glTranslatef(xt, yt, zt);
	        glRotatef(xangle,1.0,0.0,0.0);
			glRotatef(yangle,0.0,1.0,0.0);
			glRotatef(zangle,0.0,0.0,1.0);
	        glBegin(GL_QUADS); //Belakang
	            glTexCoord3f(1.0,1.0,0);  glVertex3f(-50,50,-50);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-50,-50,-50);
	            glTexCoord3f(1.0,0.0,0);  glVertex3f(50,-50,-50);
	            glTexCoord3f(0.0,1.0,0);  glVertex3f(50,50,-50);
	        glEnd();
	        glBegin(GL_QUADS); //Depan
	            glTexCoord3f(1.0,1.0,0);  glVertex3f(-50,50,50);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-50,-50,50);
	            glTexCoord3f(1.0,0.0,0);  glVertex3f(50,-50,50);
	            glTexCoord3f(0.0,1.0,0);  glVertex3f(50,50,50);
	        glEnd();
	        glBegin(GL_QUADS); //Kanan
	            glTexCoord3f(1.0,1.0,0);  glVertex3f(50,50,-50);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(50,-50,-50);
	            glTexCoord3f(1.0,0.0,0);  glVertex3f(50,-50,50);
	            glTexCoord3f(0.0,1.0,0);  glVertex3f(50,50,50);
	        glEnd();
	        glBegin(GL_QUADS); //Kiri
	            glTexCoord3f(1.0,1.0,0);  glVertex3f(-50,50,-50);
	            glTexCoord3f(0.0,0.0,0);  glVertex3f(-50,-50,-50);
	            glTexCoord3f(1.0,0.0,0);  glVertex3f(-50,-50,50);
	            glTexCoord3f(0.0,1.0,0);  glVertex3f(-50,50,50);
	        glEnd();
	        glDisable(GL_TEXTURE_2D);
	    glPopMatrix();
	    
	    // LAMP
	    if(night == 1){
		   lampPost( 3, 0.1,-0.1, 1,1,1, 0);
		   lampPost( 3, 0.1, 3.1, 1,1,1, 0);
		   lampPost(-3, 0.1,-0.1, 1,1,1, 0);
		   lampPost(-3, 0.1, 3.1, 1,1,1, 0);
		}
		
		// STANDS
		stand(xt+15, yt+1.5, zt+6, 1, 1, 1, th);
	    
	    // STREET
	    glPushMatrix();
	    	glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureStreet );
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        glTranslatef(xt, yt, zt);
	        glRotatef(xangle,1.0,0.0,0.0);
			glRotatef(yangle,0.0,1.0,0.0);
			glRotatef(zangle,0.0,0.0,1.0);
	        glBegin(GL_QUADS);
	            glTexCoord3f(0.0,70.0,1);  	glVertex3f(-24,-1.48,10);
	            glTexCoord3f(0.0,0.0,1);  	glVertex3f(-24,-1.48,3);
	            glTexCoord3f(70.0,0.0,1);  	glVertex3f(24,-1.48,3);
	            glTexCoord3f(70.0,70.0,1);  glVertex3f(24,-1.48,10);
	        glEnd();
	        
	        glBegin(GL_QUADS);
	            glTexCoord3f(0.0,70.0,1); 	glVertex3f(0.5,-1.48,0.5);
	            glTexCoord3f(0.0,0.0,1);  	glVertex3f(0.5,-1.48,3.1);
	            glTexCoord3f(70.0,0.0,1);  	glVertex3f(1.5,-1.48,3.1);
	            glTexCoord3f(70.0,70.0,1);  glVertex3f(1.5,-1.48,0.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);
	            glTexCoord3f(0.0,70.0,1); 	glVertex3f(-3.5,-1.48,0.5);
	            glTexCoord3f(0.0,0.0,1);  	glVertex3f(-3.5,-1.48,3.1);
	            glTexCoord3f(70.0,0.0,1);  	glVertex3f(-1.5,-1.48,3.1);
	            glTexCoord3f(70.0,70.0,1);  glVertex3f(-1.5,-1.48,0.5);
	        glEnd();
	        glDisable(GL_TEXTURE_2D);
	    glPopMatrix();
	    
	    // GRASS
		glPushMatrix();
	    	glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureGrass );
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        glTranslatef(xt, yt, zt);
	        glRotatef(xangle,1.0,0.0,0.0);
			glRotatef(yangle,0.0,1.0,0.0);
			glRotatef(zangle,0.0,0.0,1.0);
			
	        glBegin(GL_QUADS);
	            glTexCoord3f(0.0,70.0,1);  glVertex3f(-5,-1.49,2.8);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-5,-1.49,-6.8);
	            glTexCoord3f(70.0,0.0,1);  glVertex3f(5,-1.49,-6.8);
	            glTexCoord3f(70.0,70.0,1);  glVertex3f(5,-1.49,2.8);
	        glEnd();
	        
	        glDisable(GL_TEXTURE_2D);
	    glPopMatrix();
	    
	     // SAND
	    glPushMatrix();
	    	glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureSand );
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        glTranslatef(xt, yt, zt);
	        glRotatef(xangle,1.0,0.0,0.0);
			glRotatef(yangle,0.0,1.0,0.0);
			glRotatef(zangle,0.0,0.0,1.0);
			glBegin(GL_QUADS);
	            glTexCoord3f(0.0,20.0,1); 	glVertex3f(0.5,-1.48,-2.4);
	            glTexCoord3f(0.0,0.0,1);  	glVertex3f(0.5,-1.48,-3);
	            glTexCoord3f(20.0,0.0,1);  	glVertex3f(3.6,-1.48,-3);
	            glTexCoord3f(20.0,70.0,1);  glVertex3f(3.6,-1.48,-2.4);
	        glEnd();
			
	        glBegin(GL_QUADS);
	            glTexCoord3f(0.0,20.0,1); 	glVertex3f(4.2,-1.48,1.5);
	            glTexCoord3f(0.0,0.0,1);  	glVertex3f(4.2,-1.48,-3);
	            glTexCoord3f(20.0,0.0,1);  	glVertex3f(3.6,-1.48,-3);
	            glTexCoord3f(20.0,20.0,1);  glVertex3f(3.6,-1.48,1.5);
	        glEnd();
	        
	        glBegin(GL_QUADS);
	            glTexCoord3f(0.0,20.0,1); 	glVertex3f(4.2,-1.48,1.5);
	            glTexCoord3f(0.0,0.0,1);  	glVertex3f(4.2,-1.48,2.1);
	            glTexCoord3f(20.0,0.0,1);  	glVertex3f(1.5,-1.48,2.1);
	            glTexCoord3f(20.0,20.0,1);  glVertex3f(1.5,-1.48,1.5);
	        glEnd();
	        
	        glBegin(GL_QUADS); 
	            glTexCoord3f(0.0,20.0,1);    glVertex3f(0.5,-1.48,-3.5);
	            glTexCoord3f(0.0,0.0,1);    glVertex3f(1.5,-1.48,-3.5);
	            glTexCoord3f(20.0,0.0,1);    glVertex3f(1.5,-1.48,-2);
	            glTexCoord3f(20.0,20.0,1);    glVertex3f(0.5,-1.48,-2);
		   glEnd();
	        glDisable(GL_TEXTURE_2D);
	    glPopMatrix();
	    
	    // POOL
		glPushMatrix();
	    	glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureWater );
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        glTranslatef(xt, yt, zt);
	        glRotatef(xangle,1.0,0.0,0.0);
			glRotatef(yangle,0.0,1.0,0.0);
			glRotatef(zangle,0.0,0.0,1.0);
	        glBegin(GL_QUADS);
	            glTexCoord3f(0.0,70.0,1);  glVertex3f(-4,-1.48,-3.5);
	            glTexCoord3f(0.0,0.0,1);  glVertex3f(-4,-1.48,-6);
	            glTexCoord3f(70.0,0.0,1);  glVertex3f(4.2,-1.48,-6);
	            glTexCoord3f(70.0,70.0,1);  glVertex3f(4.2,-1.48,-3.5);
	        glEnd();
	        glDisable(GL_TEXTURE_2D);
	    glPopMatrix();
		
		// RUMAH
	    glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureWall);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				
			    glTranslatef(xt, yt, zt);
			    glRotatef(xangle,1.0,0.0,0.0);
				glRotatef(yangle,0.0,1.0,0.0);
				glRotatef(zangle,0.0,0.0,1.0);
				
				//Dinding Depan
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1,1.5,0.5);
			        glTexCoord3f(4.0,1.5,0.1);  glVertex3f(3,0.5,0.5);
			        glTexCoord3f(4.0,0.0,0.1);  glVertex3f(3,-1.5,0.5);
			        glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1,-1.5,0.5);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1,0.5,2);
			        glTexCoord3f(4.0,2.0,0.1);  glVertex3f(-4,0.5,2);
			        glTexCoord3f(4.0,0.0,0.1);  glVertex3f(-4,-1.5,2);
			        glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1,-1.5,2);
			    glEnd();
			    
			    //Dinding Belakang
		        glBegin(GL_QUADS);
		            glTexCoord3f(0.0,2.0,-1);  	glVertex3f(-1.0,1.5,-2);
		            glTexCoord3f(4.0,1.5,-1);  	glVertex3f(3,0.5,-2);
		            glTexCoord3f(4.0,0.0,-1);  	glVertex3f(3,-1.5,-2);
		            glTexCoord3f(0.0,0.0,-1);  	glVertex3f(-1,-1.5,-2);
		        glEnd();
		        glBegin(GL_QUADS);
			        glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1,0.5,-2);
			        glTexCoord3f(4.0,2.0,0.1);  glVertex3f(-4,0.5,-2);
			        glTexCoord3f(4.0,0.0,0.1);  glVertex3f(-4,-1.5,-2);
			        glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1,-1.5,-2);
			    glEnd();
			    
			    //Dinding Tengah
			    glBegin(GL_QUADS);
		            glTexCoord3f(0.0,2.0,1);    glVertex3f(-1,0.5,0.5);
		            glTexCoord3f(2.0,2.0,-1);   glVertex3f(-1,0.5,2);
		            glTexCoord3f(2.0,0.0,-1);   glVertex3f(-1,-1.5,2);
		            glTexCoord3f(0.0,0.0,1);    glVertex3f(-1,-1.5,0.5);
		        glEnd();
		        glBegin(GL_QUADS);
		            glTexCoord3f(0.0,2.0,1);    glVertex3f(-1,0.5,0.5);
		            glTexCoord3f(2.0,2.0,-1);   glVertex3f(-1,0.5,-2);
		            glTexCoord3f(2.0,0.0,-1);   glVertex3f(-1,1.5,-2);
		            glTexCoord3f(0.0,0.0,1);    glVertex3f(-1,1.5,0.5);
		        glEnd();
		    
		    	//Dinding Kanan
		        glBegin(GL_QUADS);
		            glTexCoord3f(0.0,2.0,1);	glVertex3f(3,0.5,0.5);
		            glTexCoord3f(2.0,2.0,-1); 	glVertex3f(3,0.5,-2);
		            glTexCoord3f(2.0,0.0,-1); 	glVertex3f(3,-1.5,-2);
		            glTexCoord3f(0.0,0.0,1); 	glVertex3f(3,-1.5,0.5);
		        glEnd();
		
				//Dinding Kiri
		        glBegin(GL_QUADS);
		            glTexCoord3f(0.0,2.0,1);    glVertex3f(-4,0.5,2);
		            glTexCoord3f(2.0,2.0,-1);   glVertex3f(-4,0.5,-2);
		            glTexCoord3f(2.0,0.0,-1);   glVertex3f(-4,-1.5,-2);
		            glTexCoord3f(0.0,0.0,1);    glVertex3f(-4,-1.5,2);
		        glEnd();
		        
		        //Dinding Atas
		        glBegin(GL_QUADS);
		            glTexCoord3f(0.0,2.0,1);    glVertex3f(-1,0.5,2);
		            glTexCoord3f(2.0,2.0,-1);   glVertex3f(-1,0.5,-2);
		            glTexCoord3f(2.0,0.0,-1);   glVertex3f(-4,0.5,-2);
		            glTexCoord3f(0.0,0.0,1);    glVertex3f(-4,0.5,2);
		        glEnd();
	        glDisable(GL_TEXTURE_2D);
	        
	        //Pintu
			glEnable(GL_TEXTURE_2D);
		        glBindTexture(GL_TEXTURE_2D, _textureDoor1);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		        
		        glBegin(GL_QUADS);  
		            glTexCoord3f(0.0,1.0,10001);    glVertex3f(0.5,-0.4,0.5011);
		            glTexCoord3f(1.0,1.0,10001);    glVertex3f(1.5,-0.4,0.5011);
		            glTexCoord3f(1.0,0.0,10001);    glVertex3f(1.5,-1.5,0.5011);
		            glTexCoord3f(0.0,0.0,10001);    glVertex3f(0.5,-1.5,0.5011);
		        glEnd();
		    glDisable(GL_TEXTURE_2D);
		    
		    glEnable(GL_TEXTURE_2D);
		        glBindTexture(GL_TEXTURE_2D, _textureDoor2);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		        
		        glBegin(GL_QUADS); 
		            glTexCoord3f(0.0,1.0,10001);    glVertex3f(0.5,-0.4,-2.001);
		            glTexCoord3f(1.0,1.0,10001);    glVertex3f(1.5,-0.4,-2.001);
		            glTexCoord3f(1.0,0.0,10001);    glVertex3f(1.5,-1.5,-2.001);
		            glTexCoord3f(0.0,0.0,10001);    glVertex3f(0.5,-1.5,-2.001);
		        glEnd();
		    glDisable(GL_TEXTURE_2D);
		    
		    glEnable(GL_TEXTURE_2D);
		        glBindTexture(GL_TEXTURE_2D, _textureDoor3);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		        
		        glBegin(GL_QUADS); 
		            glTexCoord3f(0.0,1.0,10001);    glVertex3f(-1.4,0,2.001);
		            glTexCoord3f(1.0,1.0,10001);    glVertex3f(-3.6,0,2.001);
		            glTexCoord3f(1.0,0.0,10001);    glVertex3f(-3.6,-1.5,2.001);
		            glTexCoord3f(0.0,0.0,10001);    glVertex3f(-1.4,-1.5,2.001);
		        glEnd();
		    glDisable(GL_TEXTURE_2D);
				    
		    //Jendela
		    glEnable(GL_TEXTURE_2D);
		        glBindTexture(GL_TEXTURE_2D, _textureWindow);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		        
		        glBegin(GL_QUADS); 
		            glTexCoord3f(0.0,1.0,1.0);    glVertex3f(-0.5,-0,0.5011);
		            glTexCoord3f(1.0,1.0,1.0);    glVertex3f(0,0,0.5011);
		            glTexCoord3f(1.0,0.0,1.0);    glVertex3f(0,-1,0.5011);
		            glTexCoord3f(0.0,0.0,1.0);    glVertex3f(-0.5,-1,0.5011);
		        glEnd();
		        
		        glBegin(GL_QUADS); 
		            glTexCoord3f(0.0,1.0,1.0);    glVertex3f(2,-0,0.5011);
		            glTexCoord3f(1.0,1.0,1.0);    glVertex3f(2.5,0,0.5011);
		            glTexCoord3f(1.0,0.0,1.0);    glVertex3f(2.5,-1,0.5011);
		            glTexCoord3f(0.0,0.0,1.0);    glVertex3f(2,-1,0.5011);
		        glEnd();
		        
		        glBegin(GL_QUADS); 
		            glTexCoord3f(0.0,1.0,1.0);    glVertex3f(-0.99,0,1.5);
		            glTexCoord3f(1.0,1.0,1.0);    glVertex3f(-0.99,0,1);
		            glTexCoord3f(1.0,0.0,1.0);    glVertex3f(-0.99,-1,1);
		            glTexCoord3f(0.0,0.0,1.0);    glVertex3f(-0.99,-1,1.5);
		        glEnd();
		        
		        glBegin(GL_QUADS); 
		            glTexCoord3f(0.0,1.0,1.0);    glVertex3f(-0.5,-0,-2.001);
		            glTexCoord3f(1.0,1.0,1.0);    glVertex3f(0,0,-2.001);
		            glTexCoord3f(1.0,0.0,1.0);    glVertex3f(0,-1,-2.001);
		            glTexCoord3f(0.0,0.0,1.0);    glVertex3f(-0.5,-1,-2.001);
		        glEnd();
		        
		        glBegin(GL_QUADS); 
		            glTexCoord3f(0.0,1.0,1.0);    glVertex3f(2,0,-2.001);
		            glTexCoord3f(1.0,1.0,1.0);    glVertex3f(2.5,0,-2.001);
		            glTexCoord3f(1.0,0.0,1.0);    glVertex3f(2.5,-1,-2.001);
		            glTexCoord3f(0.0,0.0,1.0);    glVertex3f(2,-1,-2.001);
		        glEnd();
		    glDisable(GL_TEXTURE_2D);
		    
		    glEnable(GL_TEXTURE_2D);
		        glBindTexture(GL_TEXTURE_2D, _textureWindow2);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		        glBegin(GL_QUADS);
		            glTexCoord3f(0.0,1.0,1.0);    glVertex3f(-1.01,0.75,0);
		            glTexCoord3f(1.0,1.0,1.0);	  glVertex3f(-1.01,0.75,-1.5);
		            glTexCoord3f(1.0,0.0,1.0);    glVertex3f(-1.01,1.25,-1.5);
		            glTexCoord3f(0.0,0.0,1.0);    glVertex3f(-1.01,1.25,0);
		        glEnd();
		        
		        glBegin(GL_QUADS);
		            glTexCoord3f(0.0,1.0,1.0);    glVertex3f(-2.0,0.25,-2.001);
		            glTexCoord3f(1.0,1.0,1.0);	  glVertex3f(-2.0,-0.25,-2.001);
		            glTexCoord3f(1.0,0.0,1.0);    glVertex3f(-3.0,-0.25,-2.001);
		            glTexCoord3f(0.0,0.0,1.0);    glVertex3f(-3.0,0.25,-2.001);
		        glEnd();
		    glDisable(GL_TEXTURE_2D);
			
			//Pagar
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureFence);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				
				//Depan Kanan
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(1.5,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(1.5,-1.5,3);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(5,-1.5,3);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(5,-1,2.8);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(1.5,-1,2.8);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(1.5,-1.5,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(5,-1.5,2.8);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(1.5,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(1.5,-1,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(5,-1,2.8);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(1.5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(1.5,-1.5,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(1.5,-1.5,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(1.5,-1,2.8);
			    glEnd();
			    
			    //Depan Tengah
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(0.5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-1.5,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-1.5,-1.5,3);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(0.5,-1.5,3);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(0.5,-1,2.8);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-1.5,-1,2.8);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-1.5,-1.5,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(0.5,-1.5,2.8);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(0.5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-1.5,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-1.5,-1,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(0.5,-1,2.8);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-1.5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-1.5,-1.5,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-1.5,-1.5,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-1.5,-1,2.8);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(0.5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(0.5,-1.5,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(0.5,-1.5,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(0.5,-1,2.8);
			    glEnd();
			    
			    //Depan Kiri
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-3.5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-5,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-5,-1.5,3);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-3.5,-1.5,3);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-3.5,-1,2.8);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-5,-1,2.8);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-5,-1.5,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-3.5,-1.5,2.8);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-3.5,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-3.5,-1,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-5,-1,2.8);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-3.5,-1,3);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-3.5,-1.5,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-3.5,-1.5,2.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-3.5,-1,2.8);
			    glEnd();
			    
			    //Belakang
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(5,-1,-7);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-5,-1,-7);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-5,-1.5,-7);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(5,-1.5,-7);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(5,-1,-6.8);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-5,-1,-6.8);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-5,-1.5,-6.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(5,-1.5,-6.8);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(5,-1,-7);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-5,-1,-7);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-5,-1,-6.8);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(5,-1,-6.8);
			    glEnd();
			    
			    //Kanan
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(5,-1,-7);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(5,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(5,-1.5,3);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(5,-1.5,-7);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(4.8,-1,-7);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(4.8,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(4.8,-1.5,3);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(4.8,-1.5,-7);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(4.8,-1,-7);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(4.8,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(5,-1,3);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(5,-1,-7);
			    glEnd();
			    
			    //Kiri
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-5,-1,-7);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-5,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-5,-1.5,3);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-5,-1.5,-7);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-4.8,-1,-7);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-4.8,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-4.8,-1.5,3);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-4.8,-1.5,-7);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-5,-1,-7);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-5,-1,3);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-4.8,-1,3);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-4.8,-1,-7);
			    glEnd();
			glDisable(GL_TEXTURE_2D);
			
			//Gapura
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureFence2);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    	//Kiri
		    	glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(0.51,0,3.01);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(0,0,3.01);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(0,-1.5,3.01);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(0.51,-1.5,3.01);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(0.51,0,2.79);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(0,0,2.79);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(0,-1.5,2.79);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(0.51,-1.5,2.79);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(0.51,0,2.79);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(0.51,0,3.01);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(0.51,-1.5,3.01);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(0.51,-1.5,2.79);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(0,0,2.79);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(0,0,3.01);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(0,-1.5,3.01);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(0,-1.5,2.79);
			    glEnd();
			    
			    //Kanan
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(2,0,3.01);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(1.49,0,3.01);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(1.49,-1.5,3.01);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(2,-1.5,3.01);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(2,0,2.79);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(1.49,0,2.79);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(1.49,-1.5,2.79);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(2,-1.5,2.79);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(2,0,2.79);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(2,0,3.01);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(2,-1.5,3.01);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(2,-1.5,2.79);
			        glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(1.49,0,2.79);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(1.49,0,3.01);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(1.49,-1.5,3.01);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(1.49,-1.5,2.79);
			    glEnd();
			    
			    //Atas
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-0.2,0,3.2);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(2.2,0,3.2);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(2.2,0.2,3.2);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-0.2,0.2,3.2);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-0.2,0,2.6);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(2.2,0,2.6);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(2.2,0.2,2.6);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-0.2,0.2,2.6);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-0.2,0.2,3.2);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(2.2,0.2,3.2);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(2.2,0.2,2.6);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-0.2,0.2,2.6);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-0.2,0,3.2);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(2.2,0,3.2);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(2.2,0,2.6);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-0.2,0,2.6);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(-0.2,0,3.2);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(-0.2,0.2,3.2);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(-0.2,0.2,2.6);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(-0.2,0,2.6);
			    glEnd();
			    glBegin(GL_QUADS);
			        glTexCoord3f(0.0,1.0,10001);  glVertex3f(2.2,0,3.2);
			        glTexCoord3f(7.0,1.0,10001);  glVertex3f(2.2,0.2,3.2);
			        glTexCoord3f(7.0,0.0,10001);  glVertex3f(2.2,0.2,2.6);
			        glTexCoord3f(0.0,0.0,10001);  glVertex3f(2.2,0,2.6);
			    glEnd();
		    glDisable(GL_TEXTURE_2D);
			
			//ATAP
			glEnable(GL_TEXTURE_2D);
	        glBindTexture(GL_TEXTURE_2D, _textureRoof);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		        //Kanan
				glBegin(GL_QUADS); 
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-1.4,1.5,-2.2);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(3.4,0.5,-2.2);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(3.4,0.5,0.7);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-1.4,1.5,0.7);
				glEnd();
				
				glBegin(GL_QUADS);
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-1.4,1.6,-2.2);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(3.4,0.6,-2.2);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(3.4,0.6,0.7);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-1.4,1.6,0.7);
				glEnd();
				
				glBegin(GL_QUADS);
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-1.4,1.6,-2.2);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(-1.4,1.5,-2.2);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(-1.4,1.5,0.7);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-1.4,1.6,0.7);
				glEnd();
				
				glBegin(GL_QUADS);
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-1.4,1.6,0.7);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(3.4,0.6,0.7);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(3.4,0.5,0.7);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-1.4,1.5,0.7);
				glEnd();
				
				glBegin(GL_QUADS);
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-1.4,1.6,-2.2);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(3.4,0.6,-2.2);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(3.4,0.5,-2.2);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-1.4,1.5,-2.2);
				glEnd();
				
				//Kiri
				glBegin(GL_QUADS); 
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-4.2,0.51,2.2);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(-4.2,0.51,-2.2);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(-0.8,0.51,-2.2);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-0.8,0.51,2.2);
				glEnd();
				
				glBegin(GL_QUADS); 
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-4.2,0.61,2.2);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(-4.2,0.61,-2.2);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(-0.8,0.61,-2.2);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-0.8,0.61,2.2);
				glEnd();
				
				glBegin(GL_QUADS); 
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-0.8,0.51,2.2);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(-0.8,0.51,-2.2);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(-0.8,0.61,-2.2);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-0.8,0.61,2.2);
				glEnd();
				
				glBegin(GL_QUADS); 
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-4.2,0.61,2.2);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(-4.2,0.51,2.2);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(-0.8,0.51,2.2);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-0.8,0.61,2.2);
				glEnd();
				
				glBegin(GL_QUADS); 
					glTexCoord3f(0.0,2.0,0); 	glVertex3f(-4.2,0.61,-2.2);
					glTexCoord3f(4.0,2.0,0); 	glVertex3f(-4.2,0.51,-2.2);
					glTexCoord3f(4.0,0.0,1.25); glVertex3f(-0.8,0.51,-2.2);
					glTexCoord3f(0.0,.00,1.25); glVertex3f(-0.8,0.61,-2.2);
				glEnd();
			glDisable(GL_TEXTURE_2D);	    
		glPopMatrix();
		
		//  Light position
	    float Position[]  = {-(distance*cos(zh)),-(distance*sin(zh)),0,1.0};
	
//	    //  Draw light position as ball (still no lighting here)
//	    glColor3f(1,1,1);
//	    ball(Position[0],Position[1],Position[2] , 1); //Sun
//	
//	    //  OpenGL should normalize normal vectors
//	    glEnable(GL_NORMALIZE);
//	    //  Enable lighting
//	    glEnable(GL_LIGHTING);
//	    //  Location of viewer for specular calculations
//	    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
//	    //  glColor sets ambient and diffuse color materials
//	    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
//	    glEnable(GL_COLOR_MATERIAL);
//	
	    //  Enable light 0 - Sun
	    glEnable(GL_LIGHT0);
	    //  Set ambient, diffuse, specular components and position of light 0 (Sun)
	    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	    glLightfv(GL_LIGHT0,GL_POSITION,Position);
		glutSwapBuffers();
	}
}

static void setEnvLighting()
{
   float ambScale = 0.8;

   if(sin(zh) >= 0.2) {
      Ambient[0] = (255 / 255) * ambScale;
      Ambient[1] = (255 / 255) * ambScale;
      Ambient[2] = (255 / 255) * ambScale;

      Diffuse[0] = 1.0;
      Diffuse[1] = 1.0;
      Diffuse[2] = 1.0;
   } else if (sin(zh) < 0.4 && sin(zh) >= 0) {
      Ambient[0] = (255 / 255) * ambScale;
      Ambient[1] = ((60 + (195*sin(zh)*2.5)) / 255) * ambScale;
      Ambient[2] = ((60 + (195*sin(zh)*2.5)) / 255) * ambScale;

      Diffuse[0] = sin(zh)*2.5;
      Diffuse[1] = sin(zh)*2.5;
      Diffuse[2] = sin(zh)*2.5;
   } else if (sin(zh) < 0 && sin(zh) >= -0.4){
      Ambient[0] = ((255 - (235*sin(zh)*-2.5)) / 255) * ambScale;
      Ambient[1] = (60 / 255) * ambScale;
      Ambient[2] = ((60 + (120*sin(zh)*-2.5)) / 255) * ambScale;

      Diffuse[0] = 0;
      Diffuse[1] = 0;
      Diffuse[2] = 0;
   } else {
      Ambient[0] = (20 / 255) * ambScale;
      Ambient[1] = (60 / 255) * ambScale;
      Ambient[2] = (180 / 255) * ambScale;

      Diffuse[0] = 0;
      Diffuse[1] = 0;
      Diffuse[2] = 0;
   }
}

void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(10*t,360.0);

   setEnvLighting();

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void Timer(int value) {
   glutPostRedisplay();
   glutTimerFunc(refreshMills, Timer, 0);
}

void Reshape(GLsizei width, GLsizei height) {
	const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void KeyboardHandler(unsigned char key, int x, int y)
{
	switch (key) {
		case ' ':
			view=!view;
			Display();
		break;
		
		case 'm':
			mode=!mode;
		break;
		
		case 'p':
			move=!move;
		break;
		
		case 'r':
			reset();
		break;
		
		case ',':
	    	shoot = 2;
	    break;
	    
	    case '.':
	    	shoot = 1;
	    break;
		
	    case 'z':
	    	xangle -= 10;
	    	if (xangle > 360) xangle = 0.0;
	    break;
	    
	    case 'x':
	    	xangle += 10;
	    	if (xangle > 360) xangle = 0.0;
	    break;
	    
		case 'c':  
			yangle += 10;
			if (yangle > 360) yangle = 0.0;
			break;
			
		case 'v':
	        yangle -= 10;
	        if (yangle > 360) yangle = 0.0;
		    break;
		    
	    case 'b':
	    	zangle -= 10;
	    	if (zangle > 360) zangle = 0.0;
	    break;
	    
	    case 'n':
	    	zangle += 10;
	    	if (zangle > 360) zangle = 0.0;
	    break;
	    
	    case 'a':
	    	xt += 0.4;
//	    	if(mode == 0) fpX  += 2;
//         	else refX += 2;
	    break;
	    
	    case 'd':
	    	xt -= 0.4;
//	    	if(mode == 0) fpX -= 2;
//         	else refX -= 2;
	    break;
	    
	    case 's':
	    	zt -= 0.4;
//	    	if(mode == 0) fpZ -= 2;
//         	else refZ -= 2;
	    break;
	    
	    case 'w':
	    	zt += 0.4;
//	    	if(mode == 0) fpZ += 2;
//         	else refZ += 2;
	    break;
	    
	    case 'q':
	    	yt -= 0.4;
//	    	if(mode == 0) fpY += 2;
//         	else refY += 2;
	    break;
	    
	    case 'e':
	    	yt += 0.4;
//	    	if(mode == 0) fpY -= 2;
//        	else refY -= 2;
	    break;
	    
	    case '1':
	    	printf("Mengendalikan mobil merah.\n");
	    	car=0;
	    break;
	    	
    	case '2':
    		printf("Mengendalikan mobil biru.\n");
    		car=1;
    	break;
    	
    	case '3':
    		printf("Mengendalikan mobil hijau.\n");
    		car=2;
    	break;
    	
    	case '4':
    		printf("Mengendalikan mobil kuning.\n");
    		car=3;
    	break;
    
		case ESCAPE:
			printf("Tombol ESC ditekan. program berhenti.\n");
			exit(0);
			break;
		default: 
			break;
	}
	//  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void SpecialKeyHandler(int key, int x, int y)
{
	switch (key){
		case GLUT_KEY_RIGHT:
			xc[car] += 0.2;
	     	glutPostRedisplay();
	  		break;

        case GLUT_KEY_LEFT:
			xc[car] -= 0.2;
			glutPostRedisplay();
			break;
			
		case GLUT_KEY_UP:
			zc[car] -= 0.2;
		glutPostRedisplay();
		break;
		
		case GLUT_KEY_DOWN:
			zc[car] += 0.2;
		glutPostRedisplay();
		break;
	}
}

GLuint loadTexture(const char* filename) {
	BmpLoader bl(filename);	
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bl.iWidth, bl.iHeight,
				 0, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData);
	return textureId;
}
