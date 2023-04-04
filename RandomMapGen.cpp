// Remington Ward
// 12/05/19
// Random Map Generator

// DESCRIPTION:
// This program creates a random 2d map of rooms, signified by letter characters in a 2d array.
// Each room can have doors on each side.
// I created this program to test the logic of the room generation for a larger project, so this alone is a sort-of proof of concept.

/*
	HOW TO USE
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
		Width: 2			= A A
		Height: 3			  A A
		OpensUp: 0			  |
		UpOffset: 0
		OpensDown: 1
		DownOffset: 0
		OpensLeft: 1
		LeftOffset: 1
		OpensRight: 0
		RightOffset: 0

*/


#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
using namespace std;

//Global Constants
const int ROW_NUM = 50;						// number of rows in the map
const int COL_NUM = 50;						// number of columbs in the map
const char EMPTY_CHAR = ' ';				// the character that represents an empty map space
const char DOOR_CHAR_VERT = '|';			// character for vertical doors
const char DOOR_CHAR_HORIZ = '=';			// character for horizontal doors
const int ARRAY_LIMIT = 52;					// The size of the arrays that hold the information in the file when read. Also the max number of rooms read from the room file
const string ROOM_FILE_NAME = "Rooms.txt";	// The name of the room file that stores each available room

//Global variables
//Rooms in the code are signified by thier index in the following parallel arrays
char roomChars[ARRAY_LIMIT];
int width[ARRAY_LIMIT];
int height[ARRAY_LIMIT];
bool upOpen[ARRAY_LIMIT];
int upOffset[ARRAY_LIMIT];
bool downOpen[ARRAY_LIMIT];
int downOffset[ARRAY_LIMIT];
bool leftOpen[ARRAY_LIMIT];
int leftOffset[ARRAY_LIMIT];
bool rightOpen[ARRAY_LIMIT];
int rightOffset[ARRAY_LIMIT];
int roomsInFile; //the number of rooms in the file

//Function prototypes
int askForInt();
bool askForBool();
char askForChar();
void manualWriteToRooms();
void autoWriteToRooms();
void readFromRooms();
void emptyMap(char mapArray[ROW_NUM][COL_NUM]);
void createMap(char mapArray[ROW_NUM][COL_NUM]);
void displayMap(const char mapArray[ROW_NUM][COL_NUM]);


int main()
{
	char mapArray[ROW_NUM][COL_NUM]; 
	emptyMap(mapArray);

	cout << "Do you want to create a new room list? (y/n): ";
	if (askForBool())
	{
		cout << "For manual entry press \'y\', for automatic entry press \'n\': ";
		if (askForBool())
			manualWriteToRooms();
		else
			autoWriteToRooms();
	}

	readFromRooms();
	cout << "Enter a seed for the map: ";
	srand(askForInt());

	createMap(mapArray); 

	displayMap(mapArray);
	cin.get();
	return 0;
}


//repeatedly takes user input until a posititve integer is entered
int askForInt()
{
	string input;
	bool done = false;
	do
	{
		getline(cin, input);
		const char *cInput = input.c_str();
		done = true;
		if (strlen(cInput) < 1)
			done = false;
		for (int i = 0; i < strlen(cInput); i++)
		{
			if (!isdigit(cInput[i]))
			{
				done = false;
			}
		}

		if(!done)
			cout << "Invalid input. Try Again" << endl;
	} while (!done);
	return stoi(input);
}


//repeatedly takes user input until a 'y' or an 'n' is entered
bool askForBool()
{
	string input;
	while (true)
	{
		getline(cin, input);
		if (input[0] == 'y' || input[0] == 'Y')
			return true;
		else if (input[0] == 'n' || input[0] == 'N')
			return false;
		cout << "Invalid input. Try Again" << endl;
	}
}

//repeatedly takes user input until only a single character is entered
char askForChar()
{
	string input;
	const char *cInput;
	while (true)
	{
		getline(cin, input);
		cInput = input.c_str();
		if (strlen(cInput) == 1)
			return cInput[0];
	}
}

//Makes a random list of rooms and stores it into the room file
void autoWriteToRooms()
{
	const int MAX_WIDTH = 5; //max width of a room
	const int MAX_HEIGHT = 5; //max height of a room

	ofstream roomFile;
	roomFile.open(ROOM_FILE_NAME);

	cout << "Enter a seed for the list: ";
	srand(askForInt());

	cout << "Enter the number of rooms to generate (max is " << ARRAY_LIMIT << "): ";
	int roomCount = askForInt();
	if (roomCount > ARRAY_LIMIT)
		roomCount = ARRAY_LIMIT;
	for (int i = 0; i < roomCount; i++)
	{
		//(rand() % x) gives a random number (0 - (x-1))
		int randomWidth = ((rand() % MAX_WIDTH) + 1);
		int randomHeight = ((rand() % MAX_HEIGHT) + 1);
		bool randomOpensUp = (rand() % 2);
		int randomUpOffset = (rand() % randomWidth);
		bool randomOpensDown = (rand() % 2);
		int randomDownOffset = (rand() % randomWidth);
		bool randomOpensLeft = (rand() % 2);
		int randomLeftOffset = (rand() % randomHeight);
		bool randomOpensRight = (rand() % 2);
		int randomRightOffset = (rand() % randomHeight);

		int guarenteedDoor = rand() % 4; //One door (0,1,2,3 : up, down, left, right) that will be open
		switch (guarenteedDoor)
		{
			case 0:
				randomOpensUp = true;
				break;
			case 1:
				randomOpensDown = true;
				break;
			case 2:
				randomOpensLeft = true;
				break;
			case 3: 
				randomOpensRight = true; 
				break;
			default:
				cout << "ERROR in autWriteToRooms: guarenteed door is an invalid value" << endl;
				break;
		}

		//65-90, 97-122 letters in ascii table
		if (i < 26)
			roomFile << "RoomChar: " << char(65 + i) << endl;
		else
			roomFile << "RoomChar: " << char((97 - 26) + i) << endl;

		
		roomFile << "Width: " << randomWidth << endl;
		roomFile << "Height: " << randomHeight << endl;
		roomFile << "OpensUp: " << randomOpensUp << endl;
		roomFile << "UpOffset: " << randomUpOffset << endl;
		roomFile << "OpensDown: " << randomOpensDown << endl;
		roomFile << "DownOffset: " << randomDownOffset << endl;
		roomFile << "OpensLeft: " << randomOpensLeft << endl;
		roomFile << "LeftOffset: " << randomLeftOffset << endl;
		roomFile << "OpensRight: " << randomOpensRight << endl;
		roomFile << "RightOffset: " << randomRightOffset  << endl;
		roomFile << endl << endl;
	}
}


//repeatedly asks the user to enter information for every room and stores it in the room file
void manualWriteToRooms()
{
	ofstream roomFile;
	bool tempBool;
	roomFile.open(ROOM_FILE_NAME);

	bool done = false;
	int i = 0; 
	do
	{
		cout << endl;
		//65-90, 97-122 letters in ascii table
		if (i < 26)
		{
			cout << "Room: " << char(65 + i) << endl;
			roomFile << "RoomChar: " << char(65 + i) << endl;
		}
		else
		{
			cout << "Room: " << char((97 - 26) + i) << endl;
			roomFile << "RoomChar: " << char((97 - 26) + i) << endl;
		}

		cout << "Room width: ";
		roomFile << "Width: " << askForInt() << endl;
		cout << "Room height: ";
		roomFile << "Height: " << askForInt() << endl;

		//Up Opening
		cout << "Does room open up? (y/n): ";
		tempBool = askForBool();
		roomFile << "OpensUp: " << tempBool << endl;
		if (tempBool)
		{
			cout << "Up door offset: ";
			roomFile << "UpOffset: " << askForInt() << endl;
		}
		else
			roomFile << "UpOffset: 0" << endl;

		//Down opening
		cout << "Does room open down? (y/n): ";
		tempBool = askForBool();
		roomFile << "OpensDown: " << tempBool << endl;
		if (tempBool)
		{
			cout << "Down door offset: ";
			roomFile << "DownOffset: " << askForInt() << endl;
		}
		else
			roomFile << "DownOffset: 0" << endl;

		//leftOpening
		cout << "Does room open left? (y/n): ";
		tempBool = askForBool();
		roomFile << "OpensLeft: " << tempBool << endl;
		if (tempBool)
		{
			cout << "Left door offset: ";
			roomFile << "LeftOffset: " << askForInt() << endl;
		}
		else
			roomFile << "LeftOffset: 0" << endl;
		
		//Right opening
		cout << "Does room open Right? (y/n): ";
		tempBool = askForBool();
		roomFile << "OpensRight: " << tempBool << endl;
		if (tempBool)
		{
			cout << "Right door offset: ";
			roomFile << "RightOffset: " << askForInt() << endl;
		}
		else
			roomFile << "RightOffset: 0" << endl;

		roomFile << endl << endl; // puts space between rooms in the file
		cout << "Create another room? (y/n): ";
		done = !askForBool();
		i++;
	} while (!done);
	roomFile.close();
}


//Reads the values from the room file and sets the values of the arrays acordingly
void readFromRooms()
{
	ifstream roomFile;
	roomFile.open(ROOM_FILE_NAME);

	bool done = roomFile.eof();
	int i = 0;
	while (!done)
	{
		roomFile.ignore(50, ' '); //RoomChar:
		roomFile >> roomChars[i]; 
		roomFile.ignore(50, ' '); //Width:
		roomFile >> width[i];
		roomFile.ignore(50, ' '); //Height:
		roomFile >> height[i];
		roomFile.ignore(50, ' '); //UpBool:
		roomFile >> upOpen[i];
		roomFile.ignore(50, ' '); //UpOffset:
		roomFile >> upOffset[i];
		roomFile.ignore(50, ' '); //DownBool:
		roomFile >> downOpen[i];
		roomFile.ignore(50, ' '); //DownOffset:
		roomFile >> downOffset[i];
		roomFile.ignore(50, ' '); //LeftBool:
		roomFile >> leftOpen[i];
		roomFile.ignore(50, ' '); //LeftOffset:
		roomFile >> leftOffset[i];
		roomFile.ignore(50, ' '); //RightBool:
		roomFile >> rightOpen[i];
		roomFile.ignore(50, ' '); //RightOffset:
		roomFile >> rightOffset[i];
		i++;
		done = roomFile.eof();
	}
	roomsInFile = i - 1;
	roomFile.close();
}


//sets all values of the array to the constant EMPTY_CHAR
void emptyMap(char mapArray[ROW_NUM][COL_NUM])
{
	for (int i = 0; i < ROW_NUM; i++)
	{
		for (int j = 0; j < COL_NUM; j++)
		{
			mapArray[i][j] = EMPTY_CHAR;
		}
	}
}


//checks if the room, given by roomIndex, can spawn in the given location[row, col] without overlaps in the mapArray
bool canRoomSpawn(const char mapArray[ROW_NUM][COL_NUM], int roomIndex, const int location[2])
{
	if (location[0] + height[roomIndex] > ROW_NUM || location[0] < 0)
		return false;
	if (location[1] + width[roomIndex] > COL_NUM || location[1] < 0)
		return false;

	for(int i=location[0]; i < (location[0] + height[roomIndex]); i++)
		for (int j = location[1]; j < (location[1] + width[roomIndex]); j++)
			if (mapArray[i][j] != EMPTY_CHAR)
				return false;
	return true;
}


//spawns the room at the given location (does not check if spaces are taken)
void spawnRoom(char mapArray[ROW_NUM][COL_NUM], const int roomIndex, const int location[2], int spawnDoor) //spawnDoor is the door that connects to another room room (0,1,2,3 : up, down, left, right)
{
	if (location[0] + height[roomIndex] > ROW_NUM)
	{
		cout << "ERROR: Room cannot spawn outside the map char array." << endl;
		return;
	}
	if (location[1] + width[roomIndex] > COL_NUM)
	{
		cout << "ERROR: Room cannot spawn outside the map char array." << endl;
		return;
	}

	for (int i = location[0]; i < (location[0] + height[roomIndex]); i++)
		for (int j = location[1]; j < (location[1] + width[roomIndex]); j++)
			mapArray[i][j] = roomChars[roomIndex];

	switch (spawnDoor)  //(0, 1, 2, 3 : up, down, left, right) -1 indicates no door should spawn
	{
		case -1:
			break;
		case 0:
			mapArray[location[0] - 1][location[1] + upOffset[roomIndex]] = DOOR_CHAR_VERT;
			break;
		case 1:
			mapArray[location[0] + height[roomIndex]][location[1] + downOffset[roomIndex]] = DOOR_CHAR_VERT;
			break;
		case 2:
			mapArray[location[0] + leftOffset[roomIndex]][location[1] - 1] = DOOR_CHAR_HORIZ;
			break;
		case 3:
			mapArray[location[0] + rightOffset[roomIndex]][location[1] + width[roomIndex]] = DOOR_CHAR_HORIZ;
			break;
		default:
			cout << "ERROR in spawnRoom: Invalid spawnDoor in the switch statement" << endl;
			break;
	}
}


//creates the map by choosing random rooms and stores it in mapArray
void createMap(char mapArray[ROW_NUM][COL_NUM])
{
	const int ROOM_LIMIT = 25;			// The max number of rooms in the map
	int spawnedLocations[ROOM_LIMIT][2];// The locations of every spawned room
	int spawnedRooms[ROOM_LIMIT];		// An array holding the index's of each spawned room
	int spawnedRoomCount = 0;			// The number of rooms already spawned in the map

	//This loops once for every attempt to spawn a room
	int loopCount = 0;
	int LOOP_LIMIT = 1500; //Max number of times the loop that spawns rooms may run
	while (spawnedRoomCount < ROOM_LIMIT && loopCount < LOOP_LIMIT)
	{
		int randomRoom = (rand() % roomsInFile);				// Random index of a room
		int newSpawnLocation[2] = { ROW_NUM / 2, COL_NUM / 2 };	// The location of the top-left corner of the room if it spawns
		int spawnDoor = -1;										// The door that will be connected (0, 1, 2, 3 : up ,down, left, right) (-1 indicates no doors will be generated)

			//This loops once for every room that has already been spawned to check if randomRoom can spawn at it. 
			//Sets the newSpawnLocation to match a compatable door if present
			for (int loopRoom = 0; loopRoom < spawnedRoomCount; loopRoom++)
			{
				bool compatableDoors[4] = { false }; //The sides of randomRoom with compatable doors to loopRoom. [up ,down, left, right]
				if (upOpen[randomRoom] && downOpen[spawnedRooms[loopRoom]]) 
					compatableDoors[0] = true;
				if (downOpen[randomRoom] && upOpen[spawnedRooms[loopRoom]])
					compatableDoors[1] = true;
				if (leftOpen[randomRoom] && rightOpen[spawnedRooms[loopRoom]])
					compatableDoors[2] = true;
				if (rightOpen[randomRoom] && leftOpen[spawnedRooms[loopRoom]])
					compatableDoors[3] = true;

				if (!(compatableDoors[0] || compatableDoors[1] || compatableDoors[2] || compatableDoors[3]))
					continue;
				//selects a random door from the compatable doors
				spawnDoor = 0;
				do
				{
					spawnDoor = rand() % 4;
				}while (!compatableDoors[spawnDoor]);

				//Sets the spawn location depending on which door is connected
				switch (spawnDoor) //(0, 1, 2, 3 : up ,down, left, right)
				{
					case 0:
						newSpawnLocation[0] = spawnedLocations[loopRoom][0] + height[spawnedRooms[loopRoom]] + 1;
						newSpawnLocation[1] = spawnedLocations[loopRoom][1] + (downOffset[spawnedRooms[loopRoom]] - upOffset[randomRoom]);
						break;
					case 1:
						newSpawnLocation[0] = spawnedLocations[loopRoom][0] - height[randomRoom] - 1;
						newSpawnLocation[1] = spawnedLocations[loopRoom][1] + (upOffset[spawnedRooms[loopRoom]] - downOffset[randomRoom]);
						break;
					case 2:
						newSpawnLocation[0] = spawnedLocations[loopRoom][0] + (-leftOffset[randomRoom] + rightOffset[spawnedRooms[loopRoom]]);
						newSpawnLocation[1] = spawnedLocations[loopRoom][1] + width[spawnedRooms[loopRoom]] + 1;
						break;
					case 3:
						newSpawnLocation[0] = spawnedLocations[loopRoom][0] + (-rightOffset[randomRoom] + leftOffset[spawnedRooms[loopRoom]]);
						newSpawnLocation[1] = spawnedLocations[loopRoom][1] - width[randomRoom] - 1;
						break;
					default:
						cout << "ERROR in createMap: spawnDoor is an invalid value" << endl;
						break;
				}
			}

		if (canRoomSpawn(mapArray, randomRoom, newSpawnLocation))
		{
			spawnRoom(mapArray, randomRoom, newSpawnLocation, spawnDoor);
			spawnedRooms[spawnedRoomCount] = randomRoom;
			spawnedLocations[spawnedRoomCount][0] = newSpawnLocation[0];
			spawnedLocations[spawnedRoomCount][1] = newSpawnLocation[1];
			spawnedRoomCount++;
		}
		loopCount++;
	}
}


//displays the map
void displayMap(const char mapArray[ROW_NUM][COL_NUM])
{
	cout << endl;
	for (int i = 0; i < ROW_NUM; i++)
	{
		for (int j = 0; j < COL_NUM; j++)
		{
			cout << mapArray[i][j] << ' ';
		}
		cout << endl;
	}
}