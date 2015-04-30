Student Details 
===============
|Name            | ID        |
|----------------|-----------|
|Timothy Kiyui   | 4316886   |
|Brian Sim       | 4316878   |

Features 
========
NA

Bugs 
====
NA

Missing 
=======
NA

Resources 
=========
tree.hh and tree_util.hh were acquired from http://tree.phi-sci.com/
tree.hh was slightly modified for extra requirements, needed to
simplify the making of this application. As always, it remains GPL.

License 
=======
These programs are released under the GNU GPL V3 in compliance with
used libraries and developers will

Compile 
=======
To compile the code, make sure you have sfml set up on your system.
Get it here: http://www.sfml-dev.org/download.php
After that, run make and everything should compile fine.

Usage 
=====
Run the main file as so:
./maze_solver MAP ALGORITHM [FPS: 1 | 2 | etc] [-v]

Algorithms:
-bfs    Breadth First Search
-dfs    Depth First Search
-gdfs   Greedy Best First Search
-ass    A* Search
-bs Beam Search
-hs Hill Climbing
