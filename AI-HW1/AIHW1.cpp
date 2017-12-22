// AIHW1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <list>
#include <stack>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <random>
#include <cfloat>

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
	int numberOfTrees;
	Implementation implementation;

	bool* hasTreeRow;
	bool* hasTreeColumn;
	int* treeCountRow;
	int* treeCountColumn;


	bool* hasLizardRow;
	bool* hasLizardColumn;

	Nursery() {

	}

	Nursery(char **board, int boardSize, int numberOfTargetLizards, Implementation implementation) {

		this->boardSize = boardSize;
		this->numberOfTargetLizards = numberOfTargetLizards;
		this->emptyNursery = board;
		this->implementation = implementation;
		this->numberOfTrees = 0;

		//Create arrays of appropriate sizes.
		this->hasTreeRow = new bool[boardSize];
		this->hasTreeColumn = new bool[boardSize];
		this->hasLizardColumn = new bool[boardSize];
		this->hasLizardRow = new bool[boardSize];
		this->treeCountColumn = new int[boardSize];
		this->treeCountRow = new int[boardSize];

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
					numberOfTrees++;
				}
			}
		}
	}

	void printEmptyNursery() {
		cout << "Number of rows/columns " << boardSize << endl;
		cout << "Number of number of target lizards " << numberOfTargetLizards << endl;
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
	int nextRowToPopulate = 0;
	list<int>* lizardPositions;

	State(list<int> parentLizardPositions[], int nextRow, int numberOfLizards) {
		nextRowToPopulate = nextRow;
		numberOfLizardsOnBoard = numberOfLizards;
		lizardPositions = new list<int>[globalNursery.boardSize];
		for (int i = 0; i < globalNursery.boardSize; i++)
		{
			lizardPositions[i] = parentLizardPositions[i];
		}
	}

	void addLizardToRow(int rowNumber, int columnNumber) {
		numberOfLizardsOnBoard++;
		lizardPositions[rowNumber].push_back(columnNumber);

	}


	void createChildren(int rowNumber, int sectionToFill, int totalSectionsForRow, int startingColumn, State currentState);

};


queue<State> searchQueue;



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

void printFailure() {
	//cout << "\nPrinting Failure!\n";
	ofstream out("output.txt");
	out << "FAIL";
	out.close();
}

//DFS Implementation

class DepthFS {
	stack<State> searchStack;
};

//BFS Implementation 
class Solution {

public:

	void searchSolution() {

		int numberOfRowsPopulated = 0;
		bool goalFound = false;
		list<int>* firstList = new list<int>[globalNursery.boardSize];
		State currentNode((firstList), 0, 0);
		searchDS->push(currentNode);
		beginTime = clock();
		while (searchDS->size() > 0) {
			//Get the element at the top of the queue.
			currentNode = searchDS->pop();
			//Peform goal test on the popped element.
			if (goalTest(currentNode)) {
				goalFound = true;
				break;
			}

			//Take size of the queue/stack before procesing the current node.
			double countBefore = searchDS->size();

			//Expand the current node by creating its children.
			currentNode.createChildren(currentNode.nextRowToPopulate, 1, globalNursery.treeCountRow[currentNode.nextRowToPopulate] + 1, 0, currentNode);

			//Take count of the queue/stack before procesing the current node.
			double countAfter = searchDS->size();

			//If there were no children generated for the current state by inserting lizards in the attempted row,
			//Re-add the node back to the stack by setting attempt row next row.
			if (countBefore == countAfter) {
				currentNode.nextRowToPopulate++;

				//Make sure the next attempt row is not over the bounds of the array.
				if (currentNode.nextRowToPopulate < globalNursery.boardSize)
				{
					searchDS->push(currentNode);
				}
			}
		}
		if (goalFound) {
			//If the goal found, get the time elapsed to get reached goal, and generate output file.
			endTime = clock();
			double elapsed_secs = double(endTime - beginTime) / CLOCKS_PER_SEC;
			cout << "SOLUTION FOUND in " << elapsed_secs << "seconds!" << endl;
			//globalNursery.mapLizardListOnCurrentBoard(currentNode.lizardPositions, currentNode.numberOfLizardsOnBoard, currentNode.nextRowToPopulate - 1);
			//globalNursery.printCurrentNursery();

			globalNursery.mapLizardListOnCurrentBoard(currentNode.lizardPositions, currentNode.numberOfLizardsOnBoard, globalNursery.boardSize - 1);
			globalNursery.printCurrentNurseryToFile();
		}
		else {
			//If goal not found and stack is empty, that means no solution existed.
			printFailure();
		}

	}

	bool goalTest(State currentState) {
		if (currentState.numberOfLizardsOnBoard == globalNursery.numberOfTargetLizards) {
			return true;
		}
		else
			return false;
	}
};


class SASolution {
public:
	double temperatureSchedule(double) {

	}
	void searchSolution() {
		State currentState = generateRandomState(); //Randomly started node
		globalNursery.mapLizardListOnCurrentBoard(currentState.lizardPositions, globalNursery.numberOfTargetLizards, globalNursery.boardSize);

		int conflictsOfCurrentState = calculateTotalConflicts(currentState);
		int conflictsOfNextState;
		list<int>* listPositions = new list<int>[globalNursery.boardSize];
		int result = true;
		double temperature = 297.0;
		beginTime = clock();
		bool solutionFound = false;

		while (conflictsOfCurrentState > 0 && temperature > 0) {
			//conflictsOfCurrentState = calculateTotalConflicts(currentState);
			temperature = updateTemperature();
			State newState = createNewStateFromCurrent(currentState);
			globalNursery.mapLizardListOnCurrentBoard(newState.lizardPositions, globalNursery.numberOfTargetLizards, globalNursery.boardSize);
			conflictsOfNextState = calculateTotalConflicts(newState);

			//globalNursery.mapLizardListOnCurrentBoard(newState.lizardPositions, globalNursery.numberOfTargetLizards, globalNursery.boardSize);
			//globalNursery.printCurrentNursery();
			//cout << conflictsOfNextState << endl;

			if (conflictsOfNextState < conflictsOfCurrentState) {
				currentState = newState;
				conflictsOfCurrentState = conflictsOfNextState;
			}
			else {
				int difference = conflictsOfNextState - conflictsOfCurrentState;
				if (acceptMove(temperature, difference)) {
					currentState = newState;
					conflictsOfCurrentState = conflictsOfNextState;
				}

			}
		}

		if (conflictsOfCurrentState == 0) {
			endTime = clock();
			double elapsed_secs = double(endTime - beginTime) / CLOCKS_PER_SEC;
			cout << "SOLUTION FOUND in " << elapsed_secs << "seconds!" << endl;
			/*globalNursery.mapLizardListOnCurrentBoard(currentState.lizardPositions, currentState.numberOfLizardsOnBoard, currentState.nextRowToPopulate - 1);
			globalNursery.printCurrentNursery();*/

			globalNursery.mapLizardListOnCurrentBoard(currentState.lizardPositions, currentState.numberOfLizardsOnBoard, globalNursery.boardSize - 1);
			globalNursery.printCurrentNurseryToFile();
		}
		else {

			printFailure();
			endTime = clock();
			double elapsed_secs = double(endTime - beginTime) / CLOCKS_PER_SEC;
			cout << "FAILURE FOUND in " << elapsed_secs << "seconds!" << endl;

		}

		/*globalNursery.mapLizardListOnCurrentBoard(currentState.lizardPositions, globalNursery.numberOfTargetLizards, globalNursery.boardSize);
		globalNursery.printCurrentNursery();*/
	}

	double updateTemperature() {
		clock_t nowTime = clock();
		double elapsed_secs = double(nowTime - beginTime) / CLOCKS_PER_SEC;
		return (297 - elapsed_secs) / 200;
	}
	double acceptMove(double temperature, double difference) {

		double  prob = 1 / exp((difference / temperature));

		random_device randomDevice;
		mt19937 twisterEngine(randomDevice());
		uniform_real_distribution<double> distributionForProb(0.0, 1.0);
		double random = distributionForProb(twisterEngine);
		if (random < prob) {
			return true;
		}
		else return false;
	}


	int calculateTotalConflicts(State currentNode) {
		int totalConflictCount = 0;
		for (int i = 0; i < globalNursery.boardSize; i++)
		{
			for (list<int>::iterator iter = currentNode.lizardPositions[i].begin(); iter != currentNode.lizardPositions[i].end(); iter++)
			{
				int x = i, y = *iter;
				totalConflictCount += calculateConflictsForPosition(x, y);
			}
		}
		return totalConflictCount;
	}

	int calculateConflictsForPosition(int x, int y) {
		int checkX, checkY;
		int conflictCount = 0;
		//Check up left diagonal
		checkX = x - 1, checkY = y - 1;
		while (checkX >= 0 && checkY >= 0) {
			if (globalNursery.currentNursery[checkX][checkY] == '1')
				conflictCount++;
			else if (globalNursery.currentNursery[checkX][checkY] == '2')
				break;
			checkX--, checkY--;
		}

		//Check down left diagonal
		checkX = x + 1, checkY = y - 1;
		while (checkX < globalNursery.boardSize && checkY >= 0) {
			if (globalNursery.currentNursery[checkX][checkY] == '1')
				conflictCount++;
			else if (globalNursery.currentNursery[checkX][checkY] == '2')
				break;
			checkX++; checkY--;
		}

		//Check up right diagonal
		checkX = x - 1, checkY = y + 1;
		while (checkX >= 0 && checkY < globalNursery.boardSize) {
			if (globalNursery.currentNursery[checkX][checkY] == '1')
				conflictCount++;
			else if (globalNursery.currentNursery[checkX][checkY] == '2')
				break;
			checkX--; checkY++;
		}

		//Check down right diagonal
		checkX = x + 1, checkY = y + 1;
		while (checkX < globalNursery.boardSize && checkY < globalNursery.boardSize) {
			if (globalNursery.currentNursery[checkX][checkY] == '1')
				conflictCount++;
			else if (globalNursery.currentNursery[checkX][checkY] == '2')
				break;
			checkX++; checkY++;
		}

		//Check Up
		checkX = x - 1, checkY = y;
		while (checkX >= 0) {
			if (globalNursery.currentNursery[checkX][checkY] == '1')
				conflictCount++;
			else if (globalNursery.currentNursery[checkX][checkY] == '2')
				break;
			checkX--;
		}
		//Check Down
		checkX = x + 1, checkY = y;
		while (checkX < globalNursery.boardSize) {
			if (globalNursery.currentNursery[checkX][checkY] == '1')
				conflictCount++;
			else if (globalNursery.currentNursery[checkX][checkY] == '2')
				break;
			checkX++;
		}
		//Check Left
		checkX = x, checkY = y - 1;
		while (checkY >= 0) {
			if (globalNursery.currentNursery[checkX][checkY] == '1')
				conflictCount++;
			else if (globalNursery.currentNursery[checkX][checkY] == '2')
				break;
			checkY--;
		}
		//Check Right
		checkX = x, checkY = y + 1;
		while (checkY < globalNursery.boardSize) {
			if (globalNursery.currentNursery[checkX][checkY] == '1')
				conflictCount++;
			else if (globalNursery.currentNursery[checkX][checkY] == '2')
				break;
			checkY++;
		}

		return conflictCount;
	}

	State createNewStateFromCurrent(State currentState) {
		int **pointList = new int*[globalNursery.numberOfTargetLizards];
		for (int i = 0; i < globalNursery.numberOfTargetLizards; i++)
		{
			pointList[i] = new int[2];
		}

		list<int[2]> positions;

		int lizardNumber = 0;
		for (int i = 0; i < globalNursery.boardSize; i++)
		{
			for (list<int>::iterator iter = currentState.lizardPositions[i].begin(); iter != currentState.lizardPositions[i].end(); iter++)
			{
				pointList[lizardNumber][0] = i;
				pointList[lizardNumber][1] = *iter;
				lizardNumber++;
			}
		}

		random_device randomDevice;
		mt19937 twisterEngine(randomDevice());
		uniform_real_distribution<double> distributionForLizard(0.0, nextafter(globalNursery.numberOfTargetLizards, DBL_MAX));
		uniform_real_distribution<double> distributionForPosition(0.0, nextafter(globalNursery.boardSize, DBL_MAX));

		//Which lizard to move:
		lizardNumber = distributionForLizard(twisterEngine);

		bool valid = false;

		int currentX = pointList[lizardNumber][0];
		int currentY = pointList[lizardNumber][1];

		//Make the position of that lizard 0 in current nursery.
		globalNursery.currentNursery[currentX][currentY] = '0';
		int newX, newY;

		//Find a valid position to where to place the new lizard at.
		//Keep trying until a valid position is found.
		while (!valid)
		{
			newX = distributionForPosition(twisterEngine);
			newY = distributionForPosition(twisterEngine);
			valid = globalNursery.currentNursery[newX][newY] != '1' && globalNursery.currentNursery[newX][newY] != '2';
		}

		//Remove lizard from that location in current state.
		currentState.lizardPositions[currentX].remove(currentY);
		//Update our lizard positions list to reflect new placement.
		currentState.lizardPositions[newX].push_back(newY);

		//globalNursery.mapLizardListOnCurrentBoard(currentState.lizardPositions, globalNursery.numberOfTargetLizards, globalNursery.boardSize);
		//globalNursery.printCurrentNursery();
		for (int i = 0; i<globalNursery.numberOfTargetLizards; i++)
			delete[] pointList[i];
		delete[] pointList;

		return currentState;
	}

	static State generateRandomState() {
		int numberOfLizardsOnBoard = 0;
		list<int>* lizardList = new list<int>[globalNursery.boardSize];
		globalNursery.mapLizardListOnCurrentBoard(lizardList, 0, 0);

		random_device randomDevice;
		mt19937 twisterEngine(randomDevice());
		uniform_real_distribution<double> distribution(0.0, nextafter(globalNursery.boardSize, DBL_MAX));

		while (numberOfLizardsOnBoard < globalNursery.numberOfTargetLizards) {
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(0, globalNursery.boardSize - 1);
			int x = distribution(twisterEngine);
			int y = distribution(twisterEngine);
			//cout << "Checking: " << x << " " << y << endl;
			if (globalNursery.currentNursery[x][y] != '1' && globalNursery.currentNursery[x][y] != '2') {
				//cout << "Inserted!\n";
				//cout << x << " " << y << endl;
				lizardList[x].push_back(y);
				numberOfLizardsOnBoard++;
				globalNursery.currentNursery[x][y] = '1';
			}
		}
		/*cout << "Random initial state:\n";
		globalNursery.mapLizardListOnCurrentBoard(lizardList, numberOfLizardsOnBoard, globalNursery.boardSize);
		globalNursery.printCurrentNursery();*/

		State randomState(lizardList, 0, globalNursery.numberOfTargetLizards);
		return randomState;

	}

	bool goalTest(State currentState) {
		if (currentState.numberOfLizardsOnBoard == globalNursery.numberOfTargetLizards) {
			return true;
		}
		else
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
	//globalNursery.printEmptyNursery();
	//cout << endl;
	printFailure();
	if (globalNursery.numberOfTrees == 0 && globalNursery.numberOfTargetLizards > globalNursery.boardSize)
	{
		printFailure();
	}
	else
	{
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
			SASolution solution;
			solution.searchSolution();
		}
	}
	return 0;
}


