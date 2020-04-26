#pragma once
#include <vector>
#include <fstream>
#include <math.h>
#include <random>

// пересобираю!

// скорость обучения
double E = 0.1;
// момент обучения
double A = 0.1;

// рандом, от 0.0 до 1.0
double random() {
	static const int RND_MAX = 100000;
	static default_random_engine generator;
	uniform_int_distribution<int> distribution(0, RND_MAX + 1);
	int roll = distribution(generator);
	double ret = roll / (double)RND_MAX;
	return(ret);
}

// рандом, от 0 до j
int random(int j) {
	static const int RND_MAX = 100000;
	static default_random_engine generator;
	uniform_int_distribution<int> distribution(0, RND_MAX + 1);
	int roll = distribution(generator);
	double ret = roll / (double)RND_MAX;
	int r = round(ret * j);
	return(r);
}

// местоположение синапса
struct SynapsLocation
{
	double weight;
	int lFrom, nFrom;
	int lTo, nTo;
};

// один сет данных для обучения
struct Set {
	vector<double> input;
	vector<double> output;
};

// класс нейронной сети
class NeuroNet {
private:

	// синапс
	class Synaps {
	private:
		double oldDeltaWeight;
		double delta; // дельта-ошибка
		double weight; // вес синапса
		double value; // полученное значение

	public:
		// пустой конструктор, вес = random()
		Synaps() {
			weight = random();
			oldDeltaWeight = 0; 
		}

		// конструктор с весом
		Synaps(double setWeight) {
			weight = setWeight;
			oldDeltaWeight = 0;
		}

		// ввод значения в синапс
		void input_value(double newValue) {
			value = newValue;
		}

		// вывод значения умноженного на вес
		double output_value() {
			return(value * weight);
		}
	
		// ввод дельта-ошибки в синапс
		void input_delta(double newDelta) {
			delta = newDelta;
		}

		// изменение веса
		void delta_weight() {
			double deltaWeight = E * delta * value + A * oldDeltaWeight;
			weight += deltaWeight;
			oldDeltaWeight = deltaWeight;
		}

		// вывод дельта-ошибки из синапса
		double output_delta() {
			return(delta * weight);
		}
	};

	// слой входных нейронов
	class InputLayer {
	private:

		// входной нейрон
		class InputNeuron {
		private:
			double value; // полученное значение
			vector<Synaps*> outSynapses; // исходящие синапсы

		public:
			// пустой конструктор
			InputNeuron() {};

			// получение значения на вход нейрона
			void input_value(double inputValue) {

				value = inputValue;
			}

			// вывод значения в выходящие синапсы
			void output_value() {
				for (int i = 0; i < outSynapses.size(); i++) {
					outSynapses[i]->input_value(value);
				}
			};

			// добавление исходящего синапса к нейрону
			void add_out_synaps(Synaps *outSynaps) {
				outSynapses.push_back(outSynaps);
			}
		
			// изменение весов исходящих нейронов
			void delta_weight() {
				for (int i = 0; i < outSynapses.size(); i++) {
					outSynapses[i]->delta_weight();
				}
			}
		};

		vector<InputNeuron*> layer; // данный слой

	public:
		// проход всех входных значений через входной слой 
		void get_through_layer(vector<double> inputValues) {
			/* проверка на совпадение размерностей */
			if (layer.size() != inputValues.size()) {
				cout << " Error: _Input_ size is not equial to _Input Layer_ size" << endl;
				system("pause>nul");
			}
			else {
				for (int i = 0; i < layer.size(); i++) {
					layer[i]->input_value(inputValues[i]);
					layer[i]->output_value();
				}
			}
		}

		// добавление исходящего синапса на слой
		void add_out_synaps(Synaps *outSynaps, int neuronNumber) {
			if (neuronNumber >= layer.size()) {
				cout << "Error: InputLayer: add_out_synaps: neuron number is incorrect" << endl;
				system("pause>nul");
			}
			else {
				layer[neuronNumber]->add_out_synaps(outSynaps);
			}
		}

		// добавление нейрона на слой
		void add_neuron() {
			InputNeuron *inNeuro = new InputNeuron();
			layer.push_back(inNeuro);
		}
	
		// размер слоя
		int size() {
			return(layer.size());
		}
	
		// изменение весов на всем слое исходящих нейронов
		void delta_weight() {
			for (int i = 0; i < layer.size(); i++) {
				layer[i]->delta_weight();
			}
		}
	};

	// слой скрытых нейронов
	class HiddenLayer {
	private:

		// скрытый нейрон
		class HiddenNeuron {
		private:
			double value; // полученное значение
			vector<Synaps*> inSynapses; // входящие синапсы
			vector<Synaps*> outSynapses; // исходящие синапсы

			// вычисление значения нейрона
			double count_value() {
				// сигмоид
				return(1 / (1 + exp(-value)));
			}

		public:
			// пустой конструктор
			HiddenNeuron() {};

			// ввод значения из входящих синапсов
			void input_value() {
				value = 0;
				for (int i = 0; i < inSynapses.size(); i++) {
					value += inSynapses[i]->output_value();
				}
			}

			// вывод значения в выходящие синапсы
			void output_value() {
				for (int i = 0; i < outSynapses.size(); i++) {
					double v = count_value();
					outSynapses[i]->input_value(v);
				}
			}

			// добавление входящего синапса нейрону
			void add_in_synaps(Synaps *inSynaps) {
				inSynapses.push_back(inSynaps);
			}

			// добавление исходящего синапса нейрону
			void add_out_synaps(Synaps *outSynaps) {
				outSynapses.push_back(outSynaps);
			}
			
			// получение дельта-ошибок с исходящих синапсов и отправка во входящие
			void throw_delta() {
				double delta = 0;
				for (int i = 0; i < outSynapses.size(); i++) {
					delta += outSynapses[i]->output_delta();
				}
				delta *= count_value() * (1 - count_value());
				for (int i = 0; i < inSynapses.size(); i++) {
					inSynapses[i]->input_delta(delta);
				}
			}
			
			// изменение весов исходящих нейронов
			void delta_weight() {
				for (int i = 0; i < outSynapses.size(); i++) {
					outSynapses[i]->delta_weight();
				}
			}
		};

		vector<HiddenNeuron*> layer; // данный слой

	public:
		// проход всех значений через скрытый слой
		void get_through_layer() {
			for (int i = 0; i < layer.size(); i++) {
				layer[i]->input_value();
				layer[i]->output_value();
			}
		}

		// добавление исходящего синапса на слой
		void add_out_synaps(Synaps *outSynaps, int neuronNumber) {
			if (neuronNumber >= layer.size()) {
				cout << "Error: HiddenLayer: add_out_synaps: neuron number is incorrect" << endl;
				system("pause>nul");
			}
			else {
				layer[neuronNumber]->add_out_synaps(outSynaps);
			}
		}

		// добавление входящего синапса на слой
		void add_in_synaps(Synaps *inSynaps, int neuronNumber) {
			if (neuronNumber >= layer.size()) {
				cout << "Error: HiddenLayer: add_in_synaps: neuron number is incorrect" << endl;
				system("pause>nul");
			}
			else {
				layer[neuronNumber]->add_in_synaps(inSynaps);
			}
		}

		// добавление нейрона на слой
		void add_neuron() {
			HiddenNeuron *hidNeuro = new HiddenNeuron();
			layer.push_back(hidNeuro);
		}
		
		// проход всех дельта-ошибок через слой
		void throw_delta() {
			for (int i = 0; i < layer.size(); i++) {
				layer[i]->throw_delta();
			}
		}
	
		// изменение весов на всем слое исходящих нейронов
		void delta_weight() {
			for (int i = 0; i < layer.size(); i++) {
				layer[i]->delta_weight();
			}
		}
	};

	// слой выходных нейронов
	class OutputLayer {
	
	private:

		// выходной нейрон
		class OutputNeuron {
		private:
			double value; // полученное значение
			vector<Synaps*> inSynapses; // входящие синапсы

			// вычисление значения нейрона
			double count_value() {
				/* сигмоид */
				return(1 / (1 + exp(-value)));
			}

		public:
			// пустой конструктор
			OutputNeuron() {};

			// ввод значения из входящих синапсов
			void input_value() {
				value = 0;
				for (int i = 0; i < inSynapses.size(); i++) {
					value += inSynapses[i]->output_value();
				}
			}

			// возврат посчитанного значения
			double output_value() {
				return(count_value());
			}

			// добавление входящего синапса нейрону
			void add_in_synaps(Synaps *inSynaps) {
				inSynapses.push_back(inSynaps);
			}

			// вычисление дельта-ошибки и отправка в синапсы
			void throw_delta(double ideal) {
				double output = output_value();
				double deriative = output * (1 - output);
				double delta = (ideal - output) * deriative;
				for (int i = 0; i < inSynapses.size(); i++) {
					inSynapses[i]->input_delta(delta);
				}
			}
		};

		vector<OutputNeuron*> layer; // данный слой

	public:
		// проход всех значений через выходной слой и их возврат
		vector<double> get_through_layer() {
			vector<double> values;
			for (int i = 0; i < layer.size(); i++) {
				layer[i]->input_value();
				values.push_back(layer[i]->output_value());
			}
			return(values);
		}

		// добавление входящего синапса на слой
		void add_in_synaps(Synaps *inSynaps, int neuronNumber) {
			if (neuronNumber >= layer.size()) {
				cout << "Error: HiddenLayer: add_in_synaps: neuron number is incorrect" << endl;
				system("pause>nul");
			}
			else {
				layer[neuronNumber]->add_in_synaps(inSynaps);
			}
		}

		// добавление нейрона на слой
		void add_neuron() {
			OutputNeuron *outNeuro = new OutputNeuron();
			layer.push_back(outNeuro);
		}

		// размер слоя
		int size() {
			return(layer.size());
		}
	
		// проход ошибки через слой
		void throw_delta(vector<double> ideal) {
			for (int i = 0; i < ideal.size(); i++) {
				layer[i]->throw_delta(ideal[i]);
			}
		}
	};

	InputLayer inputLayer;
	vector<HiddenLayer> hiddenLayer;
	OutputLayer outputLayer;
	int layersAmount;

public:
	// конструктор сети по количеству нейронов на слое
	NeuroNet(vector<int> layers) {
		if (layers.size() <= 2) {
			cout << "Error: NeuroNet constructor: invalid amount of layers" << endl;
		}
		else {
			layersAmount = layers.size();
			int i = 0;
			for (i = 0; i < layers[0]; i++) {
				inputLayer.add_neuron();
			}
			for (i = 1; i < layers.size() - 1; i++) {
				HiddenLayer hLayer = HiddenLayer();
				for (int j = 0; j < layers[i]; j++) {
					hLayer.add_neuron();
				}
				hiddenLayer.push_back(hLayer);
			}
			for (i = 0; i < layers[layers.size() - 1]; i++) {
				outputLayer.add_neuron();
			}
		}
	}

	// добавление синапсов
	void add_synapses(vector<SynapsLocation> synapsLocation) {
		for (int m = 0; m < synapsLocation.size(); m++) {
			Synaps *syn = new Synaps(synapsLocation[m].weight);
			if (synapsLocation[m].lFrom == 0) {
				inputLayer.add_out_synaps(syn, synapsLocation[m].nFrom);
			}
			else {
				hiddenLayer[synapsLocation[m].lFrom - 1].add_out_synaps(syn, synapsLocation[m].nFrom);
			}

			if (synapsLocation[m].lTo == (layersAmount - 1)) {
				outputLayer.add_in_synaps(syn, synapsLocation[m].nTo);
			}
			else {
				hiddenLayer[synapsLocation[m].lTo - 1].add_in_synaps(syn, synapsLocation[m].nTo);
			}
		}
	}

	// проход значений по сети с возвратом результата
	vector<double> work(vector<double> inputValues) {
		inputLayer.get_through_layer(inputValues);
		for (int i = 0; i < hiddenLayer.size(); i++) {
			hiddenLayer[i].get_through_layer();
		}
		return(outputLayer.get_through_layer());
	}

	// обучение с учителем
	void teacher_learn(vector<Set> teachingSet, int iterations) {
		for (int i = 0; i < teachingSet.size(); i++) {
			if (teachingSet[i].input.size() != inputLayer.size()) {
				cout << "Error: Teaching: " << i + 1 << " set: Input size is not equial to input layer size" << endl;
				system("pause>nul");
				return;
			}
			if (teachingSet[i].output.size() != outputLayer.size()) {
				cout << "Error: Teaching: " << i + 1 << " set: Output size is not equial to output layer size" << endl;
				system("pause>nul");
				return;
			}
		}
				
		for (int iter = 0; iter < iterations; iter++) {

			cout << iter + 1 << " ITERATION" << endl; //comment
			double iterationMistake = 0;
			double iterationMAmount = 0;

			for (int m = 0; m < teachingSet.size(); m++) {
				double setMistake = 0;
				double setMAmount = 0;

				cout << "  Set " << m + 1 << endl; //comment
				cout << "  Input is: "; //comment
				for (int i = 0; i < teachingSet[m].input.size(); i++) {
					cout << teachingSet[m].input[i] << "; ";
				}
				cout << endl; //comment

				vector<double> currentOutput = work(teachingSet[m].input);

				cout << "  Output is: "; //comment
				for (int i = 0; i < currentOutput.size(); i++) {

					cout << currentOutput[i] << "; ";
				}
				cout << endl; //comment
				cout << "  Ideal output is: "; //comment
				for (int i = 0; i < currentOutput.size(); i++) {
					cout << teachingSet[m].output[i] << "; ";
					setMAmount++;
					setMistake += pow(teachingSet[m].output[i] - currentOutput[i], 2);
				}
				cout << endl; //comment
				cout << "  Set mistake is: " << sqrt(setMistake / setMAmount) << endl; // comment

				iterationMistake += setMistake;
				iterationMAmount += setMAmount;

				outputLayer.throw_delta(teachingSet[m].output);
				for (int i = hiddenLayer.size() - 1; i >= 0; i--) {
					hiddenLayer[i].delta_weight();
					hiddenLayer[i].throw_delta();
				}
				inputLayer.delta_weight();
			}
			cout << "Iteration mistake is: " << sqrt(iterationMistake / iterationMAmount) << endl << endl;
		}
	}
};