/*
*Tank Wars
*Yongzhao Mo
*/
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <gl\gl.h>	
#include <gl\glu.h>	
#include <gl\glut.h>
#include <gl\glaux.h>
#include <mmsystem.h>
#include <conio.h>
#include "bmp.h"
#include "bullet.h"

#ifndef Pi
#define Pi            (3.14159265358979f)
#endif
#ifndef dimension
#define dimension            (20)
#endif



///////////////////////////////
/// BULLETS CONSTANTS
///////////////////////////////
#define BULLET_PAUSE          75  // pause time between bullets
#define BULLET_SPEED        0.1f    // player and enemy bullet speed
#define BULLET_DURATION    1000    // bullet duration in frames
int bullet_wait;  // wait time between bullets
GLUquadricObj *game_sphere = NULL;  // pointer for bullet spheres
#define MAX_EBULLETS      10  // max enemy bullets

///////////////////////////////
/// PARTICLE CONSTANTS
///////////////////////////////
#define EXPLODE_WALL       1
#define EXPLODE_CAMO       2
#define EXPLODE_AMMO       3
#define EXPLODE_FUEL       4
#define EXPLODE_ENEMY      5
#define EXPLODE_GROUND     6

#define GRAVITY_SPEED     0.1f    // gravity for particle effects

#define COLOR_BLK          0  // black particle explosion colors
#define COLOR_WHT          1  // white
#define COLOR_RED          2  // red
#define COLOR_TAN          3  // tan (wood)
#define COLOR_MAR          4  // maroon (dark red)
#define COLOR_DGR          5  // dark green
#define COLOR_BRN          6  // brown
#define COLOR_YEL          7  // yellow


#define MAX_AI             8

#define MAX_LISTS          8 
using namespace std;
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

GLuint RoomList;    // storage for room display list
GLuint Sky1List;    // storage for sky 1 list
GLuint Sky2List;    // storage for sky 2 list

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool	blend;				// Blending ON/OFF
bool	bp;					// B Pressed?
bool	fp;					// F Pressed?
bool    upDown;
bool    first = true;
bool    hit[8];
bool    mySound = false;
const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;
float xposOld;
float zposOld;
float tankX;
float tankZ;
GLfloat	yrot;				// Y Rotation
GLfloat	yrotOld;				// Y old Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
GLfloat	z=0.0f;				// Depth Into The Screen
GLuint	filter;				// Which Filter To Use
GLuint	texture[10];			// Storage For 3 Textures
GLUquadricObj *quadratic;	
GLint i;
GLint aiNum = MAX_AI;

typedef struct wcPt3D
{
    public:     
	GLfloat x, y, z;
} VERTEX;
typedef struct 
{
   bool alive;
   GLfloat x;
   GLfloat z;
   GLfloat dx;
   GLfloat dy;
   GLfloat dz;
   GLfloat h; 
} AI_STRUCT;

AI_STRUCT tankAI[4];
      
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

/////////////////
// FUNCTIONS
////////////////
void FireBullet();
void InsertParticles(GLfloat, GLfloat, GLfloat, int);
void MoveParticles();


/*Function to draw tank*/
void DrawTank(int type){
  
    if(type==1)glColor3f(1.0,1.0,1.0);
    else glColor3f(0.0,1.0,1.0);
    glTranslatef(0, 0, -1.4);    
    GLfloat d = 0.0;
    GLfloat frontLeftWheel[10][3] = {{ 1.5,1,0+d},{ -1.5, 1,0+d },{ -1.75,0.95,0+d },
    { -2.0,0.9,0+d },{ -2.0, 0.6,0+d },{ -1.2, 0,0+d },{ 1.2, 0,0+d},
    { 2, 0.6,0+d },{ 2, 0.9,0+d },{ -1.75,0.95,0+d }};
    d = 0.4;
    GLfloat backLeftWheel[10][3] = {{ 1.5,1,0+d},{ -1.5, 1,0+d },{ -1.75,0.95,0+d },
    { -2.0,0.9,0+d },{ -2.0, 0.6,0+d },{ -1.2, 0,0+d },{ 1.2, 0,0+d},
    { 2, 0.6,0+d },{ 2, 0.9,0+d },{ -1.75,0.95,0+d }};
    d = 2.4;
    GLfloat frontRightWheel[10][3] = {{ 1.5,1,0+d},{ -1.5, 1,0+d },{ -1.75,0.95,0+d },
    { -2.0,0.9,0+d },{ -2.0, 0.6,0+d },{ -1.2, 0,0+d },{ 1.2, 0,0+d},
    { 2, 0.6,0+d },{ 2, 0.9,0+d },{ -1.75,0.95,0+d }};
    d = 2.8;
    GLfloat backRightWheel[10][3] = {{ 1.5,1,0+d},{ -1.5, 1,0+d },{ -1.75,0.95,0+d },
    { -2.0,0.9,0+d },{ -2.0, 0.6,0+d },{ -1.2, 0,0+d },{ 1.2, 0,0+d},
    { 2, 0.6,0+d },{ 2, 0.9,0+d },{ -1.75,0.95,0+d }};
    wcPt3D base[8] = {{1.0,1.2,0.4},{-2,1.2,0.4},{-2,0.6,0.4},{1.0,0.6,0.4},
            {-2,1.2,2.4},{1.0,1.2,2.4},{1.0,0.6,2.4},{-2,0.6,2.4}};
    GLint base_faces[6][4] = { {0, 1, 2, 3}, {5,0,3,6}, {4,5,6,7},
{1,4,7,2}, {2,7,6,3}, {5,4,1,0} };    
    wcPt3D baseFront[6] = {{1.0,1.2,2.4},{1.0,1.2,0.4},{1.0,0.6,0.4},{1.0,0.6,2.4},{2.0,0.6,2.4},{2.0,0.6,0.4}};
    GLint front_faces[5][4] = { {0, 1, 2, 3},{4,0,1,5},{3,4,2,5},{5,1,2}, {0,4,3}};     
    GLfloat top1[6][3] = {{1,1.2,1.8},{0,1.2,2.2},{-1,1.2,1.8},{-1,1.2,1.0},{0,1.2,0.6},{1,1.2,1.0}};
    GLfloat top2[6][3] = {{1,1.4,1.8},{0,1.4,2.2},{-1,1.4,1.8},{-1,1.4,1.0},{0,1.4,0.6},{1,1.4,1.0}};
    
    glPushMatrix();
    if(type==2)glScalef(2,2,2);
    
    //wheels
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_POLYGON);
        for(int i = 0;i<10;i++)
        {  glTexCoord3f(frontLeftWheel[i][0],frontLeftWheel[i][1],frontLeftWheel[i][2]);    
          glVertex3fv(frontLeftWheel[i]);
        }  
    glEnd();
    glBegin(GL_POLYGON);
        for(int i = 0;i<10;i++)
          {glTexCoord3f(backLeftWheel[i][0],backLeftWheel[i][1],backLeftWheel[i][2]);  
          glVertex3fv(backLeftWheel[i]);}
    glEnd();
    glBegin(GL_POLYGON);
        for(int i = 0;i<10;i++)
          {glTexCoord3f(frontRightWheel[i][0],frontRightWheel[i][1],frontRightWheel[i][2]);
          glVertex3fv(frontRightWheel[i]);}
    glEnd();
    glBegin(GL_POLYGON);
        for(int i = 0;i<10;i++)
          {glTexCoord3f(backRightWheel[i][0],backRightWheel[i][1],backRightWheel[i][2]);
          glVertex3fv(backRightWheel[i]);}
    glEnd();

    //track	
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    for(int i = 0;i<9;i++)
    {
          glBegin(GL_POLYGON);
             glTexCoord3f(backLeftWheel[i][0],backLeftWheel[i][1],backLeftWheel[i][2]);
              glVertex3fv(backLeftWheel[i]);
              glTexCoord3f(frontLeftWheel[i][0],frontLeftWheel[i][1],frontLeftWheel[i][2]);
             glVertex3fv(frontLeftWheel[i]);
             glTexCoord3f(frontLeftWheel[i][0],frontLeftWheel[i][1],frontLeftWheel[i][2]);
             glVertex3fv(frontLeftWheel[i+1]);
             glTexCoord3f(backLeftWheel[i][0],backLeftWheel[i][1],backLeftWheel[i][2]);
             glVertex3fv(backLeftWheel[i+1]); 
          glEnd(); 
          glBegin(GL_POLYGON);
             glTexCoord3f(backRightWheel[i][0],backRightWheel[i][1],backRightWheel[i][2]);
              glVertex3fv(backRightWheel[i]);
              glTexCoord3f(frontRightWheel[i][0],frontRightWheel[i][1],frontRightWheel[i][2]);
             glVertex3fv(frontRightWheel[i]);
             glTexCoord3f(frontRightWheel[i][0],frontRightWheel[i][1],frontRightWheel[i][2]);
              glVertex3fv(frontRightWheel[i+1]);
             glTexCoord3f(backRightWheel[i][0],backRightWheel[i][1],backRightWheel[i][2]);
              glVertex3fv(backRightWheel[i+1]);  
          glEnd();       
    } 
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    for(int i = 0;i<6;i++)
    {
          glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f);
          glVertex3f (base[base_faces[i][0]].x,base[base_faces[i][0]].y,base[base_faces[i][0]].z);
          glTexCoord2f(2.0f, 0.0f);
          glVertex3f (base[base_faces[i][1]].x,base[base_faces[i][1]].y,base[base_faces[i][1]].z);
          glTexCoord2f(2.0f, 2.0f);
          glVertex3f (base[base_faces[i][2]].x,base[base_faces[i][2]].y,base[base_faces[i][2]].z);
          glTexCoord2f(0.0f, 2.0f);
          glVertex3f (base[base_faces[i][3]].x,base[base_faces[i][3]].y,base[base_faces[i][3]].z);
          glEnd();
    }
    for(int i = 0;i<3;i++)
    {
          glBegin(GL_QUADS);
          glTexCoord3f(baseFront[front_faces[i][0]].x,baseFront[front_faces[i][0]].y,baseFront[front_faces[i][0]].z); 
          glVertex3f (baseFront[front_faces[i][0]].x,baseFront[front_faces[i][0]].y,baseFront[front_faces[i][0]].z);
          glTexCoord3f(baseFront[front_faces[i][1]].x,baseFront[front_faces[i][1]].y,baseFront[front_faces[i][1]].z); 
          glVertex3f (baseFront[front_faces[i][1]].x,baseFront[front_faces[i][1]].y,baseFront[front_faces[i][1]].z);
          glTexCoord3f(baseFront[front_faces[i][2]].x,baseFront[front_faces[i][2]].y,baseFront[front_faces[i][2]].z); 
          glVertex3f (baseFront[front_faces[i][2]].x,baseFront[front_faces[i][2]].y,baseFront[front_faces[i][2]].z);
          glTexCoord3f(baseFront[front_faces[i][3]].x,baseFront[front_faces[i][3]].y,baseFront[front_faces[i][3]].z); 
          glVertex3f (baseFront[front_faces[i][3]].x,baseFront[front_faces[i][3]].y,baseFront[front_faces[i][3]].z);
          glEnd();
    }
    for(int i = 3;i<5;i++)
    {
          glBegin(GL_POLYGON);
          glTexCoord3f(baseFront[front_faces[i][0]].x,baseFront[front_faces[i][0]].y,baseFront[front_faces[i][0]].z); 
          glVertex3f (baseFront[front_faces[i][0]].x,baseFront[front_faces[i][0]].y,baseFront[front_faces[i][0]].z);
          glTexCoord3f(baseFront[front_faces[i][1]].x,baseFront[front_faces[i][1]].y,baseFront[front_faces[i][1]].z); 
          glVertex3f (baseFront[front_faces[i][1]].x,baseFront[front_faces[i][1]].y,baseFront[front_faces[i][1]].z);
          glTexCoord3f(baseFront[front_faces[i][2]].x,baseFront[front_faces[i][2]].y,baseFront[front_faces[i][2]].z);
           glVertex3f (baseFront[front_faces[i][2]].x,baseFront[front_faces[i][2]].y,baseFront[front_faces[i][2]].z);
          glEnd();
    }
    /*glBegin(GL_POLYGON);
        for(int i = 0;i<6;i++)
          {glTexCoord3f(top1[i][0],top1[i][1],top1[i][2]);
          glVertex3fv(top1[i]);}
    glEnd();*/
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_POLYGON);
        for(int i = 0;i<6;i++)
          {glTexCoord3f(top2[i][0],top2[i][1],top2[i][2]);
           glVertex3fv(top2[i]);}
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    
    
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    for(int i = 0;i<5;i++)
    {
          glBegin(GL_POLYGON);
             glTexCoord3f(top1[i][0],top1[i][1],top1[i][2]); 
             glVertex3fv(top1[i]);
             glTexCoord3f(top2[i][0],top2[i][1],top2[i][2]);
              glVertex3fv(top2[i]);
             glTexCoord3f(top2[i][0],top2[i][1],top2[i][2]);
             glVertex3fv(top2[i+1]);
             glTexCoord3f(top1[i][0],top1[i][1],top1[i][2]);
             glVertex3fv(top1[i+1]);
          glEnd(); 
    }
    glTranslatef(1,1.2,1.5);
    glRotatef(90, 0,0.1,0);
    glTexCoord2f(2*dimension, 2*dimension); 
    gluCylinder(quadratic,0.1f,0.1f,3.0f,32,32);
    glRotatef(-90, 0,-0.1,0);
    glTranslatef(-1,-1.2,-1.5);
    
    if(type==2)glScalef(0.2,0.2,0.2);
    glPopMatrix();
    glTranslatef(0, 0, 1.4);
    
}    

/*Create AI tanks*/
void AItank(void)
{
    for(int iii = 0;iii<8;iii++)
    {
        tankAI[iii].alive = true;
        tankAI[iii].x = (-17.5+5*iii)*10;
        tankAI[iii].z = (-dimension + (dimension/10))*10;
        //tankAI[iii].h = (GLfloat)(rand()%10);  
    }     
}    

GLvoid winReshapeFcn(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int checkCollision(int iii)
{
    for(int j =0;j<MAX_AI;j++)
    {
        if((iii!=j)&&tankAI[j].alive)
        {
            if(fabs(tankAI[iii].x - tankAI[j].x)<0.04)
            {if(fabs(tankAI[iii].z - tankAI[j].z)<0.04) return 1; } 
             
        }
                       
    }
    if((xpos*10 > -tankAI[iii].x-10)&&(xpos*10 < -tankAI[iii].x+10) && (zpos*10 > -tankAI[iii].z - 10)&& (zpos*10 < -tankAI[iii].z+10))
    {return 1;}
    
    return 0;      
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
    loadbmp(texture, "Data/Floor.bmp",0); 
	loadbmp(texture, "Data/TankTexture.bmp",1);
    loadbmp(texture, "Data/TankWheel.bmp",3);
    loadbmp(texture, "Data/TankTrack.bmp",4);	
    loadbmp(texture, "Data/Star.bmp",6);
    loadbmp(texture, "Data/Sky.bmp",7);
    loadbmp(texture, "Data/Wall.bmp",8);
    loadbmp(texture, "Data/CannonBall.bmp",9);
    game_sphere = gluNewQuadric();  // create storage for bullet spheres
	//loadbmp(texture, "Data/tiger_tank.bmp",2);
    //PlaySound("data/BACK.MID",NULL,SND_ASYNC);
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);
	
	////////////////////////////////////////
	for (i = 0; i < MAX_PARTICLES; i++) Particles[i].alive = FALSE;
	////////////////////////////////////////
	
	return TRUE;										// Initialization Went OK
}

//////////////////////////////////////////////////
// Fires a tank bullet
//////////////////////////////////////////////////

void FireBullet()
{
   
   int i; // for loop
      
   bullet_wait--; if (bullet_wait > 0) return;
   bullet_wait = BULLET_PAUSE;


   for (i = 0; i < MAX_BULLETS; i++) // find a free bullet to insert
   {
      if (!Bullets[i].alive)  // found a free one
      {                          
         Bullets[i].alive = TRUE;
         Bullets[i].x = xpos; Bullets[i].y = 0.1; Bullets[i].z = zpos;
         Bullets[i].heading = heading;   
         
         Bullets[i].dx =  BULLET_SPEED * (float)sin(Bullets[i].heading*piover180);   // x direction
         Bullets[i].dz =  BULLET_SPEED * (float)cos(Bullets[i].heading*piover180);   // z direction

         Bullets[i].count = BULLET_DURATION;


         PlaySound("Data/shot.wav",NULL,SND_FILENAME|SND_ASYNC); 
         return;
       } // end if not alive
   } // end for
   

} // END OF FireBullet

/////////////////////////////////////////////
// Move player bullets
/////////////////////////////////////////////

void MoveBullets()
{
     
   int i;
   GLfloat x, y, z;  // temp x y z position of bullet
   
   for (i = 0; i < MAX_BULLETS; i++)
   {
      if (Bullets[i].alive)
      {
         x = Bullets[i].x;   y = Bullets[i].y;   z = Bullets[i].z;   // save current position
         x -= Bullets[i].dx; y -= Bullets[i].dy; z -= Bullets[i].dz; // move
         
         if (x < -dimension || x > dimension || z < -dimension || z > dimension)  // out of range?
         {
            Bullets[i].alive = FALSE;
            if (y > 0) InsertParticles(x,y,z,EXPLODE_WALL);  // small explosion
            else InsertParticles(x,y,z,EXPLODE_GROUND);
         }

         for(int j = 0;j<MAX_AI;j++)
         {
            if (tankAI[j].alive)     
            {
                if((x*10 > -tankAI[j].x-10)&&(x*10 < -tankAI[j].x+10) && (z*10 > -tankAI[j].z - 10)&& (z*10 < -tankAI[j].z+10))
                {
                          Bullets[i].alive = FALSE;
                          tankAI[j].alive = FALSE;
                          

                          InsertParticles(x,y,z,EXPLODE_ENEMY); 
                          
                                                                              
                          aiNum--;
                }
            }     
         }
                  
         Bullets[i].x = x; Bullets[i].y = y; Bullets[i].z = z;  // save new position
         Bullets[i].count--; if (Bullets[i].count == 0) Bullets[i].alive = FALSE;
      } // end if bullet alive
   } // end for
   

} // END OF MoveBullets

/////////////////////////////////////////////
// Fire an enemy bullet
/////////////////////////////////////////////

void FireEBullet(GLfloat x, GLfloat y, GLfloat z, GLint n)
{
   int i;
   GLfloat m, dx, dy, dz, ax, az;
      
   for (i = 0; i < MAX_EBULLETS; i++)
   {
      if (!EBullets[i].alive)
      {
         EBullets[i].alive = TRUE;
         EBullets[i].x = x; EBullets[i].y = y; EBullets[i].z = z;
         
         
         EBullets[i].heading = tankAI[n].h;   
         
         EBullets[i].dx =  BULLET_SPEED*10* (float)sin(EBullets[i].heading*piover180);   // x direction
         EBullets[i].dz =  BULLET_SPEED*10 * (float)cos(EBullets[i].heading*piover180);   // z direction

         EBullets[i].count = BULLET_DURATION;

        
      } // end if EBullet alive
   } // end for

} // END OF FireEBullet


/////////////////////////////////////////////
// Move player bullets
/////////////////////////////////////////////

void MoveEBullets()
{
     
   int i;
   GLfloat x, y, z;  // temp x y z position of bullet
   
   for (i = 0; i < MAX_EBULLETS; i++)
   {
      if (EBullets[i].alive)
      {
         x = EBullets[i].x;   y = EBullets[i].y;   z = EBullets[i].z;   // save current position
         x -= EBullets[i].dx; y -= EBullets[i].dy; z -= EBullets[i].dz; // move
         
         if (x/10 < -dimension || x/10 > dimension || z/10 < -dimension || z/10 > dimension)  // out of range?
         {
            printf("gone\n");      
            EBullets[i].alive = FALSE;
         }

         if((x/10 > xpos - 0.5)&&(x/10 < xpos + 0.5) && (z/10 > zpos - 0.5)&& (z/10 < zpos + 0.5))
         {
               EBullets[i].alive = FALSE;
                

               InsertParticles(x,y,z,EXPLODE_ENEMY); 
               
               /*
                *  Part where player gets destroyed
                */
               MessageBox(NULL, "You are hit", "OUCH",MB_OK);            
               //sound maybe                                                                              
         
                
              
         }
                  
         EBullets[i].x = x; EBullets[i].y = y; EBullets[i].z = z;  // save new position
         EBullets[i].count--; if (EBullets[i].count == 0) EBullets[i].alive = FALSE;
      } // end if bullet alive
   } // end for
   

} // END OF MoveEBullets



/////////////////////////////////////////////
// Insert particle explosion of type t
/////////////////////////////////////////////

void InsertParticles(GLfloat x, GLfloat y, GLfloat z, int t)
{
   int i, n, p, r;
      
   if (t == EXPLODE_WALL || EXPLODE_GROUND) {p = 1;
   PlaySound("Data/bomb.wav",NULL,SND_FILENAME|SND_ASYNC);}
   if (t == EXPLODE_CAMO || t == EXPLODE_AMMO || t == EXPLODE_FUEL) p = 2;
   if (t == EXPLODE_ENEMY) {p = 3;
   PlaySound("Data/die.wav",NULL,SND_FILENAME|SND_ASYNC); 
   PlaySound("Data/Godlike.wav",NULL,SND_FILENAME|SND_ASYNC);      }

   n = p * 300 + (rand()%(p*300)); // number of particles to insert
      
   for (i = 0; i < MAX_PARTICLES; i++)
   {
      if (!Particles[i].alive) // found free particle
      {
         Particles[i].alive = TRUE;
         
         Particles[i].x = x; // position at center of collision
         Particles[i].y = y;
         Particles[i].z = z;
         
         Particles[i].dx = 0.5f * p * ((GLfloat)(rand()%10000) / 10000.0f) - .25f * p;  // movement away from center
         Particles[i].dy = 0.5f * p * ((GLfloat)(rand()%10000) / 10000.0f) - .25f * p;
         Particles[i].dz = 0.5f * p * ((GLfloat)(rand()%10000) / 10000.0f) - .25f * p;
         
         if (t == EXPLODE_WALL)
         {
            r = (rand()%2);
            if (r == 0) Particles[i].color = COLOR_RED;
            if (r == 1) Particles[i].color = COLOR_WHT;
         }
         
         if (t == EXPLODE_GROUND)
         {
            r = (rand()%2);
            if (r == 0) Particles[i].color = COLOR_TAN;
            if (r == 1) Particles[i].color = COLOR_BRN;
         
         }

         if (t == EXPLODE_CAMO || t == EXPLODE_ENEMY)
         {
            r = (rand()%4);
            if (r == 0) Particles[i].color = COLOR_BLK;
            if (r == 1) Particles[i].color = COLOR_WHT;
            if (r == 2) Particles[i].color = COLOR_MAR;
            if (r == 3) Particles[i].color = COLOR_DGR;
         
         }

         if (t == EXPLODE_AMMO)
         {
            r = (rand()%4);
            if (r == 0) Particles[i].color = COLOR_BLK;
            if (r == 1) Particles[i].color = COLOR_WHT;
            if (r == 2) Particles[i].color = COLOR_RED;
            if (r == 3) Particles[i].color = COLOR_TAN;
         }

         if (t == EXPLODE_FUEL)
         {
            r = (rand()%4);
            if (r == 0) Particles[i].color = COLOR_BLK;
            if (r == 1) Particles[i].color = COLOR_WHT;
            if (r == 2) Particles[i].color = COLOR_BRN;
            if (r == 3) Particles[i].color = COLOR_YEL;
         }
         
         Particles[i].count = p * 50 + (rand()%(p*50));  // set duration of particle
                  
         n--; if (n == 0) return;  // finished inserting all particles
      } // end if
   } // end for
  

} // END OF InsertParticles


/////////////////////////////////////////////
// Move all particles
/////////////////////////////////////////////

void MoveParticles()
{
   int i;
   GLfloat x, y, z;  // temp x y z position of particle
   
   for (i = 0; i < MAX_PARTICLES; i++)
   {
      if (Particles[i].alive)
      {
         // save current position
         x = Particles[i].x; y = Particles[i].y; z = Particles[i].z;
         
         // move to new position
         x += Particles[i].dx; y += Particles[i].dy; z += Particles[i].dz;
         
         // gravity effect on y movement
         if (Particles[i].dy < 0) Particles[i].dy += GRAVITY_SPEED;
         else Particles[i].dy -= GRAVITY_SPEED;
         
         if (x < -998 || x > 998) { Particles[i].dx *= (-1); x += Particles[i].dx; } // bounce on x
         if (z < -998 || z > 998) { Particles[i].dz *= (-1); z += Particles[i].dz; } // bounce on z
         if (y < 0) { Particles[i].dy *= (-1);  y += Particles[i].dy; } // bounce on y (ground)
                  
         // save new position
         Particles[i].x = x; Particles[i].y = y; Particles[i].z = z;
         
         // check if particle finished
         Particles[i].count--; if (Particles[i].count == 0) Particles[i].alive = FALSE;
         
      } // end if
      
   } // end for
   if (aiNum == 0){
             MessageBox(NULL, "YOU WON YAY!", "CONGRATULATIONS", MB_OK);
             aiNum = -1;
             exit(0);
   }
} // END OF MoveParticles



int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
    int r;
    
    int c;  // particle color
    GLfloat x, y, z;  // temp x y z position for particles
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;
	GLfloat ytrans = -walkbias-0.25f;
	GLfloat sceneroty = 360.0f - yrot;
  
	int numtriangles;
    RoomList = glGenLists(MAX_LISTS);
    Sky1List = RoomList + 1; Sky2List = Sky1List + 1;
	glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);
	glTranslatef(xtrans, -0.2, ztrans);
   glColor3f(1, 0, 1);	
   glEnable(GL_TEXTURE_2D);                   // enable texturing
   glBindTexture(GL_TEXTURE_2D, texture[7]);  // use sky texture

   glBegin(GL_QUADS);
      glNormal3f( 0.0f, 0.0f, dimension); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-dimension,6.0,-dimension);
      glTexCoord2f(2.0f, 0.0f); glVertex3f(-dimension,6.0,dimension);
      glTexCoord2f(2.0f, 2.0f); glVertex3f(dimension,6.0,dimension);
      glTexCoord2f(0.0f, 2.0f); glVertex3f(dimension,  6.0 ,-dimension);
   glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	glColor3f(1.0,1.0,1.0);
	//bottom
	glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, dimension); 
    glTexCoord2f(0.0, 2*dimension);
    glVertex3f(-dimension,0.0,-dimension);
    glTexCoord2f(0.0 ,0.0);
    glVertex3f(-dimension,0.0,dimension);
    glTexCoord2f(2*dimension, 0.0);
    glVertex3f(dimension,0.0,dimension);
    glTexCoord2f(2*dimension, 2*dimension); 
    glVertex3f(dimension,  0.0 ,-dimension);
    glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[8]);
    //front
    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, dimension); 
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-dimension,0.0,-dimension);
    glTexCoord2f(2.0f, 0.0f);
    glVertex3f(-dimension,6,-dimension);
    glTexCoord2f(2.0f, 2.0f);
    glVertex3f(dimension,6,-dimension);
    glTexCoord2f(0.0f, 2.0f);
    glVertex3f(dimension,  0.0 ,-dimension);
    glEnd();
    
    //right
    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, dimension); 
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(dimension,0.0,-dimension);
    glTexCoord2f(2.0f, 0.0f);
    glVertex3f(dimension,6,-dimension);
    glTexCoord2f(2.0f, 2.0f);
    glVertex3f(dimension,6,dimension);
    glTexCoord2f(0.0f, 2.0f);
    glVertex3f(dimension,  0.0 ,dimension);
    glEnd();
    
    //back
    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, dimension); 
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(dimension,0.0,dimension);
    glTexCoord2f(2.0f, 0.0f);
    glVertex3f(dimension,6,dimension);
    glTexCoord2f(2.0f, 2.0f);
    glVertex3f(-dimension,6,dimension);
    glTexCoord2f(0.0f, 2.0f);
    glVertex3f(-dimension,  0.0 ,dimension);
    glEnd();
    
    //left
    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, dimension); 
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-dimension,0.0,dimension);
    glTexCoord2f(2.0f, 0.0f);
    glVertex3f(-dimension,6,dimension);
    glTexCoord2f(2.0f, 2.0f);
    glVertex3f(-dimension,6,-dimension);
    glTexCoord2f(0.0f, 2.0f);
    glVertex3f(-dimension,  0.0 ,-dimension);
    glEnd();
    
 
    glScalef(0.1,0.1,0.1);
    if(first){AItank();}
    //printf("%f      %f \n",tankX,tankZ);
    glTranslatef(10*tankX,0,10*tankZ);
    glRotatef(-sceneroty,0,1.0f,0);
    glRotatef(90,0.0f,1.0f,0.0f);
    DrawTank(1);
    glRotatef(-90,0.0f,1.0f,0.0f);
    glRotatef(sceneroty,0,1.0f,0);
    glTranslatef(-10*tankX,0,-10*tankZ);
    
    for(int iii = 0; iii<MAX_AI;iii++)
    {
         if(tankAI[iii].alive)
         {
            float temp = (rand()%4+1)*90;
             //float temp = rand()%20+170+(i+2)*rand()%3;
             
             if (tankAI[iii].x < (-dimension+0.5)*10) 
             {
                  tankAI[iii].x =(-dimension+0.5)*10;
                  glRotatef(temp,0,1.0f,0);
                  tankAI[iii].h = tankAI[iii].h - temp;
                  hit[iii] = !hit[iii];
             }
             if (tankAI[iii].x > (dimension-0.5)*10) 
             {
                  tankAI[iii].x = (dimension-0.5)*10;
                  glRotatef(temp,0,1.0f,0);
                  tankAI[iii].h = tankAI[iii].h + temp;
                  hit[iii]= !hit[iii];
             }
             if (tankAI[iii].z <(-dimension+0.5)*10) 
             {
                  tankAI[iii].z = (-dimension+0.5)*10;
                  glRotatef(temp,0,1.0f,0);
                  tankAI[iii].h = tankAI[iii].h - temp;
                  hit[iii] = !hit[iii];
             }
             if (tankAI[iii].z > (dimension-0.5)*10) 
             {
                  tankAI[iii].z = (dimension-0.5)*10;
                  glRotatef(temp,0,1.0f,0);
                  tankAI[iii].h = tankAI[iii].h + temp;
                  hit[iii] = !hit[iii];
             }
             if(checkCollision(iii) == 1)
             {
                  //tankAI[i].x = tankAI[i].x+30;
                  //glRotatef(temp,0,1.0f,0);
                  tankAI[iii].h = tankAI[iii].h - temp;
                  hit[iii] = !hit[iii];
             }
               
             tankAI[iii].x += (float)sin(tankAI[iii].h*piover180) * 0.25f;
             tankAI[iii].z += (float)cos(tankAI[iii].h*piover180) * 0.25f;
             
             //glRotatef(tankAI[i].h,0,1.0f,0);
             glTranslatef(-tankAI[iii].x,0,-tankAI[iii].z);
             float temp2 = (rand()%2+1)*180;
             if(first)glRotatef(temp2,0.0f,1.0f,0.0f);
             //if(hit[i])glRotatef(temp,0.0f,1.0f,0.0f); 
             glRotatef(90,0.0f,1.0f,0.0f);     
             DrawTank(2);

             glRotatef(-90,0.0f,1.0f,0.0f); 
             //if(hit[i])glRotatef(-temp,0.0f,1.0f,0.0f);
             if(first){glRotatef(-temp2,0.0f,1.0f,0.0f);first = false;}
             glTranslatef(tankAI[iii].x,0,tankAI[iii].z); 

             r = (rand()%50); // chance for fire 1 in 3
               if (r == 1) FireEBullet(-tankAI[iii].x,1,-tankAI[iii].z, iii);  // fire enemy bullet
             
                glColor3f(0.65f, 0.0f, 0.0f); // dark red bullets
               for (i = 0; i < MAX_EBULLETS; i++)
               {
                  if (EBullets[i].alive)
                  {
                     glPushMatrix();
                     glTranslatef(EBullets[i].x, EBullets[i].y, EBullets[i].z);
                     gluSphere(game_sphere, 1.0f, 20, 20); // draw sphere rad=1 long=10 lat=10
                     glPopMatrix();
                  }
               } // end for
               glColor3f(1.0f, 1.0f, 1.0f); // reset color to white

             
             
         }
            // draw enemy bullets
          
    }// end for

    // draw player bullets 
    glColor3f(0.25f, 0.25f, 0.25f); // dark grey bullets                 
    glScalef(10,10,10);
    for (i = 0; i < MAX_BULLETS; i++)
    {
       if (Bullets[i].alive)
       {
         
          glPushMatrix();
          glTranslatef(Bullets[i].x, 0.1, Bullets[i].z);
          gluSphere(game_sphere, 0.1f, 20, 20); // draw sphere rad=1 long=10 lat=10
          glPopMatrix();
       }
    } // end for
    glColor3f(1.0f, 1.0f, 1.0f); // reset color to white

    
    // draw explosion particles
   for (i = 0; i < MAX_PARTICLES; i++)
   {
      if (Particles[i].alive) // if particle is alive
      {
         x = Particles[i].x; y = Particles[i].y; z = Particles[i].z; c = Particles[i].color;
         if (c == COLOR_BLK) glColor3f(0.25f, 0.25f, 0.25f);
         if (c == COLOR_WHT) glColor3f(0.90f, 0.90f, 0.90f);
         if (c == COLOR_RED) glColor3f(0.87f, 0.0f,  0.0f);
         if (c == COLOR_TAN) glColor3f(0.59f, 0.45f, 0.0f);
         if (c == COLOR_MAR) glColor3f(0.53f, 0.0f,  0.26f);
         if (c == COLOR_DGR) glColor3f(0.33f, 0.41f, 0.0f);
         if (c == COLOR_BRN) glColor3f(0.66f, 0.41f, 0.0f);
         if (c == COLOR_YEL) glColor3f(0.96f, 0.96f, 0.0f);
         
         glBegin(GL_QUADS); // draw as tiny quads
            glVertex3f(x-0.1f, y-0.1f, z);
            glVertex3f(x+0.1f, y-0.1f, z);
            glVertex3f(x+0.1f, y+0.1f, z);
            glVertex3f(x-0.1f, y+0.1f, z);
         glEnd();
      } // end if
   } // end for
   
   glColor3f(1.0f, 1.0f, 1.0f); // reset color to white
    
	return TRUE;										// Everything Went OK
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Failed.","ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Failed.","ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Failed.","ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Try again.","ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Try again.","ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	

	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;


	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	winReshapeFcn(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			winReshapeFcn(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Create Our OpenGL Window
	if (!CreateGLWindow("Tank Wars",800,600,32))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
          
          
          MoveBullets();
          MoveParticles();
          MoveEBullets();
          
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
				if (keys['B'] && !bp)
				{
					bp=TRUE;
					blend=!blend;
					if (!blend)
					{
						glDisable(GL_BLEND);
						glEnable(GL_DEPTH_TEST);
					}
					else
					{
						glEnable(GL_BLEND);
						glDisable(GL_DEPTH_TEST);
					}
				}
				if (!keys['B'])
				{
					bp=FALSE;
				}

				if (keys['F'] && !fp)
				{
					fp=TRUE;
					filter+=1;
					if (filter>2)
					{
						filter=0;
					}
				}
				if (!keys['F'])
				{
					fp=FALSE;
				}

				if (keys[VK_PRIOR])
				{
					z-=0.02f;
				}

				if (keys[VK_NEXT])
				{
					z+=0.02f;
				}

				if (keys[VK_UP] || keys['W'])
				{
                   upDown = true;

                    tankX -= (float)sin(heading*piover180) * 0.01f;
                    tankZ -= (float)cos(heading*piover180) * 0.01f;
                    
					xpos -= (float)sin(heading*piover180) * 0.01f;
					zpos -= (float)cos(heading*piover180) * 0.01f;

                  if (xpos > dimension-0.5) {xpos = dimension-0.5; tankX= xpos;}
                  if (xpos < -(dimension-0.5)) {xpos = -(dimension-0.5); tankX= xpos;}
                  if (zpos > (dimension-0.5)) {zpos = (dimension-0.5); tankZ= zpos;}
                  if (zpos < -(dimension-0.5)) {zpos = -(dimension-0.5); tankZ= zpos;}
				}

				if (keys[VK_DOWN]|| keys['S'])
				{
                    upDown = false;

                    tankX += (float)sin(heading*piover180) * 0.01f;
                    tankZ += (float)cos(heading*piover180) * 0.01f;         
                             
					xpos += (float)sin(heading*piover180) * 0.01f;
					zpos += (float)cos(heading*piover180) * 0.01f;

                  if (xpos > dimension-0.5) {xpos = dimension-0.5; tankX= xpos;}
                  if (xpos < -(dimension-0.5)) {xpos = -(dimension-0.5); tankX= xpos;}
                  if (zpos > (dimension-0.5)) {zpos = (dimension-0.5); tankZ= zpos;}
                  if (zpos < -(dimension-0.5)) {zpos = -(dimension-0.5); tankZ= zpos;}
				}

				if (keys[VK_RIGHT] || keys['D'])
				{
					heading -= 1.0f;
					yrot = heading;
				}

				if (keys[VK_LEFT] || keys['A'])
				{
					heading += 1.0f;	
					yrot = heading;
				}

				if (keys[VK_PRIOR])
				{
					lookupdown-= 1.0f;
				}

				if (keys[VK_NEXT])
				{
					lookupdown+= 1.0f;
				}
				if (keys[VK_SPACE]){FireBullet();}   // fire a bullet
			
            
			}
		}
	}
  
	// Shutdown
	if (game_sphere) gluDeleteQuadric(game_sphere); // delete bullet sphere
	KillGLWindow();										// Kill The Window
	return (msg.wParam);								// Exit The Program
}
