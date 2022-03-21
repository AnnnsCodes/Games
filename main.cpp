#include <GL/glut.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <iostream>

#define Red 1
#define Yellow 2
#define Blue 3
#define Green 4

const int block_size = 20;
const int column = 20;
const int row = 10;
const int init_pos[2] = {3, 0};
const int mycar[4][3]={
   {0,1,0},
   {1,1,1},
   {0,1,0},
   {1,0,1}};
const int notmycar[5][3]={
    {0,0,0},
   {0,1,0},
   {1,1,1},
   {0,1,0},
   {1,1,1}};
const int tetrominos[7][4][4] = {
  {{0,1,0,0},
   {0,1,0,0},
   {0,1,0,0},
   {0,1,0,0}},
  {{0,0,0,0},
   {0,1,1,0},
   {0,1,0,0},
   {0,1,0,0}},
  {{0,0,0,0},
   {0,1,0,0},
   {0,1,1,0},
   {0,1,0,0}},
  {{0,0,0,0},
   {0,1,0,0},
   {0,1,1,0},
   {0,0,1,0}},
  {{0,0,0,0},
   {0,1,1,0},
   {0,1,1,0},
   {0,0,0,0}},
  {{0,0,0,0},
   {0,1,1,0},
   {0,0,1,0},
   {0,0,1,0}},
  {{0,0,0,0},
   {0,0,1,0},
   {0,1,1,0},
   {0,1,0,0}},
};

int x,y;
struct
{ int x;
  int y;} ss[100];
char s[50];
float pos[2] = {4, 16};
float dx=0.05,dy=0.05;
int startStep=1;
int gameMode=0;
int score=0;
float sc=0;
int Bscore=0;
int bort;
int b=0;
int d=0;
int step, laststep;
int a;
int sh=1, sl=-1;
int interval = 500;
int my_pos[2] = {19,4};
int dir,num=2;
int curr_block[4][4] = {};
int curr_pos[2] = {};
int blocks[column][row] = {
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
};

void glPrint (float xp, float yp, void *font, char *format, ...)
{
    va_list args;
    char buffer[200], *p;
    va_start (args, format);
    vsprintf (buffer, format, args);
    va_end (args);
    glPushMatrix ();
    glRasterPos2f (xp, yp);
    for (p = buffer; *p; p++)
    {
        glutBitmapCharacter (font, *p);
    }
    glPopMatrix ();
}

void init_curr_block()
{
  memcpy(curr_block, tetrominos[rand() % 7], 4 * 4 * sizeof(int));
  a=rand()%4+1;
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      if (curr_block[i][j])
        curr_block[i][j]=a;
  curr_pos[0] = init_pos[0];
  curr_pos[1] = init_pos[1];
  if (a==1) glColor3f(1, 0, 0);
  if (a==2) glColor3f(1, 1, 0);
  if (a==3) glColor3f(0, 0, 1);
  if (a==4) glColor3f(0, 1, 0);
}
void init()
{
  glClearColor(0.98, 0.98, 0.99, 0.3);
  srand(time(NULL));
  memset(blocks, 0, column * row * sizeof(int));
  init_curr_block();
}

void NewApple()
{
  x=rand() % row;
  y=rand() % column;
  glColor3f(1,0,0);
  glRectf(x,y,(x+0.9),(y+0.9));
}
void renderZmejkaScreen()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0,0,0);
  glBegin(GL_LINE_STRIP);
  glVertex2d(10.1, 30);
  glVertex2d(10.1, 0);
  glEnd();

  sprintf(s,"Score: %d",score);
  glRasterPos2f(11,17);
  char *c;
  for (c=s; *c != '\0'; c++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

  if (score>Bscore){
    std::ofstream fout("ZmScore.txt");
    fout << score;
    fout.close();
  }
  std::ifstream fin("ZmScore.txt");
    fin >> Bscore;
    fin.close();

  sprintf(s,"Best Score: %d",Bscore);
  glRasterPos2f(11,15);
  for (c=s; *c != '\0'; c++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

  glPrint (10.8,5, GLUT_BITMAP_HELVETICA_18, "Help:");
  glPrint (10.8,3.5, GLUT_BITMAP_HELVETICA_18, "+ Add speed");
  glColor3d(0,0,1);
  glPrint (10.8,3.5, GLUT_BITMAP_HELVETICA_18, "+");
  glColor3f(0,0,0);
  glPrint (10.8,2.5, GLUT_BITMAP_HELVETICA_18, "- Reduce speed");
  glColor3d(0,0,1);
  glPrint (10.8,2.5, GLUT_BITMAP_HELVETICA_18, "-");
  glColor3f(0,0,0);
  glPrint (10.8,1.5, GLUT_BITMAP_HELVETICA_18, "arrows Control");
  glColor3d(0,0,1);
  glPrint (10.8,1.5, GLUT_BITMAP_HELVETICA_18, "arrows");

 // glColor3f(0,1,0);
 // for (int i=0;i<num;i++)
  //  {glRectf(ss[i].x, ss[i].y, (ss[i].x+0.9), (ss[i].y+0.9)); }

    glBegin(GL_QUADS);
  for (int i=0;i<num;i++)
      {
          glColor3f(0,0.7,0);
        glVertex2f(ss[i].x, ss[i].y);
        glColor3f(0,1,0);
        glVertex2f(ss[i].x+0.9,     ss[i].y);
        glColor3f(0,0.7,0);
        glVertex2f(ss[i].x+0.9, ss[i].y+0.9);
        glColor3f(0,1,0);
        glVertex2f(ss[i].x, ss[i].y+0.9);
      }


  glEnd();

  glColor3f(1,0,0);
  glRectf(x,y,(x+0.9),(y+0.9));

  glFlush();
  glutSwapBuffers();
}
void timer1(int = 0)
{
  for (int i=num;i>0;i--)
  {ss[i].x=ss[i-1].x;
   ss[i].y=ss[i-1].y;}

  if (dir==0) ss[0].y+=1;
  if (dir==1) ss[0].x-=1;
  if (dir==2) ss[0].x+=1;
  if (dir==3) ss[0].y-=1;

  if ((ss[0].x==x) && (ss[0].y==y))
    {num++; NewApple(); score=score+1;}

  for (int i=1;i<num;i++)
  if (ss[0].x==ss[i].x && ss[0].y==ss[i].y)  {glutSetWindowTitle("GAME OVER"); b=0;}

  if (ss[0].x>=row || ss[0].x<0 || ss[0].y>=column || ss[0].y<0) {glutSetWindowTitle("GAME OVER"); b=0;}


  if (b==2) {glutPostRedisplay();
   glutTimerFunc(interval,timer1,0);}
}

bool empty(float next_x, float next_y)
{
  if (next_x < -0.3 || row <= next_x) {
    return false;
  }
  if (next_y < -0.5 || column <= next_y) {
        int qx=(int)next_y;
    if (gameMode==3) if (qx==20) {b=0; glutSetWindowTitle("GAME OVER");}
    return false;
  }
  int qx=(int)next_y;
  int qy=(int)next_x;
  if (blocks[qx][qy]!=0) {
    if (gameMode==3) if (qx!=19) blocks[qx][qy]=0;
    return false;
  }
  return true;
}

void timer3(int value)
{
    if (!empty(pos[0] + dx*10 ,pos[1])) {dx=dx*-1;}
    if (!empty(pos[0], pos[1] + dy*5)) {dy=dy*-1;}
        pos[0]=pos[0]+dx;
        pos[1]=pos[1]+dy;
    glutPostRedisplay();
    if (b==3) glutTimerFunc(interval*0.02, timer3, 0);

}

void timer4(int value)
{
    for (int i = 21; i >= 0; i--) {
    for (int j = 0; j <= row; j++) {
      blocks[i][j] = blocks[i - 1][j];
    }
  }
    if (bort==3) bort=1; else bort++;
    sc=sc+0.2;
    score=floor(sc);
   if (d==0) {
        step=rand()%3+1;
        switch (step){
            case 1: d=4; if (laststep==2) {curr_pos[0]=20;curr_pos[1]=30; laststep=3;}
                        else {curr_pos[0]=2; curr_pos[1]=-1; laststep=step;} break;
            case 2: d=4; if (laststep==1) {curr_pos[0]=20;curr_pos[1]=30; laststep=3;}
                        else {curr_pos[0]=5; curr_pos[1]=-1; laststep=step;} break;
            case 3: d=4; curr_pos[1]=30; curr_pos[0]=20; laststep=step; break;

        } }
    else {
    d=d-1;
    curr_pos[1]=curr_pos[1]+1;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            blocks[curr_pos[1]+i][curr_pos[0]+j]=notmycar[i][j];
    }
  }

    if (b==4) {glutPostRedisplay(); glutTimerFunc(interval*1.2, timer4, 0);}
}

void renderStartScreen ()
{

  glClearColor(0.98, 0.98, 0.99, 0.3);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   {
    glColor3d(1,0,0);
    glPushMatrix();
        glTranslated(1.5,17,-1);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(1,0,0);
        glutSolidCube(0.95);
     glPopMatrix();

     glColor3d(0,0,1);
     glPushMatrix();
        glTranslated(15.5,1,-1);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(1,0,0);
        glutSolidCube(0.95);
     glPopMatrix();

     glColor3d(0,0,1);
     glPushMatrix();
        glTranslated(1.5,14,-1);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(1,0,0);
        glutSolidCube(0.95);
        glTranslated(0,-1,0);
        glutSolidCube(0.95);
     glPopMatrix();

     glColor3d(1,1,0);
     glPushMatrix();
        glTranslated(1.5,10,-1);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(1,-1,0);
        glutSolidCube(0.95);
     glPopMatrix();

    glColor3d(1,1,0);
    glPushMatrix();
        glTranslated(15.5,12,-1);
        glutSolidCube(1);
        glTranslated(0,1,0);
        glutSolidCube(1);
        glTranslated(0,1,0);
        glutSolidCube(1);
        glTranslated(0,1,0);
        glutSolidCube(1);
     glPopMatrix();

     glColor3d(0,1,0);
     glPushMatrix();
        glTranslated(15.5,9,-1);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(1,0,0);
        glutSolidCube(0.95);
        glTranslated(0,-1,0);
        glutSolidCube(0.95);
     glPopMatrix();

     glColor3d(1,0,0);
     glPushMatrix();
        glTranslated(15.5,5,-1);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(1,-1,0);
        glutSolidCube(0.95);
     glPopMatrix();

     glColor3d(0,1,0);
    glPushMatrix();
        glTranslated(2.5,5,-1);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
     glPopMatrix();

     glColor3d(1,0,0);
    glPushMatrix();
        glTranslated(15.5,17,-1);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
        glTranslated(1,0,0);
        glutSolidCube(0.95);
        glTranslated(0,1,0);
        glutSolidCube(0.95);
     glPopMatrix();

     glColor3d(0,0,1);
    glPushMatrix();
        glTranslated(2.5,3,-1);
        glutSolidCube(0.95);
        glTranslated(0,-1,0);
        glutSolidCube(0.95);
        glTranslated(-1,0,0);
        glutSolidCube(0.95);
        glTranslated(0,-1,0);
        glutSolidCube(0.95);
     glPopMatrix();
            //  glRotated(45,0,1,0);
       // glRotated(100,0,0,1);
        //glutWireCube(1);
   }

  glFlush();
  glutSwapBuffers();

  glColor4f (0.0, 0.0, 1.0, 0.0);
  glPrint (4.5, 17, GLUT_BITMAP_TIMES_ROMAN_24, "Games Collection");
  glPrint (8, 16, GLUT_BITMAP_HELVETICA_12, "by Ann Romanova");
  glPrint (7.7, 1, GLUT_BITMAP_HELVETICA_10, "(c) 2018");

  if (startStep==1) glColor4f (1, 0, 0, 0.0);
  else glColor4f (0.0, 0.0, 1.0, 0.0);
  glPrint (7.6, 13, GLUT_BITMAP_HELVETICA_18, "Tetris");

  if (startStep==2) glColor4f (1, 0, 0, 0.0);
  else glColor4f (0.0, 0.0, 1.0, 0.0);
  glPrint (7.5, 11, GLUT_BITMAP_HELVETICA_18, "Snake");

  if (startStep==3) glColor4f (1, 0, 0, 0.0);
  else glColor4f (0.0, 0.0, 1.0, 0.0);
  glPrint (7.1, 9, GLUT_BITMAP_HELVETICA_18, "Arcanoid");

  if (startStep==4) glColor4f (1, 0, 0, 0.0);
  else glColor4f (0.0, 0.0, 1.0, 0.0);
  glPrint (7.8, 7, GLUT_BITMAP_HELVETICA_18, "Race");

  if (startStep==5) glColor4f (1, 0, 0, 0.0);
  else glColor4f (0.0, 0.0, 1.0, 0.0);
  glPrint (8, 5, GLUT_BITMAP_HELVETICA_18, "Exit");
}

void timer(int value);

void renderTetrisScreen()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0,0,0);
  glBegin(GL_LINE_STRIP);
  glVertex2d(10.1, 30);
  glVertex2d(10.1, 0);
  glEnd();

  sprintf(s,"Score: %d",score);
  glRasterPos2f(11,17);
  char *c;
  for (c=s; *c != '\0'; c++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

  if (score>Bscore){
    std::ofstream fout("TetScore.txt");
    fout << score;
    fout.close();
  }
  std::ifstream fin("TetScore.txt");
    fin >> Bscore;
    fin.close();

  sprintf(s,"Best Score: %d",Bscore);
  glRasterPos2f(11,15);
  for (c=s; *c != '\0'; c++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);


  glPrint (10.6,6, GLUT_BITMAP_HELVETICA_18, "Help:");
  glPrint (10.6,4.5, GLUT_BITMAP_HELVETICA_18, "+ Add speed");
  glColor3d(0,0,1);
  glPrint (10.6,4.5, GLUT_BITMAP_HELVETICA_18, "+");
  glColor3f(0,0,0);
  glPrint (10.6,3.5, GLUT_BITMAP_HELVETICA_18, "- Reduce speed");
  glColor3d(0,0,1);
  glPrint (10.6,3.5, GLUT_BITMAP_HELVETICA_18, "-");
  glColor3f(0,0,0);
  glPrint (10.6,2.5, GLUT_BITMAP_HELVETICA_18, "up arrow Rotate");
  glColor3d(0,0,1);
  glPrint (10.6,2.5, GLUT_BITMAP_HELVETICA_18, "up arrow");
  glColor3f(0,0,0);
  glPrint (10.6,1.5, GLUT_BITMAP_HELVETICA_18, "other arrow Move");
  glColor3d(0,0,1);
  glPrint (10.6,1.5, GLUT_BITMAP_HELVETICA_18, "other arrow");


  glBegin(GL_QUADS);
  for (int i = 0; i < column; i++) {
    for (int j = 0; j < row; j++) {
      if (blocks[i][j]==Red && (blocks[i][j]!=0)) {
            glColor3f(1, 0, 0);
        glVertex2f(j,     column - i);
            glColor3f(1, 0.2, 0);
        glVertex2f(j,     column - i - 1);
            glColor3f(1, 0.2, 0.2);
        glVertex2f(j + 1, column - i - 1);
            glColor3f(0.8, 0.1, 0);
        glVertex2f(j + 1, column - i);
      }
      if (blocks[i][j]==Yellow && (blocks[i][j]!=0)) {
            glColor3f(1, 0.8, 0);
        glVertex2f(j,     column - i);
            glColor3f(0.8, 1, 0);
        glVertex2f(j,     column - i - 1);
            glColor3f(0.9, 1, 0.1);
        glVertex2f(j + 1, column - i - 1);
            glColor3f(0.8, 0.9, 0);
        glVertex2f(j + 1, column - i);
      }
      if (blocks[i][j]==Blue && (blocks[i][j]!=0)) {
            glColor3f(0, 0.2, 1);
        glVertex2f(j,     column - i);
            glColor3f(0.2, 0, 1);
        glVertex2f(j,     column - i - 1);
            glColor3f(0.2, 0.2, 1);
        glVertex2f(j + 1, column - i - 1);
            glColor3f(0, 0.1, 0.8);
        glVertex2f(j + 1, column - i);
      }
      if (blocks[i][j]==Green && (blocks[i][j]!=0)) {
            glColor3f(0, 1, 0.2);
        glVertex2f(j,     column - i);
            glColor3f(0.2, 1, 0);
        glVertex2f(j,     column - i - 1);
            glColor3f(0.2, 1, 0.2);
        glVertex2f(j + 1, column - i - 1);
            glColor3f(0.1, 0.8, 0);
        glVertex2f(j + 1, column - i);
      }
    }}

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (curr_block[i][j]==Red && curr_block[i][j]!=0) {
        glColor3f(1, 0, 0);
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i));
        glColor3f(1, 0.2, 0);
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i + 1));
        glColor3f(1, 0.2, 0.2);
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i + 1));
        glColor3f(0.8, 0.1, 0);
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i));
      }
      if (curr_block[i][j]==Yellow && curr_block[i][j]!=0){
            glColor3f(1, 0.8, 0);
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i));
        glColor3f(0.8, 1, 0);
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i + 1));
        glColor3f(0.9, 1, 0.1);
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i + 1));
        glColor3f(0.8, 0.9, 0);
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i));
      }
      if (curr_block[i][j]==Blue && curr_block[i][j]!=0){
            glColor3f(0, 0.2, 1);
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i));
        glColor3f(0.2, 0, 1);
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i + 1));
        glColor3f(0.2, 0.2, 1);
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i + 1));
        glColor3f(0, 0.1, 0.8);
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i));
      }
      if (curr_block[i][j]==Green && curr_block[i][j]!=0){
            glColor3f(0, 1, 0.2);
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i));
            glColor3f(0.2, 1, 0);
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i + 1));
            glColor3f(0.2, 1, 0.2);
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i + 1));
            glColor3f(0.1, 0.8, 0);
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i));
      }
     }
  }
  glEnd();
  glFlush();
}

void renderArcScreen()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0,0,0);
  glBegin(GL_LINE_STRIP);
  glVertex2d(10, 30);
  glVertex2d(10, 0);
  glEnd();

  if (sh==0) {glColor3d(1,0,0); glPrint (3,16, GLUT_BITMAP_HELVETICA_18, "You Win!!!"); b=0;}
  sh=0;

   glPrint (10.8,6, GLUT_BITMAP_HELVETICA_18, "Help:");
  glPrint (10.8,4.5, GLUT_BITMAP_HELVETICA_18, "+ Add speed");
  glColor3d(0,0,1);
  glPrint (10.8,4.5, GLUT_BITMAP_HELVETICA_18, "+");
  glColor3f(0,0,0);
  glPrint (10.8,3.5, GLUT_BITMAP_HELVETICA_18, "- Reduce speed");
  glColor3d(0,0,1);
  glPrint (10.8,3.5, GLUT_BITMAP_HELVETICA_18, "-");
  glPrint (10.8,2.5, GLUT_BITMAP_HELVETICA_18, "right and left");
  glColor3d(0,0,0);
  glPrint (10.8,1.5, GLUT_BITMAP_HELVETICA_18, "arrows Move");
  glColor3d(0,0,1);
  glPrint (10.8,1.5, GLUT_BITMAP_HELVETICA_18, "arrows");

  for (int i=0; i<10; i++) blocks[my_pos[0]][i]=0;
  for (int i=0; i<3; i++) blocks[my_pos[0]][my_pos[1]+i]=1;


  glBegin(GL_QUADS);
  for (int i = 0; i < column-5; i++) {
    for (int j = 0; j < row; j++) {
      if (blocks[i][j]==Red) glColor3f(1, 0, 0);
      if (blocks[i][j]==Yellow) glColor3f(1, 1, 0);
      if (blocks[i][j]==Blue) glColor3f(0, 0, 1);
      if (blocks[i][j]==Green) glColor3f(0, 1, 0);
      if (blocks[i][j]!=0) {
        sh=sh+1;
        glVertex2f(j,     column - i);
        glVertex2f(j,     column - i - 0.9);
        glVertex2f(j + 0.9, column - i - 0.9);
        glVertex2f(j + 0.9, column - i);
      }
    }
  }

        glColor3f(1,0,0);
        glVertex2f(my_pos[1],  column -my_pos[0] );

        glColor3f(1,1,0);
        glVertex2f(my_pos[1],  column -my_pos[0]-1 );

        glColor3f(1,1,0);
        glVertex2f(my_pos[1]+2 + 1, column -my_pos[0]-1 );

        glColor3f(1,0,0);
        glVertex2f(my_pos[1]+2 + 1, column -my_pos[0] );

        glVertex2f(pos[0]-0.15,     column - (pos[1]-0.4));
        glVertex2f(pos[0]-0.15,     column - (pos[1] + 0.2));
        glVertex2f(pos[0]+0.42, column - (pos[1] + 0.2));
        glVertex2f(pos[0]+0.42, column - (pos[1]-0.4));

        glVertex2f(pos[0]+0.12,     column - (pos[1]-0.5));
        glVertex2f(pos[0]-0.28 , column - (pos[1]-0.1 ));
        glVertex2f(pos[0]+0.12,     column - (pos[1] + 0.3));
        glVertex2f(pos[0] + 0.52, column - (pos[1]-0.1));

  glEnd();
  glFlush();
}

void renderRaceScreen()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0,0,0);
  glBegin(GL_LINE_STRIP);
  glVertex2d(10.1, 30);
  glVertex2d(10.1, 0);
  glEnd();

  sprintf(s,"Score: %d",score);
  glRasterPos2f(11,17);
  char *c;
  for (c=s; *c != '\0'; c++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

  if (score>Bscore){
    std::ofstream fout("RcScore.txt");
    fout << score;
    fout.close();
  }
  std::ifstream fin("RcScore.txt");
    fin >> Bscore;
    fin.close();

  sprintf(s,"Best Score: %d",Bscore);
  glRasterPos2f(11,15);
  for (c=s; *c != '\0'; c++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

 glPrint (10.8,8, GLUT_BITMAP_HELVETICA_18, "Help:");
  glPrint (10.8,6.5, GLUT_BITMAP_HELVETICA_18, "+ Add speed");
  glColor3d(0,0,1);
  glPrint (10.8,6.5, GLUT_BITMAP_HELVETICA_18, "+");
  glColor3f(0,0,0);
  glPrint (10.8,5.5, GLUT_BITMAP_HELVETICA_18, "- Reduce speed");
  glColor3d(0,0,1);
  glPrint (10.8,5.5, GLUT_BITMAP_HELVETICA_18, "-");
  glPrint (10.8,3.5, GLUT_BITMAP_HELVETICA_18, "right, left,");
  glPrint (10.8,2.5, GLUT_BITMAP_HELVETICA_18, "up and down");
  glColor3d(0,0,0);
  glPrint (10.8,1.5, GLUT_BITMAP_HELVETICA_18, "arrows Move");
  glColor3d(0,0,1);
  glPrint (10.8,1.5, GLUT_BITMAP_HELVETICA_18, "arrows");


  glBegin(GL_QUADS);
  for (int i = 0; i < column; i++) {
    for (int j = 0; j < row; j++) {
      if (blocks[i][j]==Red) glColor3f(1, 0, 0);
      if (blocks[i][j]==Yellow) glColor3f(1, 1, 0);
      if (blocks[i][j]==Blue) glColor3f(0, 0, 1);
      if (blocks[i][j]==Green) glColor3f(0, 1, 0);
      if (blocks[i][j]!=0) {
        glVertex2f(j,     column - i);
        glVertex2f(j,     column - i - 1);
        glVertex2f(j + 1, column - i - 1);
        glVertex2f(j + 1, column - i);
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      glColor3f(0, 0, 1);

      if (mycar[i][j]!=0) {
            if (blocks[my_pos[1]+i][my_pos[0]+j]==0){
        glVertex2f(my_pos[0] + j,     column - (my_pos[1] + i));
        glVertex2f(my_pos[0] + j,     column - (my_pos[1] + i + 1));
        glVertex2f(my_pos[0] + j + 1, column - (my_pos[1] + i + 1));
        glVertex2f(my_pos[0] + j + 1, column - (my_pos[1] + i));
      } else {glutSetWindowTitle("GAME OVER"); b=0;}}
    }
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 3; j++) {
      glColor3f(1, 0, 0);
      if (notmycar[i][j]!=0) {
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i));
        glVertex2f(curr_pos[0] + j,     column - (curr_pos[1] + i + 1));
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i + 1));
        glVertex2f(curr_pos[0] + j + 1, column - (curr_pos[1] + i));
      }
    }
  }
        glColor3f(1, 1, 1);
        glVertex2f(0, 20);
        glVertex2f(0, 17);
        glVertex2f(10, 17);
        glVertex2f(10, 20);

       switch (bort){
        case 1:
        {for (int i=0; i<7; i++){
             glColor3f(0, 0, 0);
            glVertex2f(0.5, 2+3*i);
            glVertex2f(0, 2+3*i);
            glVertex2f(0, 0.15+3*i);
             glColor3f(1, 1, 1);
            glVertex2f(0.5, 0.15+3*i);

             glColor3f(0.3, 0.1, 0);
            glVertex2f(0, 0.15+3*i);
            glVertex2f(0.15, 0+3*i);
            glVertex2f(0.65, 0+3*i);
            glVertex2f(0.5, 0.15+3*i);

             glColor3f(0, 0.1, 0.3);
            glVertex2f(0.65, 0+3*i);
            glVertex2f(0.5, 0.15+3*i);
            glVertex2f(0.5, 2+3*i);
            glVertex2f(0.65, 1.85+3*i);

             glColor3f(0, 0, 0);
            glVertex2f(9.5, 2+3*i);
            glVertex2f(10, 2+3*i);
            glVertex2f(10, 0.15+3*i);
             glColor3f(1, 1, 1);
            glVertex2f(9.5, 0.15+3*i);

             glColor3f(0.3, 0.1, 0);
            glVertex2f(10, 0.15+3*i);
            glVertex2f(9.85, 0+3*i);
            glVertex2f(9.35, 0+3*i);
            glVertex2f(9.5, 0.15+3*i);

             glColor3f(0, 0.1, 0.3);
            glVertex2f(9.35, 0+3*i);
            glVertex2f(9.5, 0.15+3*i);
            glVertex2f(9.5, 2+3*i);
            glVertex2f(9.35, 1.85+3*i);
            } break;}
         case 2:
        {
           for (int i=0; i<8; i++){
             glColor3f(0, 0, 0);
            glVertex2f(0.5, 1+3*i);
            glVertex2f(0, 1+3*i);
            glVertex2f(0, -0.85+3*i);
             glColor3f(1, 1, 1);
            glVertex2f(0.5, -0.85+3*i);

             glColor3f(0.3, 0.1, 0);
            glVertex2f(0, -0.85+3*i);
            glVertex2f(0.15, -1+3*i);
            glVertex2f(0.65, -1+3*i);
            glVertex2f(0.5, -0.85+3*i);

             glColor3f(0, 0.1, 0.3);
            glVertex2f(0.65, -1+3*i);
            glVertex2f(0.5, -0.85+3*i);
            glVertex2f(0.5, 1+3*i);
            glVertex2f(0.65, 0.85+3*i);

              glColor3f(0, 0, 0);
            glVertex2f(9.5, 1+3*i);
            glVertex2f(10, 1+3*i);
            glVertex2f(10, -0.85+3*i);
             glColor3f(1, 1, 1);
            glVertex2f(9.5, -0.85+3*i);

             glColor3f(0.3, 0.1, 0);
            glVertex2f(10, -0.85+3*i);
            glVertex2f(9.85, -1+3*i);
            glVertex2f(9.35, -1+3*i);
            glVertex2f(9.5, -0.85+3*i);

             glColor3f(0, 0.1, 0.3);
            glVertex2f(9.35, -1+3*i);
            glVertex2f(9.5, -0.85+3*i);
            glVertex2f(9.5, 1+3*i);
            glVertex2f(9.35, 0.85+3*i);

            } break;}

        case 3:
        {for (int i=0; i<9; i++){
             glColor3f(0, 0, 0);
            glVertex2f(0.5, 3+3*i);
            glVertex2f(0, 3+3*i);
            glVertex2f(0, 1.15+3*i);
             glColor3f(1, 1, 1);
            glVertex2f(0.5, 1.15+3*i);

             glColor3f(0.3, 0.1, 0);
            glVertex2f(0, 1.15+3*i);
            glVertex2f(0.15, 1+3*i);
            glVertex2f(0.65, 1+3*i);
            glVertex2f(0.5, 1.15+3*i);

             glColor3f(0, 0.1, 0.3);
            glVertex2f(0.65, 1+3*i);
            glVertex2f(0.5, 1.15+3*i);
            glVertex2f(0.5, 3+3*i);
            glVertex2f(0.65, 2.85+3*i);

             glColor3f(0, 0, 0);
            glVertex2f(9.5, 3+3*i);
            glVertex2f(10, 3+3*i);
            glVertex2f(10, 1.15+3*i);
             glColor3f(1, 1, 1);
            glVertex2f(9.5, 1.15+3*i);

             glColor3f(0.3, 0.1, 0);
            glVertex2f(10, 1.15+3*i);
            glVertex2f(9.85, 1+3*i);
            glVertex2f(9.35, 1+3*i);
            glVertex2f(9.5, 1.15+3*i);

             glColor3f(0, 0.1, 0.3);
            glVertex2f(9.35, 1+3*i);
            glVertex2f(9.5, 1.15+3*i);
            glVertex2f(9.5, 3+3*i);
            glVertex2f(9.35, 2.85+3*i);
            }break;}
       }

        glColor3f(1, 1, 1);
        glVertex2f(0, 20);
        glVertex2f(0, 17);
        glVertex2f(10, 17);
        glVertex2f(10, 20);

        glColor3f(0, 0, 0);
        for (int i=0; i<3; i++){
            glVertex2f(0+3.33*i, 20);
            glVertex2f(0+3.33*i, 18.5);
            glVertex2f(1.66+3.33*i, 18.5);
            glVertex2f(1.66+3.33*i, 20);
        }
        for (int i=0; i<3; i++){
            glVertex2f(1.66+3.33*i, 18.5);
            glVertex2f(1.66+3.33*i, 17);
            glVertex2f(3.33+3.33*i, 17);
            glVertex2f(3.33+3.33*i, 18.5);
        }
  glEnd();
  glFlush();
}

void mainDisplay()
{
  switch (gameMode)
    {
        case 0: renderStartScreen (); break;
        case 1: renderTetrisScreen(); break;
        case 2: renderZmejkaScreen(); break;
        case 3: renderArcScreen();    break;
        case 4: renderRaceScreen();   break;
    }
    glutSwapBuffers ();
}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glLoadIdentity();
  gluOrtho2D(0, width/block_size, 0, height/block_size);
}

bool move(int x, int y)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (curr_block[i][j]!=0) {
        int next_x = curr_pos[0] +j + x;
        int next_y = curr_pos[1] +i + y;
        if (!empty(next_x, next_y)) {
          return false;
        }
      }
    }
  }
  curr_pos[0] += x;
  curr_pos[1] += y;
  glutPostRedisplay();
  return true;
}


void flush(int value);
void shift(int value);

void timer(int value)
{
  if (!move(0, 1)) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (curr_block[i][j]!=0) {
          blocks[curr_pos[1] + i][curr_pos[0] + j] = a;
        }
      }
    }
    memset(curr_block, 0, 4 * 4 * sizeof(int));
    flush(0);
    return;
  }
  if (b==1) glutTimerFunc(interval, timer, 0);
}

void throw_new_block()
{
  init_curr_block();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (curr_block[i][j]!=0 && blocks[curr_pos[1] + i][curr_pos[0] + j]!=0) {
        memset(curr_block, 0, 4 * 4 * sizeof(int));
        glutSetWindowTitle("GAME OVER");
      //  return;
      }
    }
  }
  glutPostRedisplay();
  glutTimerFunc(interval, timer, 0);
}

void shift(int y)
{
  for (int i = y; i >= 1; i--) {
    for (int j = 0; j < row; j++) {
      blocks[i][j] = blocks[i - 1][j];
    }
  }
  glutPostRedisplay();
  glutTimerFunc(interval, flush, 0);
}

void flush(int value){
  for (int i = column - 1; i >= 0; i--) {
    int j;
    for (j = 0; j < row; j++) {
      if (blocks[i][j]==0) {
        break;
      }
    }
    if (j == row) {
      memset(blocks[i], 0, row * sizeof(int));
      score=score+10;
      glutPostRedisplay();
      glutTimerFunc(interval, shift, i);
      return;
    }
  }
  throw_new_block();
}

void rotate()
{
  int tmp[4][4] = {};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (curr_block[i][j]!=0) {
        a=curr_block[i][j];
        int next_x = curr_pos[0] + i;
        int next_y = curr_pos[1] + 4 - j;
        if (!empty(next_x, next_y)) {
          return;
        }
        tmp[4 - j - 1][i] = a;
      }
    }
  }
  memcpy(curr_block, tmp, 4 * 4 * sizeof(int));
  glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
  switch (key){
  case 'p': system("PAUSE"); break;
  case '-':
    {if (interval<1500)interval=interval+100; break;  }
  case '=':
    {if (interval>100) interval=interval-100; break;  }
  case 13:
    {
    if (gameMode==0)
     {
      switch (startStep){
      case 1:
        {
            b=1;
            gameMode=1;
            interval=500;
            score=0;
            Bscore=0;
            glutTimerFunc(interval, timer, 0);
            init();
            glutPostRedisplay();
            break;
        }
      case 2:
        {
            b=2;
            gameMode=2;
            interval=500;
            score=0;
            std::ifstream fin("ZmScore.txt");
    fin >> Bscore;
    fin.close();
        //    Bscore=0;
            glutTimerFunc(interval, timer1, 0);
            srand(time(0));
            NewApple();
            ss[0].x=2; ss[1].x=2;
            ss[0].y=2; ss[1].y=1;
            glutPostRedisplay();
            break;
        }
      case 3:
        {
            b=3;
            my_pos[0] = 19;
            my_pos[1] = 3;
            interval=500;
            gameMode=3;
            sh=1;
            pos[0]=rand()%9; pos[1]=14; dx=0.05;
            for (int i=1; i<column-8; i++)
                for (int j=1; j<row-1; j++)
                    if (i%2!=0) blocks[i][j]=rand()%4+1;
            for (int i=0; i<3; i++) blocks[my_pos[0]][my_pos[1]+i]=1;
            glutTimerFunc(interval, timer3, 0);
            glClearColor(0.98, 0.98, 0.99, 0.3);
            srand(time(NULL));
            //glutPostRedisplay();
            break;
        }
      case 4:
        {
            b=4;
            d=0;
            blocks==0;
            gameMode=4;
            interval=200;
            sc=0;
            bort=1;
            score=0;
            std::ifstream fin("RcScore.txt");
            fin >> Bscore;
            fin.close();
            my_pos[0]=3;
            my_pos[1]=16;
            glutTimerFunc(interval*1.2, timer4, 0);

            break;
        }
      case 5:
        {
            exit(0);
            break;
        }
     }
     break;
     }
    }
  case 27:
      {
          if (gameMode==0) exit (0);
          else
            {
                glutSetWindowTitle("Games");
                gameMode=0;
                b=0;
                score=0;
                memset(blocks, 0, row * column * sizeof(int));
                memset(curr_block, 0, 4 * 4 * sizeof(int));
                num=2; dir=0;
                glutPostRedisplay();
            }
          break;
      }

  }
}

void special(int key, int xx, int yy)
{
  switch (key) {
  case GLUT_KEY_LEFT:
    switch (gameMode){
      case 1: move(-1, 0); break;
      case 2: if (dir!=2) dir=1; break;
      case 3: if (my_pos[1]>0) my_pos[1]=my_pos[1]-1; break;
      case 4: if (my_pos[0]>1) {my_pos[0]=my_pos[0]-1; glutPostRedisplay();} break;
    }
    break;
  case GLUT_KEY_RIGHT:
    switch (gameMode){
      case 1: move(1, 0); break;
      case 2: if (dir!=1) dir=2; break;
      case 3: if (my_pos[1]<7) my_pos[1]=my_pos[1]+1; break;
      case 4: if (my_pos[0]<6) {my_pos[0]=my_pos[0]+1; glutPostRedisplay();} break;
    }
    break;
  case GLUT_KEY_UP:
    switch (gameMode){
      case 0: if (startStep!=1) {startStep=startStep-1; glutPostRedisplay();} break;
      case 1: rotate(); break;
      case 2: if (dir!=3) dir=0; break;
      case 4: if (my_pos[1]>3) {my_pos[1]=my_pos[1]-1; glutPostRedisplay();} break;
    }
    break;
  case GLUT_KEY_DOWN:
    switch (gameMode){
      case 0:
        if (startStep!=5) {startStep=startStep+1; glutPostRedisplay();} break;
      case 1: move(0, 1); break;
      case 2: if (dir!=0) dir=3; break;
      case 4: if (my_pos[1]<16) {my_pos[1]=my_pos[1]+1; glutPostRedisplay();} break;
    }
    break;
  }
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(block_size * row * 1.8, block_size * column);
  glutInitDisplayMode(GLUT_RGB);
  glutCreateWindow("Games");

  glutDisplayFunc(mainDisplay);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);

  glutMainLoop();
}
