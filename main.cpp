/*Все значения позиционных переменных начинаются с 0. Т.е.первый элемент списка
- [0], первая позиция - pos[0]. Для удобства тестирования и визуального
восприятия полученных результатов значения базового списка трехзначные, а
вставок - четырехзначные.*/
#include <iostream>
#include <iomanip>
#include <vector>
#include "fine_grained_queue.h"

//Вкл./выкл. режим тестирования - выводит все данные в консоль.
auto constexpr TEST_MODE_ENABLED = false;
auto constexpr MAX_THREADS = 10;
auto NODE_COUNT = 100000;

auto main() -> int
{
  system("chcp 1251");
  system("cls");
  srand((unsigned int) time(NULL));

  //Создает односвязный список со случайными значениями 3-хзначных чисел.
  FineGrainedQueue fgq(NODE_COUNT);
  std::vector<std::thread>listNodes;
  for (size_t i = 0; i < MAX_THREADS; ++i)
    listNodes.push_back(std::thread([&fgq, i] ()
      { 
        srand((unsigned int) (time(NULL) + i));
        for (auto i = 0; i < NODE_COUNT / MAX_THREADS; ++i)
          fgq.PushBack(100 + rand() % (900)); 
      }));
  for (auto &th : listNodes)
    if (th.joinable())
      th.join();

  /*Создает случайный список пар значение - позиция для вставки в односвязный
  список. Согласно условию задачи позиция начала списка исключена.*/
  std::vector<int> insertionList;
  for (auto i = 0; i < MAX_THREADS; ++i)
  {
    //Генерирует значение.
    insertionList.push_back(1000 + rand() % 9000);

    /*Генерирует позицию для вставки. Удваивает при генерации (* 2) диапазон
    позиций вставки относительно длины списка, чтобы получить выход за пределы
    длины списка для отработки пункта задания "если pos больше длины списка" и
    (+ 1) для пункта "вставку в начало списка не нужно рассматривать".*/
    insertionList.push_back(rand() % (int) (NODE_COUNT * 2) + 1);
  }
  std::cout << "Количество узлов исходного списка: " << NODE_COUNT
    << std::endl << std::endl;
  //Блок кода для тестирования.
  if (TEST_MODE_ENABLED)
  {
    //Выводит исходный список для проверки корректной работы алгоритмов.
    fgq.PrintNodes();
    system("pause");
    std::cout << std::endl;

    //Выводит полученный список пар для тестирования.
    std::cout << "Данные для вставки:" << std::endl;
    std::cout << "VAL \tPOS" << std::endl;
    for (size_t i = 0; i < insertionList.size(); i += 2)
      std::cout << insertionList[i] << "\t" << insertionList[i + 1]
      << std::endl;
    system("pause");
    std::cout << std::endl;
  }
  //Выполняет вставку в однопоточном режиме.
  auto timeStart = std::chrono::system_clock::now();
  for (size_t i = 0; i < insertionList.size(); i += 2)
    fgq.Insert(insertionList[i], insertionList[i + 1]);
  auto timeFinish = std::chrono::system_clock::now();
  auto timeSpent = std::chrono::duration_cast<std::chrono::milliseconds>
    (timeFinish - timeStart).count();
  std::cout << "Вставки в однопоточном режиме выполнена за " << timeSpent
    << " мс." << std::endl << std::endl;
  std::cout << "Количество узлов списка после вставок: " << NODE_COUNT
    << std::endl << std::endl;
  //Блок кода для тестирования.
  if (TEST_MODE_ENABLED)
  {
    fgq.PrintNodes();
    system("pause");
    std::cout << std::endl;
  }
  //Возвращает список в исходный вид.
  std::cout << "Восстанавливаем исходный односвязный список..." << std::endl
    << std::endl;
  fgq.Restore();
  std::cout << "Количество узлов после восстановления исходного списка: "
    << NODE_COUNT << std::endl << std::endl;
  //Блок кода для тестирования.
  if (TEST_MODE_ENABLED)
  {
    fgq.PrintNodes();
    system("pause");
    std::cout << std::endl;
  }
  //Выполняет вставку в многопоточном режиме.
  timeStart = std::chrono::system_clock::now();
  std::vector<std::thread>threads;
  for (size_t i = 0; i < MAX_THREADS; ++i)
    threads.push_back(std::thread([&fgq, i, &insertionList] ()
      { fgq.Insert(insertionList[i * 2], insertionList[i * 2 + 1]); }));
  for (auto &th : threads)
    if (th.joinable())
      th.join();
  timeFinish = std::chrono::system_clock::now();
  timeSpent = std::chrono::duration_cast<std::chrono::milliseconds>
    (timeFinish - timeStart).count();
  std::cout << "Вставки в многопоточном режиме выполнены за " << timeSpent
    << " мс." << std::endl << std::endl;
  std::cout << "Количество узлов списка после вставок: " << NODE_COUNT
    << std::endl << std::endl;
  //Блок кода для тестирования.
  if (TEST_MODE_ENABLED)
  {
    fgq.PrintNodes();
    system("pause");
    std::cout << std::endl;
  }
  return 0;
}