# Chess-Engine

A modern C++ chess engine with an SDL2 front-end.

---

## Summary  
* **Language:** C++20  
* **Graphics:** [SDL2](https://github.com/libsdl-org/SDL/tree/SDL2) (+ SDL2_image)  
* **Design:** Separation between **engine** (board logic) and **renderer** (view).

---

## Build & Run

```bash
TODO
```

---

## Current Progress


### Board representation - Done
* `std::array<uint8_t, 64>` — one byte per square.
* **Bit layout**: |`?`|`?`|`?`|`?`|`C`|`T T T`|
  * `TTT` Piece type codes
  `0 = None`, `1 = Pawn`, `2 = Knight`, `3 = Bishop`,  
  `4 = Rook`, `5 = Queen`, `6 = King`, `7 = Ghost-Pawn (en passant)`
  * `C` = color bit (0 = white, 1 = black)
  * `?` = future flags (castling rights, promotion info, etc)
* Board construction from FEN string. Useful for future test cases.
  * Example (starting): `rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1`

### Renderer - Done
* Draws 800x800 board + PNG sprites for pieces.  
* Drag-and-drop with selection highlights.  
* Lives in its own module as a view so test cases can run headless.

### Basic gameplay - Done
* Move/capture pieces on the board.  
* Play a full casual game exactly as on a physical board.

---

## TODO List

* **Full move rules**: Individual piece legality, turn order, castling, en-passant, etc.  
* **FEN test cases**: Compare to Stockfish and validate move rules  
* **Game end detection**: Check, check-mate, stalemate, 50-move, repetition  
* **Basic engine algos**: Assign piece-square values, implement **minimax**  
* **Engine optimisation**: Zobrist hashing, opening book, piece-square tables, etc.  
