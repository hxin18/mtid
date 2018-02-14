

#ifndef data_h
#define data_h
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include "constant.h"
#include "strtokenizer.h"
#include <map>

using namespace std;
extern int EMBEDDING_SIZE;
extern int STARTING_YEAR;
extern int ENDING_YEAR;

class paper{
public:
    int id;
    int year;
    int num_cit;
    vector<int> reference;//reference is the papers that  refers this
    int* citation;//citation is the paper that it cites
    paper(int id);
    double* summ;
    double* f_a;
    double* i_a;
    double* g_a;
    ~paper(){
        delete(citation);
    }
};


class dataset{
public:
    int num_pap;
    double *sum_i;
    double **sum_it;
    paper ** pap;
    dataset(int num_pap);
    ~dataset(){
        for(int i = 0; i < num_pap; i++){
            delete(pap[i]);
        }
        delete(pap);
    };
    void read_cascade_data(char* citation_file,char* year_file);
    void read_influence(char* influence_file);
    void out_put(string output_file);
};

#endif /* data_hpp */
