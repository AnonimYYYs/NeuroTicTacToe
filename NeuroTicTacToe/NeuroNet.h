#pragma once
#include <vector>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

class Synaps {

	// синапс. получает значение от одного нейрона, передает другому значение * вес этого синапса

private:
	string rawData; // инфо о контактах синапса формата ";12;4;1;6" - синапс от 12 нейрона 4 слоя к 4 нейрону 6 слоя
	double weight; // вес синапса 
	double into;
public:
	Synaps(double newWeight, string data) {
		rawData = data;
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

	string Info() {
		return(to_string(weight) + rawData);
	}
};

class InNeuron {
private:
	double gotValue;
	vector<Synaps*> outSynapses;
public:
	InNeuron() {};

	void DestroySynapses() {
		if (outSynapses.size() > 0) {
			Synaps *synAt;
			while (outSynapses.size() > 0) {
				synAt = outSynapses[outSynapses.size() - 1];
				outSynapses.pop_back();
				delete synAt;
			}
		}
	}

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

	vector<string> SynapsesData() {
		vector<string> data;
		for (int i = 0; i < outSynapses.size(); i++) {
			data.push_back(outSynapses[i]->Info());
		}
		return(data);
	}

	int SynAmount() {
		return(outSynapses.size());
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

	vector<string> SynapsesData() {
		vector<string> data;
		for (int i = 0; i < outSynapses.size(); i++) {
			data.push_back(outSynapses[i]->Info());
		}
		return(data);
	}

	void DestroySynapses() {
		if (outSynapses.size() > 0) {
			Synaps *synAt;
			while (outSynapses.size() > 0) {
				synAt = outSynapses[outSynapses.size() - 1];
				outSynapses.pop_back();
				delete synAt;
			}
		}
	}

	int SynAmount() {
		return(outSynapses.size());
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
	string filePath;

	int SynAmount() {
		int synapsesAmount = 0;
		for (int i = 0; i < inputLayer.size(); i++) {
			synapsesAmount += inputLayer[i].SynAmount();
		}
		for (int j = 0; j < hidLayer.size(); j++) {
			for (int i = 0; i < hidLayer[j].size(); i++) {
				synapsesAmount += hidLayer[j][i].SynAmount();
			}
		}
		return(synapsesAmount);
	}

public:
	NeuroNet(string path) {
		// создание нейросети на основе данных из файла
		filePath = path;
		ifstream file(filePath);
		file.precision(17);

		int layersAmount, synapsAmount, neuroAmount;

		/*
		считывание инфо о сети с файла
		первая строка - количество слоев (layersAmount)
		потом layersAmount строк с количеством нейронов на каждой
		потом количество синапос (synapsAmount)
		потом synapsAmount строк с инфо о каждом синапсе
		*/
		file >> layersAmount;
		

		// входной слой
		file >> neuroAmount;
		for (int i = 0; i < neuroAmount; i++) {
			InNeuron iN = InNeuron();
			inputLayer.push_back(iN);
		}

		// скрытые слои
		for (int j = 0; j < (layersAmount - 2); j++) {
			file >> neuroAmount;
			vector<HidNeuron> layer;
			hidLayer.push_back(layer);
			for (int i = 0; i < neuroAmount; i++) {
				HidNeuron hN = HidNeuron();
				hidLayer[j].push_back(hN);
			}
		}

		// выходной слой
		file >> neuroAmount;
		for (int i = 0; i < neuroAmount; i++) {
			OutNeuron oN = OutNeuron();
			outLayer.push_back(oN);
		}

		// ввод синапсов
		// формат *вес;слой откуда;нейрон откуда;слой куда;нейрон куда*
		file >> synapsAmount;
		for (int i = 0; i < synapsAmount; i++) {
			string synapsRawData;
			int layerFrom, neuroFrom, layerTo, neuroTo;
			file >> synapsRawData;
			// создание синапса
			Synaps *synaps = new Synaps(stod(synapsRawData.substr(0, synapsRawData.find(";"))), synapsRawData.substr(synapsRawData.find(";"), synapsRawData.size()));
			// определение нейронов которые он связывает
			synapsRawData = synapsRawData.substr(synapsRawData.find(";") + 1, synapsRawData.size());
			layerFrom = stoi(synapsRawData.substr(0, synapsRawData.find(";"))); // слой откуда
			synapsRawData = synapsRawData.substr(synapsRawData.find(";") + 1, synapsRawData.size());
			neuroFrom = stoi(synapsRawData.substr(0, synapsRawData.find(";"))); // нейрон откуда
			synapsRawData = synapsRawData.substr(synapsRawData.find(";") + 1, synapsRawData.size());
			layerTo = stoi(synapsRawData.substr(0, synapsRawData.find(";"))); // слой куда
			synapsRawData = synapsRawData.substr(synapsRawData.find(";") + 1, synapsRawData.size());
			neuroTo = stoi(synapsRawData); // нейрон куда
			
			// откуда выходит
			if (layerFrom == 0) {
				inputLayer[neuroFrom].AddOutSynaps(synaps);
			}
			else {
				hidLayer[layerFrom - 1][neuroFrom].AddOutSynaps(synaps);
			}

			// куда входит
			if (layerTo == hidLayer.size() + 1) {
				outLayer[neuroTo].AddIntoSynaps(synaps);
			}
			else {
				hidLayer[layerTo - 1][neuroTo].AddIntoSynaps(synaps);
			}

		}

		file.close();

	}

	NeuroNet(string path, vector<int> layers) {
		// создание нейросети по количеству слоев и нейронов на каждом, все нейроны Н слоя соединены со всеми нейронами Н+1 слоя
		filePath = path;
		
		// создаются нейроны
		for (int i = 0; i < layers[0]; i++) {
			InNeuron iN = InNeuron();
			inputLayer.push_back(iN);
		}

		for (int j = 0; j < layers.size() - 2; j++) {
			vector<HidNeuron> layer;
			hidLayer.push_back(layer);
			for (int i = 0; i < layers[j]; i++) {
				HidNeuron hN = HidNeuron();
				hidLayer[j].push_back(hN);
			}
		}

		for (int i = 0; i < layers[layers.size() - 1]; i++) {
			OutNeuron oN = OutNeuron();
			outLayer.push_back(oN);
		}
		
		// создаются синапсы, инициализированные единицами
		for (int i = 0; i < inputLayer.size(); i++) {
			//todo!!!
		}

	}

	~NeuroNet() {
		ofstream file(filePath);
		/*
		ввод обратно в файл
		первая строка - количество слоев (layersAmount)
		потом layersAmount строк с количеством нейронов на каждой
		потом количество синапос (synapsAmount)
		потом synapsAmount строк с инфо о каждом синапсе
		*/

		file << (hidLayer.size() + 2) << endl;
		file << inputLayer.size() << endl;
		for (int i = 0; i < hidLayer.size(); i++) {
			file << hidLayer[i].size() << endl;
		}
		file << outLayer.size() << endl;
		
		file << SynAmount() << endl;
		
		for (int i = 0; i < inputLayer.size(); i++) {
			vector<string> synapsesData = inputLayer[i].SynapsesData();
			for (int k = 0; k < synapsesData.size(); k++) {
				file << synapsesData[k] << endl;
			}
		}
		
		for (int j = 0; j < hidLayer.size(); j++) {
			for (int i = 0; i < hidLayer[j].size(); i++) {
				vector<string> synapsesData = hidLayer[j][i].SynapsesData();
				for (int k = 0; k < synapsesData.size(); k++) {
					file << synapsesData[k] << endl;
				}
			}
		}

		file.close();
	}

	void DestroySynapses() {
		// удаление всех синапсов. использование NeuroNet(string, vector<int>); и после DestroySynapses(); позволяет создать сеть с заданными слоями но без связей
		Synaps *synAt;
		for (int i = 0; i < inputLayer.size(); i++) {
			inputLayer[i].DestroySynapses();
		}
		for (int j = 0; j < hidLayer.size(); j++) {
			for (int i = 0; i < hidLayer[j].size(); i++) {
				hidLayer[j][i].DestroySynapses();
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