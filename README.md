# DSA-ASSIGNMENT_1
My Data Structures Assignment 1 (Polynomial ADT, Text Editor, UNO Game)

https://github.com/Ubaidcode11/DSA-ASSIGNMENT_1/tree/main

1. Polynomial ADT (Linked List Implementation)
 Approach:

Polynomials stored as a linked list (sorted in descending order of exponents).

Each node = coefficient + exponent.

While inserting:

If exponent already exists -> combine coefficients.

Else -> insert in correct sorted place.

All operations return new polynomial objects (originals unchanged).

Implemented operations:

Addition -> merge two lists, combine like terms.

Multiplication -> multiply every term, merge results.

Differentiation -> apply (a*n)x^(n-1) to each term.

Challenges:

Handling duplicate exponents correctly.

Keeping list sorted at all times.

Display formatting (proper math style).

Doing everything with fixed header (Polynomial.h).

Verifying correctness with multiple tests.

2. Text Editor (Linked List Implementation)
 Approach:

Made with a Doubly Linked List -> O(1) insert/delete.

Each node holds one character (prev + next pointers).

Cursor always points after current position (nullptr = end).

Operations handled in .cpp using a helper class (EditorList).

TextEditor just calls those functions (header unchanged).

Functions:

insertChar(c) -> insert at cursor

deleteChar() -> delete before cursor

moveLeft() / moveRight() -> shift cursor

getTextWithCursor() -> show text with | marking cursor

 Challenges:

Keeping cursor in right place after every operation.

Managing everything inside .cpp due to header restriction.

Edge cases → empty text, cursor at start/end.

Making sure long mixed sequences worked correctly.

3. UNO Game (Linked List + Structures)
 Approach:

Instructor gave uno.h -> all code written in uno.cpp.

Internal structures:

CardInfo (color + value)

Impl struct -> deck, hands, discard, current player, direction, winner, RNG.

Deck: 76 cards (0–9 numbers, Skip, Reverse, Draw Two ×3 each color).

Game Rules implemented:

Match top card’s color or value.

If can’t play -> draw one card, play if possible.

Action cards:

Skip -> next player skipped

Reverse -> change play direction

Draw Two -> next player draws 2 + skips

“UNO” printed when player has 1 card.

Game ends ->

player has 0 cards (win), or

deck empty + no one can play (stalemate).

Output format:

Player X's turn, Direction: Y, Top: COLOR VALUE, Players cards: P0:A0, P1:A1, ...

 Challenges:

Sticking to strict header restrictions.

Handling Skip/Reverse/Draw Two correctly.

Detecting UNO condition.

Stalemate handling.
