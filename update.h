
#ifndef update_h
#define update_h
#include "data.h"
#include <math.h>
#include <stdio.h>

extern int EMBEDDING_SIZE;
extern int STARTING_YEAR;
extern int ENDING_YEAR;
double Norm2(double* x);
double DotProduct(double* x, double* y);
double inline Prediction(const int FU, const int FV,dataset* data);
double inline Prediction(double*NewVarV, const int FV,dataset* data);
double LikelihoodForRow(const int UID, dataset* data);
double LikelihoodForRow(const int UID,double*NewVarV, dataset* data);
double GetStepSizeByLineSearch(const int UID, double* DeltaV, double*  GradV, double Alpha, const double Beta, const int MaxIter,dataset *data);
void GradientForRow(int UID, dataset*data);
double Likelihood(dataset* data);
int MLEGradAscent(dataset* data, int MaxIter, const double StepAlpha, const double StepBeta);
#endif /* update_hpp */
