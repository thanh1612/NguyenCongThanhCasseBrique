#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ctime>
using namespace std;

int read_keybord();

/******************************************************************************/

#define H 27
#define L 29

char screen[H][L];

void display()
{
  system("clear"); 
  cout << "Bienvenue au jeux casse brique!" << endl;
  cout << "Prendre <space> pour commencer le jeux." << endl;
  cout << "Prendre 'a' pour deplacer le barre a la gauche." << endl;
  cout << "Prendre 'd' pour deplacer le barre a la droite." << endl;
  cout << endl;
  
  for (int i=0; i<H; i++)
    for (int j=0; j<L; j++)
      if (i==0||i==H-1||j==0||j==L-1) screen[i][j]='*';
  
  for (int i=0;i<H;i++)
    {
      for (int j=0; j<L; j++)
	cout << screen[i][j];
      cout << endl;
    }
  
  }

/******************************************************************************/

int start=0;
void clear_screen()
{
  for (int i=1; i<H-1; i++)
    for (int j=1; j<L-1; j++)
      if (screen[i][j]=='~'||(screen[i][j]=='o' && start==0)||screen[i][j]=='=')
	continue;
      else
	screen[i][j] = ' ';
}

/*****************************************************************************/

int x=H-3; int y=L/2;
int haut,gauche; 

void update_game(int key)
{
  if (start==1)
    {
      if (gauche==1) //le ball va de gauche a droite//
	{
	  if (haut==1) //le ball va de haut a bas//
	    {
	      screen[x][y]='o';
	      x++; y++;
	      if (screen[x][y]=='=')
		haut=0;
	    }
	  
	  if (screen[x][y]== '~' || screen[x][y-1]== '~') //defendre//
	    {
	      haut=0;
	      x--; y--;
	    }
	  
	  	  
	  if (x==H+1)
	    {
	      cout << "YOU LOSE" <<endl;
	      exit(0);
	    }
	  
	  if (haut==0) //le ball va de bas a haut// 
	    { 
	      screen[x][y]='o';
	      x--; y++;
	      if (screen[x][y]=='=')
		haut=1;
	      if (screen[x][y]=='=' && screen[x][y+1]==' ')
		gauche=0;
	    }
	  
	  if (x==0)
	    { haut=1; x=x+2; }
	  
	  if (y==L-1 && haut==1)
	    {	
	      gauche=0;
	      x--; y--;
	    } 
	  if (y==L-1 && haut==0)
	    {
	      gauche=0;
	      x++; y--;
	    }
	}
      
      if (gauche==0)  //le ball va de droite a gauche//
	{
	  if (haut==1)  //le ball va de haut a bas//
	    {
	      screen[x][y]='o';
	      x++; y--;
	      if (screen[x][y]=='=')
		{haut=0;}
	    }
	  
	  if (screen[x][y]== '~' || screen[x][y+1]== '~') //defendre//
	    {
	      haut=0;
	      x--; y++;
	    }
	  
	  if (x==H+1) 
	    { 
	      cout << "YOU LOSE" <<endl;
	      exit(0);
	    }
	  
	  if (haut==0) //le ball va de bas a haut//
	    {
	      screen[x][y]='o';
	      x--; y--;
	      if (screen[x][y]=='=')
		haut=1;
	      if (screen[x][y]=='=' && screen[x][y-1]==' ')
		gauche=1;
	    
	    }
	  
	  
	  if (x==0)
	    { haut=1; x=x+2; }
	  
	  if (y==0 && haut==1) 
	    {
	      gauche=1;
	      y=y+2;
	    }
	  if (y==0 && haut==0)
	    {
	      gauche=1;
	      y=y+2;
	    }
	}
    }
}	
/*****************************************************************************/

int m=0; int n=8; bool begin=true;
void preparer() //faire la barre//
{
  if (begin)
    {
      if (m<n)
	{
	  screen[H-2][L/2 - m]= '~';
	  screen[H-2][L/2 + m]= '~';
	  m++;
	}
     
      if (m==n)  
	screen[H-3][L/2]='o';
    }
}

/*****************************************************************************/

int main()
{
  int key; int a=L/2;
  for (int i=1;i<11;i++)         //faire la brique//
    for (int j=5; j<L-5; j++)
      if (i%2!=0)
	screen[i][j]='=';
  do
    {
      srand(time(NULL));
      int i=0+rand()%2;
      if (start==0)
	if (i==1) gauche=1;
	else gauche=0;
 
      key=read_keybord();			
      preparer();       
      clear_screen();
      update_game(key);
      display();     
      usleep(100*1000);
      if (key==' ') start=1; //prendre <space> pour commencer le jeux//
      if (key=='a' && a-m!=0 && start==1)//prendre 'a' pour aller a la gauche// 
	{
	  m=0;
	  begin=false;
	  a--;
	  screen[H-3][a]='o';
	  screen[H-3][a+1]=' ';
	  while (m<n)
	    {
	      screen[H-2][a - m]= '~';
	      screen[H-2][a + m]= '~';
	      screen[H-2][a + n]= ' ';
	      m++;
	    }
	}
      
      if (key=='d' && a+m!=L-1 && start==1)//prendre 'd' pour aller a la droite// 
	{
	  m=0;
	  begin=false;
	  a++;
	  screen[H-3][a]='o';
	  screen[H-3][a-1]=' ';
	  while (m<n)
	    {
	      screen[H-2][a - m]= '~';
	      screen[H-2][a + m]= '~';
	      screen[H-2][a - n]= ' ';
	      m++;
	    }
	}
      
      for (int i=1; i<5; i++)
	for (int j=1; j<L-1; j++)
	  if (screen[i][j]=='o')
	    screen[i][j]=' ';
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
