#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <ctime>

/*
 * Работу выполнил Филатов Юрий Алексеевич
 * Группа: БПИ193
 * Вариант определен: 23 % 22 = 1.
 * Задача о парикмахере.
 *
 * Условие задания:
 * В тихом городке есть парикмахерская.
 * Салон парикмахерской мал, ходить там может только парикмахер и один посетитель.
 * Парикмахер всю жизнь обслуживает посетителей.
 * Когда в салоне никого нет, он спит в кресле.
 * Когда посетитель приходит и видит спящего парикмахера, он будет его,
 * садится в кресло и спит, пока парикмахер занят стрижкой.
 * Если посетитель приходит, а парикмахер занят, то он встает в очередь и засыпает.
 * После стрижки парикмахер сам провожает посетителя.
 * Если есть ожидающие посетители, то парикмахер будит одного из них и ждет
 * пока тот сядет в кресло парикмахера и начинает стрижку.
 * Если никого нет, он снова садится в свое кресло и засыпает до прихода посетителя.
 * Создать многопоточное приложение, моделирующее рабочий день парикмахерской.
 *
 */

using namespace std;

//типа причесок
string hairCuts[12] =
        {"Crop",
         "Flat-top",
         "Fade",
         "Pompadour",
         "Irokez",
         "Quiff",
         "Canada",
         "Side part",
         "Undercut",
         "Man bun",
         "Mullet",
         "Care"};

//номер посетителя
int v = 0;

bool armChair = false; //кресло
const int queueSize = 10000;
int visitors[queueSize];

sem_t chair; //семафор, отображающий кресло и претендентов на попадание в него

pthread_mutex_t mutexChair; //мутекс для кресла

//генерация очереди
void *hairCutting(void *param) {
    while (true) {
            //критическая секция
            pthread_mutex_lock(&mutexChair); //защита очереди
            sem_wait(&chair);
            visitors[v] = v;
            cout << "Visitor" + to_string(v) + " sitting down to the chair.\n";
            armChair = true;    //имитируем, что посетитель сел в кресло
            string str = armChair ? " sitting in the chair.\n" : "THIS STRING NEVER USED, BECAUSE OF MUTEXES";
            cout << "Visitor" + to_string(v) + str;
            cout << "Visitor" + to_string(v) + " have done a haircut - " + hairCuts[rand() % 12] + ".\n";
            armChair = false;   //имитируем, что посетитель встал с кресла
            v++;
            sem_post(&chair);
            pthread_mutex_unlock(&mutexChair);
            //break;
    }
}


int main() {
    srand(time(NULL));

    //инициализация мутекса и семафора
    pthread_mutex_init(&mutexChair, nullptr);
    sem_init(&chair, 0, 1); //количество свободных ячеек равно 1 - всего одно кресло

    //создание очереди
    pthread_t threadQueue[queueSize];

    for (int i = 0; i < queueSize; ++i) {
        pthread_create(&threadQueue[i], nullptr, hairCutting, (void *) (0));
    }

    //пока в очереди стоят люди пропускаем их
    while (v <= queueSize) {
    }

    return 0;
}
