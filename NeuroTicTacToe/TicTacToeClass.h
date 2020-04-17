#pragma once
#include <iostream>
#include <string>
#include <vector>


using namespace std;

class TicTacToeClass
{
private:

	/*	0 1 2
		3 4 5
		6 7 8	*/
		/* ����� ����� ����� */
	vector<int> Field;
	int playerMove;

	/* ���� ������� */
	void Player1Move(int move) {
		Field[move] = 1;
	}
	void Player2Move(int move) {
		Field[move] = -1;
	}

public:

	/* ������� ���� */
	TicTacToeClass() {
		int i, count = 0;
		for (i = 0; i < 9; i++) {
			Field.push_back(0);
		}
		playerMove = 1;
	};

	/* ������������ ��� ������� */
	void Move(int move) {
		if (playerMove == 1) {
			Player1Move(move);
		}
		else {
			Player2Move(move);
		}
		playerMove *= -1;
	}

	/* ���������� ���� � ������� ������� ����� */
	vector<int> GetField() {
		return(Field);
	}

	/* �������� �� ������ ������ �� ������� */
	int CheckEnd() {
		if (Field[0] == Field[1] && Field[1] == Field[2] && Field[1] != 0) {
			return(Field[1]);
		}
		if (Field[3] == Field[4] && Field[4] == Field[5] && Field[5] != 0) {
			return(Field[3]);
		}
		if (Field[6] == Field[7] && Field[7] == Field[8] && Field[8] != 0) {
			return(Field[7]);
		}
		if (Field[0] == Field[3] && Field[3] == Field[6] && Field[6] != 0) {
			return(Field[6]);
		}
		if (Field[1] == Field[4] && Field[4] == Field[7] && Field[7] != 0) {
			return(Field[7]);
		}
		if (Field[2] == Field[5] && Field[5] == Field[8] && Field[8] != 0) {
			return(Field[8]);
		}
		if (Field[0] == Field[4] && Field[4] == Field[8] && Field[8] != 0) {
			return(Field[8]);
		}
		if (Field[6] == Field[4] && Field[4] == Field[2] && Field[2] != 0) {
			return(Field[2]);
		}
		return(0);
	}

	/* ����� ���� �� ����� ��� ������� */
	void ShowField() {
		system("cls");
		cout << "  " << Field[0] << "  " << Field[1] << "  " << Field[2] << endl;
		cout << "  " << Field[3] << "  " << Field[4] << "  " << Field[5] << endl;
		cout << "  " << Field[6] << "  " << Field[7] << "  " << Field[8] << endl;
		cout << "------------" << endl;
	}
};

