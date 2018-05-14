// AK_Project.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "conio.h"
#include <cstdlib>
#include <ctime>
using namespace std;

class Data {
private:
	int** matrixSmezh; // матрица смежности
	int** matrixDistance; // матрица расстояний между позициями
	int n; // количество элементов
public:
	void initializeMatrixSmezh() { // инициализация матрицы смежности
		ifstream read("task.txt"); // открытие потока для чтения из файла
		if (!read.is_open()) cout << "Error! File can't be open!" << endl;
		read >> n;
		matrixSmezh = new int*[n]; 
		for (int i = 0; i < n; i++) { // инициализация матрицы смежности с n строками и n столбцами
			matrixSmezh[i] = new int[n];
		}
		for (int i = 0; i < n; i++) { // заполнение матрицы смежности данными из файла
			for (int j = 0; j < n; j++) {
				read >> matrixSmezh[i][j];
			}
		}
	}
	void printMatrixSmezh() { // вывод матрицы смежности в файл
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "Матрица смежности:" << endl;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				print << matrixSmezh[i][j] << " ";
			}
			print << endl;
		}
		print.close();
	}
	void initializeMatrixDistance() {
		matrixDistance = new int*[n];
		for (int i = 0; i < n; i++) {
			matrixDistance[i] = new int[n];
		}
		/* у нас есть i - первая вершина и j - вторая вершина
		как найти между ними расстояние в коммутационной плоскости?
		i / 3 - номер строки, где находится i-ая вершина
		i % 3 - номер столбца, где находится i-ая вершина
		Расстояние между i-ой и j-ой вершиной в коммутационной плоскости равно
		abs(i / 3 - j / 3) + abs(i % 3 + j % 3)
	    */
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				matrixDistance[i][j] = abs(i / 3 - j / 3) + abs(i % 3 - j % 3);
			}
		}
	}
	void printMatrixDistance() {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "Матрица расстояний:" << endl;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				print << matrixDistance[i][j] << " ";
			}
			print << endl;
		}
		print.close();
	}
	int getN() {
		return n;
	}
	int** getMatrixSmezh() {
		return matrixSmezh;
	}
	int** getMatrixDistance() {
		return matrixDistance;
	}
};

class Algorythm {
private:
	Data data; // матрица смежности и матрица расстояний
	int F0 = 0; // нижняя граница целевой функции
	int **X; // матрица размещения вершин в узлы решетки
public:
	Algorythm() {
		data.initializeMatrixSmezh();
		data.printMatrixSmezh();
		data.initializeMatrixDistance();
		data.printMatrixDistance();
		initializeX();
	}
	void initializeX() {
		X = new int*[data.getN()];
		for (int i = 0; i < data.getN(); i++) { // инициализация
			X[i] = new int[data.getN()];
		}
		for (int i = 0; i < data.getN(); i++) { // заполняем матрицу Х нулями
			for (int j = 0; j < data.getN(); j++) {
				X[i][j] = 0;
			}
		}
	}
	void findF0() { // нахождение нижней границы
		int *cVector = new int[(data.getN()*data.getN()) / 2 + 1];
		int *dVector = new int[(data.getN()*data.getN()) / 2 + 1];
		int tmpIndex = 0; // индекс для последовательного помещения элементов в вектора c и d
		for (int i = 0; i < data.getN(); i++) { // заполняем вектора данными из соответствующих половин матриц
			for (int j = 0; j < data.getN(); j++) {
				if (j > i) {
					cVector[tmpIndex] = data.getMatrixSmezh()[i][j];
					dVector[tmpIndex] = data.getMatrixDistance()[i][j];
					tmpIndex++;
				}
			}
		}
		for (int i = 0; i < tmpIndex; i++) { // сортировка вектора c по убыванию
			for (int j = 0; j < tmpIndex-1; j++) {
				if (cVector[j] < cVector[j + 1]) {
					int tmp = cVector[j];
					cVector[j] = cVector[j + 1];
					cVector[j + 1] = tmp;
				}
			}
		}
		for (int i = 0; i < tmpIndex; i++) { // сортировка вектора d по возрастанию
			for (int j = 0; j < tmpIndex - 1; j++) {
				if (dVector[j] > dVector[j + 1]) {
					int tmp = dVector[j];
					dVector[j] = dVector[j + 1];
					dVector[j + 1] = tmp;
				}
			}
		}
		for (int i = 0; i < tmpIndex; i++) {
			F0 = F0 + cVector[i] * dVector[i];
		}
	}
	void printF0() {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "Нижняя граница целевой функции: " << F0 << endl;
		print.close();
	}
	void branchesAndBoundariesMethod() {
		int *notRazmesh = new int[data.getN()]; // массив еще не размещенных элементов
		int *razmesh = new int[data.getN()]; // массив уже размещенных элементов
		int *notZanyat = new int[data.getN()]; // массив незанятых позиций
		int *zanyat = new int[data.getN()]; // массив занятых позиций

		for (int i = 0; i < data.getN(); i++) { // заполнение массивов элементов и позиций начальными значениями
			/* 0 - вершины с таким номером нет в этом списке, 1 - есть
			Таким образом, с самого начала все вершины не размещены	*/
			razmesh[i] = 0;
			notRazmesh[i] = 1;
			zanyat[i] = 0;
			notZanyat[i] = 1;
		}

		for (int i = 0; i < data.getN(); i++) {
			razmesh[i] = 1; // помещаем i-ый элемент
			notRazmesh[i] = 0;
			int minFp = INT_MAX;
			int minFpPosition = 0;
			int *FpVector = new int[data.getN() - i];
			for (int k = 0; k < data.getN() - i; k++) FpVector[k] = -1;			

			for (int j = 0; j < data.getN(); j++) {
				if (zanyat[j] == 1) continue;
					printPushingElement(i, j);
					zanyat[j] = 1; // в j-ую позицию
					notZanyat[j] = 0;
					X[j][i] = 1;

					int Fq = 0; // суммарная длина между размещ
					int Wp = 0; // суммарная длина между размещ и неразмещ
					int Vp = 0; // суммарная длина между неразмещ
					int Fp = 0;
					int **WpVector_C = new int*[data.getN()];
					int **WpVector_D = new int*[data.getN()];
					int *VpVector_С = new int[data.getN()*data.getN()];
					int *VpVector_D = new int[data.getN()*data.getN()];
					
					for (int s = 0; s < data.getN(); s++) {
						WpVector_C[s] = new int[data.getN()];
						WpVector_D[s] = new int[data.getN()];
					}					
					for (int k = 0; k < data.getN()*data.getN(); k++)
					{
						VpVector_С[k] = -1;
						VpVector_D[k] = -1;
					}
					for (int k = 0; k < data.getN(); k++) { // элементам каждого вектора присваиваем начальные значения -1
						for (int s = 0; s < data.getN(); s++) {
							WpVector_C[k][s] = -1;
							WpVector_D[k][s] = -1;
						}
					}
					

					// Fq 
					for (int k = 0; k < data.getN(); k++) {
						for (int s = 0; s < data.getN(); s++) {
							if ((s > k) && (razmesh[k] == 1) && (razmesh[s] == 1)) { // те элементы, которые размещены
								int position_k = 0;
								int position_s = 0;
								for (int z = 0; z < data.getN(); z++) {
									if (X[z][k] == 1) position_k = z;
									if (X[z][s] == 1) position_s = z;
								}
								Fq = Fq + data.getMatrixSmezh()[k][s] * data.getMatrixDistance()[position_k][position_s];
							}
						}
					}
					printFq(Fq);
					// Fq

					// Условие на последний размещаемый элемент
					if (i == data.getN() - 1) {
						printEmpty();
					}
					// Условие на последний размещаемый элемент

					else {						
						//Wp
						// формирование всех векторов Wp
						for (int k = 0; k <= i; k++) { // количество векторов w = i, так как i уже размещ. элементов
							for (int s = 0; s < data.getN(); s++) {
								if (razmesh[s] != 1) { // берем размещенный элемент k и неразмещ элемент s
									WpVector_C[k][s] = data.getMatrixSmezh()[k][s]; // в вектор Wp_C добавляем ячейку матрицы смежности этих элементов
									int position_k = 0;
									for (int z = 0; z < data.getN(); z++) { // проходимся по матрице X
										if (X[z][k] == 1) position_k = z;
										// ищем в какой позиции размещен элемент k
									}
									for (int z = 0; z < data.getN(); z++) {
										if (zanyat[z] != 1) {
											WpVector_D[k][z] = data.getMatrixDistance()[position_k][z];
										}
									}
								}
							}
						}
						// формирование всех векторов Wp

						// сортировка всех векторов Wp
						for (int k = 0; k <= i; k++) {
							for (int s = 0; s < data.getN(); s++) {
								for (int z = 0; z < data.getN() - 1; z++) {
									if (WpVector_C[k][z] < WpVector_C[k][z + 1]) {
										int tmp = WpVector_C[k][z];
										WpVector_C[k][z] = WpVector_C[k][z + 1];
										WpVector_C[k][z + 1] = tmp;
									}
									if (WpVector_D[k][z] > WpVector_D[k][z + 1]) {
										int tmp = WpVector_D[k][z];
										WpVector_D[k][z] = WpVector_D[k][z + 1];
										WpVector_D[k][z + 1] = tmp;
									}
								}
							}
						}
						// сортировка всех векторов Wp

						// Подсчет числа Wp
						for (int k = 0; k < data.getN(); k++) {
							int z = 0;
							while (WpVector_D[k][z] == -1) {
								z++;
							}
							for (int s = 0; s < data.getN(); s++, z++) {
								if (WpVector_C[k][s] != -1 && WpVector_D[k][z] != -1) {
									Wp = Wp + WpVector_C[k][s] * WpVector_D[k][z];
								}
							}
						}
						// Подсчет числа Wp
						printWp(Wp, WpVector_C, WpVector_D);						
						//Wp

						//Vp
						//формирование Vp 
						int Vp_C_Index = 0;
						int Vp_D_Index = 0;
						for (int k = 0; k < data.getN(); k++) {
							if (razmesh[k] != 1) {
								for (int s = 0; s < data.getN(); s++) {
									if ((razmesh[s] != 1) && (s > k)) {
										VpVector_С[Vp_C_Index] = data.getMatrixSmezh()[k][s];
										Vp_C_Index++;
									}
								}
							}
							if (zanyat[k] != 1) {
								for (int s = 0; s < data.getN(); s++) {
									if ((zanyat[s] != 1) && (s > k)) {
										VpVector_D[Vp_D_Index] = data.getMatrixDistance()[k][s];
										Vp_D_Index++;
									}
								}
							}
						}
						//формирование Vp 

						//сортировка Vp
						for (int k = 0; k < data.getN()*data.getN(); k++) {
							for (int s = 0; s < data.getN()*data.getN() - 1; s++) {
								if (VpVector_С[s] < VpVector_С[s + 1]) {
									int tmp = VpVector_С[s];
									VpVector_С[s] = VpVector_С[s + 1];
									VpVector_С[s + 1] = tmp;
								}
								if (VpVector_D[s] > VpVector_D[s + 1]) {
									int tmp = VpVector_D[s];
									VpVector_D[s] = VpVector_D[s + 1];
									VpVector_D[s + 1] = tmp;
								}
							}
						}
						//сортировка Vp

						//подсчет числа Vp
						int s = 0;
						while (VpVector_D[s] == -1) s++;
						for (int k = 0; k < data.getN()*data.getN(); k++) {
							if (VpVector_С[k] != -1 && VpVector_D[s] != -1) {
								Vp = Vp + VpVector_С[k] * VpVector_D[s];
								s++;
							}

						}
						//подсчет числа Vp
						printVp(Vp, VpVector_С, VpVector_D);
						//Vp
					}

					//Fp
					Fp = Fq + Wp + Vp;
					printFp(Fp);
					//Fp

					//FpVector
					int l = 0;
					while (FpVector[l] != -1) l++;
					FpVector[l] = Fp;
					//FpVector

					if (Fp < minFp) // вычисляем минимальное Fp и позицию
					{
						minFp = Fp;
						minFpPosition = j;
					}
					for (int k = 0; k < data.getN(); k++) { // возвращаем все вектора к начальному состоянию 
						for (int s = 0; s < data.getN(); s++) {
							WpVector_C[k][s] = -1;
							WpVector_D[k][s] = -1;
							VpVector_С[k] = -1;
							VpVector_D[k] = -1;
						}
					}
					X[j][i] = 0; // осовобождаем i-ый элемент из j-ой позиции
					zanyat[j] = 0; // освобождаем j-ую позиций и переходим к следующему шагу
					notZanyat[j] = 1;
				
			}
			X[minFpPosition][i] = 1; // ставим 1 в матрицу X в соответствии с минимальной позицией
			zanyat[minFpPosition] = 1;
			notZanyat[minFpPosition] = 0;
			razmesh[i] = 1;
			notRazmesh[i] = 0;
			printFinalInfo(i, minFp, minFpPosition); // печатаем окончательную информацию
		}
		printXMatrix();
	}
	void printPushingElement(int i, int j) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "Помещаем " << i + 1 << "-й элемент в(о) " << j + 1 << "-ю позицию" << endl;
		print.close();
	}
	void printFq(int Fq) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "Fq = " << Fq << endl;
		print.close();
	}
	void printEmpty() {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "Векторы Wp_C и Wp_D пусты" << endl;
		print << "Векторы Vp_C и Vp_D пусты" << endl;
		print.close();
	}
	void printWp(int Wp, int** WpVector_C, int** WpVector_D) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		for (int k = 0; k < data.getN(); k++) {
			if ((WpVector_C[k][0] != -1) || (WpVector_D[k][0] != -1)) {
				print << "Вектор Wp_C" << k << ": ";
				for (int s = 0; s < data.getN(); s++) {
					if (WpVector_C[k][s] != -1) print << WpVector_C[k][s] << " ";
				}
				print << endl;
				print << "Вектор Wp_D" << k << ": ";
				for (int s = 0; s < data.getN(); s++) {
					if (WpVector_D[k][s] != -1) print << WpVector_D[k][s] << " ";
				}
				print << endl;
			}
		}
		print << "Wp = " << Wp << endl;
		print.close();
	}
	void printVp(int Vp, int* VpVector_C, int* VpVector_D) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		if (VpVector_C[0] != -1) {
			print << "Вектор Vp_C: ";
			for (int k = 0; k < data.getN()*data.getN(); k++) {
				if (VpVector_C[k] != -1) {
					print << VpVector_C[k] << " ";
				}
			}
			print << endl;
			print << "Вектор Vp_D: ";
			for (int k = 0; k < data.getN()*data.getN(); k++) {
				if (VpVector_D[k] != -1) {
					print << VpVector_D[k] << " ";
				}
			}
			print << endl;
		}
		else { print << "Векторы Vp_C и Vp_D пусты" << endl; }
		print << "Vp = " << Vp << endl;
		print.close();
	}
	void printFp(int Fp) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "Целевая функция для данного случая Fp = " << Fp << endl;
	}
	void printFinalInfo(int i, int minFp, int minFpPosition) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "Таким образом, минимальная целевая функция на данном шаге равна " << minFp << endl;
		print << "Следовательно, окончательно устанавливаем " << i + 1 << "-й элемент в " << minFpPosition + 1 << "-й позиции" << endl << endl;	
		print.close();
	}
	void printXMatrix() {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "Матрица X:" << endl;
		for (int i = 0; i < data.getN(); i++) {
			for (int j = 0; j < data.getN(); j++) {
				print << X[i][j] << " ";
			}
			print << endl;
		}
		print.close();
	}
	void mainAlgorythm() {
		findF0(); // нахождим нижнюю границу целевой функции
		printF0(); // выводим ее в файл
		branchesAndBoundariesMethod();
	}
};

int main()
{
	setlocale(LC_ALL, "rus");
	ofstream print("result.txt", ios_base::trunc); // для того, чтобы каретка была в начале файла
	if (!print.is_open()) cout << "Error! File can't be open!" << endl;
	print.close();
	unsigned int start_time = clock(); // начальное время
	Algorythm alg;
	alg.mainAlgorythm();
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время
	cout << search_time/ (double) 1000 << endl;
	system("pause");
    return 0;
}

