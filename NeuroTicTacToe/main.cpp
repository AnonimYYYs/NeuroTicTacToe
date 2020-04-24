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
	cout.precision(17);

	/*-- xor example --*/
	vector<int> layers = { 2, 2, 1 };
	NeuroNet net = NeuroNet(layers);
	vector<SynapsLocation> synapses;
	synapses.push_back({ 0.987, 0, 0, 1, 0 });
	synapses.push_back({ 8.033, 0, 0, 1, 1 });
	synapses.push_back({ 0.987, 0, 1, 1, 0 });
	synapses.push_back({ 8.033, 0, 1, 1, 1 });
	synapses.push_back({ -90.284, 1, 0, 2, 0 });
	synapses.push_back({ 72.690, 1, 1, 2, 0 });
	net.add_synapses(synapses);

		/*-- xor input --*/
	vector<double> inVect = { 0, 0 };
	vector<double> outVect = net.work(inVect);
	for (int i = 0; i < outVect.size(); i++) {
		cout << outVect[i] << endl;
	}
	/*-- end example --*/

	system("pause");
	return(0);
}