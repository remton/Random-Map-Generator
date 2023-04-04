# Random-Map-Generator
Creates a randomized map of rooms of different sizes.

This was my final project for my introductory programming class in 2019.
This program creates a random 2d map of rooms, signified by letter characters in a 2d array.
Each room can have doors on each side.
I created this program to test the logic of the room generation for a larger project, so this alone is a sort-of proof of concept.

## Compile with some command like:
g++ RandomMapGen.cpp

## Run with some command like:
./a.out

## HOW TO USE
	The Rooms.txt file can be written to directly but it is read in a specific way, so I created a way to auto-generate rooms and a way to create rooms manualy within the program.
	The map and automatic room list are given a seed for the random number generator. This can be used to get the same generation repeatedly

	Every room has:
		-width 
		-height

		-UpOpen (If the room has a door to the north)
		-UpOffset (The Left-Right offset of the door)

		-DownOpen 
		-DownOffset 

		-LeftOpen 
		-LeftOffset  (int, the Top-Bottom offset of the door)

		-RightOpen 
		-RightOffset

	EX:		
		RoomChar: A			  A A
		Width: 2			  = A A
		Height: 3			    A A
		OpensUp: 0			  |
		UpOffset: 0
		OpensDown: 1
		DownOffset: 0
		OpensLeft: 1
		LeftOffset: 1
		OpensRight: 0
		RightOffset: 0
