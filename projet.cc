#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
using namespace std;

int read_keybord();

/******************************************************************************/

#define H 20
#define L 50

char screen[H][L];

void display()
{
  system("clear"); 
  for (int i=0; i<H; i++)
    {
      for (int j=0; j<L; j++)
	if (i==0||i==H-1||j==0||j==L-1) screen[i][j]='*';
      cout<< endl;
    }
  
  for (int i=0;i<H;i++)
    {
      for (int j=0; j<L; j++)
	cout << screen[i][j];
      cout << endl;
    }
}

/******************************************************************************/

void clear_screen()
{
  for (int i=1; i<H-1; i++)
    for (int j=1; j<L-1; j++)
      screen[i][j] = ' ';
}			  

/*****************************************************************************/

int x=1; int y=1;
int oppo=1;
int trans=1;

void update_game(int key)
{
  //le ball va de gauche a droite//
  if ( oppo==1 && trans==1 && (x>=1 || y>=1)) 
   {
     screen[x][y]='o';
     x++;
     y++;
   }
  if (x==H-1) 
    {
      trans=0;
      x--;
      y--;
    } 
  if (oppo==1 && trans==0)
    {
      if (x>1 && y<L-1)
	{
	  screen[x][y]='o';
	  x--;
	  y++;
	}
      else trans=1;
    }   
  if (y==L-1 && trans==1)
      {
	oppo=0;
	x--;
	y--;
      } 
   
  if (y==L-1 && trans==0)
    {
      oppo=0;
      x++;
      y--;
    }



  /***************************************************/
  
  //le ball va de droite a gauche//
  if ( oppo==0 && trans==1 && (x>1 || y<L-1))
    {
      screen[x][y]='o';
      x++;
      y--;
    }
  if (x==H-1)
    {
      trans=0;
      x--;
      y++;    
    }
  if ( oppo==0 && trans==0)
    {
      if (x>1 && y>1)
	{
	  screen[x][y]='o';
	  x--;
	  y--;
	}
      else trans=1;
    }     
  if (y==1)
    {
      oppo = 1;
      x++;
      y++;
    }
}
	
/*****************************************************************************/

int main()
{
  int key;
  do
    {
      key=read_keybord();			
      clear_screen();
      update_game(key);
      display();
      usleep(50*1000);
    }
  while (key!='q');
}

/*****************************************************************************/

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

int read_keybord() 
{
  int key;
  if (kbhit())
    key = getc(stdin);
  else
    key = 0;
  return key;
}

/******************************************************************************/
