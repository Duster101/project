# MazeGame

A simple 2D maze game built in C++ using console graphics.

##  Gameplay

- Navigate the maze using **W**, **A**, **S**, **D** keys.
- Collect `*` to gain points.
- Avoid `X` enemies.
- Reach the exit `E` to win the level.

##  Features

- Multiple levels (load from text files)
- Collectibles and score tracking
- Random enemy movement
- Win/Lose game states
- Basic OOP structure and file I/O

##  How to Run

1. Clone or download this repo.
2. Navigate to the folder and compile:

```bash
g++ src/*.cpp -Iinclude -o MazeGame
./MazeGame
```

> Make sure to have your `levels/` folder with `level1.txt` and `level2.txt`.

##  Folder Structure

```
MazeGame/
├── src/
├── include/
├── levels/
├── README.md
```

##  Author

Kushal Deshar – 2025
