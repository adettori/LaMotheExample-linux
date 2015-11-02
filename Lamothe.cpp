
//  A game fragment adapted from Andre La Mothe's book
//  Ported to GNU/Linux by Antonello Dettori

//	The Black Art of 3D Games Programming

//  Modified by CJM 24/9/'08 to run without error or warnings:

//	Note: this is very old-fashioned code originally written for 16-bit PCs



// INCLUDES ///////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>


// DEFINES ////////////////////////////////////////////////

#define MAX_X        80  // maximum x position for player
#define SCROLL_POS   24  // the point that scrolling occurs

// PROTOTYPES /////////////////////////////////////////////

void Init_Graphics();
void Set_Color(int fcolor, int bcolor);
void Draw_String(int x, int y, char *string);

// GLOBALS ////////////////////////////////////////////////

int    game_running = 1; // state of game, 0=done, 1=run

// FUNCTIONS //////////////////////////////////////////////

void Init_Graphics()
{
	// this function initializes the console graphics engine

	setlocale(LC_ALL,"uk");

	initscr();

	cbreak();			/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

	scrollok(stdscr, TRUE);

	curs_set(0);			/*Set cursor invisible*/

	start_color();

	init_pair(1, COLOR_BLACK, COLOR_WHITE);

	// seed the random number generator with time
	srand((unsigned)time(NULL));

} // end Init_Graphics

///////////////////////////////////////////////////////////

//void Set_Color(int fcolor, int bcolor = 0)
//{
//	init_pair(1, COLOR_RED, COLOR_BLACK);
//	refresh();
//} // Set_Color

///////////////////////////////////////////////////////////

void Draw_String(int x, int y, char *string)
{
	// this function draws a string at the given x,y

	int cursor_pos_x = x;
	int cursor_pos_y = y;

	// print the string in current color and position
	mvaddstr(cursor_pos_y, cursor_pos_x, string);

	refresh();

} // end Draw_String

///////////////////////////////////////////////////////////

void Clear_Screen(void)
{
	// this function clears the screen

	// set color to white on black
//	Set_Color(15, 0);

//	color_set(COLOR_PAIR(1));

	// clear the screen
	for (int x = 0; x <= MAX_X; x++){
		for (int y = 0; y <= SCROLL_POS; y++){
			Draw_String(x, y, "\n");
		}
	}

	refresh();

} // end Clear_Screen


int kbhit()
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


// MAIN GAME LOOP /////////////////////////////////////////
using namespace std;
int main()
{
	char key;            // player input data
	int  player_x = 40;  // player's x 

	// SECTION: initialization

	// set up the console text graphics system
	Init_Graphics();
	
	// clear the screen
	Clear_Screen();

	// SECTION: main event loop, this is where all the action  
	// takes place, the general loop is erase-move-draw

	while (game_running)
	{
		// SECTION: erase all the objects or clear screen

		// nothing to erase in our case   

		// SECTION: get player input
		
		if(kbhit()){
		
			key = getchar();

			// is player trying to exit, if so exit
			if (key == 27)
				game_running = 0;

			// is player moving left        
			if (key == 'a')
				player_x--;

			// is player moving right
			if (key == 'd')
				player_x++;

		}

		// SECTION: game logic and further processing

		// make sure player stays on screen 
		if (++player_x > MAX_X)
			player_x = MAX_X;

		if (--player_x < 0)
			player_x = 0;

		// SECTION: draw everything

		// draw next star at random position
//		Set_Color(15, 0);
		Draw_String(rand() % MAX_X, SCROLL_POS, ".\n");

		// Scroll
		scroll(stdscr);

		// draw player 
//		Set_Color(rand() % 15, 0);
		Draw_String(player_x, 0, "<--*-->");
		Draw_String(0, 0, "");

		// SECTION: synchronize to a constant frame rate
		usleep(1);

	} // end while

	// SECTION: shutdown and bail
	Clear_Screen();

	Draw_String(MAX_X/2,SCROLL_POS/2, "G A M E  O V E R");

	getch();

	endwin();

	return 0;

} // end main
