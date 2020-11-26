#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<random>

using namespace std;

int main(int argc, char *argv[]) {
    std::random_device rd;

    std::mt19937 e2(rd());
    //std::knuth_b e2(rd());
    //std::default_random_engine e2(rd()) ;

    //
    // Distribtuions
    //
    std::uniform_real_distribution<> dist(0, 10);
    // std::uniform_real_distribution<> dist(0, 10);
    //std::normal_distribution<> dist(2, 2);
    //std::student_t_distribution<> dist(5);
    //std::poisson_distribution<> dist(2);
    //std::extreme_value_distribution<> dist(0,2);

    if (argc != 3) {
        cerr << "Usage: ./grid_gen M N" << endl;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);

    ofstream of;
    of.open("test.txt", ios::trunc);
    of << M << " " << N << endl;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            of << dist(e2);
            if (j != N - 1) of << " ";
        }
        of << endl;
    }

    of.close();

    return 0;
}