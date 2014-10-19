#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
using namespace std;

int read_keybord();

/******************************************************************************/

#define H 17
#define L 27

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
      if( screen[i][j]== '=') continue;
      else
	screen[i][j] = ' ';
}

/*****************************************************************************/

int x=1; int y=1; int point=0;
int oppo=1;
int trans=1;

void update_game(int key)
{
  if (oppo==1) //le ball va de gauche a droite//
    {
      if (trans==1) //le ball va de haut a bas//
	{
	  screen[x][y]='o';
	  x++; y++;
	}
      
      if ( screen[x][y]== '=' || screen[x][y-1]== '=')
	{
	  trans=0; x--; y--;
	 
	  }
	
      if (x==H+1)
	{
	  cout << "YOU LOSE" <<endl;
	  cout << "Your point is: " << point*100 << endl;
	  exit(0);
	}
      
      if (trans==0) //le ball va de bas a haut// 
	{ 
	  screen[x][y]='o';if (x==H-3) point++;
	  x--; y++;
	}
      if (x==0)
	{ trans=1; x=x+2; }
      
      if (y==L-1 && trans==1)
	{	
	  oppo=0;
	  x--; y--;
	} 
      if (y==L-1 && trans==0)
	{
	  oppo=0;
	  x++; y--;
	}
    }
  
  
  if (oppo==0)  //le ball va de droite a gauche//
    {
      if (trans==1)  //le ball va de haut a bas//
	{
	  screen[x][y]='o';
	  x++; y--;
	}

      if (screen[x][y]== '=' || screen[x][y+1]== '=') 
	{
	  trans=0; x--; y++;
	  
	}
      
       
      if (x==H+1) 
	{ 
	  cout << "YOU LOSE" <<endl;
	  cout << "Your point is: " << point*100 << endl;
	  exit(0);
	}
      
      if (trans==0) //le ball va de bas a haut//
	{
	  screen[x][y]='o';if (x==H-3) point++;
	  x--; y--;
	}
      if (x==0)
	{ trans=1; x=x+2; }
      
      if (y==0 && trans==1) 
	{
	  oppo=1;
	  y=y+2;
	}
      if (y==0 && trans==0)
	{
	  oppo=1;
	  y=y+2;
	}
    }
}	
/*****************************************************************************/
int m=0; int n=5; int a=L/2;
int key; bool begin=true;

void play()
{
  key=read_keybord();
  if (begin)
    {  
      if (m<n)
	{
	  screen[H-2][L/2 - m]= '=';
	  screen[H-2][L/2 + m]= '=';
	  m++;
	  cout << screen[H-2][L/2 - m];
	  cout << screen[H-2][L/2 + m];
	}
      if (m==n) m=0;
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
      play(); 
      usleep(100*1000);
  
      if (key== 'a' && a-m!=0)       //prendre 'a' pour aller a la gauche// 
	{
	  m=0;
	  begin=false;
	  a--;
	  while (m<n)
	    {
	      screen[H-2][a - m]= '=';
	      screen[H-2][a + m]= '=';
	      screen[H-2][a + n]= ' ';
	      
	      cout << screen[H-2][a - m];
	      cout << screen[H-2][a + m];
	      cout << screen[H-2][a + n];
	      m++;
	    }
	}
      
      if (key== 'd' && a+m!=L-1)      //prendre 'd' pour aller a la droite// 
	{
	  m=0;
	  begin=false;
	  a++;
	  while (m<n)
	    {
	      screen[H-2][a - m]= '=';
	      screen[H-2][a + m]= '=';
	      screen[H-2][a - n]= ' ';
	      
	      cout << screen[H-2][a - m];
	      cout << screen[H-2][a + m];
	      cout << screen[H-2][a - n];
	      m++;
	    }
	}
    }
  while (key!='q');
  return(0);
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
