
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "update.h"
void print_help(){
    cout<<"MTID usage:"<<endl;
    cout<<"main [number of document] [citation_file] [topic_file] [year_file] [number of topic] ";
    cout<<"[MaxIter] [alpha] [beta] [STARTING_YEAR] [ENDING_YEAR] [output_file]"<<endl;


}
bool compare(pair<int, double> a,pair<int, double> b){
    return a.second>b.second;
}
int main(int argc, const char * argv[]) {
    if(argc==12){
    int num_pap=atoi(argv[1]);
    char citation_file[100];
    char influence_file[100];
    char year_file[100];
    char output_file[100];
    sprintf(citation_file,argv[2]);
    sprintf(influence_file,argv[3]);
    sprintf(year_file,argv[4]);
    EMBEDDING_SIZE = atoi(argv[5]);
    int MaxIter =atoi(argv[6]);
    double StepAlpha = atof(argv[7]);
    double StepBeta =atof(argv[8]);
    STARTING_YEAR = atoi(argv[9]);
	ENDING_YEAR=atoi(argv[10]);
    sprintf(output_file,argv[11]);
    dataset *data = new dataset(num_pap);
    data->read_cascade_data(citation_file,year_file);
    data->read_influence(influence_file);
   cout<<"reading complete"<<endl;
    for(int i = 0; i< num_pap;i++){
        data->pap[i]->f_a=new double[EMBEDDING_SIZE];
        for(int j = 0;j < EMBEDDING_SIZE;j++)
            data->pap[i]->f_a[j]= 1;
    }
    cout<<"initial complete"<<endl;
    MLEGradAscent(data, MaxIter,  StepAlpha,  StepBeta);
    data->out_put(output_file);
    }
    else{
        print_help();

    }
    return 0;
}
