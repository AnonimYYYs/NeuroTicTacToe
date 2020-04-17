#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "TicTacToeClass.h"
#include "NeuroNet.h"

using namespace std;


/* Ввод корректного хода с консоли */
int GetMoveCons(vector<int> field) {
	int x;
	do {
		cin >> x;
	} while (x < 0 || x > 8 || field[x] != 0);
	return(x);
}

int GetMoveNeuro();

/*
int main() {
	TicTacToeClass tte = TicTacToeClass();
	vector<int> field;

	/* Процесс игры *//*
	for (int i = 0; i < 9; i++) {
		tte.ShowField();
		tte.Move(GetMoveCons(tte.GetField()));
		if (tte.CheckEnd() != 0) {
			break;
		}
	}

	/* Вывод победителя *//*
	tte.ShowField();
	cout << "Won player " << tte.CheckEnd() << endl;
	system("pause");
	return(0);
}*/

int main() {

	int one = 1;

	cout.precision(17);

	// 0, 0 -> 0.000 151 162;		0.000 151 163
	// 0, 1 -> 0.998 983;			0.998 988 
	// 1, 0 -> 0.998 984;			0.998 988 
	// 1, 1 -> 0.001 377;			0.001 381

	vector<double> toTest = { 1, 1 };
	vector<double> getTest;

	NeuroNet xorNet = NeuroNet(one);

	getTest = xorNet.Work(toTest);

	for (int i = 0; i < getTest.size(); i++) {
		cout << getTest[i] << endl;
	}


	system("pause");
	return(0);
}