
#include "update.h"
#include "data.h"
#include <math.h>
#include <time.h>

double Norm2(double* x) {
    double result = 0.0; int Len = EMBEDDING_SIZE;
    for (int i = 0; i < Len; i++)
        result += x[i] * x[i];
    return result;
}

double DotProduct(double* x, double* y) {
    double result = 0.0; int Len = EMBEDDING_SIZE;
    for (int i = 0; i < Len; i++)
        result += x[i] * y[i];
    return result;
}

double inline Prediction(const int FU, const int FV,dataset* data) {
    double DP = log (1.0 / (1.0 - 1.0/data->num_pap)) + DotProduct(data->pap[FU]->f_a, data->pap[FV]->i_a);
    //IAssertR(DP > 0.0, TStr::Fmt("DP: %f", DP));
    //cout<<DP<<endl;
    return exp(- DP);
}
double inline Prediction(double*NewVarV, const int FV,dataset* data) {
    double DP = log (1.0 / (1.0 - 1.0/data->num_pap)) + DotProduct(NewVarV, data->pap[FV]->i_a);
    //IAssertR(DP > 0.0, TStr::Fmt("DP: %f", DP));
    return exp(- DP);
}
double LikelihoodForRow(const int UID, dataset* data) {
    double L = 0.0;
    double NegWgt =1;
    int year = data->pap[UID]->year;
    int gap = year-STARTING_YEAR>0?year-STARTING_YEAR:0;
    for (int e = 0; e < data->pap[UID]->reference.size(); e++) {
        int v = data->pap[UID]->reference[e];
        L += log (1.0 - Prediction(UID, v,data)) + NegWgt * DotProduct(data->pap[UID]->f_a, data->pap[v]->i_a);
        
        }
    L += NegWgt *DotProduct(data->pap[UID]->f_a, data->pap[UID]->i_a);
    L-= NegWgt *DotProduct(data->pap[UID]->f_a, data->sum_it[gap]);
    return L;
}

double LikelihoodForRow(const int UID,double*NewVarV, double StepSize,dataset* data) {
    double L = 0.0;
    double NegWgt = 1;
    int year = data->pap[UID]->year;
    int gap = year-STARTING_YEAR>0?year-STARTING_YEAR:0;
    double* xplusthetax = new double[EMBEDDING_SIZE];
    for(int i = 0; i< EMBEDDING_SIZE;i++)
        xplusthetax[i]=(StepSize*NewVarV[i]+data->pap[UID]->f_a[i])>0?(StepSize*NewVarV[i]+data->pap[UID]->f_a[i]):0;
    
    for (int e = 0; e < data->pap[UID]->reference.size(); e++) {
        int v = data->pap[UID]->reference[e];
        L += log (1.0 - Prediction(xplusthetax, v,data)) + NegWgt * DotProduct(xplusthetax, data->pap[v]->i_a);
    }
    L+=NegWgt *DotProduct(xplusthetax, data->pap[UID]->i_a);
    L-= NegWgt *DotProduct(xplusthetax, data->sum_it[gap]);
    delete(xplusthetax);
    return L;
}

double GetStepSizeByLineSearch(const int UID, double* DeltaV, double*  GradV, double Alpha, const double Beta, const int MaxIter,dataset *data) {
    double StepSize = 1.0;
    double InitLikelihood = LikelihoodForRow(UID,data);
    double*NewVarV = new double[EMBEDDING_SIZE];
    for(int iter = 0; iter < MaxIter; iter++) {
        if (LikelihoodForRow(UID,DeltaV,StepSize,data) < InitLikelihood ||isnan(LikelihoodForRow(UID,DeltaV,StepSize,data))) {
            StepSize *= Beta;
        } else {
            break;
        }
        if (iter == MaxIter - 1) {
            StepSize = 0.0;
            break;
        }
    }
    delete(NewVarV);
    
    return StepSize;
}

void GradientForRow(int UID, dataset*data) {
    int year = data->pap[UID]->year;
    int gap = year-STARTING_YEAR>0?year-STARTING_YEAR:0;
    for(int i = 0; i < EMBEDDING_SIZE;i++){
        data->pap[UID]->g_a[i]=0;
        }
    for (int e = 0; e < data->pap[UID]->reference.size(); e++) {
        //cout<<e<<endl;
        for(int i = 0; i < EMBEDDING_SIZE;i++){
            data->pap[UID]->g_a[i]+=data->pap[data->pap[UID]->reference[e]]->i_a[i]*Prediction(UID,data->pap[UID]->reference[e],data)/(1-Prediction(UID,data->pap[UID]->reference[e],data))+1*data->pap[data->pap[UID]->reference[e]]->i_a[i];
            }
        }
    for(int i = 0; i < EMBEDDING_SIZE;i++){
        data->pap[UID]->g_a[i]+=1*data->pap[UID]->i_a[i];
        data->pap[UID]->g_a[i] -=1*data->sum_it[gap][i];
    }
}
double Likelihood(dataset* data){
    double res = 0;
    for(int i = 0;i <data->num_pap;i++)
        res+=LikelihoodForRow(i, data);
    
    return res;
}
int MLEGradAscent(dataset* data, int MaxIter, const double StepAlpha, const double StepBeta) {
    time_t InitTime = time(NULL);
    double old_likelihood = 0.0;
    double new_likelihood;
    //TExeTm ExeTm, CheckTm;
    int iter = 0;
    while(iter < MaxIter) {
        iter++;
        #pragma omp parallel for shared(data) 
        for (int u = 0; u < data->num_pap; u+=1) {
            if(data->pap[u]->reference.size() != 0){
            	GradientForRow(u, data);
            }
        }
        #pragma omp barrier
        #pragma omp parallel for shared(data) 
        for (int u = 0; u < data->num_pap; u+=1) {
        	if(data->pap[u]->reference.size() == 0){
                for(int i = 0; i< EMBEDDING_SIZE;i++)
                    data->pap[u]->f_a[i]=0;
            }
            else{	
				if (Norm2(data->pap[u]->g_a) > 1e-4) { 
           				double LearnRate = GetStepSizeByLineSearch(u, data->pap[u]->g_a, data->pap[u]->g_a, StepAlpha, StepBeta,1000,data);
           				if (LearnRate > 0.0) { 
           		 			for (int dim = 0; dim < EMBEDDING_SIZE; dim++) {
                				double Change = LearnRate * data->pap[u]->g_a[dim];
               					double NewFuc = data->pap[u]->f_a[dim]+ Change;
                				if (NewFuc <= 0.0) {
                    				data->pap[u]->f_a[dim]=0;
                				} 
                				else{
                   				 	data->pap[u]->f_a[dim]=NewFuc;
               					}
            				}
        				}
   					}
        		}
        	}
         #pragma omp barrier
        
       
        data->out_put("./F500");
        new_likelihood = Likelihood(data);
        double gap =  new_likelihood-old_likelihood;
        cout<<new_likelihood <<" "<<gap<<endl;
        old_likelihood = new_likelihood;
        //cout<<"========"<<endl;
        printf("\r%d iterations[%lu sec]", iter,  time(NULL) - InitTime);
        printf("\n");
        if(gap>0&&gap<0.0001)
            break;
    }
    return iter;
}

