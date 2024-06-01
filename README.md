# game-of-life

[Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) implemented in C using [Raylib](https://github.com/raysan5/raylib).

# Building

Install [Raylib](https://github.com/raysan5/raylib) for your system.

```console
gcc game_of_life.c -O3 -lraylib -o game_of_life
./game_of_life
```

# Controls

- `SPACE` - Pause/Unpause simulation and enter/exit edit mode
  - Use Left and Right mouse buttons can be used to add or remove cells while in edit mode
- `ESC` - Quit
