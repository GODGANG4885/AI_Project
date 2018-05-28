#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NLayer 3
#define MLayerSize 1000
#define m0 20
#define m1 60
#define m2 10
#define N 784+1 //input vector

int M[NLayer] = { m0, m1, m2 };
double s[NLayer][MLayerSize];
double f[NLayer][MLayerSize];
double delta[NLayer][MLayerSize];
double W[NLayer][MLayerSize][MLayerSize];

#define N_tr_examples  60000 // 훈련 데이터
#define N_te_examples  10000 // 테스트 데이터

int TrainData[N_tr_examples][N - 1], d_tr[N_tr_examples][m2];
int TestData[N_te_examples][N - 1], d_te[N_te_examples][m2];