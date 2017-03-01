////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	comboset.h
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	A COMBOROW is a set of 4-points that when all filled by one
//		player will result in a win.  A COMBOSET is the set of all such
//	COMBOROWs associated with the game.  
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
#ifndef	COMBOSET_H
#define	COMBOSET_H

#include "bool.h"
#include "gboard.h"
#include "comborow.h"

// The number of possible ways to win in 4x4x4 tic-tac-toe
#define	NUM_COMBOROWS	(16*3+4*7)

typedef	struct	COMBOSET_S {
		// A combination row is in play as long as no more than one
		// player has moved within it.
	int	m_ninplay,
		// If one of our combinations has resulted in a win, let's
		// record the index of that combination.
		m_winningid;

	// These are the set of the possible winning combinations
	COMBOROW	m_data[NUM_COMBOROWS];
} COMBOSET, *LPCOMBOSET;

/*
 * cs_init
 *
 * Fill a structure with appropriate initial values.
 */
extern void	cs_init(LPCOMBOSET cs);

/*
 * cs_place
 *
 * "who" has placed his piece "where".  Adjust our data structure as
 * appropriate to reflect that fact.
 */
extern bool	cs_place(LPCOMBOSET cs, GB_PIECE who, int where);

/*
 * cs_debug
 *
 * Dump a comboset to the screen for debugging evaluation
 */
extern	void	cs_debug(LPCOMBOSET cs);

#endif
