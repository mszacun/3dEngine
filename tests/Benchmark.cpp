#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <sys/time.h>
#include "../src/Matrix.h"

using std::vector;

typedef std::function<void()> BenchmarkFunc;

long long Benchamark(const BenchmarkFunc& f)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    f();

    gettimeofday(&end, NULL);

    return (end.tv_sec * (unsigned int)1e6 +   end.tv_usec) - 
         (start.tv_sec * (unsigned int)1e6 + start.tv_usec);
}

void BenchamarkNTimes(const BenchmarkFunc& f, const std::string& name, int n)
{
    long long sum = 0;
    for (int i = 0; i < n; i++)
    {
        long long actual = Benchamark(f);
        std::cout << name << " " << i + 1 << ": " << actual << std::endl;
        sum += actual;
    }
    double avg = sum / (double) n;
    std::cout << "AVG: " << avg << std::endl;
}

void BenchmarkMatrixMultiplication()
{
    const int N_TIMES = 100;
    vector<vector<double>> matrixData { { 1, 2, 3, 4},
                                         { 5, 6, 7, 8},
                                         { 9, 10, 11, 12 },
                                         { 13, 14, 15, 16} };
    Matrix m1(matrixData);
    Matrix m2(matrixData);
    Matrix result(4, 4);

    BenchamarkNTimes(std::bind(Matrix::Multiply, std::cref(m1), std::cref(m2), std::ref(result)),
        "MatrixMultiplication", N_TIMES);
}

int main()
{
    BenchmarkMatrixMultiplication();
    return 0;
}
