////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	main.c
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	To play a game of 4x4x4 Tic-Tac-Toe.  This includes
//		allocating and initializing all game variables, reading and
//	processing user input, adjudicating whether or not someone has won,
//	and eventually automatically restarting the game upon a win (or loss)
//	when on an embedded (ZipCPU) device.
//
// Creator:	Dan Gisselquist, Ph.D.
//		Gisselquist Technology, LLC
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2017, Gisselquist Technology, LLC
//
// This program is free software (firmware): you can redistribute it and/or
// modify it under the terms of  the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  (It's in the $(ROOT)/doc directory, run make with no
// target there if the PDF file isn't present.)  If not, see
// <http://www.gnu.org/licenses/> for a copy.
//
// License:	GPL, v3, as defined and found on www.gnu.org,
//		http://www.gnu.org/licenses/gpl.html
//
//
////////////////////////////////////////////////////////////////////////////////
//
//
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "gboard.h"
#include "comboset.h"
#include "strategy.h"

void	print_instructions(void) {
	printf(
"Welcome to 4x4x4 Tic-Tac-Toe\n"
"\n"
"The goal of this game is to get 4 pieces in a row.  The board is three\n"
"dimensional, even though it will be displayed on a terminal screen.  Imagine\n"
"instead of seeing four 4x4 boards side by side, that these boards are\n"
"actually standing on top of each other.  A winning four in a row can exist\n"
"on any of the 4x4 levels.  A winning four in a row can also cross through\n"
"all levels.  Diagonals are valid, as are diagonal diagonals.\n"
"\n"
"To specify your move, type in a string of three numbers each in the range of\n"
"1-4.  The first two numbers describe where you wish to move within one 4x4\n"
"board, where the first number is the position counting left to right and the\n"
"second number is the position counting from top down.  The last number is\n"
"which 4x4 board you wish to move to, counting from the 4x4 on the left to\n"
"the right\n\n");
}

/*
 * getmove
 *
 * Invoke fgets to get a line of input data from the user.  We'll take the
 * first three digits the user gives us and try to map them into a position.
 * We'll return -1 on an invalid move, or -2 on any other reason (such as one
 * where we need to shut down (EOF)).
 */
int	getmove(void) {
	char	line[40], *ptr = line;
	int	x=0, y=0, z=0;

	// Request the user give us a move
	printf("Your move : "); fflush(stdout);

	if (NULL == fgets(line, sizeof(line), stdin)) {
		printf("EOF!\n");
		return -2;
	}

	while(isspace(*ptr))
		ptr++;

	if ('h' == tolower(*ptr)) {
		print_instructions();
		// Return an invalid move, so we'll be called again
		return -1;
	}

	while(!isdigit(*ptr))
		ptr++;
	if (!*ptr) return -1;	// If no digits are given, its an invalid move
	x = *ptr++ - '0' - 1;
	
	while(!isdigit(*ptr))
		ptr++;
	if (!*ptr)	// If only one digit is given, its also invalid
		return -1;
	y = *ptr++ - '0' - 1;
	
	while(!isdigit(*ptr))
		ptr++;
	if (!*ptr)	// If only two digits are given, its also invalid
		return -1;
	z = *ptr++ - '0' - 1;

	return coordtoint(x, y, z);
}

/*
 * play_game()
 *
 * Play the game once.  This includes allocating and initializing the board,
 * knowledge/reasoning base (COMBOSET), and the STRATEGY that will be used by
 * the computer.  It also includes requesting the user input, and causing the
 * game board to be printed after ever move of the computers.
 *
 */
void	play_game(void) {
	GBOARD		brd;
	COMBOSET	cs;
	STRATEGY	s;

	gb_reset(&brd);
	set_difficulty(&s, 1000);
	cs_init(&cs);

	while(!gb_gameover(&brd)) {
		int	mv;

		gb_print(&brd);
		do {
			mv = getmove();
			if (mv < -1)
				return;
		} while(!legal(&brd, GB_BLACK, mv));

		gb_place(&brd, GB_BLACK, mv);
		if (!cs_place(&cs,  GB_BLACK, mv)) {
			mv = makemove(&s, &brd, &cs, GB_WHITE);

			printf("The Computer moves (%d, %d, %d)\n\n",
				xcoord(mv)+1, ycoord(mv)+1,
				zcoord(mv)+1);

			gb_place(&brd, GB_WHITE, mv);
			if (cs_place(&cs,  GB_WHITE, mv))
				brd.m_winner = GB_WHITE;
		} else
			brd.m_winner = GB_BLACK;
	}

	// Print the final (winning) copy of the board
	gb_print(&brd);

	if (gb_winner(&brd) == GB_WHITE)
		printf("The computer wins\n");
	else if (gb_winner(&brd) == GB_BLACK)
		printf("CONGRATULATIONS, YOU WIN!!!!\n");
	else
		printf("The game is over ... somehow.\n");
}

/*
 * main
 *
 * The classic entry point for any C program.
 */
int	main(int argc, char **argv) {
	// Randomize the random number generator, so that we can truly pick
	// our computer moves from a random set of equally valid moves.
	srand(time(NULL));

	// Start by printing the instructions, before actually playing the game.
	print_instructions();

	// Now ... let's have fun and PLAY!
	//
	// On an embedded machine, this should be in a loop until the kingdom
	// comes (or power gets pulled, or the debugger takes control, etc.).
	// For now, we'll just play it once. 
	play_game();
}
