#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "TicTacToeClass.h"
#include "NeuroNet.h"

using namespace std;


/*
int main() {

//-------------------------------------------------------------------------------------------
//игра в кресики - нолики, ходы вводятся в консоли (поле описано в заголовке TicTacToeClass)
//-------------------------------------------------------------------------------------------

	 //Ввод корректного хода с консоли 
	int GetMoveCons(vector<int> field) {
		int x;
		do {
			cin >> x;
		} while (x < 0 || x > 8 || field[x] != 0);
		return(x);
	}	

	int GetMoveNeuro();
	
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
	
	vector<double> inVect = { 1,1 };
	vector<double> outVect;
	vector<int> layers = { 2, 3, 2, 1 };

	string path = "test.txt";
	
	NeuroNet net = NeuroNet(path, layers);
	outVect = net.Work(inVect);
	

	cout.precision(17);

	for (int i = 0; i < outVect.size(); i++) {
		cout << outVect[i] << endl;
	}
	

	system("pause");
	return(0);
}
