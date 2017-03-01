////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	strategy.c
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	Here's the "strategy" defining how the computer makes its
//		decisions.  The strategy is the result of a lot of work
//	trying to build a brute-force algorithm for 4x4x4 tic-tac-toe, only
//	to come to the slow realization that brute-forcing something with
//	nearly 64! possibilities just ... isn't going to happen in my lifetime.
//	Hence, the result is a rule-based strategy.  Because it is rule based,
//	it is neither perfect nor unbeatable.  However, it is both very
//	effective, as well as computationally light and fast.
//
//	Basically, each rule is a function that returns a VSET.  The strategy
//	is nothing more than a table of functions/rules.  Rules are done in
//	priority order.  The result must lie within the set created by the 
//	first rule.  Subsequent rules are used only if their results intersect
//	with the first sets results.
//
//	Difficulty is determined by the set of rules in play.  If all rules are
//	in play, the difficulty is maximized.
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
#include "strategy.h"

const static RULE ruleset[];

/*
 * set_difficulty
 *
 * This is in many ways our initialization routine.  Given a difficulty level,
 * find all the rules that apply when less than or equal to this difficulty
 * level.  Order them into a set of function pointers, so that we can then use
 * them in order.
 */
void set_difficulty(LPSTRATEGY s, int difficulty) {
	int		idx;
	const RULE	*rp;

	idx = 0;
	s->m_difficulty_level = difficulty;

	// For each potential rule
	for(rp = ruleset; rp->m_fn; rp++) {
		// Check the difficulty level
		if (rp->m_level <= difficulty)
			// and include it in our set if its for our difficulty
			// level
			s->m_rules[idx++] = rp;
	}

	// Count the number of rules we actually chose to use for this
	// difficulty level.
	s->m_num_rules = idx;
}

/*
 * makemove
 *
 * If you are trying to look for where/how the magic happens in the computers
 * strategy, you really want to start here.  This is the routine the computer
 * uses to make his move(s).  This routine calls the rules in order, applying
 * each of them, to determine a set of "good" places to move.  We then pick
 * from among that list randomly.
 */
int
makemove(LPSTRATEGY s, LPGBOARD brd, LPCOMBOSET cs, GB_PIECE whosemove)
{
	VSET	spots;
	int	rule_number;

	vs_clear(&spots);

	// Find one rule that gives us some result we can work with.  This
	// should be the first rule that returns any valid/legal move.
	for(rule_number=0; rule_number < s->m_num_rules; rule_number++) {
		(s->m_rules[rule_number]->m_fn)(brd, cs, whosemove, &spots);
		if (spots.m_active)
			break;
	}

	// Move on to the next rule
	rule_number++;

	// As long as we haven't exhausted all of the rules, let's see if we
	// can't refine our pick with a lower priority rule.
	if (rule_number < s->m_num_rules) {
		// Keep track of this refining value in a separate VSET
		VSET	others;

		do {
			// Apply a subsequent rule
			(s->m_rules[rule_number]->m_fn)(brd, cs,
						whosemove, &others);

			// Attempt to combine its results with our own.
			vs_combine(&spots, &others);

		// We are done when we have exhausted all of our rules, or 
		// equivalently when there's only one possible move to chose
		// from and therefore nothing left to refine.
		} while((++rule_number < s->m_num_rules)&&(spots.m_active > 1));
	}

	// Finally, now that we have our set of spots that we might wish to move
	// from, pick one at random from the set.
	return vs_pickmember(&spots);
}

/*
 * sum
 *
 * A simple helper function.  This just counts up the number of times a 
 * piece on the board is a part of a combo either owned by who or by no one,
 * that has nfilled spaces filled within it.
 */
static void
sum(LPCOMBOSET cs, LPVSET spots, GB_PIECE who, int nfilled)
{
	int	i, j;

	vs_clear(spots);

	for(i=0; i< cs->m_ninplay; i++) {
		if ((cs->m_data[i].m_nfilled == nfilled)
				&&(cs->m_data[i].m_owner == who)) {
			for(j=0; j< NUM_ON_SIDE-cs->m_data[i].m_nfilled; j++)
				vs_incscore(spots, cs->m_data[i].m_spots[j]);
		}
	}
}

/*
 * RULE: any
 *
 * Returns any legal move.  All choices of moves past this must intersect this
 * set.
 */
static void
any(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	int	i;

	for(i=0; i<NUM_SQUARES; i++)
		if (legal(brd, who, i))
			vs_incscore(spots, i);
}

/*
 * RULE: win
 *
 * If you can win on this move, do so
 */
static void
win(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	sum(cs, spots, who, 3);
}

/*
 * RULE: block
 *
 * If you must block your opponent to keep him from winning, do so.
 * Because this is an intersection with previous rules, though, we only block
 * on legal moves, on moves that will also cause us to win, or on legal moves
 * if the set of winning moves is the empty set.
 */
static void
block(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	sum(cs, spots, opponent(who), 3);
}

/*
 * RULE: makethree
 *
 * Kind of a stupid rule, but it keeps things lively: if you have a two in a 
 * row, make a three in a row out of it.
 */
static void
makethree(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	sum(cs, spots, who, 2);
}

/*
 * RULE: blocktwo
 *
 * The opposite of make three--if your opponent has a two in a row, block
 * it lest he get three in a row.
 */
static void
blocktwo(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	sum(cs, spots, opponent(who), 2);
}

/*
 * RULE: maketwo
 *
 * Even simpler than makethree, if we are going to choose with no information,
 * at least try to place a second value in the same row as something else
 * we already have in a row.
 */
static void
maketwo(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	sum(cs, spots, who, 1);
}

/*
 * RULE: blockone
 *
 * At this point, we are really grasping at straws for a move: find something
 * that intersects what my opponent might be doing, and block his efforts--this
 * reduces by one the number of possible winning moves he might have, so ... its
 * decent, but not too smart.
 */
static void
blockone(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	sum(cs, spots, opponent(who), 1);
}

/*
 * RULE: force
 *
 * The smart way to win is to set things up to where your opponent must block
 * two places at once.  If such is set up, then let's grab the linchpin and
 * force the win.  This requires, though, that we have two combinations, each
 * with two of my pieces in them, that intersect in an empty space.  In that
 * case, we take the empty space.
 */
static void
force(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	int	i;

	sum(cs, spots, who, 2);

	for(i=0; i<NUM_SQUARES; i++) {
		if (spots->m_data[i] < 2)
			vs_disable(spots, i);
	}
}

/*
 * RULE: blockforce
 *
 * Our opponent will force us to lose on his next move, if we don't block
 * him on this one.  Block him now.
 */
static void
blockforce(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	force(brd, cs, opponent(who), spots);
}

/*
 * RULE: setupforce
 *
 * Here we look for two intersecting combinations where the intersection is
 * still open.  Of those two, one must have two of our pieces in it, the other
 * must have one.  Choose to move into that one piece combination to set up a
 * force on the next move.
 */
static void
setupforce(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	VSET	onesum, twosum;
	int	i, j, k;

	vs_clear(spots);
	sum(cs, &onesum, who, 1);
	sum(cs, &twosum, who, 2);

	for(i=0; i<NUM_SQUARES; i++) {
		if ((!vs_isable(&onesum, i))||(!vs_isable(&twosum, i)))
			continue;

		for(j=0; j < cs->m_ninplay; j++) {
			LPCOMBOROW cr = &cs->m_data[j];
			if (cr->m_nfilled != 3)
				continue;
			if (cr->m_owner != who)
				continue;
			for(k=0; k < 4-cr->m_nfilled; k++) {
				if (cr->m_spots[k] == i)
					break;
			} if (k != 4-cr->m_nfilled) {
				for(k=0; k<4-cr->m_nfilled; k++) {
					if (cr->m_spots[k] == i)
						break;
					vs_incscore(spots, cr->m_spots[k]);
				}
			}
		}
	}
}

/*
 * RULE: nixsetup
 *
 * Keep our opponent from setting up a force, by moving into his pivot location
 * first.
 */
static void
nixsetup(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	VSET	onesum, twosum;
	int	i;

	vs_clear(spots);
	sum(cs, &onesum, who, 1);
	sum(cs, &twosum, who, 2);

	for(i=0; i<NUM_SQUARES; i++) {
		if ((!vs_isable(&onesum, i))||(!vs_isable(&twosum, i)))
			continue;

		vs_incscore(spots, i);
	}
}

/*
 * killn
 *
 * Attempt to DESTROY our opponent.  The way to do this is to force the opponent
 * to block, block, and block until he is stuck into a forced position.  Look
 * here for such a comborow which he might use for that purpose.  If you find
 * such a row intersecting other rows, move into the cross rows to try to
 * generate two pieces per row in each cross row before walking down this row
 * turning each of those cross rows into a 3-in a row that the opponent must
 * block.
 */
static void
killn(LPCOMBOSET cs, GB_PIECE who, LPVSET spots, int twos, int ones)
{
	VSET		onesum, twosum;
	GB_PIECE	opp = opponent(who);
	int	i, j, k, nv, found_ones, found_twos, found_zeros;
	LPCOMBOROW	cr;
	int	fo[80], ft[80], match[80];

	vs_clear(spots);
	sum(cs, &onesum, who, 1);
	sum(cs, &twosum, who, 2);

	if (onesum.m_active < ones*3)
		return;
	if (twosum.m_active < twos*2)
		return;

	nv = 0;
	for(i=0; i<80; i++)
		fo[i] = ft[i] = 0;

	// First step ... find a cross-bar--an uncompleted combo that we might
	// own (i.e. our opponent doesn't own) for which "twos" of the unused
	// locations connect to combos having two of our pieces within them,
	// and "ones" of the unused locations connect to combos having one
	// of our pieces within them.
	for(i=0; i < cs->m_ninplay; i++) {
		LPCOMBOROW	cr = &cs->m_data[i];
		if (cr->m_owner == opp)
			continue;
		if (cr->m_nfilled != NUM_ON_SIDE - ones - twos)
			continue;

		found_twos  = 0;
		found_ones  = 0;
		found_zeros = 0;
		// Now, cycle through the unused locations within this
		// row of four
		for(j=0; j < NUM_ON_SIDE - cr->m_nfilled; j++) {
			// First check: is this unused location connected to
			// another combo already having two filled?
			if (cr->m_nfilled == 2) {
				// Let's be careful not to count this particular
				// combo more than once
				if ((twosum.m_data[cr->m_spots[j]])>1) {
					found_twos++;

					// We continue, so as not to count this
					// square twice--it either counts toward
					// the number of twos (our preference)
					// or the number of ones, but not both.
					continue;
				}
			} else if ((twosum.m_data[cr->m_spots[j]])>0) {
					found_twos++;
					continue;
			}

			if (cr->m_nfilled == 3) {
				// Only count this one if it's not this comborow
				if ((onesum.m_data[cr->m_spots[j]])>1) {
					found_ones++;
					continue;
				}
			} else if ((onesum.m_data[cr->m_spots[j]])>0) {
				found_ones++;
				continue;
			}

			// found_zeros counts the number of things that dont
			// match at all
			found_zeros++;
		}

		// A zero found means that one of our cross rows had *nothing*
		// in it.  If that's the case, we aren't interested in it here,
		// so go on and look for another.
		if (found_zeros > 0)
			continue;
		if ((found_ones == ones)&&(found_twos >= twos)) {
			fo[nv] = found_ones;
			ft[nv] = found_twos;
			match[nv++] = i;
		}
	}

	for(int i=0; i< nv; i++) {
		// i is the index of the base we are working with
		LPCOMBOROW	cross = &cs->m_data[match[i]];
		for(int j=0; j< cs->m_ninplay; j++) {
			if (j == i)
				continue;
			cr = &cs->m_data[j];

			if (cr->m_owner == opp)
				continue;

			if (fo[i] != 0) {
				// Filling in the ones is our first priority
				if (cr->m_nfilled != 3)
					continue;
			} else if (ft[i] != 0) {
				// If there are no ones and only twos left,
				// then its time to force the win
				if (cr->m_nfilled != 2)
					continue;
			}

			if (!cr_isects(cr, cross))
				continue;
			for(k=0; k<NUM_ON_SIDE-cr->m_nfilled; k++) {
				// All spots, but the ones on the cross piece,
				// are ones we'll want to advance.  Those on the
				// cross we save for the very end.
				if (cr_isable(cross, cr->m_spots[k]))
					continue;
				vs_incscore(spots, cr->m_spots[k]);
			}
		}
	}
}

/*
 * live
 *
 * This is very similar to killn() above, but in this case we wish to keep our
 * opponent from killing us.  Hence, we find any row that might be used for that
 * purpose and rather than building up the cross rows, we try to cleanup that
 * pivot row.
 */
static void
live(LPCOMBOSET cs, GB_PIECE who, LPVSET spots, int twos, int ones)
{
	VSET		onesum, twosum;
	GB_PIECE	opp = opponent(who);
	int	i, j, nv, found_ones, found_twos, found_zeros;
	LPCOMBOROW	cr;
	int	match[80];

	vs_clear(spots);
	sum(cs, &onesum, opp, 1);
	sum(cs, &twosum, opp, 2);

	if (onesum.m_active < ones*3)
		return;
	if (twosum.m_active < twos*2)
		return;

	nv = 0;

	// First step ... find a cross-bar--an uncompleted combo that we might
	// own (i.e. our opponent doesn't own) for which "twos" of the unused
	// locations connect to combos having two of our pieces within them,
	// and "ones" of the unused locations connect to combos having one
	// of our pieces within them.
	for(i=0; i < cs->m_ninplay; i++) {
		LPCOMBOROW	cr = &cs->m_data[i];
		if (cr->m_owner == who)
			continue;
		if (cr->m_nfilled != NUM_ON_SIDE - ones - twos)
			continue;

		found_twos  = 0;
		found_ones  = 0;
		found_zeros = 0;
		// Now, cycle through the unused locations within this
		// row of four
		for(j=0; j < NUM_ON_SIDE - cr->m_nfilled; j++) {
			// First check: is this unused location connected to
			// another combo already having two filled?
			if (cr->m_nfilled == 2) {
				// Let's be careful not to count this particular
				// combo more than once
				if ((twosum.m_data[cr->m_spots[j]])>1) {
					found_twos++;

					// We continue, so as not to count this
					// square twice--it either counts toward
					// the number of twos (our preference)
					// or the number of ones, but not both.
					continue;
				}
			} else if ((twosum.m_data[cr->m_spots[j]])>0) {
					found_twos++;
					continue;
			}

			if (cr->m_nfilled == 3) {
				// Only count this one if it's not this comborow
				if ((onesum.m_data[cr->m_spots[j]])>1) {
					found_ones++;
					continue;
				}
			} else if ((onesum.m_data[cr->m_spots[j]])>0) {
				found_ones++;
				continue;
			}

			// found_zeros counts the number of things that dont
			// match at all
			found_zeros++;
		}

		if (found_zeros > 0)
			continue;
		if ((found_ones == ones)&&(found_twos >= twos))
			match[nv++] = i;
	}

	// Unlike the killn approach above, we aren't trying to set ourselves
	// up to force a win.  We are simply trying to keep our opponent from
	// setting something like that up.  Hence, we recommend moving into
	// the pivot points, rather than the quiet setups
	// 
	for(i=0; i< nv; i++) {
		cr = &cs->m_data[match[i]];
		for(j=0; j< 4-cr->m_nfilled; j++)
			vs_incscore(spots, cr->m_spots[j]);
	}
}

/*
 * newforce
 *
 * If you can find a row with a single two in a row cross and no single crosses,
 * pull the pin on it and force a win.
 */
static void
newforce(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	killn(cs, who, spots, 1, 0);
}

/*
 * newblockforce
 *
 * Opposite of newforce--block any means our opponent might try to forcce us to
 * lose.
 */
static void
newblockforce(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	live(cs, who, spots, 1, 0);
}

/*
 * kill_setup_1
 *
 * We don't have any kill options set up right now, so look for an opportunity
 * and try to set one up one move from now.
 */
static void
kill_setup_1(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	killn(cs, who, spots, 2, 1);
}

/*
 * kill_block_1
 *
 * Same as kill_setup_1, but our goal is to block.
 */
static void
kill_block_1(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	live(cs, who, spots, 2, 1);
}

/*
 * kill_setup_2
 *
 * Same as kill_setup_1, but the winning approach requires a longer set of
 * force opponent to do X moves.
 */
static void
kill_setup_2(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	killn(cs, who, spots, 3, 2);
}

/*
 * kill_block_2
 *
 * Same as kill_block_1, but block a longer trick.
 */
static void
kill_block_2(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots) {
	live(cs, who, spots, 3, 2);
}

/*
 * kill_setup_3
 *
 * Same as kill_setup_2, but the winning approach requires one more in the set
 * of force opponent to do X moves.
 */
static void
kill_setup_3(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots) {
	killn(cs, who, spots, 4, 3);
}

/*
 * kill_block_3
 *
 * Same as kill_block_2, but block a longer trick.
 */
static void
kill_block_3(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots) {
	live(cs, who, spots, 4, 3);
}

static void
prekill(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	killn(cs, who, spots, 1, 1);
}

static void
prekill_1(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{
	killn(cs, who, spots, 0, 2);
}

/*
 * corners
 *
 * The prior strategy rules just ... havent been enough to keep me from winning.
 * So ... let's try to defeat the strategy here by going for and grabbing a 
 * corner.  It's better than nothing, so if we would've otherwise just picked
 * something at random, now we have some more method to our madness.
 */
static void
corners(LPGBOARD brd, LPCOMBOSET cs, GB_PIECE who, LPVSET spots)
{

	vs_clear(spots);

	// Simply rate the corners more valuable than anywhere else
	vs_incscore(spots, coordtoint(0, 0, 0));
	vs_incscore(spots, coordtoint(3, 0, 0));
	vs_incscore(spots, coordtoint(0, 3, 0));
	vs_incscore(spots, coordtoint(3, 3, 0));
	vs_incscore(spots, coordtoint(0, 0, 3));
	vs_incscore(spots, coordtoint(3, 0, 3));
	vs_incscore(spots, coordtoint(0, 3, 3));
	vs_incscore(spots, coordtoint(3, 3, 3));
}

/*
 * ruleset
 *
 * Here's our list of rules that we use in decision making.  Every rule has a
 * name, a difficulty level at or above which it will be applied, and a pointer
 * to the function that applies the rule.
 */
const static RULE ruleset[] = {
	{ "ANY",	0, any },
	{ "WIN",	1, win },
	{ "BLOCK",	1, block },
	{ "NEW-FORCE", 	6, newforce },
	{ "NWBK-FORCE",	6, newblockforce },
	{ "KBLOCK-1",	7, kill_block_1 },
	{ "KSETUP-1",	7, kill_setup_1 },
	{ "KBLOCK-2",	8, kill_block_2 },
	{ "KBLOCK-3",	9, kill_block_3 },
	{ "KSETUP-2",	7, kill_setup_2 },
	{ "KSETUP-3",	7, kill_setup_3 },
	{ "PREK",	10, prekill },
	{ "PREK-1",	10, prekill_1 },
	{ "FORCE",	4, force },
	{ "BLOCK-FORCE", 4, blockforce },
	{ "SETUP-FORCE", 5, setupforce },
	{ "NIX-SETUP",	5, nixsetup },
	{ "MAKE-THREE",	2, makethree },
	{ "BLOCK-TWO",	2, blocktwo },
	{ "MAKE-TWO",	3, maketwo },
	{ "BLOCK-ONE",	3, blockone },
	{ "CORNERS",	9, corners },
	// The last entry has a NULL rule, so that we can tell that it is the
	// last entry and use no more.
	{ "", 0, NULL }
};

