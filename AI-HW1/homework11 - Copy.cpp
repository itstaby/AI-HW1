// AIHW1.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <list>
#include <stack>
#include <ctime>
#include <stdio.h>
#include <string.h>

using namespace std;

clock_t beginTime, endTime;

enum Implementation {
	DFS = 0, BFS = 1, SA = 2
};


class Nursery {
public:

	int boardSize;
	char** emptyNursery;
	char** currentNursery;
	int numberOfTargetLizards;
	Implementation implementation;

	bool* hasTreeRow;
	bool* hasTreeColumn;
	int* treeCountRow;
	int* treeCountColumn;

	list<int>* treeLocations;

	bool* hasLizardRow;
	bool* hasLizardColumn;

	Nursery() {

	}

	Nursery(char **board, int boardSize, int numberOfTargetLizards, Implementation implementation) {

		this->boardSize = boardSize;
		this->numberOfTargetLizards = numberOfTargetLizards;
		this->emptyNursery = board;
		this->implementation = implementation;

		//Create arrays of appropriate sizes.
		this->hasTreeRow = new bool[boardSize];
		this->hasTreeColumn = new bool[boardSize];
		this->hasLizardColumn = new bool[boardSize];
		this->hasLizardRow = new bool[boardSize];
		this->treeCountColumn = new int[boardSize];
		this->treeCountRow = new int[boardSize];
		this->treeLocations = new list<int>[boardSize];

		//Preparation Steps:
		//Create a new currentBoard array to hold the board that is currently being processed.
		//Populate the row first.
		this->currentNursery = new char*[boardSize];

		//Make sure the lizard value containers are false instead of garbage, which may be true or false and may cause a bug.
		for (int i = 0; i < boardSize; i++)
		{
			hasLizardRow[i] = false;
			hasLizardColumn[i] = false;
			hasTreeRow[i] = false;
			hasTreeColumn[i] = false;
			treeCountColumn[i] = 0;
			treeCountRow[i] = 0;
		}
		//Loops to traverse the fatherBoard.
		for (int i = 0; i < boardSize; i++)
		{
			//Populate columns.
			currentNursery[i] = new char[boardSize];

			for (int j = 0; j < boardSize; j++)
			{
				//If current position has a tree, mark the row and column to have trees
				if (emptyNursery[i][j] == '2') {
					hasTreeRow[i] = true;
					hasTreeColumn[j] = true;
					treeCountRow[i]++;
					treeLocations[i].push_back(j);
				}
			}
		}
	}

	void printEmptyNursery() {
		cout << "Number of rows/columns " << boardSize << endl;
		cout << "Number of number of target lizards " << boardSize << endl;
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				cout << emptyNursery[i][j];
			}
			cout << " Tree count: " << treeCountRow[i] << endl;
		}
	}

	void printCurrentNursery() {
		/*cout << "Number of rows/columns " << boardSize << endl;
		cout << "Number of number of target lizards " << boardSize << endl;*/
		cout << endl;
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				cout << currentNursery[i][j];
			}
			cout << endl;
		}
	}void printCurrentNurseryToFile() {
		/*cout << "Number of rows/columns " << boardSize << endl;
		cout << "Number of number of target lizards " << boardSize << endl;*/
		ofstream out("output.txt");
		out << "OK" << endl;
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				out << currentNursery[i][j];
			}
			out << endl;
		}
		out.close();
	}


	void mapLizardListOnCurrentBoard(list<int>lizardPositions[], int numberOfLizardsInState, int rowNumber) {
		//rowNumber++;
		for (int i = 0; i < boardSize; i++)
		{
			hasLizardRow[i] = false;
			for (int j = 0; j < boardSize; j++)
			{
				//Overwrite currentBoard with fatherBoard values. (Refresh it)
				currentNursery[i][j] = emptyNursery[i][j];

				hasLizardColumn[j] = false;
			}

			//Place lizards on the given row of the currentBoard.
			for (list<int>::iterator iter = lizardPositions[i].begin(); iter != lizardPositions[i].end(); iter++)
			{
				currentNursery[i][*iter] = '1';
				hasLizardRow[i] = true;
				hasLizardColumn[*iter] = true;
			}
		}
	}

	list<int>* shrinkCurrentBoardToLizardList(int numberOfRowsProcessed) {
		list<int>* lizardPositions = new list<int>[numberOfRowsProcessed];
		for (int i = 0; i < numberOfRowsProcessed; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				if (currentNursery[i][j] == '1')
					lizardPositions[i].push_back(j);
				hasLizardRow[i] = false;
				hasLizardColumn[j] = false;
			}
		}
		return lizardPositions;
	}

	bool checkValidityOfLizard(int x, int y) {

		if (x >= boardSize || y >= boardSize) {
			return false;
		}
		int checkX, checkY;
		if (currentNursery[x][y] == '1')
			return false;
		if (currentNursery[x][y] == '2')
			return false;
		if (!hasTreeRow[x] && hasLizardRow[x])
			return false;
		else if (!hasTreeColumn[y] && hasLizardColumn[y])
			return false;

		//Check up left diagonal
		checkX = x - 1, checkY = y - 1;
		while (checkX >= 0 && checkY >= 0) {
			if (currentNursery[checkX][checkY] == '1')
				return false;
			else if (currentNursery[checkX][checkY] == '2')
				break;
			checkX--, checkY--;
		}

		//Check up right diagonal
		checkX = x + 1, checkY = y - 1;
		while (checkX < boardSize && checkY >= 0) {
			if (currentNursery[checkX][checkY] == '1')
				return false;
			else if (currentNursery[checkX][checkY] == '2')
				break;
			checkX++; checkY--;
		}

		//Check down left diagonal
		checkX = x - 1, checkY = y + 1;
		while (checkX >= 0 && checkY < boardSize) {
			if (currentNursery[checkX][checkY] == '1')
				return false;
			else if (currentNursery[checkX][checkY] == '2')
				break;
			checkX--; checkY++;
		}

		//Check down right diagonal
		checkX = x + 1, checkY = y + 1;
		while (checkX < boardSize && checkY < boardSize) {
			if (currentNursery[checkX][checkY] == '1')
				return false;
			else if (currentNursery[checkX][checkY] == '2')
				break;
			checkX++; checkY++;
		}

		//Check Up
		checkX = x - 1, checkY = y;
		while (checkX >= 0) {
			if (currentNursery[checkX][checkY] == '1')
				return false;
			else if (currentNursery[checkX][checkY] == '2')
				break;
			checkX--;
		}
		//Check Down
		checkX = x + 1, checkY = y;
		while (checkX < boardSize) {
			if (currentNursery[checkX][checkY] == '1')
				return false;
			else if (currentNursery[checkX][checkY] == '2')
				break;
			checkX++;
		}
		//Check Left
		checkX = x, checkY = y - 1;
		while (checkY >= 0) {
			if (currentNursery[checkX][checkY] == '1')
				return false;
			else if (currentNursery[checkX][checkY] == '2')
				break;
			checkY--;
		}
		//Check Right
		checkX = x, checkY = y + 1;
		while (checkY < boardSize) {
			if (currentNursery[checkX][checkY] == '1')
				return false;
			else if (currentNursery[checkX][checkY] == '2')
				break;
			checkY++;
		}

		return true;
	}

	char getElementAtLocation(int x, int y) {
		if (x >= boardSize || y >= boardSize) {
			return ' ';
		}
		return emptyNursery[x][y];
	}

	int getNextTreeInRow(int rowNumber, int columnNumber)
	{
		if (rowNumber >= boardSize || columnNumber >= boardSize) {
			return false;
		}
		for (int j = columnNumber + 1; j < boardSize; j++)
		{
			if (emptyNursery[rowNumber][j] == '2')
				return j;
		}
		return -1;

	}
};


Nursery globalNursery;

class Solution;



class State {
public:
	int numberOfLizardsOnBoard = 0;
	//int numberOfRowsPopulated = 0;
	int nextRowToPopulate = 0;
	list<int>* lizardPositions;

	State(list<int> parentLizardPositions[], int nextRow, int numberOfLizards/*, int numberOfLizardsInState=0*/) {
		//numberOfRowsPopulated = numberOfRowsFilled;
		nextRowToPopulate = nextRow;
		numberOfLizardsOnBoard = numberOfLizards;
		/*if (numberOfRowsFilled > 0)
		{*/
		lizardPositions = new list<int>[globalNursery.boardSize];
		for (int i = 0; i < globalNursery.boardSize; i++)
		{
			lizardPositions[i] = parentLizardPositions[i];
			/*list<int>::iterator iter = parentLizardPositions[i].begin();
			while (iter!= parentLizardPositions[i].end()) {
			lizardPositions[i].
			iter++;
			}*/
		}
		//}
	}

	/*State(const State& stateToCopy) {

	}*/

	void addLizardToRow(int rowNumber, int columnNumber) {
		//*this;
		numberOfLizardsOnBoard++;
		lizardPositions[rowNumber].push_back(columnNumber);
		//if (lizardPositions[rowNumber].size() == 1) { numberOfRowsPopulated++; }

	}


	void createChildren(int rowNumber, int sectionToFill, int totalSectionsForRow, int startingColumn, State currentState);

};


queue<State> searchQueue;
//stack<State> searchStack;



class SearchDS {
public:
	virtual State pop() = 0;
	virtual void push(State) = 0;
	virtual double size() = 0;
};

SearchDS* searchDS;

class SearchQueue : public SearchDS {
private:
	queue<State> searchQueue;
public:
	virtual void push(State s) {
		searchQueue.push(s);
	}

	virtual State pop() {
		State topElement = searchQueue.front();
		searchQueue.pop();
		return topElement;
	}

	virtual double size() {
		return searchQueue.size();
	}
};

class SearchStack : public SearchDS {
private:
	stack<State> searchStack;
public:
	virtual void push(State s) {
		searchStack.push(s);
	}

	virtual State pop() {
		State topElement = searchStack.top();
		searchStack.pop();
		return topElement;
	}

	virtual double size() {
		return searchStack.size();
	}
};

void State::createChildren(int rowNumber, int sectionToFill, int totalSectionsForRow, int startingColumn, State currentState)
{
	//rowNumber = rowNumber - 1; //Because starts with 0

	//Base condition, check if all sections have been filled at least once.
	if (sectionToFill <= totalSectionsForRow) {

		int x = rowNumber, y = startingColumn;
		//Place lizard in each slot until you hit a tree.
		while (y < globalNursery.boardSize) {
			if (globalNursery.getElementAtLocation(x, y) == '2')
			{
				y++;
				sectionToFill++;
				continue;
			}

			globalNursery.mapLizardListOnCurrentBoard(currentState.lizardPositions, currentState.numberOfLizardsOnBoard, rowNumber);

			//If it's a valid placement
			if (globalNursery.checkValidityOfLizard(x, y)) {
				//Create a new state with this lizard placed.
				State newState(currentState.lizardPositions, currentState.nextRowToPopulate, currentState.numberOfLizardsOnBoard);
				newState.addLizardToRow(x, y);

				if (globalNursery.hasTreeRow[x])
				{
					//For this lizard placement, start placing lizards in the next section.
					int nextTreeColumn = globalNursery.getNextTreeInRow(x, startingColumn);
					createChildren(rowNumber, sectionToFill + 1, totalSectionsForRow, nextTreeColumn + 1, newState);
				}

				//When one complete set of sections is filled, add the starting state to queue.
				newState.nextRowToPopulate++;
				searchDS->push(newState);

			}
			y++;
		}
	}

	return;
}


//DFS Implementation

class DepthFS {
	stack<State> searchStack;
};

//BFS Implementation 
class Solution {

public:

	void printFailure() {
		cout << "\nPrinting Failure!\n";
		ofstream out("output.txt");
		out << "FAIL";
		out.close();
	}

	void searchSolution() {

		int numberOfRowsPopulated = 0;
		bool result = false;
		list<int>* firstList = new list<int>[globalNursery.boardSize];
		State currentNode((firstList), 0, 0);
		searchDS->push(currentNode);
		beginTime = clock();
		while (searchDS->size() > 0) {
			//Get the element at the top of the queue.
			currentNode = searchDS->pop();
			//Peform goal test on the popped element.
			if (goalTest(currentNode)) {
				result = true;
				break;
			}
			double countBefore = searchDS->size();
			currentNode.createChildren(currentNode.nextRowToPopulate, 1, globalNursery.treeCountRow[currentNode.nextRowToPopulate] + 1, 0, currentNode);
			double countAfter = searchDS->size();
			if (countBefore==countAfter) {
				currentNode.nextRowToPopulate++;
				if (currentNode.nextRowToPopulate<globalNursery.boardSize)
				{
					searchDS->push(currentNode);
				}
			}
		}
		if (result) {
			endTime = clock();
			double elapsed_secs = double(endTime - beginTime) / CLOCKS_PER_SEC;
			cout << "SOLUTION FOUND in " << elapsed_secs << "seconds!" << endl;
			globalNursery.mapLizardListOnCurrentBoard(currentNode.lizardPositions, currentNode.numberOfLizardsOnBoard, currentNode.nextRowToPopulate - 1);
			globalNursery.printCurrentNursery();

			globalNursery.mapLizardListOnCurrentBoard(currentNode.lizardPositions, currentNode.numberOfLizardsOnBoard, globalNursery.boardSize - 1);
			globalNursery.printCurrentNurseryToFile();
		}
		else {
			printFailure();
		}

	}

	bool goalTest(State currentState) {
		if (currentState.numberOfLizardsOnBoard == globalNursery.numberOfTargetLizards) {
			return true;
		}
		return false;
	}
};



Nursery parseFile(string fileName) {
	ifstream myfile;
	myfile.open(fileName);
	Implementation implementation;

	if (myfile.is_open())
	{
		char fileContent[4];
		myfile >> fileContent;
		if (strcmp(fileContent, "DFS") == 0)
		{
			implementation = DFS;
		}
		else if (strcmp(fileContent, "BFS") == 0)
		{
			implementation = BFS;
		}
		else if (strcmp(fileContent, "SA") == 0)
		{
			implementation = SA;
		}

		int boardSize;
		myfile >> boardSize;

		int numberOfTargetLizards;
		myfile >> numberOfTargetLizards;

		char** board = new char*[boardSize];
		for (int i = 0; i < boardSize; i++)
		{
			board[i] = new char[boardSize + 1];
			myfile >> board[i];
		}
		myfile.close();

		Nursery fatherBoard(board, boardSize, numberOfTargetLizards, implementation);

		return fatherBoard;
	}
	else {
		throw - 1;
	}
}



int main()
{
	globalNursery = parseFile("input.txt");

	globalNursery.printEmptyNursery();
	cout << endl;

	//State* goalState = NULL;
	Solution solution;
	if (globalNursery.implementation == BFS)
	{
		searchDS = new SearchQueue;
		solution.searchSolution();
	}
	else if (globalNursery.implementation == DFS) {
		searchDS = new SearchStack;
		solution.searchSolution();
	}
	else {
		cout << "\nThis is not supported yet!";
	}

	return 0;
}


