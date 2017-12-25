# State-Space-Search-N-Lizard
A state-space search solution for an N-Queen style problem using Breadth First, Depth Search, and Simulated Annealing search methods.

This is an N-Queen style problem involving obstacles. Two or more lizards can attack each other if they are in the same row, column, or diagonal, without an obstacle between them.

Objective is to find a possible placement of N lizards on the given nursery (board) such that no lizards attack each other.

The input.txt and output.txt are used as follows:

Lizards are represented as 1's, and obstacles as 0's.

Input: The	file	input.txt in	the	current	directory	of	your	program	will	be	formatted	as	follows:

First	line:	 instruction	of	which	algorithm	to	use:	BFS,	DFS	or	SA

Second	line:	 strictly	positive	32-bit	integer	n,	the	width	and	height	of	the	square	nursery

Third	line:	 strictly	positive	32-bit	integer p,	the	number	of	baby	lizards

Next	n	lines:	 the	n	x	n	nursery,	one	file	line	per	nursery	row	(to	show	you	where	the	trees	are).

It	will	have	a	0	where	there	is	nothing,	and	a	2	where	there	is	a	tree.	

So	for	instance,	an	input	file	arranged	like	figure	2B (but	with	no	lizards	yet)	and	requesting	you	
to	use	the DFS	algorithm	to	place	8	lizards	in	the	8x8	nursery	would	look	like:

DFS

8

8

00000000

00000000

00000000

00002000

00000000

00000200

00000000

00000000

Output:	The	file	output.txt which	your	program	creates	in	the	current	directory	should be	
formatted	as	follows:

First	line:	 OK or FAIL, indicating	whether	a	solution	was	found	or	not.

If	FAIL,	any following	lines	are	ignored.

Next	n	lines:	 the	n	x	n	nursery,	one	line in	the	file per	nursery	row,	including	the	baby	lizards
and	trees. It	will	have	a	0	where	there	is	nothing,	a	1	where	you	placed	a	baby	
lizard, and	a	2	where	there	is	a	tree.	

For	example,	a	correct	output.txt	for	the	above	sample	input.txt	(and	matching	Figure	2B) is:

OK

00000100

10000000

00001000

01002001

00000000

00100200

00000010

00010000




