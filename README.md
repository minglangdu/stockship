# Stockship

I came up with this idea with my friend while walking for a while. Basically it is a Monte Carlo simulation that uses your record of your hits and misses in [Battleship](https://en.wikipedia.org/wiki/Battleship_(game)) to find your best move. 

There will be two options to choose from when using this program: you can simulate every possible ship positioning your opponent has or make a set number of simulations that use random chance to find a ship position (even though I have no idea why you would use it).

The algorithm I have in mind currently (4/18/2025) is quite unoptimal, so I will likely need to change it soon.

## Controls

To cycle between an empty square, a miss, and a hit, left click a square on the grid. To cycle between that sequence backwards, right click the square on the grid. 
Press space to toggle between giving input and receiving output. Gold tiles in the output are locations that are optimal moves. In the rest of the tiles, whiter tiles are more optimal than blacker tiles.