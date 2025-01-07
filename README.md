# Morpion_4_octets

## Description

This project is a Tic-Ta-Toe game that stock all the datas into one 4 bytes unsigned integer.
The game is played on a 3x3 grid, and the goal is to align 3 of your pieces in a row, column or diagonal. 
The game is played by two players, one after the other, and the game ends when a player have 5 wins or with 20 nuls.

## How this work

With defined macros as masks and shift values, we can easily manipulate the 4 bytes integer to get the informations we need.
The game is played on a 3x3 grid, so we need 18 bits to represent the board. 
We use 1 bit to represent the player who have to play, 2 bits for the state of the game, 3 bits for the number of wins per players and 5 bits for the number of nuls.
