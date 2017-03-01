////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	vset.h
//
// Project:	tttt, a simple 4x4x4 Tic-Tac-Toe Program
//
// Purpose:	Defines the interface necessary to work with a valued set.  That
//		is, every element, if it is a member of the set, has a value
//	or score associated with it.  We can do intersections of these sets,
//	picking the score of one or the other, combine them, check for
//	membership within them, etc.
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
#ifndef	VSET_H
#define	VSET_H

typedef	struct	VSET_S {
	int	m_active;
	int	m_data[NUM_SQUARES];
} VSET, *LPVSET;

extern	void	vs_clear(LPVSET);
extern	void	vs_set(LPVSET dst, LPVSET src);
extern	bool	vs_isempty(LPVSET);
extern	void	vs_incscore(LPVSET, int);
extern	void	vs_decscore(LPVSET, int);
extern	bool	vs_isable(LPVSET, int);
extern	void	vs_addscore(LPVSET, int, int);
extern	void	vs_subscore(LPVSET, int, int);
extern	void	vs_disable(LPVSET, int);
extern	int	vs_pickmember(LPVSET);
extern	void	vs_add(LPVSET,LPVSET);
extern	void	vs_sub(LPVSET,LPVSET);
extern	void	vs_combine(LPVSET,LPVSET);
extern	int	vs_numactive(LPVSET);

// Dump something interesting to the screen
extern	void	vs_debug(LPVSET);

#endif
