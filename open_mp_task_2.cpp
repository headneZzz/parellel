
// OMPBegin2. Реализовать два варианта вычисления функции F(X, N):
// непараллельный и параллельный низкоуровневый, использующий функцию
// omp_get_thread_num() (число потоков равно 2). Формула для функции F:

// В разделе отладки вывести дополнительную информацию, описание которой приведено
// в преамбуле к данной группе заданий. Обеспечить возможно большее ускорение
// параллельного варианта за счет равномерной загрузки потоков. Для этого
// использовать алгоритм распределения слагаемых по полосам, вычисляя все
// слагаемые с четными значениями I в одном из потоков, а все слагаемые с
// нечетными значениями I — в другом.
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <omp.h>

using namespace std;

double summa(int start_i, double x, int n) {
  double result = 0;
  for (int i=start_i; i<=n; i+=2) {
    double tempResult = 0;
    for (int j=1; j<=i+n; j++) {
      tempResult += (j+pow(x+j, 1/3))/(2*i*j-1);
    }
    result += 1/tempResult;
  }
  return result;
}

double f(double x, int n) {
  double result = 0;
  #pragma omp parallel num_threads(2)
  {
    #pragma omp sections
    {
      #pragma omp section
      {
        result+=summa(1, x, n);
      }
      #pragma omp section
      {
        result+=summa(2, x, n);
      }
    }
    printf("num_threads: %d\n", omp_get_thread_num());
  }

  return result;
}

int main(){
  unsigned int start_time =  clock();
  #ifdef _OPENMP
  printf("OpenMP is supported!\n");
  printf("%f\n", f(5.0, 5));
  #endif
  unsigned int end_time = clock();
  double search_time = (end_time - start_time)/1000.0;
  printf("%f\n", search_time);
}
