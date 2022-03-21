#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>

#define BYTE unsigned char 

using namespace std;
string FILENAME = "numbers";
MPI_Status status;


void print_vec(const vector<BYTE>& v){
        for(auto &w: v){
                cout << (int) w << " ";
        }
        cout << endl;
}

vector<BYTE> get_input(const int rank) {
    // open the file:
    ifstream file(FILENAME, ifstream::binary);
    if (file) {
        // get length of file:
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        char * buffer = new char [length];
        file.read(buffer, length);

        vector<BYTE> in(buffer, buffer + length);
        delete[] buffer;
        /*
        if(rank == 0){
            cout << hex;
            print_vec(in);
            cout.unsetf(ios::hex);
        }*/

        return in;
    }
    return {};
}

vector<BYTE> net_1x1(const BYTE in1,const BYTE in2){
    if (in1 > in2) {
        return  vector<BYTE> {in2, in1};
    }
    return  vector<BYTE> {in1, in2};
}

vector<BYTE> net_2x2(const vector<BYTE> &in1,const vector<BYTE> &in2, const int &rank, const vector<int> &ranks){
    if (rank == ranks[0]){
        vector<BYTE> out1 = net_1x1(in1[0], in2[0]);
        MPI_Send(&out1[0], 2, MPI_UNSIGNED_CHAR, ranks[2], 0, MPI_COMM_WORLD);
    }
    if (rank == ranks[1]){
        vector<BYTE> out2 = net_1x1(in1[1], in2[1]);
        MPI_Send(&out2[0], 2, MPI_UNSIGNED_CHAR, ranks[2], 0, MPI_COMM_WORLD);
    }

    if (rank == ranks[2]){
        vector<BYTE> out;
        out.resize(4);
        MPI_Recv(&out[0], 2, MPI_UNSIGNED_CHAR, ranks[0], 0, MPI_COMM_WORLD, &status); 
        MPI_Recv(&out[2], 2, MPI_UNSIGNED_CHAR, ranks[1], 0, MPI_COMM_WORLD, &status);
        vector<BYTE> tmp = net_1x1(out[1], out[2]);
        out[1] = tmp[0];
        out[2] = tmp[1];
        return out;
    }
    return {};
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<int> ranks;
    vector<BYTE> in = get_input(rank);
    if (in.size() != 8) {
        if (rank == 0) {
            cerr << "wrong number count (" << in.size() << ") in file " << FILENAME << endl;

        }
        return 2;
    }


    if (rank == 0) {
        cout << "in: ";
        print_vec(in);
        cout << "\n";
    }
    /*
    vector<BYTE> in_2x2(in.begin(), in.begin() + 4);
    vector<BYTE> in1(in_2x2.begin(), in_2x2.begin() + 2);
    vector<BYTE> in2(in_2x2.begin() + 2, in_2x2.end());

    if (rank == 0){
    }
    if (0 <= rank && rank <= 2) {

        ranks.push_back(0);
        ranks.push_back(1);
        ranks.push_back(2);
        vector<BYTE> out_2x2 = net_2x2(in1, in2, rank, ranks);
        if (rank == 2){
            cout << "net_2x2 out: ";
            print_vec(out_2x2);
        }
        ranks.clear();

    }
    */
    
    MPI_Finalize();
}

