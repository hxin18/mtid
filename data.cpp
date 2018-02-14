
#include <cmath>
#include "data.h"
#include <fstream>
using namespace std;
int EMBEDDING_SIZE;
int STARTING_YEAR;
int ENDING_YEAR;



bool compare(paper i,paper j) {
    return (i.year<j.year);
}

void dataset::read_influence(char* influence_file){
    dataset::sum_it = new double *[ENDING_YEAR-STARTING_YEAR];
    for(int i = 0; i < ENDING_YEAR-STARTING_YEAR; i++){
        dataset::sum_it[i] =  new double[EMBEDDING_SIZE];
    }
    dataset::sum_i = new double[EMBEDDING_SIZE];
    ifstream fin;
    fin.open(influence_file);
    string ReadLine;
    int current = 0;
    while(getline(fin,ReadLine))
    {
        dataset::pap[current]->i_a = new double[EMBEDDING_SIZE];
        dataset::pap[current]->g_a = new double[EMBEDDING_SIZE];
        strtokenizer strtok(ReadLine, " \t\r\n");
        for(int i = 0; i < EMBEDDING_SIZE; i++){
            dataset::pap[current]->i_a[i] = atof(strtok.token(i).c_str());
            dataset::sum_i[i]+=dataset::pap[current]->i_a[i];
            int yearr = dataset::pap[current]->year;
            if(yearr<=STARTING_YEAR){
                dataset::sum_it[0][i]+=dataset::pap[current]->i_a[i];
            }
            int gap = yearr - STARTING_YEAR;
            if(gap>0)
            for(int ii = 0; ii<gap+1;ii++){
            dataset::sum_it[ii][i]+=dataset::pap[current]->i_a[i];
            }
        }
        current++;
    }
    fin.close();
    for(int i = 0; i < dataset::num_pap;i++){
    		dataset::pap[i]->summ=new double[EMBEDDING_SIZE];
        for(int j = 0; j< EMBEDDING_SIZE;j++){
            dataset::pap[i]->summ[j]=0;
            if(dataset::pap[i]->reference.size()>0)
            for(int p = 0; p<dataset::pap[i]->reference.size();p++){
                dataset::pap[i]->summ[j]+=dataset::pap[dataset::pap[i]->reference[p]]->i_a[j]/float(dataset::pap[i]->reference.size());
            }
        }
    }
}


dataset::dataset(int num_pap)
{
    dataset::num_pap = num_pap;
    dataset::pap = new paper* [num_pap];

    for(int i =0; i < num_pap; i++){
        dataset::pap[i] = new paper(i);
    }
  
}


paper::paper(int id){
    paper::id = id;
    paper::f_a = new double[EMBEDDING_SIZE];
}

void dataset::read_cascade_data(char* citation_file,char* year_file)
{
    ifstream fin_c(citation_file);
    string ReadLine_c;
    while(getline(fin_c,ReadLine_c))
    {
        //cout<<ReadLine_c<<endl;
        strtokenizer strtok_c(ReadLine_c, " \t\r\n");
        dataset::pap[atoi(strtok_c.token(0).c_str())]->num_cit = strtok_c.count_tokens() - 1;
        dataset::pap[atoi(strtok_c.token(0).c_str())]->citation = new int [dataset::pap[atoi(strtok_c.token(0).c_str())]->num_cit];
        for(int i =0; i < dataset::pap[atoi(strtok_c.token(0).c_str())]->num_cit; i++){
            dataset::pap[atoi(strtok_c.token(0).c_str())]->citation[i] = atoi(strtok_c.token(i+1).c_str());
             //dataset::pap[atoi(strtok_c.token(0).c_str())]->reference.push_back(atoi(strtok_c.token(i+1).c_str()));
            //cout<<strtok_c.token(i+1)<<endl;
            dataset::pap[atoi(strtok_c.token(i+1).c_str())]->reference.push_back(atoi(strtok_c.token(0).c_str()));
        }
    }
    fin_c.close();
    fstream fin_y(year_file);
    string ReadLine_y;
    while(getline(fin_y,ReadLine_y))
    {
        strtokenizer strtok_y(ReadLine_y, " \t\r\n");
        dataset::pap[atoi(strtok_y.token(0).c_str())]->year = atoi(strtok_y.token(1).c_str());
       
    }
    
    fin_y.close();
};



void dataset::out_put(string output_file){
    string output_file1 = output_file;
    FILE * authori = fopen(output_file1.c_str(),"w");
    for(int i = 0; i < num_pap; i++){
        for(int j = 0; j < EMBEDDING_SIZE; j++){
            fprintf(authori, "%lf ",pap[i]->f_a[j]);
        }
            fprintf(authori, "\n");
    }
    fclose(authori);
}
