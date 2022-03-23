/********************** Odd-even merge sort ***************************/
/************ for 8 numbers (stored in binary file 'numbers') *********/
/**************************** PRL project *****************************/
/********** Michal Sova (xsovam00@stud.fit.vutbr.cz) ******************/
/**********************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>

#define BYTE unsigned char 
#define COMMON_TAG 0

using namespace std;
string FILENAME = "numbers";
MPI_Status status;

// TODO: describe! + add comments in oems.cpp file
void print_vec(const vector<BYTE>& v, string delimiter);
vector<BYTE> get_input(const int rank);
vector<BYTE> net_1x1(const BYTE in1,const BYTE in2);
vector<BYTE> net_2x2(const vector<BYTE> &in1,const vector<BYTE> &in2, const int &rank, const vector<int> &ranks);
vector<BYTE> net_4x4(const vector<BYTE> &in1,const vector<BYTE> &in2, const int &rank, const vector<int> &ranks);