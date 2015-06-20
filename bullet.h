/*
*Tank Wars
*Yongzhao Mo
*/
#define MAX_BULLETS        5  // max player bullets
#define MAX_PARTICLES   2500  // max explosion particles
#define MAX_EBULLETS      10  // max AI bullets

typedef struct  // Struct for bullets
{
   bool alive;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat dx;
   GLfloat dy;
   GLfloat dz;
   GLfloat heading;
   int count;
} BULLET_STRUCT;

BULLET_STRUCT Bullets[MAX_BULLETS];  // up to 5 bullets at a time ?


typedef struct    // Struct for Particles
{
   bool alive;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat dx;
   GLfloat dy;
   GLfloat dz;
   int color;
   int count;
} PARTICLE_STRUCT;

PARTICLE_STRUCT Particles[MAX_PARTICLES];  // Particle info

typedef struct  // Struct for enemy bullets
{
   bool alive;
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat dx;
   GLfloat dy;
   GLfloat dz;
   GLfloat heading;
   int count;
} EBULLET_STRUCT;

EBULLET_STRUCT EBullets[MAX_EBULLETS];
