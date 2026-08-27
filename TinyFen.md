# TinyFen

Fen notion is used in chess to describe ALL important aspects of the current game position. Since Tinyhouse has slightly different rules to *normal* chess I came up with my own TinyFen notation.

First lets us unveil what is all contained in the standard Fen to then filter out and add new notation.

## Fen Conversion

The original Fen has the following 6 parts. To avoid repetition, each section is separated by a whitespace character.

### Piece Placement

The Fen notation uses a clever run length encoding with line breaks for the large 8x8 grid. Only empty tiles get compressed and are simply represented by a number that indicates how many empty line occur in one succession. The six pieces in chess are represented by their first letter and to distinguish white from black peaces, white pieces are written in capital. 

This will be handled very similar in TinyFen. However since the board is only 4x4 the part is a bit smaller. Fortunately the initials of all five pieces in Tinyhouse are also unique as well:

- Pawn: p
- Ferz: f
- Hors: h
- Wazir: w
- King: k

### Active Color

This section simply indicates with a single character which turn it is currently. w for white and b for black. This is also used in TinyFen

### Castling Rights

Castling is not enabled in Tinyhouse, therefore no need to keep this in TinyFen

### Possible En Passant Targets

Similar to Castling, en passant is also not allowed in Tinyhouse and therefore not represented in TinyFen

### Halfmove Clock

This counter keeps track since the last pawn advancement or piece capture. If it reaches 100 (50 moves per player) the game must end in a draw

### Fullmove Number

Strictly increasing round counter without any meaning.

---

### TinyFen Addition

1. Three move repetition is not represented in normal Fen, but I will incorporate it in TinyFen right before the Halfmove Clock.
2. The house! In Tinyhouse -- or any house variant -- captured peaces are still in the game and can be placed back onto the board. 

### The House

Similar to the piece placement I opt to represent them with the piece letters in upper and lower case. Right after the piece placement add the "house" part separated with a backslash. An empty house will be represented with '-' and an non empty house be the set of all pieces in the house.

## Example

### Starting Position

- Fen: ```"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"```
- TinyTen ```"fhwk/3p/P3/KWHF\- w 0 0 0"```