#include "Rain.h"
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdexcept>
#include <pthread.h>

using std::string;

const int n = 100;

#define RAIN_SIZE 20
#define RANDOM_RAIN_SIZE 30
#define RAIN_SPEED 250000
#define RANDOM_RAIN_SPEED 50000
#define PRINT_UPDATE_SPEED RAIN_SPEED / 3

typedef std::vector<Rain*> Row;
typedef std::vector<Row> Matrix;

Matrix mat;

void* print(void*);
void* rain(void*);

int main() {

    mat.resize(n, Row(n));

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {

            mat[i][j] = new Rain();
            mat[i][j]->disable();
        }
    }

    std::vector<pthread_t> threads(mat.size() / 2);

    for(int i = 0; i < threads.size(); ++i) {

        std::srand(seed++);
        pthread_create(&threads[i], NULL, rain, new int(i * 2));
    }

    pthread_t thread_print;
    pthread_create(&thread_print, NULL, print, NULL);
    threads.push_back(thread_print);

    for(int i = 0; i < threads.size(); ++i)
        pthread_join(threads[i], NULL);

    for(int i = 0; i < threads.size(); ++i)
        pthread_cancel(threads[i]);
}

int get_last_in_column(Matrix const& m, int i) {

    int last = 0;
    for(int j = 0; j < m.size(); ++j) {

        if(m[j][i]->is_enabled()) {

            last = j;
        }
    }
    return last;
}

void* rain(void* arg) {

    int i = *((int *)arg);

    while(true) {

        std::srand(seed++);
        int speed = std::rand() % (RAIN_SPEED) + RANDOM_RAIN_SPEED;
        int size = std::rand() % RAIN_SIZE + RANDOM_RAIN_SIZE;

        for(int j = 0; j < mat.size(); ++j) {

            mat[j][i]->enable();
            if(j >= size) {

                mat[j - size][i]->disable();
            }
            usleep(speed);
        }

        for(int j = mat.size() - size; j < mat.size(); ++j) {

            mat[j][i]->disable();
            usleep(speed);
        }
    }
}

void* print(void*)
{
    while(true) {

        int n = mat.size();
        system("clear");    
        
        for(int i = 0; i < n; ++i) {

            for(int j = 0; j < n; ++j) {

                if(mat[i][j]->is_enabled()) {

                    std::cout << "\033[0;32m";
                    if(i == get_last_in_column(mat, j) && i != mat.size() - 1) {

                        std::cout << "\033[1;37m";
                    }
                    std::cout << *mat[i][j];
                }
                else {

                    std::cout << ' ';
                }
            }
            std::cout << std::endl;
        }
        usleep(PRINT_UPDATE_SPEED);
    }
}