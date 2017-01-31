////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	comborow.h
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	A COMBOROW is a data type designed to hold the four values
//		necessary to describe a four-in-a-row or equivalently a set
//	of points necessary to win.  This file describes an interface to be
//	used with this data type.
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
#ifndef	COMBOROW_H
#define	COMBOROW_H

#include "gboard.h"

typedef	struct COMBOROW_S {
	// The "owner" of a COMBOROW is the one individual who has chips
	// in part of the row.  If Black has some in the row, it will be owned
	// by black.  The exception is if both black and white have pieces in
	// the row.  Then it is owned by NOONE and no longer interesting.
	GB_PIECE	m_owner;
	// The number of filled spots within the row
	int		m_nfilled;
	// A row is interesting if a win may still be made within it.  Otherwise
	// it is uninteresting.
	bool		m_interesting;
	// The m_spots array contains a list of the locations of the four parts
	// of the row.  As items are filled, unfilled items are brought to the
	// front of the array, so the first 4-m_nfilled entries are always
	// unfilled.
	int		m_spots[NUM_ON_SIDE];	// The 4 values in the row
} COMBOROW, *LPCOMBOROW;

// Operations on comborows:
//
// Initialize a row
void	cr_init(LPCOMBOROW cr, int one, int two, int three, int four);
// Refreshing a row--sorting its m_spots, clearing the owner and the number
// filled, and setting the interesting flag back to true.
void	cr_refresh(LPCOMBOROW cr);
// Register that who has moved where.
bool	cr_register(LPCOMBOROW cr, GB_PIECE who, int where);
// Answer the question of, can someone move to the where location?  That is,
// is "where" still able to accept pieces?
bool	cr_isable(LPCOMBOROW cr, int where);
// Do two comborows intersect?
bool	cr_isects(LPCOMBOROW cr, LPCOMBOROW br);
// Does "who" "own" this row?  That is, does he have pieces in the row, and
// is it such that his opponent does not?
bool	cr_isownedby(LPCOMBOROW cr, GB_PIECE who);


#endif
