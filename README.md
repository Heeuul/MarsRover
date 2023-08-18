# MarsRover

Tile based game made with C++ to simulate the exploration of Mars rover.

**Goal**: Control the movement of Mars rover in collecting all the gold while avoid falling into the holes.

## How to Play

1. Clone/download whole repository, or only MarsRover.exe.
2. Double-click MarsRover.exe to run.
3. Enter desired amount of rows, columns, and golds of the tile space.
4. Insert movement commands to control the rover. You may enter multiple commands in a single input.
5. Enter command "Q" to close and exit the software.

## Commands

| Key | Action                                     |
| --- | ------------------------------------------ |
| M   | Move rover 1 tile towards facing direction |
| L   | Rotate rover left on same tile             |
| R   | Rotate rover right on same tile            |
| Q   | Quit the application                       |

## Gameplay

### Map Contents

| Symbol | Name      | Explanation                            |
| ------ | --------- | -------------------------------------- |
| X      | Hill      | Blocked tile that cannot be moved into |
| #      | Hole      | Player loses upon moving enter it      |
| $      | Gold      | Objective to be collected              |
| ?      | Anonymous | Undiscovered area                      |

### Scoring System

| Method            | Points |
| ----------------- | ------ |
| Collect a gold    | 5      |
| Move a turn       | -1     |
| Perform a command | -1     |
