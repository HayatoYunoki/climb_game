#include <GLUT/glut.h>
#include <glpng/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>


void Display(void);
void Reshape(int, int);
void Timer(int);
void PutSprite(int, int, int, pngInfo *);
void SpecialKey(int,int,int);
void make_map(void);

int zure=0,p_move=0,p_right=0,p_left=0,coin_count=0,level=0,heru=0;
int coin0=0,coin1=0,coin2=0,ban=0,bkht=0,toge=0,dead=0,shi=0;
int teki_zure=0,t_dead=0,t_shi=0,move_key_shi=0,level_up=0,move_key_level=0;
int life=5,s_p=0,s_count=0;
char map1[50][6],meter[2]={'1','0'};

GLuint img[34];
pngInfo info[34];




int main(int argc, char **argv)
{
  int i;
  char fname[20];

  glutInit(&argc,argv);
  glutInitWindowSize(700,100*5);
  glutCreateWindow("Climb Game");
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA);
  glClearColor(0.0,0.0,1.0,1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  for(i=0;i<34;i++)
  {
    sprintf(fname,"climbgame%d.png",i+1);
    img[i]=pngBind(fname,PNG_NOMIPMAP,PNG_ALPHA,&info[i],GL_CLAMP,GL_NEAREST,GL_NEAREST);
  }

  make_map();
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutSpecialFunc(SpecialKey);
  if(dead==0&&bkht==0&&t_dead==0)
  {
    if(level==1)
      glutTimerFunc(1000,Timer,0);
    else if(level==2)
      glutTimerFunc(900,Timer,0);
    else if(level==3)
      glutTimerFunc(800,Timer,0);
    else if(level==4)
      glutTimerFunc(700,Timer,0);
    else if(level==5)
      glutTimerFunc(600,Timer,0);
  }

  glutMainLoop();

  return 0;
}




void Timer(int t)
{
  if(bkht==0&&shi==0&&t_shi==0&&level_up!=1)
  {
    if(level==1)
      glutTimerFunc(1000,Timer,0);
    else if(level==2)
      glutTimerFunc(900,Timer,0);
    else if(level==3)
      glutTimerFunc(800,Timer,0);
    else if(level==4)
      glutTimerFunc(700,Timer,0);
    else if(level==5)
      glutTimerFunc(600,Timer,0);
  //}
  if(ban==1)
  {
    bkht=1; //次爆弾を踏む→上を踏んだら爆発
  }
  if(dead==1)
  {
    shi=1;
  }
  if(t_dead==1)
  {
    t_shi=1;
  }
  //printf("hairu\n");
  toge++;
  make_map();
  zure+=100;
  p_move++;
  teki_zure++;
  if(coin0==1)
  {
    life++;
    coin0=0;
  }
  if(p_move==5)
    meter[1]='9';
  else if(p_move==10)
    meter[1]='8';
  else if(p_move==15)
    meter[1]='7';
  else if(p_move==20)
    meter[1]='6';
  else if(p_move==25)
    meter[1]='5';
  else if(p_move==30)
    meter[1]='4';
  else if(p_move==35)
    meter[1]='3';
  else if(p_move==40)
    meter[1]='2';
  else if(p_move==45)
    meter[1]='1';
  if(map1[49-p_move][2+p_right-p_left]=='T')
  {
    if(s_p==0)
    {
      if(heru==1)
        heru=0;
      else if(heru==0)
      {
        //printf("hairu\n");
        t_dead=1;
      }
    }
  }
  else if(map1[49-p_move][2+p_right-p_left]=='B')
  {
    if(s_p==0)
      ban=1;
    //printf("kai\n");
  }
  else if(map1[49-p_move][2+p_right-p_left]=='N')
  {
    if(s_p==0)
      dead=1;
  }
  else if(map1[49-p_move][2+p_right-p_left]=='H')
  {
    if(heru==0)
      heru=1;
  }
  else if(map1[49-p_move][2+p_right-p_left]=='S')
  {
    s_p=1;
  }
  if(s_p==1)
  {
    s_count++;
  }
  if(s_count==8)
  {
    s_p=0;
  }
  if(map1[49-p_move][2+p_right-p_left]=='1')
  {
    if(coin2==9)
    {
      if(coin1==9)
      {
        coin1=0;
        coin2=0;
        coin0++;
      }
      else
      {
        coin2=0;
        coin1++;
      }
    }
    else
      coin2++;
  }
  if(map1[49-p_move][2+p_right-p_left]=='3')
  {
    if(coin1==9)
    {
      coin1=0;
      coin0++;
    }
    else
      coin1++;
  }

  map1[49-p_move][2+p_right-p_left]='P';

  if(p_move==51)
  {
    sleep(2);
    level++;
    level_up=1;
    //printf("kaisuu\n");
  }

  glutPostRedisplay();
  //printf("displaytimer\n");
 }
}

void Reshape(int w,int h)
{
  glViewport(0,0,w,h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0,w,0,h);
  glScaled(1,-1,1);
  glTranslated(0,-h,0);
  glutReshapeWindow(700,100*5);
}

void Display(void)
{
  int i,j,im;

  glClear(GL_COLOR_BUFFER_BIT);

  if(level==0)
  {
    PutSprite(img[18],0,0,&info[18]);
    glFlush();
  }
  else if(level>0)
  {
    for(i=0;i<50;i++)
    {
      for(j=0;j<5;j++)
      {
        im=map1[i][j];
        if(im=='N')
          PutSprite(img[6],j*100,i*100-100*45+zure,&info[6]);

        if(im=='P')
        {
          if(s_p==1)
            PutSprite(img[24],j*100,i*100-100*45+zure,&info[24]);
          else
          {
            if(dead==1&&t_dead==0&&ban==0)
              PutSprite(img[6],j*100,i*100-100*45+zure,&info[6]);
            else if(heru==0)
              PutSprite(img[1],j*100,i*100-100*45+zure,&info[1]);
            else if(heru==1)
              PutSprite(img[2],j*100,i*100-100*45+zure,&info[2]);

            if(t_dead==1)
              PutSprite(img[13],j*100,i*100-100*45+zure,&info[13]);
            if(ban==1)
              PutSprite(img[3],j*100,i*100-100*45+zure,&info[3]);
          }
        }
        if(im=='K')
          PutSprite(img[0],j*100,i*100-100*45+zure,&info[0]);//-100*45:配列のしたから画面に表示
        if(im=='B')
          PutSprite(img[3],j*100,i*100-100*45+zure,&info[3]);
        if(im=='T')
          PutSprite(img[13],j*100,i*100-100*45+zure,&info[13]);
        if(im=='1')
          PutSprite(img[31],j*100,i*100-100*45+zure,&info[31]);
        if(im=='H')
          PutSprite(img[14],j*100,i*100-100*45+zure,&info[14]);
        if(im=='3')
          PutSprite(img[19],j*100,i*100-100*45+zure,&info[19]);
        if(im=='S')
          PutSprite(img[23],j*100,i*100-100*45+zure,&info[23]);

      }
    }
    PutSprite(img[15],0,-100*46+zure,&info[15]);

    PutSprite(img[21],500,50,&info[21]);
    PutSprite(img[4],500,0,&info[4]);
    PutSprite(img[11],500,100,&info[11]);
    PutSprite(img[16],500,200,&info[16]);
    PutSprite(img[12],500,300,&info[12]);
    PutSprite(img[22],500,350,&info[22]);

    //残りのメートル
    glColor3ub(0,0,0);
    glRasterPos2d(595,85);
    if(meter[1]=='0')
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,meter[0]);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,meter[1]);
    glColor3ub(0,0,0);
    glRasterPos2d(627,85);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0');
    //コインのカウント
    glColor3ub(0,0,0);
    glRasterPos2d(645,33);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+coin0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+coin1);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+coin2);
    //ステージカウント
    glColor3ub(0,0,0);
    glRasterPos2d(595,180);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+level);
    //残りLIFE
    glColor3ub(0,0,0);
    glRasterPos2d(645,335);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+life);



    if(bkht==1)
    {
      //printf("ban%d\n",ban);
      //printf("bkht%d\n",bkht);
      PutSprite(img[5],0,0,&info[5]);
      glFlush();
      sleep(1);
      life--;
      if(life>0)
      {
        PutSprite(img[8],0,0,&info[8]);
        glFlush();
        move_key_shi=1;
      }
      if(life==0)
      {
        PutSprite(img[17],0,0,&info[17]);
        glFlush();
        level=0;
        glutPostRedisplay();
      }
    }
    else if(shi==1)
    {
      PutSprite(img[7],0,0,&info[7]);
      glFlush();
      sleep(1);
      life--;
      if(life>0)
      {
        PutSprite(img[8],0,0,&info[8]);
        glFlush();
        move_key_shi=1;
      }
      if(life==0)
      {
        PutSprite(img[17],0,0,&info[17]);
        glFlush();
        level=0;
        glutPostRedisplay();
      }
    }
    else if(t_shi==1)
    {
      //printf("t_dead%d\n",t_dead);
      //printf("t_shi%d\n",t_shi);
      PutSprite(img[7],0,0,&info[7]);
      glFlush();
      sleep(1);
      life--;
      if(life>0)
      {
        PutSprite(img[8],0,0,&info[8]);
        glFlush();
        move_key_shi=1;
      }
      if(life==0)
      {
        PutSprite(img[17],0,0,&info[17]);
        glFlush();
        level=0;
        glutPostRedisplay();
      }
    }

    if(level_up==1)
    {
      //printf("a\n");
      if(level<=5)
      {
        PutSprite(img[9],0,0,&info[9]);
        glFlush();
        move_key_level=1;
      }
      else if(level==6)
      {
        PutSprite(img[10],0,0,&info[10]);
        glFlush();
        sleep(3);
        level=0;
        glutPostRedisplay();
      }
    }


    glFlush();
  }
}

void PutSprite(int num,int x,int y,pngInfo *info)
{
  int w,h;

  w=info->Width;
  h=info->Height;

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,num);
  glColor4ub(255,255,255,255);

  glBegin(GL_QUADS);

  glTexCoord2i(0,0);
  glVertex2i(x,y);

  glTexCoord2i(0,1);
  glVertex2i(x,y+h);

  glTexCoord2i(1,1);
  glVertex2i(x+w,y+h);

  glTexCoord2i(1,0);
  glVertex2i(x+w,y);

  glEnd();

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void SpecialKey(int key,int x,int y)
{
  switch(key)
  {
    case GLUT_KEY_F1:
      if(level==0)
      {
        level++;
        level_up=0;
        dead=0;
        t_dead=0;
        shi=0;
        t_shi=0;
        bkht=0;
        ban=0;
        heru=0;
        move_key_shi=0;
        move_key_level=0;
        zure=0;
        p_move=0;
        p_right=0;
        p_left=0;
        toge=0;
        teki_zure=0;
        meter[0]='1';
        meter[1]='0';
        coin0=0;
        coin1=0;
        coin2=0;
        life=5;
        s_count=0;
        make_map();
        glutTimerFunc(1000,Timer,0);
        //glutPostRedisplay();
      }
      break;
    case GLUT_KEY_RIGHT:
      if(2+p_right-p_left!=4&&ban==0&&dead==0&&t_dead==0)//爆弾を踏んだ後動かさないため
      {
        p_right++;
        if(map1[49-p_move][2+p_right-p_left]=='T')
        {
          if(s_p==0)
          {
            if(heru==1)
              heru=0;
            else if(heru==0)
              t_dead=1;
          }
        }
        else if(map1[49-p_move][2+p_right-p_left]=='B')
        {
          if(s_p==0)
            ban=1;
          //printf("kai2\n");
        }
        else if(map1[49-p_move][2+p_right-p_left]=='N')
        {
          if(s_p==0)
            dead=1;
        }
        else if(map1[49-p_move][2+p_right-p_left]=='H')
        {
          if(heru==0)
            heru=1;
        }
        else if(map1[49-p_move][2+p_right-p_left]=='S')
        {
          s_p=1;
        }
        //if(s_p==1)
        //{
        //  s_count++;
        //}
        if(s_count==8)
        {
          s_p=0;
        }
        if(map1[49-p_move][2+p_right-p_left]=='1')
        {
          if(coin2==9)
          {
            if(coin1==9)
            {
              coin1=0;
              coin2=0;
              coin0++;
            }
            else
            {
              coin2=0;
              coin1++;
            }
          }
          else
            coin2++;
        }
        if(map1[49-p_move][2+p_right-p_left]=='3')
        {
          if(coin1==9)
          {
            coin1=0;
            coin0++;
          }
          else
            coin1++;
        }
        map1[49-p_move][2+p_right-p_left]='P';
        map1[49-p_move][2+p_right-p_left-1]='K';

      }
      break;

    case GLUT_KEY_LEFT:
      if(2+p_right-p_left!=0&&ban==0&&dead==0&&t_dead==0)
      {
        p_left++;
        if(map1[49-p_move][2+p_right-p_left]=='T')
        {
          if(s_p==0)
          {
            if(heru==1)
              heru=0;
            else if(heru==0)
             t_dead=1;
          }
        }
        else if(map1[49-p_move][2+p_right-p_left]=='B')
        {
          if(s_p==0)
            ban=1;
          //printf("kai3\n");
        }
        else if(map1[49-p_move][2+p_right-p_left]=='N')
        {
          if(s_p==0)
            dead=1;
        }
        else if(map1[49-p_move][2+p_right-p_left]=='H')
        {
          if(heru==0)
            heru=1;
        }
        else if(map1[49-p_move][2+p_right-p_left]=='S')
        {
          s_p=1;
        }
      //  if(s_p==1)
      //  {
        //  s_count++;
      //  }
        if(s_count==8)
        {
          s_p=0;
        }
        if(map1[49-p_move][2+p_right-p_left]=='1')
        {
          if(coin2==9)
          {
            if(coin1==9)
            {
              coin1=0;
              coin2=0;
              coin0++;
            }
            else
            {
              coin2=0;
              coin1++;
            }
          }
          else
            coin2++;
        }
        if(map1[49-p_move][2+p_right-p_left]=='3')
        {
          if(coin1==9)
          {
            coin1=0;
            coin0++;
          }
          else
            coin1++;
        }
        map1[49-p_move][2+p_right-p_left]='P';
        map1[49-p_move][2+p_right-p_left+1]='K';
      }
      break;

      case GLUT_KEY_UP:
        if(move_key_shi==1||move_key_level==1)
        {

          level_up=0;
          dead=0;
          t_dead=0;
          shi=0;
          t_shi=0;
          bkht=0;
          ban=0;
          heru=0;
          move_key_shi=0;
          move_key_level=0;
          zure=0;
          p_move=0;
          p_right=0;
          p_left=0;
          toge=0;
          teki_zure=0;
          meter[0]='1';
          meter[1]='0';
          s_count=0;
          make_map();
          //printf("%d\n",level);
          if(level==1)
            glutTimerFunc(1000,Timer,0);
          else if(level==2)
            glutTimerFunc(900,Timer,0);
          else if(level==3)
            glutTimerFunc(800,Timer,0);
          else if(level==4)
            glutTimerFunc(700,Timer,0);
          else if(level==5)
            glutTimerFunc(600,Timer,0);
        }
        break;

      case GLUT_KEY_DOWN:
        exit(0);
        break;

  }
  glutPostRedisplay();
  //printf("displaykey\n");
}

void make_map(void)
{
  int i;
  if(level==1)
  {
    strcpy(map1[0],"KK1KK");
    strcpy(map1[1],"1KTK1");
    strcpy(map1[2],"1K1TT");
    strcpy(map1[3],"TTKKK");
    strcpy(map1[4],"T11KK");
    strcpy(map1[5],"TTTKH");
    strcpy(map1[6],"KK1KT");
    strcpy(map1[7],"KKKTK");
    strcpy(map1[8],"1HK1T");
    strcpy(map1[9],"TTTKK");
    strcpy(map1[10],"KK1KK");
    strcpy(map1[11],"T1TKK");
    strcpy(map1[12],"KK1K1");
    strcpy(map1[13],"KT1KT");
    strcpy(map1[14],"KK1KK");
    strcpy(map1[15],"1KHKK");
    strcpy(map1[16],"TTTKK");
    strcpy(map1[17],"3KKKK");
    strcpy(map1[18],"TTK1K");
    strcpy(map1[19],"KKKKK");
    strcpy(map1[20],"K11KK");
    strcpy(map1[21],"KTTKK");
    strcpy(map1[22],"KKKK1");
    strcpy(map1[23],"KHKKK");
    strcpy(map1[24],"1KT1T");
    strcpy(map1[25],"1KTKK");
    strcpy(map1[26],"KKK1K");
    strcpy(map1[27],"KTTTH");
    strcpy(map1[28],"K3KKK");
    strcpy(map1[29],"TTTTK");
    strcpy(map1[30],"K1KKK");
    strcpy(map1[31],"K1KKK");
    strcpy(map1[32],"KKK1K");
    strcpy(map1[33],"K1KKK");
    strcpy(map1[34],"KTTKK");
    strcpy(map1[35],"K11KK");
    strcpy(map1[36],"KKKHK");
    strcpy(map1[37],"KKTTK");
    strcpy(map1[38],"K1KKK");
    strcpy(map1[39],"TTKKK");
    strcpy(map1[40],"K3KKT");
    strcpy(map1[41],"KTKKS");
    strcpy(map1[42],"KKTTK");
    strcpy(map1[43],"T11KK");
    strcpy(map1[44],"KTKKK");
    strcpy(map1[45],"KT111");
    strcpy(map1[46],"KK1TK");
    strcpy(map1[47],"KKTHK");
    strcpy(map1[48],"KK1KK");
    strcpy(map1[49],"KKPKK");
  }
 else if(level==2)
 {
   strcpy(map1[0],"KKK1K");
   strcpy(map1[1],"1KKT1");
   strcpy(map1[2],"HT1KK");
   strcpy(map1[3],"KK1TK");
   strcpy(map1[4],"HKBK1");
   strcpy(map1[5],"B111B");
   strcpy(map1[6],"KTKKT");
   strcpy(map1[7],"KKT1K");
   strcpy(map1[8],"HKKB1");
   strcpy(map1[9],"1KKTK");
   strcpy(map1[10],"KHKK1");
   strcpy(map1[11],"BT1KK");
   strcpy(map1[12],"1KKKT");
   strcpy(map1[13],"HKBK3");
   strcpy(map1[14],"TKKKT");
   strcpy(map1[15],"TKBKK");
   strcpy(map1[16],"KKBTK");
   strcpy(map1[17],"1KBKH");
   strcpy(map1[18],"KK1KT");
   strcpy(map1[19],"KTBKK");
   strcpy(map1[20],"TKKKH");
   strcpy(map1[21],"1KBKK");
   strcpy(map1[22],"TKKK1");
   strcpy(map1[23],"1TKKT");
   strcpy(map1[24],"KBKK1");
   strcpy(map1[25],"HKKK1");
   strcpy(map1[26],"BKKKK");
   strcpy(map1[27],"1KKKK");
   strcpy(map1[28],"SKTK1");
   strcpy(map1[29],"BTKKH");
   strcpy(map1[30],"111KK");
   strcpy(map1[31],"KKTKK");
   strcpy(map1[32],"K1HTK");
   strcpy(map1[33],"1BTBH");
   strcpy(map1[34],"1KKK3");
   strcpy(map1[35],"HKBBB");
   strcpy(map1[36],"T1KKK");
   strcpy(map1[37],"HKKKK");
   strcpy(map1[38],"KKT11");
   strcpy(map1[39],"KTHKK");
   strcpy(map1[40],"1KKBK");
   strcpy(map1[41],"KHKKT");
   strcpy(map1[42],"KT1K3");
   strcpy(map1[43],"KKKBB");
   strcpy(map1[44],"1HTKK");
   strcpy(map1[45],"T1KKK");
   strcpy(map1[46],"KK1KK");
   strcpy(map1[47],"KKBHK");
   strcpy(map1[48],"KKKKK");
   strcpy(map1[49],"KKPKK");

 }
 else if(level==3)
 {
   strcpy(map1[0],"1KKKK");
   strcpy(map1[1],"KKTKK");
   strcpy(map1[2],"BTKK1");
   strcpy(map1[3],"HKK1K");
   strcpy(map1[4],"1KBKK");
   strcpy(map1[5],"KTTKK");
   strcpy(map1[6],"B1KKK");
   strcpy(map1[7],"KK1KK");
   strcpy(map1[8],"TBK1K");
   strcpy(map1[9],"HKKTK");
   strcpy(map1[10],"KBKK1");
   strcpy(map1[11],"TKHKK");
   strcpy(map1[12],"1KTK1");
   strcpy(map1[13],"BTKKK");
   strcpy(map1[14],"TKKTK");
   strcpy(map1[15],"3KT1K");
   strcpy(map1[16],"BKB11");
   strcpy(map1[17],"1K1BK");
   strcpy(map1[18],"TKHKK");
   strcpy(map1[19],"1KKKK");
   strcpy(map1[20],"TBTK1");
   strcpy(map1[21],"K1KHK");
   strcpy(map1[22],"KTKKK");
   strcpy(map1[23],"KHKBK");
   strcpy(map1[24],"KK1K1");
   strcpy(map1[25],"BKKBK");
   strcpy(map1[26],"1KKK3");
   strcpy(map1[27],"KKTBK");
   strcpy(map1[28],"1KKHK");
   strcpy(map1[29],"KKTBK");
   strcpy(map1[30],"TKK11");
   strcpy(map1[31],"KKHBK");
   strcpy(map1[32],"KTKK1");
   strcpy(map1[33],"1KHTK");
   strcpy(map1[34],"BKHTK");
   strcpy(map1[35],"1KBKK");
   strcpy(map1[36],"HKKK1");
   strcpy(map1[37],"KKTBK");
   strcpy(map1[38],"SKKK3");
   strcpy(map1[39],"K1TKK");
   strcpy(map1[40],"BKK1H");
   strcpy(map1[41],"1KTHK");
   strcpy(map1[42],"THBK1");
   strcpy(map1[43],"KK1KK");
   strcpy(map1[44],"1KKBK");
   strcpy(map1[45],"1THKK");
   strcpy(map1[46],"KKKK1");
   strcpy(map1[47],"KKT1K");
   strcpy(map1[48],"1HKKK");
   strcpy(map1[49],"KKPKK");
   if(toge%2==0)
   {
     for(i=0;i<50;i++)
     {
       map1[i][4]='N';
     }
   }
 }
 else if(level==4)
 {
   strcpy(map1[0],"KKKKK");
   strcpy(map1[1],"KKKKK");//
   strcpy(map1[2],"KKBKK");
   strcpy(map1[3],"K1BKK");
   strcpy(map1[4],"KKKK1");
   strcpy(map1[5],"HKKKB");
   strcpy(map1[6],"BBKKK");
   strcpy(map1[7],"11KKK");//
   strcpy(map1[8],"1KKBK");
   strcpy(map1[9],"BKBKK");
   strcpy(map1[10],"K1KKB");
   strcpy(map1[11],"KBKKH");
   strcpy(map1[12],"K1KKK");
   strcpy(map1[13],"KKKB1");
   strcpy(map1[14],"1KBKK");
   strcpy(map1[15],"K3KKK");//
   strcpy(map1[16],"KBKKK");
   strcpy(map1[17],"KB1BK");
   strcpy(map1[18],"K1BKH");
   strcpy(map1[19],"KBKKK");
   strcpy(map1[20],"KKBHK");
   strcpy(map1[21],"K1KKK");//
   strcpy(map1[22],"HBK1K");
   strcpy(map1[23],"1KKBK");
   strcpy(map1[24],"KK3KK");//
   strcpy(map1[25],"1KKBK");
   strcpy(map1[26],"HKKKK");
   strcpy(map1[27],"1KK1K");//
   strcpy(map1[28],"KBKKB");
   strcpy(map1[29],"BKK1K");
   strcpy(map1[30],"KKBKB");
   strcpy(map1[31],"K1KKK");//
   strcpy(map1[32],"KKK1K");
   strcpy(map1[33],"BKBKB");
   strcpy(map1[34],"KKKK1");
   strcpy(map1[35],"3KBKK");//
   strcpy(map1[36],"BKKKS");
   strcpy(map1[37],"KKB1K");
   strcpy(map1[38],"KBKBK");
   strcpy(map1[39],"1KKKK");
   strcpy(map1[40],"KBKKH");
   strcpy(map1[41],"KK1KK");//
   strcpy(map1[42],"KKBKK");
   strcpy(map1[43],"1KHKK");
   strcpy(map1[44],"KKKKK");//
   strcpy(map1[45],"KHKK1");
   strcpy(map1[46],"KKBKK");
   strcpy(map1[47],"KKK1K");//
   strcpy(map1[48],"K1KHK");
   strcpy(map1[49],"KKPKK");

   map1[47][teki_zure%5]='T';
   map1[44][teki_zure%5]='T';
   map1[41][teki_zure%5]='T';
   map1[35][teki_zure%5]='T';
   map1[31][teki_zure%5]='T';
   map1[27][teki_zure%5]='T';
   map1[24][teki_zure%5]='T';
   map1[21][teki_zure%5]='T';
   map1[15][teki_zure%5]='T';
   map1[7][teki_zure%5]='T';
   map1[1][teki_zure%5]='T';
 }
 else if(level==5)
 {
   strcpy(map1[0],"KKKKK");
   strcpy(map1[1],"K1KK1");
   strcpy(map1[2],"KBKBK");
   strcpy(map1[3],"KBK1K");
   strcpy(map1[4],"K1KKK");
   strcpy(map1[5],"BKKKB");
   strcpy(map1[6],"KBKBK");
   strcpy(map1[7],"1KKK1");
   strcpy(map1[8],"BBKBB");
   strcpy(map1[9],"3KKKH");
   strcpy(map1[10],"BHB11");
   strcpy(map1[11],"BKBKK");
   strcpy(map1[12],"BKBKB");
   strcpy(map1[13],"KKK11");
   strcpy(map1[14],"KBKKK");
   strcpy(map1[15],"1KBKK");
   strcpy(map1[16],"KKKB1");
   strcpy(map1[17],"B1KKK");
   strcpy(map1[18],"KKHBK");
   strcpy(map1[19],"1KBK1");
   strcpy(map1[20],"KK3KK");
   strcpy(map1[21],"KB1KK");
   strcpy(map1[22],"KBKKH");
   strcpy(map1[23],"1KKHB");
   strcpy(map1[24],"BBKK1");
   strcpy(map1[25],"SKHKB");
   strcpy(map1[26],"KKBKH");
   strcpy(map1[27],"BKHKK");
   strcpy(map1[28],"K1KBK");
   strcpy(map1[29],"KBBKK");
   strcpy(map1[30],"KKHKK");
   strcpy(map1[31],"B3KKK");
   strcpy(map1[32],"KBKKK");
   strcpy(map1[33],"KK1KK");
   strcpy(map1[34],"HKKB1");
   strcpy(map1[35],"KKBKK");
   strcpy(map1[36],"K1BK1");
   strcpy(map1[37],"KBKKK");
   strcpy(map1[38],"KKKBK");
   strcpy(map1[39],"BKHBK");
   strcpy(map1[40],"1K1KK");
   strcpy(map1[41],"1TKKK");
   strcpy(map1[42],"KBHK1");
   strcpy(map1[43],"BKKBK");
   strcpy(map1[44],"HK1KK");
   strcpy(map1[45],"KBHKK");
   strcpy(map1[46],"1KKB1");
   strcpy(map1[47],"BBKKK");
   strcpy(map1[48],"KKHK1");
   strcpy(map1[49],"KKPKK");
   if(toge%2==0)
   {
     for(i=0;i<50;i++)
     {
       map1[i][4]='N';
     }
   }
   map1[47][teki_zure%5]='T';
   map1[44][teki_zure%5]='T';
   map1[41][teki_zure%5]='T';
   map1[40][teki_zure%5]='T';
   map1[34][teki_zure%5]='T';
   map1[31][teki_zure%5]='T';
   map1[30][teki_zure%5]='T';
   map1[24][teki_zure%5]='T';
   map1[21][teki_zure%5]='T';
   map1[20][teki_zure%5]='T';
   map1[14][teki_zure%5]='T';
   map1[12][teki_zure%5]='T';
   map1[10][teki_zure%5]='T';
   map1[4][teki_zure%5]='T';
   map1[3][teki_zure%5]='T';
   map1[1][teki_zure%5]='T';
 }
}
