#pragma once
#include <vector>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

class Synaps {
private:
	double weight;
	double into;
public:
	Synaps(double newWeight) {
		weight = newWeight;
	}

	void SetWeight(double newWeight) {
		weight = newWeight;
	}

	void Get(double gotten) {
		into = gotten;
	}

	double Give() {
		return(into * weight);
	}
};

class InNeuron {
private:
	double gotValue;
	vector<Synaps*> outSynapses;

public:
	InNeuron() {};

	void AddOutSynaps(Synaps* outSynaps) {
		outSynapses.push_back(outSynaps);
	}

	void SetInput(double newInput) {
		gotValue = newInput;
	}

	void ThrowResult() {
		for (int i = 0; i < outSynapses.size(); i++) {
			outSynapses[i]->Get(gotValue);
		}
	}

};

class HidNeuron {
private:
	double gotValue;
	vector<Synaps*> intoSynapses;
	vector<Synaps*> outSynapses;

public:
	HidNeuron() {}

	void AddIntoSynaps(Synaps* intoSynaps) {
		intoSynapses.push_back(intoSynaps);
	}

	void AddOutSynaps(Synaps* outSynaps) {
		outSynapses.push_back(outSynaps);
	}

	void GetValue() {
		gotValue = 0;
		for (int i = 0; i < intoSynapses.size(); i++) {
			gotValue += intoSynapses[i]->Give();
		}
	}

	void ThrowValue() {
		//double result = (exp(2 * gotValue) - 1) / (exp(2 * gotValue) + 1);
		double result = 1 / (1 + exp(-gotValue));
		for (int i = 0; i < outSynapses.size(); i++) {
			outSynapses[i]->Get(result);
		}
	}
};

class OutNeuron {
private:
	double gotValue;
	vector<Synaps*> intoSynapses;

public:
	OutNeuron() {}

	void AddIntoSynaps(Synaps* intoSynaps) {
		intoSynapses.push_back(intoSynaps);
	}

	void GetValue() {
		gotValue = 0;
		for (int i = 0; i < intoSynapses.size(); i++) {
			gotValue += intoSynapses[i]->Give();
		}
	}

	double RetValue() {
		//double result = (exp(2 * gotValue) - 1) / (exp(2 * gotValue) + 1);
		double result = 1 / (1 + exp(-gotValue));
		return(result);
	}
};

class BiasNeuron {
private:
	vector<Synaps*> outSynapses;

public:
	BiasNeuron() {};

	void AddOutSynaps(Synaps* outSynaps) {
		outSynapses.push_back(outSynaps);
	}

	void throwResult() {
		for (int i = 0; i < outSynapses.size(); i++) {
			outSynapses[i]->Get(1);
		}
	}
};

class NeuroNet {
private:
	vector<InNeuron> inputLayer;
	vector<vector<HidNeuron>> hidLayer;
	vector<OutNeuron> outLayer;
	vector<BiasNeuron> biases;

public:
	NeuroNet(int num) {

		double gotten;
		int hiddens, neurons;
		string filePath = to_string(num) + ".txt";
		ifstream file(filePath);
		file.precision(17);

		/*
		1. колво скрытых слоев (z)
		2. колво входных нейронов
		3. z строк с количеством скрытых нейронов на слое
		4. колво выходных нейронов
		5. z+1 раз:
			5.1 колво исходящих синапсов на слое
			5.2 строка(вес синапса, выходной нейрон -> входной нейрон)

		*/

		// вводим количество (скрытых) слоев и создаем соответствующие нейроны
		file >> hiddens;
		for (int i = 0; i < hiddens; i++) {
			vector<HidNeuron> vect;
			hidLayer.push_back(vect);
		}

		// входной слой
		file >> neurons;
		for (int i = 0; i < neurons; i++) {
			InNeuron iN = InNeuron();
			inputLayer.push_back(iN);
		}

		// скрытые слои
		for (int j = 0; j < hiddens; j++) {
			file >> neurons;
			for (int i = 0; i < neurons; i++) {
				HidNeuron hN = HidNeuron();
				hidLayer[j].push_back(hN);
			}
		}

		// выходной слой
		file >> neurons;
		for (int i = 0; i < neurons; i++) {
			OutNeuron oN = OutNeuron();
			outLayer.push_back(oN);
		}

		// создание синапсов
		double synapses, tch, iWeight, iFrom, iTo;
		string synapsInfo;
		for (int j = 0; j < hiddens + 1; j++) {
			file >> synapses;

			for (int i = 0; i < synapses; i++) {
				file >> synapsInfo;
				iWeight = stod(synapsInfo.substr(0, synapsInfo.find(";")));
				iFrom = stod(synapsInfo.substr(synapsInfo.find(";") + 1, synapsInfo.rfind(";") - synapsInfo.find(";") - 1));
				iTo = stod(synapsInfo.substr(synapsInfo.rfind(";") + 1, synapsInfo.size()));
				Synaps *syn = new Synaps(iWeight);
				if (j == 0) {
					// входной слой -> первый скрытый слой
					inputLayer[iFrom].AddOutSynaps(syn);
					hidLayer[0][iTo].AddIntoSynaps(syn);
				}
				else if (j == hiddens) {
					// последний скрытый слой -> выходной слой
					hidLayer[hiddens - 1][iFrom].AddOutSynaps(syn);
					outLayer[iTo].AddIntoSynaps(syn);
				}
				else {
					// скрытый слой -> скрытый слой
					hidLayer[j - 1][iFrom].AddOutSynaps(syn);
					hidLayer[j][iTo].AddIntoSynaps(syn);
				}
			}
		}

	}

	vector<double> Work(vector<double> inVector) {

		vector<double> retVal;

		// проход по всем слоям сети с последовательными вычислениями значений

		for (int i = 0; i < inputLayer.size(); i++) {
			inputLayer[i].SetInput(inVector[i]);
			inputLayer[i].ThrowResult();
		}

		for (int j = 0; j < hidLayer.size(); j++) {
			for (int i = 0; i < hidLayer[j].size(); i++) {
				hidLayer[j][i].GetValue();
				hidLayer[j][i].ThrowValue();
			}
		}

		for (int i = 0; i < outLayer.size(); i++) {
			outLayer[i].GetValue();
			retVal.push_back(outLayer[i].RetValue());
		}

		return(retVal);
	}
};