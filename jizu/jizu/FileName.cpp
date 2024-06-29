//普通高斯消元
/*
普通高斯消元
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int N;
vector<vector<double>> f(2020, vector<double>(2020, 0));
const double esp = 1e-6;
const int UNROLL_FACTOR = 4; // 展开因子

void input() {
    cin >> N;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N + 1; ++j) {
            if(j==N+1)
            f[i][j]=1;
            if (i == j) {
                f[i][j] = 1;
            }
        }
    }
}

void gaussian_elimination() {
    for (int i = 1; i <= N; ++i) {
        // 主元选择和行交换
        int r = i;
        for (int k = i; k <= N; ++k) {
            if (fabs(f[k][i]) > esp) {
                r = k;
                break;
            }
        }
        if (r != i) swap(f[i], f[r]);
        if (fabs(f[i][i]) < esp) {
            cout << "No Solution" << endl;
            return;
        }

        // 归一化第 i 行
        for (int j = i; j <= N + 1; ++j) {
            f[i][j] /= f[i][i];
        }

        // 消元操作，使用循环展开优化
        for (int k = i + 1; k <= N; ++k) {
            double factor = f[k][i];
            int j = i;
            // 循环展开
            for (; j <= N - UNROLL_FACTOR + 1; j += UNROLL_FACTOR) {
                f[k][j] -= factor * f[i][j];
                f[k][j + 1] -= factor * f[i][j + 1];
                f[k][j + 2] -= factor * f[i][j + 2];
                f[k][j + 3] -= factor * f[i][j + 3];
            }
            // 处理剩余部分
            for (; j <= N + 1; ++j) {
                f[k][j] -= factor * f[i][j];
            }
        }
    }

    // 回代过程
    for (int k = N - 1; k >= 1; --k) {
        for (int j = k + 1; j <= N; ++j) {
            f[k][N + 1] -= f[k][j] * f[j][N + 1];
        }
    }
}

int main() {
    input();
    gaussian_elimination();
    for (int i = 1; i <= N; ++i) {
        cout << fixed << setprecision(2) << f[i][N + 1] << endl;
    }
    return 0;
}
*/
//cache优化
/*
* cache优化
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int N;
vector<vector<double>> f(102, vector<double>(102, 0));
const double esp = 1e-6;
const int UNROLL_FACTOR = 4; // 展开因子

void input() {
    cin >> N;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N + 1; ++j) {
            cin >> f[i][j];
        }
    }
}

void gaussian_elimination() {
    for (int i = 1; i <= N; ++i) {
        // 主元选择和行交换
        int r = i;
        for (int k = i; k <= N; ++k) {
            if (fabs(f[k][i]) > esp) {
                r = k;
                break;
            }
        }
        if (r != i) swap(f[i], f[r]);
        if (fabs(f[i][i]) < esp) {
            cout << "No Solution" << endl;
            return;
        }

        // 归一化第 i 行
        for (int j = i; j <= N + 1; ++j) {
            f[i][j] /= f[i][i];
        }

        // 消元操作，使用循环展开优化
        for (int k = i + 1; k <= N; ++k) {
            double factor = f[k][i];
            int j = i;
            // 循环展开
            for (; j <= N - UNROLL_FACTOR + 1; j += UNROLL_FACTOR) {
                f[k][j] -= factor * f[i][j];
                f[k][j + 1] -= factor * f[i][j + 1];
                f[k][j + 2] -= factor * f[i][j + 2];
                f[k][j + 3] -= factor * f[i][j + 3];
            }
            // 处理剩余部分
            for (; j <= N + 1; ++j) {
                f[k][j] -= factor * f[i][j];
            }
        }
    }

    // 回代过程
    for (int k = N - 1; k >= 1; --k) {
        for (int j = k + 1; j <= N; ++j) {
            f[k][N + 1] -= f[k][j] * f[j][N + 1];
        }
    }
}

int main() {
    input();
    gaussian_elimination();
    for (int i = 1; i <= N; ++i) {
        cout << fixed << setprecision(2) << f[i][N + 1] << endl;
    }
    return 0;
}

*/
//SIMD优化
/*
SIMD指令优化


#include <immintrin.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

const double esp = 1e-6;
int N;
vector<vector<double>> f(2020, vector<double>(2020, 0));


void input() {
    cin >> N;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N + 1; ++j) {
            if(j==N+1)
            f[i][j]=1;
            if (i == j) {
                f[i][j] = 1;
            }
        }
    }
}
void eliminate_simd(int i) {
    for (int k = i + 1; k <= N; ++k) {
        __m256d factor = _mm256_set1_pd(f[k][i]);
        for (int j = i; j <= N + 1; j += 4) {
            __m256d row_i = _mm256_loadu_pd(&f[i][j]);
            __m256d row_k = _mm256_loadu_pd(&f[k][j]);
            __m256d result = _mm256_sub_pd(row_k, _mm256_mul_pd(factor, row_i));
            _mm256_storeu_pd(&f[k][j], result);
        }
    }
}

int solve() {
    for (int i = 1; i <= N; ++i) {
        // 主元选择和行交换
        int r = i;
        for (int k = i; k <= N; ++k) {
            if (fabs(f[k][i]) > esp) {
                r = k;
                break;
            }
        }
        if (r != i) swap(f[i], f[r]);
        if (fabs(f[i][i]) < esp) return 0;

        for (int j = N + 1; j >= i; --j) {
            f[i][j] /= f[i][i];
        }

        eliminate_simd(i);
    }

    // 回代过程
    for (int k = N - 1; k >= 1; --k) {
        for (int j = N; j > k; --j) {
            f[k][N + 1] -= f[j][N + 1] * f[k][j];
        }
    }
    return 1;
}

int main() {
    input();
    if (solve()) {
        for (int i = 1; i <= N; ++i) {
            cout << fixed << setprecision(2) << f[i][N + 1] << endl;
        }
    } else {
        cout << "No Solution" << endl;
    }
    return 0;
}

*/
//OpenMP优化
/*
* OpenMP优化
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <omp.h>
using namespace std;

const double esp = 1e-6;
int N;
vector<vector<double>> f(102, vector<double>(102, 0));

void input() {
    cin >> N;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N + 1; ++j) {
            cin >> f[i][j];
        }
    }
}

int solve() {
    for (int i = 1; i <= N; ++i) {
        // 寻找主元
        int r = i;
        for (int k = i; k <= N; ++k) {
            if (fabs(f[k][i]) > esp) {
                r = k;
                break;
            }
        }
        if (r != i) swap(f[i], f[r]);
        if (fabs(f[i][i]) < esp) return 0;

        // 归一化主元行
        for (int j = N + 1; j >= i; --j) {
            f[i][j] /= f[i][i];
        }

        // 使用 OpenMP 并行化消去
        #pragma omp parallel for
        for (int k = i + 1; k <= N; ++k) {
            for (int j = N + 1; j >= i; --j) {
                f[k][j] -= f[k][i] * f[i][j];
            }
        }
    }

    // 回代
    for (int k = N - 1; k >= 1; --k) {
        for (int j = N; j > k; --j) {
            f[k][N + 1] -= f[j][N + 1] * f[k][j];
        }
    }
    return 1;
}

int main() {
    input();
    if (solve()) {
        for (int i = 1; i <= N; ++i) {
            cout << fixed << setprecision(2) << f[i][N + 1] << endl;
        }
    } else {
        cout << "No Solution" << endl;
    }
    return 0;
}

*/
//MPI编程优化
/*
* MPI编程优化
#include <mpi.h>
#include <vector>
#include <cmath>

const double esp = 1e-6;
int N;
std::vector<std::vector<double>> f;

void input() {
    // 输入代码
}

void mpi_gaussian_elimination() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = N / size;

    for (int i = 1; i <= N; ++i) {
        // 广播主元行
        if (rank == i % size) {
            // 广播
            MPI_Bcast(&f[i][0], N+1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
        } else {
            // 接收
            MPI_Bcast(&f[i][0], N+1, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
        }

        // 消去
        for (int k = i + 1; k <= N; ++k) {
            if (rank == k % size) {
                for (int j = N + 1; j >= i; --j) {
                    f[k][j] -= f[k][i] * f[i][j];
                }
            }
        }
    }
    // 回代逻辑
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    input();
    mpi_gaussian_elimination();
    MPI_Finalize();
    return 0;
}

*/

//使用GPU进行优化
/*
#include <cuda_runtime.h>
#include <vector>
#include <iostream>

__global__ void gaussian_elimination_kernel(double* d_f, int N, int i) {
    int k = blockIdx.x * blockDim.x + threadIdx.x;
    if (k > i && k <= N) {
        for (int j = N + 1; j >= i; --j) {
            d_f[k * (N + 1) + j] -= d_f[k * (N + 1) + i] * d_f[i * (N + 1) + j];
        }
    }
}

void cuda_gaussian_elimination(std::vector<std::vector<double>>& f, int N) {
    double* d_f;
    size_t size = (N + 1) * N * sizeof(double);
    cudaMalloc(&d_f, size);
    cudaMemcpy(d_f, &f[0][0], size, cudaMemcpyHostToDevice);

    for (int i = 0; i < N; ++i) {
        gaussian_elimination_kernel << <(N - i) / 256 + 1, 256 >> > (d_f, N, i);
        cudaDeviceSynchronize();
    }

    cudaMemcpy(&f[0][0], d_f, size, cudaMemcpyDeviceToHost);
    cudaFree(d_f);
}

int main() {
    // 输入初始化
    cuda_gaussian_elimination(f, N);
    // 输出结果
    return 0;
}
*/

//基于Gröbner基的高斯消元
/*
#include<iostream>
#include<iomanip>
#include<vector>
#include<cmath>
#include<fstream>
#include <fstream>  // 用于文件输入输出
#include <string>   // 用于字符串处
#include<sstream>
using namespace std;
const int N = 400;
const int Len = 255;
vector<vector<int> > a(5, vector<int>(90000, 0));//消元子
int c[90000];
int String_to_int(string a) {
    int i = 0;
    int res = 0;
    for (i; i < a.length(); i++) {
        res *= 10;
        res += a[i] - '0';
    }
    return res;
}
string int_to_String(int a) {
    ostringstream os;
    os << a;
    return os.str();
}
vector<int> reca(5, 0);
void input(istringstream s, vector<int> q) {
    string st;
    while (s >> st) {
        q[String_to_int(st)] = 1;
    }
    return;
}
void inFile(string load, vector<int> s) {
    ofstream fil;
    fil.open(load, ios::app);
    bool flag = false;
    for (int i = Len; i >= 0; i--) {
        if (s[i]) {
            if (!flag)   c[i] = 1;
            flag = true;
            fil << int_to_String(i) << " ";
        }
    }

    if (!flag) {
        if(load== "D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt")
        fil << endl;
        return;
    }
    fil << endl;
    fil.close();
    return;
}
vector<int> xiaoyuan(vector<int>s, vector<int>q) {
    for (int i = Len; i >= 0; i--) {
        s[i] = s[i] ^ q[i];
    }
    return s;
}
int  signal[2];
void get_duijiaoxian(int s[]) {

}
void get_xyz() {

}
int main() {

    //文件的初始化
    std::ofstream ofs1("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt", std::ios::trunc); // 使用 std::ios::trunc 标志来清空文件
    ofs1.close(); // 关闭文件流
    std::ofstream ofs2("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res0.txt", std::ios::trunc); // 使用 std::ios::trunc 标志来清空文件
    ofs2.close(); // 关闭文件流
    std::ofstream ofs3("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt", std::ios::trunc); // 使用 std::ios::trunc 标志来清空文件
    ofs3.close(); // 关闭文件流

    std::string sourceFile = "D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//被消元行.txt";
    // 目标文件路径
    std::string targetFile = "D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt";
    // 打开源文件进行读取
    std::ifstream source(sourceFile, std::ios::binary);
    // 打开目标文件进行写入（如果不存在会创建，存在则覆盖）
    std::ofstream target(targetFile, std::ios::binary);
    // 检查文件是否成功打开
    if (!source.is_open() || !target.is_open()) {
        std::cerr << "Error: Could not open files." << std::endl;
        return 1;
    }
    // 使用流的拷贝操作，将源文件内容复制到目标文件
    target << source.rdbuf();
    // 关闭文件流
    source.close();
    target.close();



    ifstream file("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//消元子 - 副本.txt");
    string line;
    int i = 0;
    int curfile = 1;
    string curFile = "res" + int_to_String(curfile) + ".txt";
    ofstream fileoutres;//最终的结果。
    fileoutres.open("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt", ios::app);
    ifstream fileout("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt");  //被消元
    ifstream fileout1("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res0.txt");
    bool flagg = true;
    while (flagg) {
        ifstream fileout("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt");  //被消元
        ifstream fileout1("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res0.txt");
        int num = 0;
        int num1 = 0;
        flagg = true;
        int needle = 0;
        while (a.size() > 5) {
            a.pop_back();
        }
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j <= Len; j++) {
                a[i][j] = 0;
            }
        }
        while (needle < 5 && getline(file, line)) {
            string str;
            istringstream stream(line);
            int flag = false;
            while (stream >> str) {
                if (!flag) {
                    reca[needle] = String_to_int(str);
                    flag = true;
                }
                a[needle][String_to_int(str)] = 1;
            }
            needle++;
        }
        int p = 0;
        while (p < signal[curfile]) {
            getline(curfile == 1 ? fileout : fileout1, line);
            p++;
        }

        while (getline(curfile == 1 ? fileout : fileout1, line)) {// 从文件中读取一行
            signal[curfile]++;
            flagg = false;
            int start = 0;
            string str;
            istringstream stream(line);
            vector<int> b(90000, 0);
            bool flag = true;
            while (stream >> str) {
                if (flag) {
                    start = String_to_int(str);
                    flag = false;
                }
                b[String_to_int(str)] = 1;  //读取被消元素
            }
            flag = false;
            for (int i = 0; i < a.size(); i++) {
                if (start > reca[i]) {
                    flag = true;
                    a.insert(a.begin() + i, b);
                    reca.insert(reca.begin() + i, start);
                    inFile("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt", b);
                    break;
                }
                else if (start < reca[i]) {
                    continue;
                }
                else if (start == reca[i]) {
                    b = xiaoyuan(b, a[i]);
                    for (start; start >= 0; start--) {
                        if (b[start]) {
                            break;
                        }
                    }
                    continue;
                }
            }
            if (!flag) {
                num1++;
                string curF = "res" + int_to_String(curfile ^ 1) + ".txt";
                inFile("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//" + curF, b);
            }
        }
        curfile = curfile ^ 1;
        if (flagg) {
            break;
        }
        fileout.close();
        fileout1.close();
        flagg = true;
    }
    fileout.close();
    fileout1.close();
    fileoutres.close();
    return 0;
}

*/
 
//Pthread优化
/*
Pthread优化
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <pthread.h>

using namespace std;

const int N = 400;
const int Len = 255;
const int NUM_THREADS = 4; // 使用的线程数

vector<vector<int>> a(5, vector<int>(90000, 0)); // 消元子
vector<int> reca(5, 0); // 用于存储消元子的最大元素
int c[90000];
int signal[2] = {0, 0};
pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadData {
    string line;
    int curfile;
    string outputFilePath;
};

int String_to_int(const string &a) {
    int res = 0;
    for (char ch : a) {
        res = res * 10 + (ch - '0');
    }
    return res;
}

string int_to_String(int a) {
    ostringstream os;
    os << a;
    return os.str();
}

void input(istringstream &s, vector<int> &q) {
    string st;
    while (s >> st) {
        q[String_to_int(st)] = 1;
    }
}

void inFile(const string &load, const vector<int> &s) {
    ofstream fil;
    fil.open(load, ios::app);
    bool flag = false;
    for (int i = Len; i >= 0; i--) {
        if (s[i]) {
            if (!flag) c[i] = 1;
            flag = true;
            fil << int_to_String(i) << " ";
        }
    }
    if (!flag) {
        if (load == "D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt")
            fil << endl;
        return;
    }
    fil << endl;
    fil.close();
}

vector<int> xiaoyuan(const vector<int> &s, const vector<int> &q) {
    vector<int> result(s.size());
    for (int i = 0; i <= Len; i++) {
        result[i] = s[i] ^ q[i];
    }
    return result;
}

void *processLine(void *threadArg) {
    ThreadData *data = (ThreadData *) threadArg;
    string line = data->line;
    int curfile = data->curfile;
    string outputFilePath = data->outputFilePath;

    istringstream stream(line);
    vector<int> b(90000, 0);
    string str;
    int start = 0;
    bool flag = true;
    while (stream >> str) {
        if (flag) {
            start = String_to_int(str);
            flag = false;
        }
        b[String_to_int(str)] = 1;
    }

    pthread_mutex_lock(&fileMutex); // 加锁
    flag = false;
    for (int i = 0; i < a.size(); i++) {
        if (start > reca[i]) {
            flag = true;
            a.insert(a.begin() + i, b);
            reca.insert(reca.begin() + i, start);
            inFile("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt", b);
            break;
        } else if (start < reca[i]) {
            continue;
        } else if (start == reca[i]) {
            b = xiaoyuan(b, a[i]);
            for (; start >= 0; start--) {
                if (b[start]) break;
            }
            continue;
        }
    }
    if (!flag) {
        string curF = "res" + int_to_String(curfile ^ 1) + ".txt";
        inFile("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//" + curF, b);
    }
    pthread_mutex_unlock(&fileMutex); // 解锁
    pthread_exit(nullptr);
}

int main() {
    // 文件的初始化
    std::ofstream ofs1("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt", std::ios::trunc);
    ofs1.close();
    std::ofstream ofs2("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res0.txt", std::ios::trunc);
    ofs2.close();
    std::ofstream ofs3("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt", std::ios::trunc);
    ofs3.close();

    std::string sourceFile = "D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//被消元行.txt";
    std::string targetFile = "D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt";

    // 拷贝文件内容
    std::ifstream source(sourceFile, std::ios::binary);
    std::ofstream target(targetFile, std::ios::binary);
    if (!source.is_open() || !target.is_open()) {
        std::cerr << "Error: Could not open files." << std::endl;
        return 1;
    }
    target << source.rdbuf();
    source.close();
    target.close();

    ifstream file("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//消元子 - 副本.txt");
    string line;
    int curfile = 1;
    string curFile = "res" + int_to_String(curfile) + ".txt";

    ofstream fileoutres;
    fileoutres.open("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt", ios::app);

    ifstream fileout("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt");
    ifstream fileout1("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res0.txt");

    bool flagg = true;
    while (flagg) {
        ifstream fileout("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt");
        ifstream fileout1("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res0.txt");
        flagg = false;
        int needle = 0;
        while (a.size() > 5) {
            a.pop_back();
        }
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j <= Len; j++) {
                a[i][j] = 0;
            }
        }
        while (needle < 5 && getline(file, line)) {
            istringstream stream(line);
            int flag = false;
            while (stream >> str) {
                if (!flag) {
                    reca[needle] = String_to_int(str);
                    flag = true;
                }
                a[needle][String_to_int(str)] = 1;
            }
            needle++;
        }

        vector<pthread_t> threads(NUM_THREADS);
        vector<ThreadData> threadData(NUM_THREADS);
        int threadCount = 0;

        ifstream &currentFile = (curfile == 1) ? fileout : fileout1;

        while (getline(currentFile, line)) {
            if (threadCount == NUM_THREADS) {
                for (int i = 0; i < NUM_THREADS; i++) {
                    pthread_join(threads[i], nullptr);
                }
                threadCount = 0;
            }

            threadData[threadCount] = {line, curfile, curFile};
            pthread_create(&threads[threadCount], nullptr, processLine, (void *)&threadData[threadCount]);
            threadCount++;
        }

        for (int i = 0; i < threadCount; i++) {
            pthread_join(threads[i], nullptr);
        }

        curfile = curfile ^ 1;
        if (flagg) {
            break;
        }
        fileout.close();
        fileout1.close();
        flagg = true;
    }
    fileout.close();
    fileout1.close();
    fileoutres.close();
    return 0;
}

*/

//手机用OpenMP进行并行化优化
/*
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <omp.h>

using namespace std;

const int N = 400;
const int Len = 255;
const int NUM_THREADS = 4; // 使用的线程数

vector<vector<int>> a(5, vector<int>(90000, 0)); // 消元子
vector<int> reca(5, 0); // 用于存储消元子的最大元素
int c[90000];

int String_to_int(const string &a) {
    int res = 0;
    for (char ch : a) {
        res = res * 10 + (ch - '0');
    }
    return res;
}

string int_to_String(int a) {
    ostringstream os;
    os << a;
    return os.str();
}

void inFile(const string &load, const vector<int> &s) {
    ofstream fil;
    fil.open(load, ios::app);
    bool flag = false;
    for (int i = Len; i >= 0; i--) {
        if (s[i]) {
            if (!flag) c[i] = 1;
            flag = true;
            fil << int_to_String(i) << " ";
        }
    }
    if (!flag) {
        if (load == "D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt")
            fil << endl;
        return;
    }
    fil << endl;
    fil.close();
}

vector<int> xiaoyuan(const vector<int> &s, const vector<int> &q) {
    vector<int> result(s.size());
    for (int i = 0; i <= Len; i++) {
        result[i] = s[i] ^ q[i];
    }
    return result;
}

int main() {
    // 文件的初始化
    std::ofstream ofs1("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt", std::ios::trunc);
    ofs1.close();
    std::ofstream ofs2("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res0.txt", std::ios::trunc);
    ofs2.close();
    std::ofstream ofs3("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt", std::ios::trunc);
    ofs3.close();

    std::string sourceFile = "D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//被消元行.txt";
    std::string targetFile = "D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt";

    // 拷贝文件内容
    std::ifstream source(sourceFile, std::ios::binary);
    std::ofstream target(targetFile, std::ios::binary);
    if (!source.is_open() || !target.is_open()) {
        std::cerr << "Error: Could not open files." << std::endl;
        return 1;
    }
    target << source.rdbuf();
    source.close();
    target.close();

    ifstream file("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//消元子 - 副本.txt");
    string line;
    int curfile = 1;
    string curFile = "res" + int_to_String(curfile) + ".txt";

    ofstream fileoutres;
    fileoutres.open("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt", ios::app);

    bool flagg = true;
    while (flagg) {
        ifstream fileout("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res1.txt");
        ifstream fileout1("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res0.txt");

        int num = 0;
        flagg = true;
        int needle = 0;

        while (a.size() > 5) {
            a.pop_back();
        }

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j <= Len; j++) {
                a[i][j] = 0;
            }
        }

        while (needle < 5 && getline(file, line)) {
            istringstream stream(line);
            int flag = false;
            while (stream >> line) {
                if (!flag) {
                    reca[needle] = String_to_int(line);
                    flag = true;
                }
                a[needle][String_to_int(line)] = 1;
            }
            needle++;
        }

        omp_set_num_threads(NUM_THREADS);

        #pragma omp parallel
        {
            int thread_id = omp_get_thread_num();
            int num_threads = omp_get_num_threads();
            bool localFlag = false;

            #pragma omp for
            for (int i = 0; i < num_threads; i++) {
                ifstream &currentFile = (curfile == 1) ? fileout : fileout1;

                int linesProcessed = 0;
                while (getline(currentFile, line)) {
                    linesProcessed++;
                    istringstream stream(line);
                    vector<int> b(90000, 0);
                    string str;
                    int start = 0;
                    bool flag = true;
                    while (stream >> str) {
                        if (flag) {
                            start = String_to_int(str);
                            flag = false;
                        }
                        b[String_to_int(str)] = 1;
                    }

                    #pragma omp critical
                    {
                        flag = false;
                        for (int i = 0; i < a.size(); i++) {
                            if (start > reca[i]) {
                                flag = true;
                                a.insert(a.begin() + i, b);
                                reca.insert(reca.begin() + i, start);
                                inFile("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//res2.txt", b);
                                break;
                            } else if (start < reca[i]) {
                                continue;
                            } else if (start == reca[i]) {
                                b = xiaoyuan(b, a[i]);
                                for (; start >= 0; start--) {
                                    if (b[start]) break;
                                }
                                continue;
                            }
                        }
                        if (!flag) {
                            string curF = "res" + int_to_String(curfile ^ 1) + ".txt";
                            inFile("D://Gusee//Groebner//测试样例2 矩阵列数254，非零消元子106，被消元行53//" + curF, b);
                        }
                    }

                    if (linesProcessed % num_threads == thread_id) {
                        signal[curfile]++;
                        localFlag = true;
                    }
                }
                if (localFlag) flagg = false;
            }
        }

        curfile = curfile ^ 1;
        if (flagg) {
            break;
        }
        fileout.close();
        fileout1.close();
    }

    fileoutres.close();
    return 0;
}

*/

//使用MPI多进程进行编程
/*
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <mpi.h> // 引入MPI头文件

using namespace std;

const int N = 400;
const int Len = 255;

vector<vector<int>> a(5, vector<int>(90000, 0)); // 消元子
int c[90000];
vector<int> reca(5, 0);
int signal[2];

int String_to_int(string a) {
    int res = 0;
    for (char ch : a) {
        res = res * 10 + (ch - '0');
    }
    return res;
}

string int_to_String(int a) {
    ostringstream os;
    os << a;
    return os.str();
}

void input(istringstream& s, vector<int>& q) {
    string st;
    while (s >> st) {
        q[String_to_int(st)] = 1;
    }
}

void inFile(const string& load, const vector<int>& s) {
    ofstream fil(load, ios::app);
    bool flag = false;
    for (int i = Len; i >= 0; i--) {
        if (s[i]) {
            if (!flag) c[i] = 1;
            flag = true;
            fil << int_to_String(i) << " ";
        }
    }
    if (!flag && load == "res2.txt") {
        fil << endl;
    }
    fil << endl;
}

vector<int> xiaoyuan(vector<int> s, const vector<int>& q) {
    for (int i = Len; i >= 0; i--) {
        s[i] = s[i] ^ q[i];
    }
    return s;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv); // 初始化MPI环境

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // 获取当前进程的rank
    MPI_Comm_size(MPI_COMM_WORLD, &size); // 获取总进程数

    if (rank == 0) { // 主进程
        std::ofstream ofs1("res1.txt", std::ios::trunc);
        std::ofstream ofs2("res0.txt", std::ios::trunc);
        std::ofstream ofs3("res2.txt", std::ios::trunc);
    }

    MPI_Barrier(MPI_COMM_WORLD); // 同步所有进程

    ifstream file("消元子 - 副本.txt");
    string line;
    int curfile = 1;

    ifstream fileout("res1.txt");  //被消元
    ifstream fileout1("res0.txt");

    while (true) {
        if (rank == 0) { // 主进程负责读取文件并分配任务
            int needle = 0;
            while (needle < 5 && getline(file, line)) {
                istringstream stream(line);
                string str;
                int flag = false;
                while (stream >> str) {
                    if (!flag) {
                        reca[needle] = String_to_int(str);
                        flag = true;
                    }
                    a[needle][String_to_int(str)] = 1;
                }
                needle++;
            }
        }

        MPI_Bcast(reca.data(), reca.size(), MPI_INT, 0, MPI_COMM_WORLD); // 广播reca
        for (int i = 0; i < 5; i++) {
            MPI_Bcast(a[i].data(), 90000, MPI_INT, 0, MPI_COMM_WORLD); // 广播a
        }

        int num_lines;
        vector<string> lines;
        if (rank == 0) { // 主进程读取被消元行
            string curfile_name = "res" + int_to_String(curfile) + ".txt";
            ifstream infile(curfile_name);
            string line;
            while (getline(infile, line)) {
                lines.push_back(line);
            }
            num_lines = lines.size();
        }

        MPI_Bcast(&num_lines, 1, MPI_INT, 0, MPI_COMM_WORLD); // 广播行数

        int lines_per_proc = (num_lines + size - 1) / size;
        vector<string> local_lines(lines_per_proc);

        MPI_Scatter(lines.data(), lines_per_proc, MPI_CHAR, local_lines.data(), lines_per_proc, MPI_CHAR, 0, MPI_COMM_WORLD); // 分发行

        vector<vector<int>> results;
        for (const string& line : local_lines) {
            if (line.empty()) continue;
            int start = 0;
            istringstream stream(line);
            string str;
            vector<int> b(90000, 0);
            bool flag = true;
            while (stream >> str) {
                if (flag) {
                    start = String_to_int(str);
                    flag = false;
                }
                b[String_to_int(str)] = 1;
            }

            flag = false;
            for (int i = 0; i < a.size(); i++) {
                if (start > reca[i]) {
                    flag = true;
                    a.insert(a.begin() + i, b);
                    reca.insert(reca.begin() + i, start);
                    inFile("res2.txt", b);
                    break;
                } else if (start < reca[i]) {
                    continue;
                } else if (start == reca[i]) {
                    b = xiaoyuan(b, a[i]);
                    for (start; start >= 0; start--) {
                        if (b[start]) break;
                    }
                    continue;
                }
            }
            if (!flag) {
                results.push_back(b);
            }
        }

        // 收集结果
        vector<int> result_sizes(size);
        int local_size = results.size();
        MPI_Gather(&local_size, 1, MPI_INT, result_sizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

        vector<int> displs(size, 0);
        int total_size = 0;
        for (int i = 0; i < size; i++) {
            displs[i] = total_size;
            total_size += result_sizes[i];
        }

        vector<vector<int>> all_results(total_size, vector<int>(90000, 0));
        MPI_Gatherv(results.data(), local_size * 90000, MPI_INT, all_results.data(), result_sizes.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

        if (rank == 0) { // 主进程输出结果
            for (const auto& b : all_results) {
                inFile("res" + int_to_String(curfile ^ 1) + ".txt", b);
            }
        }

        curfile ^= 1;
        if (rank == 0 && file.eof()) break; // 终止条件

        MPI_Barrier(MPI_COMM_WORLD); // 同步
    }

    MPI_Finalize(); // 终止MPI环境
    return 0;
}

*/


//使用GPU编程
/*
#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

const int N = 400;
const int Len = 255;
const int MAX_SIZE = 90000;

__global__ void xiaoyuan_kernel(int* s, int* q, int len) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx <= len) {
        s[idx] ^= q[idx];
    }
}

__host__ int String_to_int(const string& a) {
    int res = 0;
    for (char ch : a) {
        res = res * 10 + (ch - '0');
    }
    return res;
}

__host__ string int_to_String(int a) {
    ostringstream os;
    os << a;
    return os.str();
}

__host__ void input(istringstream& s, vector<int>& q) {
    string st;
    while (s >> st) {
        q[String_to_int(st)] = 1;
    }
}

__host__ void inFile(const string& load, const vector<int>& s) {
    ofstream fil(load, ios::app);
    bool flag = false;
    for (int i = Len; i >= 0; i--) {
        if (s[i]) {
            flag = true;
            fil << int_to_String(i) << " ";
        }
    }
    fil << endl;
}

int main() {
    vector<vector<int>> a(5, vector<int>(MAX_SIZE, 0));
    vector<int> reca(5, 0);
    int c[MAX_SIZE] = {0};
    int* d_b;
    int* d_q;

    cudaMalloc((void**)&d_b, MAX_SIZE * sizeof(int));
    cudaMalloc((void**)&d_q, MAX_SIZE * sizeof(int));

    // 清空结果文件
    std::ofstream ofs1("res1.txt", std::ios::trunc);
    std::ofstream ofs2("res0.txt", std::ios::trunc);
    std::ofstream ofs3("res2.txt", std::ios::trunc);
    ofs1.close();
    ofs2.close();
    ofs3.close();

    ifstream file("消元子.txt");
    string line;
    int curfile = 1;
    string curFile = "res" + int_to_String(curfile) + ".txt";
    ofstream fileoutres("res2.txt", ios::app);
    ifstream fileout("res1.txt");
    ifstream fileout1("res0.txt");
    bool flagg = true;
    int signal[2] = {0, 0};

    while (flagg) {
        ifstream fileout("res1.txt");
        ifstream fileout1("res0.txt");
        int needle = 0;

        while (needle < 5 && getline(file, line)) {
            string str;
            istringstream stream(line);
            int flag = false;
            while (stream >> str) {
                if (!flag) {
                    reca[needle] = String_to_int(str);
                    flag = true;
                }
                a[needle][String_to_int(str)] = 1;
            }
            needle++;
        }

        int p = 0;
        while (p < signal[curfile]) {
            getline(curfile == 1 ? fileout : fileout1, line);
            p++;
        }

        while (getline(curfile == 1 ? fileout : fileout1, line)) {
            signal[curfile]++;
            flagg = false;
            int start = 0;
            string str;
            istringstream stream(line);
            vector<int> b(MAX_SIZE, 0);
            bool flag = true;
            while (stream >> str) {
                if (flag) {
                    start = String_to_int(str);
                    flag = false;
                }
                b[String_to_int(str)] = 1;
            }
            flag = false;

            cudaMemcpy(d_b, b.data(), MAX_SIZE * sizeof(int), cudaMemcpyHostToDevice);

            for (int i = 0; i < needle; i++) {
                if (start > reca[i]) {
                    flag = true;
                    a.insert(a.begin() + i, b);
                    reca.insert(reca.begin() + i, start);
                    inFile("res2.txt", b);
                    break;
                } else if (start < reca[i]) {
                    continue;
                } else if (start == reca[i]) {
                    cudaMemcpy(d_q, a[i].data(), MAX_SIZE * sizeof(int), cudaMemcpyHostToDevice);
                    int blockSize = 256;
                    int numBlocks = (MAX_SIZE + blockSize - 1) / blockSize;
                    xiaoyuan_kernel<<<numBlocks, blockSize>>>(d_b, d_q, MAX_SIZE);

                    cudaMemcpy(b.data(), d_b, MAX_SIZE * sizeof(int), cudaMemcpyDeviceToHost);

                    for (; start >= 0; start--) {
                        if (b[start]) break;
                    }
                    continue;
                }
            }
            if (!flag) {
                string curF = "res" + int_to_String(curfile ^ 1) + ".txt";
                inFile(curF, b);
            }
        }
        curfile = curfile ^ 1;
        if (flagg) break;
        fileout.close();
        fileout1.close();
        flagg = true;
    }
    fileout.close();
    fileout1.close();
    fileoutres.close();

    cudaFree(d_b);
    cudaFree(d_q);

    return 0;
}

*/