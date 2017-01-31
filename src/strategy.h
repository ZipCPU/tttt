////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	strategy.h
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	To define the interface to the computers strategy.  The 
//		computers strategy itself is a list of rules, and so the
//	strategy interface sets up those rules, and then handles requests for
//	moves.
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
#ifndef	STRATEGY_H
#define	STRATEGY_H

#include "gboard.h"
#include "comboset.h"
#include "vset.h"

#define	MAX_RULES	32

// Here's the definition of a "rule".  It's a function that sets the values
// of a given VSET, in this case, the VSET named spots.
typedef	void	(*RULEFN)(LPGBOARD brd, LPCOMBOSET, GB_PIECE, LPVSET spots);

// We keep track of rules by more than just the function pointer.  We allow
// every rule to have a name and a difficulty level.  The rule will apply
// to any difficulty level at or above the level of the rule.
typedef	struct RULE_S {
	const char	*m_name;
	int	m_level;
	RULEFN	m_fn;
} RULE, *LPRULE;

// Finally, we define our strategy.  The strategy is nothing more than a list
// of rules for a given difficulty level.
typedef	struct STRATEGY_S {
	// The difficulty level determines which rules were applied
	int	m_difficulty_level,
		// Keep track of how many rules are valid ones
		m_num_rules;
	// And here's where we point to all of our rules
	const RULE *m_rules[MAX_RULES];
} STRATEGY, *LPSTRATEGY;

/*
 * set_difficulty
 *
 * Pick from among the rule set those rules appropriate for the given difficulty
 * level selected.
 */
extern	void	set_difficulty(LPSTRATEGY s, int difficulty);

/*
 * makemove
 *
 * Use our ruleset, as applied to the board and the set of 4-in a row
 * combinations to choose a move.
 */
extern	int	makemove(LPSTRATEGY, LPGBOARD, LPCOMBOSET, GB_PIECE);

#endif
