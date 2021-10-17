#include "pt4.h"
#include "omp.h"
#include <cmath>
#include <chrono>
#include <string>

using namespace std;

// OMPBegin2.Реализовать два варианта вычисления функции F(X, N) : непараллельный и параллельный низкоуровневый, 
// использующий функцию omp_get_thread_num() (число потоков равно 2). 
// Формула для функции F :F(X, N) = Σ(I=1 до N) 1 / (Σ(J=1 до I + N (J + (X + J)^(1 / 3)) / (2IJ − 1))
// В разделе отладки вывести дополнительную информацию, описание которой приведено
// в преамбуле к данной группе заданий.Обеспечить возможно большее ускорение параллельного варианта за счет равномерной загрузки потоков.
// Для этого использовать алгоритм распределения слагаемых по полосам, вычисляя все слагаемые с четными значениями I в одном из потоков, а все слагаемые с нечетными значениями I — в другом.
void Solve() {
    Task("OMPBegin2");

    //non paralell
    int N = 0;
    double X = 0;
    unsigned int start_time = clock();
    pt >> X >> N;
    double rez = 0.0;
    for (int i = 1; i <= N; i++) {
        double sumj = 0.0;
        for (int j = 1; j <= i + N; j++)
            sumj += (j + pow(X + j, 1.0 / 3.0)) / (2 * i * j - 1);
        rez += 1 / sumj;
    }

    unsigned int end_time = clock();
    double non_paralell_search_time = (end_time - start_time) / 1000.0;
    ShowLine("Non-paralell time: " + to_string(non_paralell_search_time));
    pt << rez;

    //paralell 
    pt >> X >> N;
    omp_set_num_threads(2);
    start_time = clock();
    double threadSum = 0.0;
    double sumj;
#pragma omp parallel private(sumj) shared(X, N) reduction(+:threadSum)
    {
        int id = omp_get_thread_num();
        unsigned int start_time_m = clock();
        for (int i = id + 1; i <= N; i = i + 2) {
            sumj = 0.0;
            for (int j = 1; j <= i + N; j++)
                sumj += (j + pow(X + j, 1.0 / 3.0)) / (2 * i * j - 1);
            threadSum += 1 / sumj;
        }
        unsigned int end_time_m = clock();
        double search_time_m = (end_time_m - start_time_m) / 1000.0;
        ShowLine("thread_num: " + to_string(id) + " Count: " + to_string(threadSum) + " Thread time: " +
            to_string(search_time_m));
    }
    end_time = clock();
    double paralell_search_time = (end_time - start_time) / 1000.0;
    ShowLine("Total parallel time: " + to_string(paralell_search_time));
    double rate = non_paralell_search_time / paralell_search_time;
    ShowLine("Rate: " + to_string(rate));
    pt << threadSum;

}
