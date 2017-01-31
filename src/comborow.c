////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	comborow.c
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
#include <stdlib.h>
#include <string.h>
#include "comborow.h"

void	cr_refresh(LPCOMBOROW cr) {
	int	i, j;

	cr->m_owner = GB_NOONE;
	cr->m_nfilled = 0;
	cr->m_interesting = true;

	// Resort the spots
	for(i=0; i<NUM_ON_SIDE-1; i++) {
		for(j=i+1; j<NUM_ON_SIDE; j++) {
			if (cr->m_spots[i] > cr->m_spots[j]) {
				int tmp = cr->m_spots[j];
				cr->m_spots[j] = cr->m_spots[i];
				cr->m_spots[i] = tmp;
			}
		}
	}
}

void	cr_init(LPCOMBOROW cr, int one, int two, int three, int four) {
	cr->m_spots[0] = one;
	cr->m_spots[1] = two;
	cr->m_spots[2] = three;
	cr->m_spots[3] = four;
	cr_refresh(cr);
}

/*
 * cr_register
 *
 * Register into our little piece of knowledge that "who" moved "where".
 */
bool	cr_register(LPCOMBOROW cr, GB_PIECE who, int where) {
	if (!cr->m_interesting)
		return false;

	int	nc = 4-cr->m_nfilled, i;

	// Look through al of our spots to ask whether or not where is relevant
	// to us.
	for(i=0; i<nc; i++) {
		// If where is one of our spots
		if (cr->m_spots[i] == where) {
			// Then increment our counter keeping track of how many
			// spots are filled
			cr->m_nfilled ++;

			// Move this position to the back of the array, since
			// it has now been filled.  Other things working with
			// this position will then no longer search through
			// positions that have been filled.
			if (i != 4-cr->m_nfilled) {
				// We don't need to do a proper 3-part copy,
				// since cr->m_spots[i] is already equal to
				// where
				cr->m_spots[i] = cr->m_spots[4-cr->m_nfilled];
				cr->m_spots[4-cr->m_nfilled] = where;
			}

			// If we are already owned by this player,
			if (cr->m_owner == who) {
				// Check to see if he has won, and return true
				// if so.  "true" here just means that whoever
				// is calling us has a condition they need to
				// check, winning being one such condition.
				if (cr->m_nfilled == NUM_ON_SIDE)
					return true;
				return false;
			} if (cr->m_owner != GB_NOONE) {
				// If this row is already owned by someone, not
				// us, then a potential win has been blocked,
				// and it is no longer possible to win in this
				// fashion.  This piece of information is now
				// irrelevant, so mark it as uninteresting.
				cr->m_interesting = false;
				cr->m_owner = GB_NOONE;
				return true;
			} else {
				// Otherwise, if the owner was no-one, then we
				// can claim ownership ourselves.
				cr->m_owner = who;
				return false;
			}

			// In all cases, once we've matched, there will be no
			// more matches, so return (as above) to our caller.
		}
	}

	// If the piece wasn't a part of our row, then we have discovered
	// nothing interesting.  Return false and go on.
	return false;
}

/*
 * cr_isable
 *
 * A combo row routine, testing whether or not where is a member of the combo
 * row, that is, is a player ABLE to move here.
 */
bool	cr_isable(LPCOMBOROW cr, int where) {
	int	i;

	for(i=0; i<cr->m_nfilled; i++)
		if (cr->m_spots[i] == where)
			return true;
	return false;
}

/*
 * cr_isects
 *
 * Do two combo rows intersect each other?  To interesect, they must both be
 * interesting (i.e. still in play), and they must both contain a location in
 * their 4-in-a-row set of spots that it common between both.  Unless we've
 * made a mistake, there will only ever be one such intersection.
 */
bool	cr_isects(LPCOMBOROW cr, LPCOMBOROW br) {
	int	i, j;

	if ((!cr->m_interesting)||(!br->m_interesting))
		return false;
	for(i=0; i<4-cr->m_nfilled; i++) {
		for(j=0; j<4-br->m_nfilled; j++)
			if (cr->m_spots[i]==br->m_spots[j])
				return true;
	} return false;
}

/*
 * cr_isownedby
 *
 * Just simply check this combo row and return if we are the sole owner of the
 * pieces within the row.
 */
bool	cr_isownedby(LPCOMBOROW cr, GB_PIECE who) {
	if ((cr->m_interesting)&&(who == cr->m_owner))
		return true;
	return false;
}

