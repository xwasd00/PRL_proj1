/**
 * @file oems.cpp
 * @brief      Odd-even merge sort - for 8 numbers (stored in binary file
 *             'numbers')
 * @author     Michal Sova (xsovam00@stud.fit.vutbr.cz)
 */
#include "oems.h"

void print_vec(const vector<BYTE>& v, string delimiter){
    for(auto &w: v){
            cout << (int) w << delimiter;
    }
}

vector<BYTE> get_input(){
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
    // net 1x1 : first processor
    // selects from 'in1' and 'in2' first elements
    if (rank == ranks[0]){
        vector<BYTE> out1 = net_1x1(in1[0], in2[0]);
        MPI_Send(&out1[0], 2, MPI_UNSIGNED_CHAR, ranks[2], COMMON_TAG, MPI_COMM_WORLD);
    }

    // net 1x1 : second processor
    // selects from 'in1' and 'in2' second(last) elements
    if (rank == ranks[1]){
        vector<BYTE> out2 = net_1x1(in1[1], in2[1]);
        MPI_Send(&out2[0], 2, MPI_UNSIGNED_CHAR, ranks[2], COMMON_TAG, MPI_COMM_WORLD);
    }

    // net 1x1 + output : third and last processor
    // does not need to know inputs, stores output vector
    if (rank == ranks[2]){
        vector<BYTE> out(4);
        // recieve 2 ordered vectors (of size 2)
        MPI_Recv(&out[0], 2, MPI_UNSIGNED_CHAR, ranks[0], COMMON_TAG, MPI_COMM_WORLD, &status); 
        MPI_Recv(&out[2], 2, MPI_UNSIGNED_CHAR, ranks[1], COMMON_TAG, MPI_COMM_WORLD, &status);
        // order two middle numbers
        vector<BYTE> tmp = net_1x1(out[1], out[2]);
        out[1] = tmp[0];
        out[2] = tmp[1];
        return out;
    }
    return {};
}

vector<BYTE> net_4x4(const vector<BYTE> &in1,const vector<BYTE> &in2, const int &rank, const vector<int> &ranks){

    // net 2x2 - processors {x_0, x_1, x_4} from ranks = {x_0, ..., x_9}
    //
    // in processors x_0 and x_1 needs to be stored ordered vectors 'in1' and 'in2'
    //
    // processor x_4 gets the output of this net_2x2 and sends it to processors
    // {x_6, ..., x_9} for further sorting
    if (rank == ranks[0] || rank == ranks[1]){
        net_2x2({in1[0], in1[2]}, {in2[0], in2[2]}, rank, {ranks[0], ranks[1], ranks[4]});
    }
    if (rank == ranks[4]){
        vector<BYTE> out1_2x2 = net_2x2({}, {}, rank, {ranks[0], ranks[1], ranks[4]});
        MPI_Send(&out1_2x2[1], 1, MPI_UNSIGNED_CHAR, ranks[6], COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out1_2x2[2], 1, MPI_UNSIGNED_CHAR, ranks[7], COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out1_2x2[3], 1, MPI_UNSIGNED_CHAR, ranks[8], COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out1_2x2[0], 1, MPI_UNSIGNED_CHAR, ranks[9], COMMON_TAG, MPI_COMM_WORLD);
    }

    // net 2x2 - processors {x_2, x_3, x_5} from ranks = {x_0, ..., x_9}
    //
    // in processors x_2 and x_3 needs to be stored ordered vectors 'in1' and
    // 'in2'
    //
    // processor x_5 gets the output of this net_2x2 and sends appropriate
    // numbers to processors {x_6, ..., x_9} for further sorting
    if (rank == ranks[2] || rank == ranks[3]){
        net_2x2({in1[1], in1[3]}, {in2[1], in2[3]}, rank, {ranks[2], ranks[3], ranks[5]});
    }
    if (rank == ranks[5]){
        vector<BYTE> out2_2x2 = net_2x2({}, {}, rank, {ranks[2], ranks[3], ranks[5]});
        MPI_Send(&out2_2x2[0], 1, MPI_UNSIGNED_CHAR, ranks[6], COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out2_2x2[1], 1, MPI_UNSIGNED_CHAR, ranks[7], COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out2_2x2[2], 1, MPI_UNSIGNED_CHAR, ranks[8], COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out2_2x2[3], 1, MPI_UNSIGNED_CHAR, ranks[9], COMMON_TAG, MPI_COMM_WORLD);
    }

    // net 1x1 - processor x_6 recieves numbers from x_4 and x_5, sort them and
    // sends them to processor x_9
    if (rank == ranks[6]){
        BYTE in11, in12;
        MPI_Recv(&in11, 1, MPI_UNSIGNED_CHAR, ranks[4], COMMON_TAG, MPI_COMM_WORLD, &status); 
        MPI_Recv(&in12, 1, MPI_UNSIGNED_CHAR, ranks[5], COMMON_TAG, MPI_COMM_WORLD, &status); 
        vector<BYTE> out1 = net_1x1(in11, in12);
        MPI_Send(&out1[0], 2, MPI_UNSIGNED_CHAR, ranks[9], COMMON_TAG, MPI_COMM_WORLD);
    }

    // net 1x1 - processor x_7 recieves numbers from x_4 and x_5, sort them and
    // sends them to processor x_9
    if (rank == ranks[7]){
        BYTE in21, in22;
        MPI_Recv(&in21, 1, MPI_UNSIGNED_CHAR, ranks[4], COMMON_TAG, MPI_COMM_WORLD, &status); 
        MPI_Recv(&in22, 1, MPI_UNSIGNED_CHAR, ranks[5], COMMON_TAG, MPI_COMM_WORLD, &status); 
        vector<BYTE> out2 = net_1x1(in21, in22);
        MPI_Send(&out2[0], 2, MPI_UNSIGNED_CHAR, ranks[9], COMMON_TAG, MPI_COMM_WORLD);
    }

    // net 1x1 - processor x_8 recieves numbers from x_4 and x_5, sort them and
    // sends them to processor x_9
    if (rank == ranks[8]){
        BYTE in31, in32;
        MPI_Recv(&in31, 1, MPI_UNSIGNED_CHAR, ranks[4], COMMON_TAG, MPI_COMM_WORLD, &status); 
        MPI_Recv(&in32, 1, MPI_UNSIGNED_CHAR, ranks[5], COMMON_TAG, MPI_COMM_WORLD, &status); 
        vector<BYTE> out3 = net_1x1(in31, in32);
        MPI_Send(&out3[0], 2, MPI_UNSIGNED_CHAR, ranks[9], COMMON_TAG, MPI_COMM_WORLD);
    }

    // output - in this processor (x_9) is stored output processor collects
    // ordered vectors/numbers from other processors {x_4, ..., x_8} and stores
    // them as one ordered vector
    if (rank == ranks[9]){
        vector<BYTE> out(8);
        MPI_Recv(&out[0], 1, MPI_UNSIGNED_CHAR, ranks[4], COMMON_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&out[1], 2, MPI_UNSIGNED_CHAR, ranks[6], COMMON_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&out[3], 2, MPI_UNSIGNED_CHAR, ranks[7], COMMON_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&out[5], 2, MPI_UNSIGNED_CHAR, ranks[8], COMMON_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&out[7], 1, MPI_UNSIGNED_CHAR, ranks[5], COMMON_TAG, MPI_COMM_WORLD, &status);
        return out;
    }
    return {};
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<BYTE> in;


    /*************************** FIRST LAYER **********************************/

    // input handling + first layer - processors {0, 1, 2, 3, 4)
    if (0 <= rank && rank <= 4){
        in = get_input();

        // processor 0 checks size of vector and prints it
        if (rank == 0) {
            if (in.size() != 8) {
                cerr << "wrong number count (" << in.size() << ") in file " << FILENAME << endl;
                MPI_Abort(MPI_COMM_WORLD, 2);
                return 2;
            }
            print_vec(in, " ");
            cout << endl;
        }

        // net 1x1 - processor 1
        // 
        // sends output to processors 5 and 6
        if (rank == 1){
            vector<BYTE> o1 = net_1x1(in[0], in[1]);
            MPI_Send(&o1[0], 2, MPI_UNSIGNED_CHAR, 5, 0, MPI_COMM_WORLD);
            MPI_Send(&o1[0], 2, MPI_UNSIGNED_CHAR, 6, 0, MPI_COMM_WORLD);
            
        }

        // net 1x1 - processor 2
        // 
        // sends output to processors 5 and 6
        if (rank == 2){
            vector<BYTE> o2 = net_1x1(in[2], in[3]);
            MPI_Send(&o2[0], 2, MPI_UNSIGNED_CHAR, 5, 0, MPI_COMM_WORLD);
            MPI_Send(&o2[0], 2, MPI_UNSIGNED_CHAR, 6, 0, MPI_COMM_WORLD);
            
        }

        // net 1x1 - processor 3
        // 
        // sends output to processors 7 and 8
        if (rank == 3){
            vector<BYTE> o3 = net_1x1(in[4], in[5]);
            MPI_Send(&o3[0], 2, MPI_UNSIGNED_CHAR, 8, 0, MPI_COMM_WORLD);
            MPI_Send(&o3[0], 2, MPI_UNSIGNED_CHAR, 9, 0, MPI_COMM_WORLD);
            
        }

        // net 1x1 - processor 4
        // 
        // sends output to processors 7 and 8
        if (rank == 4){
            vector<BYTE> o4 = net_1x1(in[6], in[7]);
            MPI_Send(&o4[0], 2, MPI_UNSIGNED_CHAR, 8, 0, MPI_COMM_WORLD);
            MPI_Send(&o4[0], 2, MPI_UNSIGNED_CHAR, 9, 0, MPI_COMM_WORLD);
        }
    }


    /*************************** SECOND LAYER *********************************/
    
    // net 2x2 - processors 5, 6 (input) and 7 (output)
    // 
    // processors 5 and 6 recieves data from first layer (processors 1 and 2)
    // 
    // processor 7 sends ordered vector to processors 11, 12, 13, 14
    if (rank == 5 || rank == 6){
        vector<BYTE> in1(2);
        vector<BYTE> in2(2);
        MPI_Recv(&in1[0], 2, MPI_UNSIGNED_CHAR, 1, COMMON_TAG, MPI_COMM_WORLD, &status); 
        MPI_Recv(&in2[0], 2, MPI_UNSIGNED_CHAR, 2, COMMON_TAG, MPI_COMM_WORLD, &status);
        net_2x2(in1, in2, rank, {5, 6, 7});
    }
    if (rank == 7){
        vector<BYTE> out1 = net_2x2({}, {}, rank, {5, 6, 7});
        MPI_Send(&out1[0], 4, MPI_UNSIGNED_CHAR, 11, COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out1[0], 4, MPI_UNSIGNED_CHAR, 12, COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out1[0], 4, MPI_UNSIGNED_CHAR, 13, COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out1[0], 4, MPI_UNSIGNED_CHAR, 14, COMMON_TAG, MPI_COMM_WORLD);
    }

    // net 2x2 - processors 8, 9 (input) and 10 (output)
    //
    // processors 8 and 9 recieves data from first layer (processors 3 and 4)
    //
    // processor 10 sends ordered vector to processors 11, 12, 13, 14
    if (rank == 8 || rank == 9){
        vector<BYTE> in3(2);
        vector<BYTE> in4(2);
        MPI_Recv(&in3[0], 2, MPI_UNSIGNED_CHAR, 3, COMMON_TAG, MPI_COMM_WORLD, &status); 
        MPI_Recv(&in4[0], 2, MPI_UNSIGNED_CHAR, 4, COMMON_TAG, MPI_COMM_WORLD, &status);
        net_2x2(in3, in4, rank, {8, 9, 10});
    }
    if (rank == 10){
        vector<BYTE> out2 = net_2x2({}, {}, rank, {8, 9, 10});
        MPI_Send(&out2[0], 4, MPI_UNSIGNED_CHAR, 11, COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out2[0], 4, MPI_UNSIGNED_CHAR, 12, COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out2[0], 4, MPI_UNSIGNED_CHAR, 13, COMMON_TAG, MPI_COMM_WORLD);
        MPI_Send(&out2[0], 4, MPI_UNSIGNED_CHAR, 14, COMMON_TAG, MPI_COMM_WORLD);
    }


    /********************** THIRD LAYER (LAST) ********************************/

    // net 4x4 - processors {11, 12, 13, 14} for input, {15, 16, 17, 18, 19} for
    // processors needed by net_4x4 and processor 0 for output
    //
    // processors {11, 12, 13, 14} recieves data from second layer (processors 7
    // and 10)
    // 
    // processor 0 gets ordered vector and prints vector
    if (11 <= rank && rank < 15){
        vector<BYTE> in1_4x4(4);
        vector<BYTE> in2_4x4(4);
        MPI_Recv(&in1_4x4[0], 4, MPI_UNSIGNED_CHAR, 7, COMMON_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&in2_4x4[0], 4, MPI_UNSIGNED_CHAR, 10, COMMON_TAG, MPI_COMM_WORLD, &status);
        vector<BYTE> out_part = net_4x4(in1_4x4, in2_4x4, rank, {11, 12, 13, 14, 15, 16, 17, 18, 19, 0});
    }
    if (15 <= rank && rank < 20){
        net_4x4({}, {}, rank, {11, 12, 13, 14, 15, 16, 17, 18, 19, 0});
    }
    if (rank == 0){
        vector<BYTE> out = net_4x4({}, {}, rank, {11, 12, 13, 14, 15, 16, 17, 18, 19, 0});
        print_vec(out, "\n");
    }
    
    MPI_Finalize();
}
