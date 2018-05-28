#include "NN_STRUCTURE.h"

void Setup_weight();
void Forward(int t);
void Forward_test(int t);
void Backward(int t);
void WeightUpdate(int t);
double Avg_sq_Error(int t);
int find_max();



int main() {
	int t = 0, epoch = 1; // example
	FILE *trFile = NULL, *teFile = NULL;
	unsigned int str;
	double train_threshold = 0.01, test_accuracy, avg_sq_error = 0.0;
	int num_correct, index = 0;

	srand(time(NULL));

	trFile = fopen("train.txt", "r");

	if (trFile == NULL) {
		printf("파일 열기 실패\n");
	}
	else {
		printf("Training Start\n");
		Setup_weight();
		printf(" Setup_weight Complete \n");

		while (fscanf(trFile, "%d", &str) > 0) {
			for (int i = 0; i < m2; i++) {
				if (i != str)
					d_tr[t][i] = 0;
				else
					d_tr[t][i] = 1;
			}// 훈련 입력벡터
			for (int j = 0; j < (N - 1); j++) {
				fscanf(trFile, "%d", &TrainData[t][j]);
			}

			t++;
		}// 파일 끝

		printf("훈련 데이터 파일 완료\n");

		while (1) {
			for (t = 0; t < (N_tr_examples - 1); t++) {
				Forward(t);
				Backward(t);
				WeightUpdate(t);
			}
			printf("%d번째 epoch ", epoch);
			epoch++;
			avg_sq_error = Avg_sq_Error(t);

			teFile = fopen("test.txt", "r");
			t = 0;
			if (teFile == NULL) {
				printf("파일 열기 실패\n");
			}
			else // test 데이터
			{
				num_correct = 0;
				// 테스트 target label
				while (fscanf(teFile, "%d", &str) > 0) {
					

					for (int i = 0; i < m2; i++) {
						if (i != str)
							d_te[t][i] = 0;
						else
							d_te[t][i] = 1;
					}

					for (int j = 0; j < (N - 1); j++) {
						fscanf(teFile, "%d", &TestData[t][j]);
					}
					t++;
				}

				for (t = 0; t < (N_te_examples - 1); t++) {
					Forward_test(t);

					for (int i = 0; i < m2; i++) {
						if (d_te[t][i] == 1) {
							index = i;
							break;
						}
					}
					if (find_max() == index)
						num_correct++;
				}

				test_accuracy = (double)num_correct / N_te_examples;
				

				if (avg_sq_error < train_threshold){
					printf("정확도 : %.3lf%%\n", test_accuracy * 100.0);
					break;
				}
					
			}
		}
	}

	fclose(trFile);
	fclose(teFile);
}
void Setup_weight() {
	int i, j, k, pre_layer;
	double r;

	for (i = 0; i < NLayer; i++) {
		for (j = 0; j < M[i]; j++) {
			if (i == 0)
				pre_layer = N;
			else
				pre_layer = M[i - 1] + 1;

			for (k = 0; k < pre_layer; k++) {
				r = (double)(rand());
				W[i][j][k] = r / (double)(RAND_MAX)-0.5;
			}
		}
	}
}
void Forward(int t) {
	// 층0
	for (int i = 0; i < M[0]; i++) {
		s[0][i] = 0.0;   // s 초기화
		for (int j = 0; j < (N - 1); j++) {
			s[0][i] += TrainData[t][j] / 255.0 * W[0][i][j];
		}
		s[0][i] += 1 / 255.0 * W[0][i][N - 1]; // dummy

		f[0][i] = 1.0 / (1.0 + exp(-1.0 * s[0][i]));
		f[0][m0] = 1.0; //dummy
	}

	//층0이 아닐 때 
	for (int L = 1; L < NLayer; L++) {
		for (int i = 0; i < M[L]; i++) {
			s[L][i] = 0.0; // s 초기화

			for (int j = 0; j < (M[L - 1] + 1); j++) // +1 dummy값 포함
				s[L][i] += f[L - 1][j] * W[L][i][j];

			f[L][i] = 1.0 / (1.0 + exp(-1.0 * s[L][i]));
		}
		f[L][M[L]] = 1.0; //dummy
	}
}
void Forward_test(int t) {
	// 층0
	for (int i = 0; i < M[0]; i++) {
		s[0][i] = 0.0;   // s 초기화
		for (int j = 0; j < (N - 1); j++) {
			s[0][i] += TestData[t][j] / 255.0 * W[0][i][j];
		}
		s[0][i] += 1 / 255.0 * W[0][i][N - 1]; // dummy

		f[0][i] = 1.0 / (1.0 + exp(-1.0 * s[0][i]));
		f[0][m0] = 1.0; //dummy
	}

	//층0이 아닐 때 
	for (int L = 1; L < NLayer; L++) {
		for (int i = 0; i < M[L]; i++) {
			s[L][i] = 0.0; // s 초기화

			for (int j = 0; j < (M[L - 1] + 1); j++) // +1 dummy값 포함
				s[L][i] += f[L - 1][j] * W[L][i][j];

			f[L][i] = 1.0 / (1.0 + exp(-1.0 * s[L][i]));
		}
		f[L][M[L]] = 1.0; //dummy
	}
}
void Backward(int t) {
	int k = NLayer - 1; //최종층 번호
	double tsum;

	//최종층
	for (int i = 0; i < M[k]; i++) {
		delta[k][i] = (d_tr[t][i] - f[k][i]) * f[k][i] * (1 - f[k][i]);
	}

	//최종층 이전
	for (int L = (NLayer - 2); L >= 0; L--) {
		for (int i = 0; i < M[L]; i++) {
			tsum = 0.0;
			for (int j = 0; j < M[L + 1]; j++) {
				tsum += delta[L + 1][j] * W[L + 1][j][i];
			}
			delta[L][i] = f[L][i] * (1 - f[L][i]) *tsum;
		}
	}
}
void WeightUpdate(int t) {
	double c = 0.005;
	// 층0
	for (int i = 0; i < M[0]; i++) {
		for (int j = 0; j < (N - 1); j++)
			W[0][i][j] += c * delta[0][i] * TrainData[t][j] / 255.0;

		W[0][i][N - 1] += c * delta[0][i] * 1 / 255.0; //dummy
	}

	// 층0 아닐 때
	for (int L = 1; L < NLayer; L++) {
		for (int i = 0; i < M[L]; i++) {
			for (int j = 0; j < (M[L - 1] + 1); j++) { // +1 dummy
				W[L][i][j] += c * delta[L][i] * f[L - 1][j];
			}
		}
	}
}
double Avg_sq_Error(int t) {
	double sum_sq_error = 0.0, avg_sq_error = 0.0;

	for (int t = 0; t < N_tr_examples; t++) {
		Forward(t);

		for (int i = 0; i < M[NLayer - 1]; i++) {
			sum_sq_error += (d_tr[t][i] - f[NLayer - 1][i]) * (d_tr[t][i] - f[NLayer - 1][i]);
		}
	}

	avg_sq_error = sum_sq_error / (N_tr_examples * M[NLayer - 1]);
	printf("avg_sq_error : %lf \n  ", avg_sq_error);

	return avg_sq_error;
}
int find_max() {
	double max = 0.0;
	int index = 0;

	max = f[NLayer - 1][0];

	for (int i = 1; i < m2; i++) {
		if (max < f[NLayer - 1][i]) {
			max = f[NLayer - 1][i];
			index = i;
		}
	}

	return index;
}