NEATmaze visualizes the process of a NEAT algorithm completing a maze. A set number of bots are controlled by a neural network, 
and at the end of every round their respective fitness scores are calculated. The fitness score represents how "good" the bot is 
at completing the maze. Then every bot is sent back to the start and their networks are adjusted to be similar to the networks 
with the highest fitness. 

This app uses MultiNEAT, SFML, and Boost.

https://github.com/peter-ch/MultiNEAT

https://github.com/SFML/SFML

https://github.com/boostorg

One bot is controlled by the player, but this does not affect the algorithm.

Arrow keys to control player

Space bar to end round

Del key to reset population

Esc to exit game
