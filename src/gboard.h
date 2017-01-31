////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	gboard.h
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	Defines the interface(s) that other parts of the game will have
//		with the gameboard itself.
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
#ifndef	GBOARD_H
#define	GBOARD_H

#include "bool.h"

#define	NUM_ON_SIDE	4
#define	NUM_SQUARES	(NUM_ON_SIDE * NUM_ON_SIDE * NUM_ON_SIDE)

typedef	enum	GB_PIECE_E {
	GB_NOONE=0, GB_WHITE, GB_BLACK, GB_TIE
} GB_PIECE;

typedef	struct GBOARD_S {
	int	m_lastturn, m_winner, m_nfilled;
	int	m_spots[NUM_SQUARES];
} GBOARD, *LPGBOARD;

LPGBOARD gb_new(void);
void	gb_reset(LPGBOARD brd);
int	coordtoint(int x, int y, int z);
int	opponent(int who);
int	xcoord(int spt);
int	ycoord(int spt);
int	zcoord(int spt);
bool	legal(LPGBOARD brd, GB_PIECE who, int where);
void	gb_place(LPGBOARD brd, GB_PIECE who, int where);
bool	inuse(LPGBOARD brd, int where);
GB_PIECE whoseturn(LPGBOARD brd);
bool	gb_gameover(LPGBOARD brd);
GB_PIECE gb_winner(LPGBOARD brd);
GB_PIECE pieceat(LPGBOARD brd, int where);
void	gb_print(LPGBOARD brd);


#endif
