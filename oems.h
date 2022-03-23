/**
 * @file oems.h
 * @brief      Odd-even merge sort - for 8 numbers (stored in binary file
 *             'numbers')
 * @author     Michal Sova (xsovam00@stud.fit.vutbr.cz)
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>

#define BYTE unsigned char 
#define COMMON_TAG 0

using namespace std;
string FILENAME = "numbers";
MPI_Status status;

/**
 * @brief      Prints a vector.
 *
 * @param[in]  v          vector to print
 * @param[in]  delimiter  The delimiter
 */
void print_vec(const vector<BYTE>& v, string delimiter);


/**
 * @brief      Gets the input from file.
 *
 * @return     The vector of numbers from input file.
 */
vector<BYTE> get_input();

/**
 * @brief      compares 2 numbers and returns them ordered, 
 *             implements 1x1 net
 *
 * @param[in]  in1   first number
 * @param[in]  in2   second number
 *
 * @return     ordered vector of size 2
 */
vector<BYTE> net_1x1(const BYTE in1,const BYTE in2);

/**
 * @brief      function implements 2x2 net, needs 3 processors in vector \p ranks
 *
 * @param[in]  in1    first ordered vector of size 2
 * @param[in]  in2    second ordered vector of size 2
 * @param[in]  rank   the rank of the processor calling the function
 * @param[in]  ranks  vector of processors (3) needed for implementation of 2x2
 *                    network
 *
 * @return     ordered vector of numbers (size 4) by third processor in \p ranks
 */
vector<BYTE> net_2x2(const vector<BYTE> &in1,const vector<BYTE> &in2, const int &rank, const vector<int> &ranks);

/**
 * @brief      implements 4x4 net, needs 10 processors in vector @p ranks
 *
 * @param[in]  in1    first ordered vector of size 4
 * @param[in]  in2    second ordered vector of size 4
 * @param[in]  rank   the rank of the processor calling the function
 * @param[in]  ranks  vector of processors (size 10)
 *
 * @return     ordered vector of numbers (size 8) by last (10th) processor in \p ranks
 */
vector<BYTE> net_4x4(const vector<BYTE> &in1,const vector<BYTE> &in2, const int &rank, const vector<int> &ranks);
