# NeuroTicTacToe
Trying to make neural network which would play TicTacToe; making kind of neural network lib.


!!! No checing of exeptions or correct input !!!


class NeuralNetwork - class of neural network, work with .txt file to save structure and weights between launches of program.
Can be created with existing correct description of neural network in .txt "NeuroNet(string path)"
Or created empty, based only on amount of neurons on each layer "NeuroNet(string path, vector<int> layers)"


Function "vector<double> Work(vector<double> inVector)" 
  Makes neuronet object makes calculations and return result of already thought (if so) neuronet.

Function "double TeacherLearning(vector<vector<double>> inVector, vector<vector<double>> rightAnswers)" 
  Get arrays of input values and ideal answers on them (one set).
  Depending on mistake (ideal - current), weights are changing.
  To learn - make N iterations of learning sets (loop N times this funct in main() or where you need)
  N can be near 1'000'000'000, or depending on mistake "double Mistake()"
  Speed and correct of learning can be changed by changing Epsilon and Alpha
  ( can find them by ctrl+f "ChangeHere" comment )
  

Guides that help me with making neural networks:
  https://habr.com/ru/post/312450/ (ru)
  https://habr.com/ru/post/313216/ (ru)
