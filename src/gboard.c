////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	gboard.c
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	Implements the interface between the board and the rest of the
//		game.
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
#include <stdlib.h>
#include "gboard.h"

LPGBOARD	gb_new(void) {
	LPGBOARD	brd;

	brd = (LPGBOARD)malloc(sizeof(GBOARD));
	gb_reset(brd);
	return brd;
}

void	gb_reset(LPGBOARD brd) {
	brd->m_lastturn = GB_WHITE;
	brd->m_winner   = GB_NOONE;
	brd->m_nfilled  = 0;
	for(int i=0; i<NUM_SQUARES; i++)
		brd->m_spots[i] = GB_NOONE;
	brd->m_nfilled  = 0;
}

int	coordtoint(int x, int y, int z) {
	if ((x >= NUM_ON_SIDE)||(x < 0))
		return -1;
	if ((y >= NUM_ON_SIDE)||(y < 0))
		return -1;
	if ((z >= NUM_ON_SIDE)||(z < 0))
		return -1;

	return (z * NUM_ON_SIDE + y) * NUM_ON_SIDE + x;
}

int	opponent(int who) {
	if (who == GB_WHITE)
		return GB_BLACK;
	else if (who == GB_BLACK)
		return GB_WHITE;
	else
		return GB_NOONE;
}

int	xcoord(int spt) {
	if ((spt >= NUM_SQUARES)||(spt < 0))
		return -1;
	return spt % NUM_ON_SIDE;
}

int	ycoord(int spt) {
	if ((spt >= NUM_SQUARES)||(spt < 0))
		return -1;
	return (spt / NUM_ON_SIDE)%NUM_ON_SIDE;
}

int	zcoord(int spt) {
	if ((spt >= NUM_SQUARES)||(spt < 0))
		return -1;
	return (spt / (NUM_ON_SIDE*NUM_ON_SIDE));
}

bool	legal(LPGBOARD brd, GB_PIECE who, int where) {
	if (brd->m_winner != GB_NOONE)
		return false;
	if ((who != GB_BLACK)&&(who != GB_WHITE))
		return false;
	if ((who == GB_WHITE)&&(brd->m_lastturn != GB_BLACK))
		return false;
	if ((who == GB_BLACK)&&(brd->m_lastturn != GB_WHITE))
		return false;
	if ((where < 0)||(where >= NUM_SQUARES))
		return false;
	if (brd->m_spots[where] != GB_NOONE)
		return false;
	return true;
}
	
void	gb_place(LPGBOARD brd, GB_PIECE who, int where) {
	if (!legal(brd, who, where)) {
		printf("ILLEGAL-MOVE ATTEMPTED: %s tried to move to %d",
			(who == GB_BLACK)?"black"
			:(who == GB_WHITE)?"white"
			: "? someone ?", where);
	}

	brd->m_spots[where] = who;
	brd->m_lastturn = who;
	brd->m_nfilled++;
}

bool	inuse(LPGBOARD brd, int where) {
	if ((where < 0)||(where >= NUM_SQUARES))
		return true;
	return (brd->m_spots[where] != GB_NOONE)?true:false;
}

GB_PIECE whoseturn(LPGBOARD brd) {
	if (brd->m_winner != GB_NOONE)
		return GB_NOONE;
	else if (brd->m_lastturn == GB_WHITE)
		return GB_BLACK;
	else if (brd->m_lastturn == GB_BLACK)
		return GB_WHITE;
	else
		printf("Cannot tell whose turn it is from %d (%s)\n", 
			brd->m_lastturn,
			(brd->m_lastturn == GB_BLACK)?"black"
			:(brd->m_lastturn == GB_WHITE)?"white"
			: "? someone ?");
	return GB_NOONE;
}

bool	gb_gameover(LPGBOARD brd) {
	return	(whoseturn(brd) == GB_NOONE)?true:false;
}

GB_PIECE gb_winner(LPGBOARD brd) {
	return brd->m_winner;
}

GB_PIECE pieceat(LPGBOARD brd, int where) {
	if ((where < 0)||(where >= NUM_SQUARES))
		return	GB_NOONE;
	return	brd->m_spots[where];
}

void gb_print(LPGBOARD brd) {
	int	x, y, z, loc;

	if (brd->m_nfilled == 0)
		printf("Current Board: (Empty)\n");
	else if (brd->m_winner == GB_NOONE)
		printf("Current Board:\n");
	else if (brd->m_winner == GB_BLACK)
		printf("Current Board: (X wins)\n");
	else if (brd->m_winner == GB_WHITE)
		printf("Current Board: (O wins)\n");
		
	for(y=0; y<NUM_ON_SIDE; y++) {
		for(z=0; z<NUM_ON_SIDE; z++) {
			for(x=0; x<NUM_ON_SIDE; x++) {
				loc = coordtoint(x, y, z);
				GB_PIECE who = brd->m_spots[loc];
				if (who == GB_BLACK)
					printf("x");
				else if (who == GB_WHITE)
					printf("o");
				else
					printf("-");
			}

			printf("  ");
		}
		printf("\n");
	} printf("\n");
}

