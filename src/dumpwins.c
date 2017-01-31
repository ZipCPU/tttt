////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	dumpwins.c
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	This is a piece of test infrastructure, designed to prove that
//		the 4-in-a-row combinations within the combo set are in fact
//	valid combinations.  To do that, we print them all out for visual
//	inspection.
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
#include "gboard.h"
#include "comboset.h"

int main(int argc, char **argv) {
	int	i, j;
	GBOARD	brd;
	COMBOSET	cs;
	LPCOMBOROW	cr;

	cs_init(&cs);

	for(i=0; i< NUM_COMBOROWS; i++) {
		// Create/initialize an empty game board
		gb_reset(&brd);

		// Place pieces on that board
		//
		// Specifically, we place a piece into all the squares
		// associated with a particular four-in-a-row possibility.
		cr = &cs.m_data[i];
		for(j=0; j < NUM_ON_SIDE; j++)
			brd.m_spots[cr->m_spots[j]] = GB_BLACK;

		// And print it out.
		gb_print(&brd);
	}

	return 0;
}
