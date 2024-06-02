# game-of-life

[Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) implemented in C using [Raylib](https://github.com/raysan5/raylib).

https://github.com/screaminglineage/game-of-life/assets/106450040/505425f8-089a-4df8-a297-2fdbb785afaf

# Building

Install [Raylib](https://github.com/raysan5/raylib) for your system.

```console
gcc game_of_life.c -O3 -lraylib -o game_of_life
./game_of_life
```

# Controls

- `SPACE` - Play/Pause the simulation
- `E` - Enter edit mode
  - Use `Left` and `Right` mouse buttons can be used to add or remove cells while in edit mode
- `S` - Enter step mode (Keep pressing `S` to step through each generation)
- `ESC` - Quit
