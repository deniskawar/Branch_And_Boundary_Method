// AK_Project.cpp: ���������� ����� ����� ��� ����������� ����������.
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
	int** matrixSmezh; // ������� ���������
	int** matrixDistance; // ������� ���������� ����� ���������
	int n; // ���������� ���������
public:
	void initializeMatrixSmezh() { // ������������� ������� ���������
		ifstream read("task.txt"); // �������� ������ ��� ������ �� �����
		if (!read.is_open()) cout << "Error! File can't be open!" << endl;
		read >> n;
		matrixSmezh = new int*[n]; 
		for (int i = 0; i < n; i++) { // ������������� ������� ��������� � n �������� � n ���������
			matrixSmezh[i] = new int[n];
		}
		for (int i = 0; i < n; i++) { // ���������� ������� ��������� ������� �� �����
			for (int j = 0; j < n; j++) {
				read >> matrixSmezh[i][j];
			}
		}
	}
	void printMatrixSmezh() { // ����� ������� ��������� � ����
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "������� ���������:" << endl;
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
		/* � ��� ���� i - ������ ������� � j - ������ �������
		��� ����� ����� ���� ���������� � �������������� ���������?
		i / 3 - ����� ������, ��� ��������� i-�� �������
		i % 3 - ����� �������, ��� ��������� i-�� �������
		���������� ����� i-�� � j-�� �������� � �������������� ��������� �����
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
		print << "������� ����������:" << endl;
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
	Data data; // ������� ��������� � ������� ����������
	int F0 = 0; // ������ ������� ������� �������
	int **X; // ������� ���������� ������ � ���� �������
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
		for (int i = 0; i < data.getN(); i++) { // �������������
			X[i] = new int[data.getN()];
		}
		for (int i = 0; i < data.getN(); i++) { // ��������� ������� � ������
			for (int j = 0; j < data.getN(); j++) {
				X[i][j] = 0;
			}
		}
	}
	void findF0() { // ���������� ������ �������
		int *cVector = new int[(data.getN()*data.getN()) / 2 + 1];
		int *dVector = new int[(data.getN()*data.getN()) / 2 + 1];
		int tmpIndex = 0; // ������ ��� ����������������� ��������� ��������� � ������� c � d
		for (int i = 0; i < data.getN(); i++) { // ��������� ������� ������� �� ��������������� ������� ������
			for (int j = 0; j < data.getN(); j++) {
				if (j > i) {
					cVector[tmpIndex] = data.getMatrixSmezh()[i][j];
					dVector[tmpIndex] = data.getMatrixDistance()[i][j];
					tmpIndex++;
				}
			}
		}
		for (int i = 0; i < tmpIndex; i++) { // ���������� ������� c �� ��������
			for (int j = 0; j < tmpIndex-1; j++) {
				if (cVector[j] < cVector[j + 1]) {
					int tmp = cVector[j];
					cVector[j] = cVector[j + 1];
					cVector[j + 1] = tmp;
				}
			}
		}
		for (int i = 0; i < tmpIndex; i++) { // ���������� ������� d �� �����������
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
		print << "������ ������� ������� �������: " << F0 << endl;
		print.close();
	}
	void branchesAndBoundariesMethod() {
		int *notRazmesh = new int[data.getN()]; // ������ ��� �� ����������� ���������
		int *razmesh = new int[data.getN()]; // ������ ��� ����������� ���������
		int *notZanyat = new int[data.getN()]; // ������ ��������� �������
		int *zanyat = new int[data.getN()]; // ������ ������� �������

		for (int i = 0; i < data.getN(); i++) { // ���������� �������� ��������� � ������� ���������� ����������
			/* 0 - ������� � ����� ������� ��� � ���� ������, 1 - ����
			����� �������, � ������ ������ ��� ������� �� ���������	*/
			razmesh[i] = 0;
			notRazmesh[i] = 1;
			zanyat[i] = 0;
			notZanyat[i] = 1;
		}

		for (int i = 0; i < data.getN(); i++) {
			razmesh[i] = 1; // �������� i-�� �������
			notRazmesh[i] = 0;
			int minFp = INT_MAX;
			int minFpPosition = 0;
			int *FpVector = new int[data.getN() - i];
			for (int k = 0; k < data.getN() - i; k++) FpVector[k] = -1;			

			for (int j = 0; j < data.getN(); j++) {
				if (zanyat[j] == 1) continue;
					printPushingElement(i, j);
					zanyat[j] = 1; // � j-�� �������
					notZanyat[j] = 0;
					X[j][i] = 1;

					int Fq = 0; // ��������� ����� ����� ������
					int Wp = 0; // ��������� ����� ����� ������ � ��������
					int Vp = 0; // ��������� ����� ����� ��������
					int Fp = 0;
					int **WpVector_C = new int*[data.getN()];
					int **WpVector_D = new int*[data.getN()];
					int *VpVector_� = new int[data.getN()*data.getN()];
					int *VpVector_D = new int[data.getN()*data.getN()];
					
					for (int s = 0; s < data.getN(); s++) {
						WpVector_C[s] = new int[data.getN()];
						WpVector_D[s] = new int[data.getN()];
					}					
					for (int k = 0; k < data.getN()*data.getN(); k++)
					{
						VpVector_�[k] = -1;
						VpVector_D[k] = -1;
					}
					for (int k = 0; k < data.getN(); k++) { // ��������� ������� ������� ����������� ��������� �������� -1
						for (int s = 0; s < data.getN(); s++) {
							WpVector_C[k][s] = -1;
							WpVector_D[k][s] = -1;
						}
					}
					

					// Fq 
					for (int k = 0; k < data.getN(); k++) {
						for (int s = 0; s < data.getN(); s++) {
							if ((s > k) && (razmesh[k] == 1) && (razmesh[s] == 1)) { // �� ��������, ������� ���������
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

					// ������� �� ��������� ����������� �������
					if (i == data.getN() - 1) {
						printEmpty();
					}
					// ������� �� ��������� ����������� �������

					else {						
						//Wp
						// ������������ ���� �������� Wp
						for (int k = 0; k <= i; k++) { // ���������� �������� w = i, ��� ��� i ��� ������. ���������
							for (int s = 0; s < data.getN(); s++) {
								if (razmesh[s] != 1) { // ����� ����������� ������� k � �������� ������� s
									WpVector_C[k][s] = data.getMatrixSmezh()[k][s]; // � ������ Wp_C ��������� ������ ������� ��������� ���� ���������
									int position_k = 0;
									for (int z = 0; z < data.getN(); z++) { // ���������� �� ������� X
										if (X[z][k] == 1) position_k = z;
										// ���� � ����� ������� �������� ������� k
									}
									for (int z = 0; z < data.getN(); z++) {
										if (zanyat[z] != 1) {
											WpVector_D[k][z] = data.getMatrixDistance()[position_k][z];
										}
									}
								}
							}
						}
						// ������������ ���� �������� Wp

						// ���������� ���� �������� Wp
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
						// ���������� ���� �������� Wp

						// ������� ����� Wp
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
						// ������� ����� Wp
						printWp(Wp, WpVector_C, WpVector_D);						
						//Wp

						//Vp
						//������������ Vp 
						int Vp_C_Index = 0;
						int Vp_D_Index = 0;
						for (int k = 0; k < data.getN(); k++) {
							if (razmesh[k] != 1) {
								for (int s = 0; s < data.getN(); s++) {
									if ((razmesh[s] != 1) && (s > k)) {
										VpVector_�[Vp_C_Index] = data.getMatrixSmezh()[k][s];
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
						//������������ Vp 

						//���������� Vp
						for (int k = 0; k < data.getN()*data.getN(); k++) {
							for (int s = 0; s < data.getN()*data.getN() - 1; s++) {
								if (VpVector_�[s] < VpVector_�[s + 1]) {
									int tmp = VpVector_�[s];
									VpVector_�[s] = VpVector_�[s + 1];
									VpVector_�[s + 1] = tmp;
								}
								if (VpVector_D[s] > VpVector_D[s + 1]) {
									int tmp = VpVector_D[s];
									VpVector_D[s] = VpVector_D[s + 1];
									VpVector_D[s + 1] = tmp;
								}
							}
						}
						//���������� Vp

						//������� ����� Vp
						int s = 0;
						while (VpVector_D[s] == -1) s++;
						for (int k = 0; k < data.getN()*data.getN(); k++) {
							if (VpVector_�[k] != -1 && VpVector_D[s] != -1) {
								Vp = Vp + VpVector_�[k] * VpVector_D[s];
								s++;
							}

						}
						//������� ����� Vp
						printVp(Vp, VpVector_�, VpVector_D);
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

					if (Fp < minFp) // ��������� ����������� Fp � �������
					{
						minFp = Fp;
						minFpPosition = j;
					}
					for (int k = 0; k < data.getN(); k++) { // ���������� ��� ������� � ���������� ��������� 
						for (int s = 0; s < data.getN(); s++) {
							WpVector_C[k][s] = -1;
							WpVector_D[k][s] = -1;
							VpVector_�[k] = -1;
							VpVector_D[k] = -1;
						}
					}
					X[j][i] = 0; // ������������ i-�� ������� �� j-�� �������
					zanyat[j] = 0; // ����������� j-�� ������� � ��������� � ���������� ����
					notZanyat[j] = 1;
				
			}
			X[minFpPosition][i] = 1; // ������ 1 � ������� X � ������������ � ����������� ��������
			zanyat[minFpPosition] = 1;
			notZanyat[minFpPosition] = 0;
			razmesh[i] = 1;
			notRazmesh[i] = 0;
			printFinalInfo(i, minFp, minFpPosition); // �������� ������������� ����������
		}
		printXMatrix();
	}
	void printPushingElement(int i, int j) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "�������� " << i + 1 << "-� ������� �(�) " << j + 1 << "-� �������" << endl;
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
		print << "������� Wp_C � Wp_D �����" << endl;
		print << "������� Vp_C � Vp_D �����" << endl;
		print.close();
	}
	void printWp(int Wp, int** WpVector_C, int** WpVector_D) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		for (int k = 0; k < data.getN(); k++) {
			if ((WpVector_C[k][0] != -1) || (WpVector_D[k][0] != -1)) {
				print << "������ Wp_C" << k << ": ";
				for (int s = 0; s < data.getN(); s++) {
					if (WpVector_C[k][s] != -1) print << WpVector_C[k][s] << " ";
				}
				print << endl;
				print << "������ Wp_D" << k << ": ";
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
			print << "������ Vp_C: ";
			for (int k = 0; k < data.getN()*data.getN(); k++) {
				if (VpVector_C[k] != -1) {
					print << VpVector_C[k] << " ";
				}
			}
			print << endl;
			print << "������ Vp_D: ";
			for (int k = 0; k < data.getN()*data.getN(); k++) {
				if (VpVector_D[k] != -1) {
					print << VpVector_D[k] << " ";
				}
			}
			print << endl;
		}
		else { print << "������� Vp_C � Vp_D �����" << endl; }
		print << "Vp = " << Vp << endl;
		print.close();
	}
	void printFp(int Fp) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "������� ������� ��� ������� ������ Fp = " << Fp << endl;
	}
	void printFinalInfo(int i, int minFp, int minFpPosition) {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "����� �������, ����������� ������� ������� �� ������ ���� ����� " << minFp << endl;
		print << "�������������, ������������ ������������� " << i + 1 << "-� ������� � " << minFpPosition + 1 << "-� �������" << endl << endl;	
		print.close();
	}
	void printXMatrix() {
		ofstream print("result.txt", ios_base::app);
		if (!print.is_open()) print << "Error! File can't be open!" << endl;
		print << "������� X:" << endl;
		for (int i = 0; i < data.getN(); i++) {
			for (int j = 0; j < data.getN(); j++) {
				print << X[i][j] << " ";
			}
			print << endl;
		}
		print.close();
	}
	void mainAlgorythm() {
		findF0(); // �������� ������ ������� ������� �������
		printF0(); // ������� �� � ����
		branchesAndBoundariesMethod();
	}
};

int main()
{
	setlocale(LC_ALL, "rus");
	ofstream print("result.txt", ios_base::trunc); // ��� ����, ����� ������� ���� � ������ �����
	if (!print.is_open()) cout << "Error! File can't be open!" << endl;
	print.close();
	unsigned int start_time = clock(); // ��������� �����
	Algorythm alg;
	alg.mainAlgorythm();
	unsigned int end_time = clock(); // �������� �����
	unsigned int search_time = end_time - start_time; // ������� �����
	cout << search_time/ (double) 1000 << endl;
	system("pause");
    return 0;
}

