# Maze Navigation Robot (KPP Programming Task)

A C++ program that guides a robot named **Atlas** through grid mazes. The robot starts at `S`, captures the flag `F`, then returns to the base `G`, always taking the shortest route. It prints the moves and shows the maze after every step.

This was my programming task for the KPP of Pelatihan Dasar Robotika ITS (ITS Robotics Student Club), 2026.

## Maze Legend

| Symbol | Meaning |
| ------ | ------- |
| `S` | Start position |
| `F` | Flag the robot must capture first |
| `G` | Base the robot must reach after the flag |
| `.` | Open tile |
| `#` | Wall (blocked) |
| `X` | Obstacle (blocked) |
| `R` | Robot's current position (shown in the visualization) |

The program solves three mazes of increasing size: 7x7, 9x9, and 11x11.

## How It Works

1. **Load the maze** into a 2D character grid.
2. **Locate** the coordinates of `S`, `F`, and `G` by scanning the grid.
3. **Find the shortest path** twice with Breadth-First Search (BFS): from start to flag, then from flag to base.
4. **Print the result:** the move list (`UP`, `DOWN`, `LEFT`, `RIGHT`), the maze at every step, and the total number of moves.

### Breadth-First Search

BFS explores the maze in rings of increasing distance from the start, using a queue. Because of this, the first time it reaches the destination, the route it found is the shortest one.

- A `Visited` grid prevents the robot from revisiting tiles.
- A `ParentMap` records which tile each tile was reached from.
- After the destination is reached, the path is rebuilt by following the parents back to the start, then reversing the list.

### Program Structure

- `RobotNavigator` is an abstract base class. It holds the maze, the marker positions, and the helper methods for loading the maze, finding paths, and printing. Its `methodExecuteMission` is a pure