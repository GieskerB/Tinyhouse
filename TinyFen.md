# TinyFen

[Fen notion](https://www.chess.com/de/terms/forsyth-edwards-notation-fen) is used in chess to describe the most important aspects of the current game position. Since Tinyhouse has slightly different rules to *normal* chess I came up with my own TinyFen notation.

First I will unveil what is contained in the standard Fen to then filter out and add new notation.

## Fen Conversion

The original Fen has the following 6 parts. To avoid repetition, each section is separated by a whitespace character.

### Piece Placement

The Fen notation uses a clever run length encoding with line breaks for the large 8x8 grid. Only empty tiles get compressed and are simply represented by a number that indicates how many empty files occur in one succession. The six pieces in chess are represented by their first letter and to distinguish white from black pieces, white pieces are written in capital.

This will be handled very similarly in TinyFen. However, since the board is only 4x4 this section is a bit smaller. Fortunately, the initials of all five pieces in Tinyhouse are also unique:

- Pawn: p
- Ferz: f
- Hors: h
- Wazir: w
- King: k

### Active Color

This section simply indicates with a single character whose turn it is currently. w for white and b for black. This is also used in TinyFen.

### Castling Rights

Castling is not enabled in Tinyhouse, therefore no need to keep this in TinyFen.

### Possible En Passant Targets

Similar to Castling, en passant is also not allowed in Tinyhouse and therefore not required in TinyFen.

### Halfmove Clock

This counter keeps track since the last pawn advancement or piece capture. If it reaches 100 (50 moves per player) the game must end in a draw.

### Fullmove Number

A strictly increasing counter of the total played rounds.

## TinyFen Addition

1. The house! In Tinyhouse -- or any Crazyhouse variant -- captured pieces are still in the game and can be placed back onto the board.
2. Promoted pieces! It is important to know *which* peace is prompted since when it is captured it is turned into a pawn in the house.

### The House

Similar to the piece placement I opt to represent them with the piece letters in upper and lower case. Right after the piece placement add the "house" part separated with a backslash. An empty house will be represented with ```-``` and an non-empty house be the set of all pieces in the house.

### Promoted pieces

I opt to write the exact location of a promoted piece in the middle of the TinyFen at the location of the Castling and En Passant notation in normal Fen.

If there is no promoted piece on the board, the notation is a simple ```-```
In case of one or two promoted pieces, write the pieces location -- like ```a3``` -- and separate them with a space in case of two promoted pieces.

## Examples

### Starting Position

|Type|Notations|
|-|-|
|Fen|```"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"```|
|TinyTen|```"fhwk/3p/P3/KWHF\- w - 0 0"```|
