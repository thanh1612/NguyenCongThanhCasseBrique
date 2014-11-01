#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ctime>
using namespace std;

int read_keybord();

/************************ Display ****************************/

#define H 17
#define L 33

char screen[H][L]; int level=1;

void display()
{ system("clear"); 
  cout << "          Bienvenue au jeux casse brique!"<< endl;
  cout << " Appuyez sur le bouton <space> pour commencer ou continuer le jeux." << endl;
  cout << " Appuyez sur le bouton 'a' pour deplacer le barre a la gauche." << endl;
  cout << " Appuyez sur le bouton 'd' pour deplacer le barre a la droite." << endl;
  cout << " Appuyer sur le bouton 's' pour passer le niveau ensuite." << endl; 
  cout << " Appuyer sur le bouton 'q' pour quitter le jeux." << endl; 
  cout << endl;
  if (level>1) cout << " Bravo!!! Tu passes level " << level-1 << '.' << endl << endl;;
  cout << "             LEVEL " << level << endl;
  for (int i=0; i<H; i++) //faire le cadre//
    for (int j=0; j<L; j++)
      if (i==0||i==H-1||j==0||j==L-1) screen[i][j]='*';
  
  for (int i=0;i<H;i++) //display//
    { for (int j=0; j<L; j++)
 	cout << screen[i][j];
      cout << endl;
    }
  
}

/**************** Clear Screen ****************************************/

int start=0;
void clear_screen()
{ for (int i=1; i<H-1; i++)
    for (int j=1; j<L-1; j++)
      if (screen[i][j]=='~'||(screen[i][j]=='o' && start==0) || screen[i][j]=='=')
	continue;
      else
	screen[i][j] = ' ';
}

/*****************************************************************************/

int x=H-3; int y=L/2;
int haut,gauche; 

void update_game(int key)
{ if (start==1) //commencer le jeux
    {
      if (gauche==1)  //le ball va de gauche a droite
	{ if (haut==1)  //le ball va de haut a bas
	    { screen[x][y]='o';
	      x++; y++;
	      if (screen[x][y]=='=') //casse brique
		{ haut=0;
		  screen[x-1][y-1]=' ';
		}
	    }
	  
	  if (haut==0) //le ball va de bas a haut
	    { screen[x][y]='o';
	      x--; y++;
	      if (screen[x][y]=='=') //casse brique
		haut=1;
	    }
	  
	  if (x==0)
	    { haut=1; x+=2;
	    }
	  
	  if (y==L-1 && haut==1)
	    { gauche=0;
	      x--; y--;
	    } 
	  if (y==L-1 && haut==0)
	    { gauche=0;
	      x++; y--;
	    }
	  
	  if (screen[x][y]== '~' || screen[x][y-1]== '~') //defendre//
	    { haut=0;
	      x-=2;
	    }
	  	  
	  if (x==H) //fin du jeux//
	    { cout << "YOU LOSE" <<endl;
	      exit(0);
	    }
	}
      
      if (gauche==0)  //le ball va de droite a gauche//
	{ if (haut==1)  //le ball va de haut a bas//
	    { screen[x][y]='o';
	      x++; y--;
	      if (screen[x][y]=='=') //casse brique//
		{ screen[x-1][y+1]=' ';
		  haut=0;
		}
	    }
	  
	  if (haut==0) //le ball va de bas a haut//
	    { screen[x][y]='o';
	      x--; y--;
	      if (screen[x][y]=='=') //casse brique//
		haut=1;
	    }
	  
	  if (x==0)
	    { haut=1; x+=2;
	    }
	  
	  if (y==0)
	    { gauche=1;
	      y+=2;
	    }
	
	  if (screen[x][y]== '~' || screen[x][y+1]== '~') //defendre//
	    { haut=0;
	      x-=2;
	    }
	
	  if (x==H)  //fin du jeux//
	  { cout << "YOU LOSE" <<endl;
	    exit(0);
	  }
	}
    }
}	
/****************************Faire La Barre***********************************/

int k=0; int MoitieLongueurDeBarre=7;
void FaireLaBarre() 
{ if (k < MoitieLongueurDeBarre)
    { screen[H-2][L/2 - k]= '~';
      screen[H-2][L/2 + k]= '~';
      k++;
    }
  
  if (k == MoitieLongueurDeBarre)  
    screen[H-3][L/2]='o'; 
}

/*****************************************************************************/

int main()
{ int key; int a=L/2; int m=0; int VitesseDuBall=100; int pause=0;
    
  do
    { srand(time(NULL));
      int i=0+rand()%2;
      if (start==0)
	if (i==1) gauche=1;
	else gauche=0;

      /*************************Faire La Brique******************/
      if (start==0)
	for (int i=1;i<6;i++)         
	  for (int j=5; j<L-5; j++)
	    { if (i%2!=0)
	       screen[i][j]='=';
	    }
      
      if (MoitieLongueurDeBarre==2)
	{ cout << " Félicitations!!! Vous gagnez le jeux!!!" << endl;
	  exit(0);
	}
      /**********************************************************/    
      
      int g=0; //compter si le ball detruit les briques.
      for (int i=0; i<H-2; i++)
	for (int j=0; j<L; j++)
	  { if (screen[i][j]=='o')
	      {	screen[i][j]=' ';
		screen[i][j-1]=' ';
		screen[i][j+1]=' ';
	      }
	    if (screen[i][j]=='=') g++;
	  }

      /************* Le Niveau Ensuite ******************/
      if (g==0) //Quand le ball detruit toutes les briques, le jeux passe le niveau ensuite.
	{ level+=1;
	  start=0;
	  x=H-3; y=L/2;
	  k=0;
	  MoitieLongueurDeBarre-=1;
	  VitesseDuBall-=10;
	  for (int j=0;j<L;j++)
	    screen[H-2][j]=' ';
	}
      /*************************************************/
      
      key=read_keybord();			
      FaireLaBarre();
      clear_screen();
      update_game(key);
      display();     
      usleep(VitesseDuBall*1000);

      if (key=='s') //cheat game
	for (int i=1;i<6;i++)         
	  for (int j=5; j<L-5; j++)
	    { if (i%2!=0)
		screen[i][j]=' ';
	      key='g';
	    }
    
      /************************** Deplacer la barre **************************/
      if (key==' ') start=1; //Appuyer sur le bouton <space> pour commencer le jeux
      
      if (key=='a' && start==1) //Appuyer sur le bouton 'a' pour aller a la gauche 
	{ int DistanceDeplacer=3;
	  if (a-MoitieLongueurDeBarre < DistanceDeplacer)
	    DistanceDeplacer = a-MoitieLongueurDeBarre;
	  a-= DistanceDeplacer;
	  m=0;
	  while (m < MoitieLongueurDeBarre)
	    { screen[H-2][a-m]= '~';
	      screen[H-2][a+m]= '~';
	      m++;
	    }
	     
	  int v=0;
	  while (v < L)
	    { screen[H-2][a + MoitieLongueurDeBarre + v]= ' ';
	      v++;
	    }
	}
      
      if (key=='d' && start==1) //Appuyer sur le bouton 'd' pour aller a la droite 
	{ int DistanceDeplacer=3;
	  if ((L-1)-a-MoitieLongueurDeBarre < DistanceDeplacer)
	    DistanceDeplacer = (L-1)-a-MoitieLongueurDeBarre;
	  a+= DistanceDeplacer;
	  m=0;
	  while (m < MoitieLongueurDeBarre)
	    { screen[H-2][a-m]= '~';
	      screen[H-2][a+m]= '~';
	      m++;
	    }
	  int v=0;
	  while (v < L)
	    { screen[H-2][a - MoitieLongueurDeBarre - v]= ' ';
	      v++;
	    }
	}
      /*************************************************/
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
