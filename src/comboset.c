////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	comboset.c
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	
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
#include <string.h>
#include <assert.h>

#include "comboset.h"

void	cs_init(LPCOMBOSET cs) {
	cs->m_ninplay   = NUM_COMBOROWS;
	cs->m_winningid = -1;

	int	idx, i;

	idx = 0;

	// Our three types of sixteen
	for(i=0; i<16; i++)
		cr_init(&cs->m_data[idx++], i*4, i*4+1, i*4+2, i*4+3);
	for(i=0; i<16; i++)
		cr_init(&cs->m_data[idx++],
			(i&3)+(i&12)*4,
			(i&3)+(i&12)*4+4,
			(i&3)+(i&12)*4+8,
			(i&3)+(i&12)*4+12);
	for(i=0; i<16; i++)
		cr_init(&cs->m_data[idx++], i, i+16, i+32, i+48);

	// Our seven types of four
	for(i=0; i<4; i++)
		cr_init(&cs->m_data[idx++], i*16, i*16+5, i*16+10, i*16+15);
	for(i=0; i<4; i++)
		cr_init(&cs->m_data[idx++], i*16+3, i*16+6, i*16+9, i*16+12);
	for(i=0; i<4; i++)
		cr_init(&cs->m_data[idx++], i, i+20, i+40, i+60);
	for(i=0; i<4; i++)
		cr_init(&cs->m_data[idx++], i+12, i+24, i+36, i+48);
	for(i=0; i<4; i++)
		cr_init(&cs->m_data[idx++], i*4, i*4+16+1, i*4+32+2, i*4+48+3);
	for(i=0; i<4; i++)
		cr_init(&cs->m_data[idx++], i*4+3, i*4+16+2, i*4+32+1, i*4+48);

	// The four diagonal, diagonals.
	cr_init(&cs->m_data[idx++],  0,    16+4+1,    32+8+2,     48+12+3);
	cr_init(&cs->m_data[idx++],  3,  3+16+4-1,  3+32+8-2,  3+ 48+12-3);
	cr_init(&cs->m_data[idx++], 12, 12+16-4+1, 12+32-8+2, 12+ 48-12+3);
	cr_init(&cs->m_data[idx++], 15, 15+16-4-1, 15+32-8-2, 15+ 48-12-3);

	assert(idx == NUM_COMBOROWS);
}

void	cs_swap(LPCOMBOROW a, LPCOMBOROW b) {
	COMBOROW	tmp;

	memcpy(&tmp, a, sizeof(COMBOROW));
	memcpy(a, b, sizeof(COMBOROW));
	memcpy(b, &tmp, sizeof(COMBOROW));
}

bool	cs_place(LPCOMBOSET cs, GB_PIECE who, int where) {
	int	i;

	if (cs->m_winningid >= 0)
		return true;

	for(i=0; i < cs->m_ninplay; i++) {
		if (cr_register(&cs->m_data[i], who, where)) {
			if ((cs->m_data[i].m_nfilled == NUM_ON_SIDE)
					&&(cs->m_data[i].m_owner == who)) {
				cs->m_winningid = i;
			} else if (!cs->m_data[i].m_interesting) {
				if (i != cs->m_ninplay-1) {
					cs_swap(&cs->m_data[i],
						&cs->m_data[cs->m_ninplay-1]);
					i--;
				} cs->m_ninplay--;
			}
		}
	} return (cs->m_winningid >= 0) ? true : false;
}

