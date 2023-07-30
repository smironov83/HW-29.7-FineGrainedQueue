#pragma once
#include <shared_mutex>
/*Проверка на утечки памяти.
#include <vld.h>*/

struct Node
{
  Node(int val): value_(val) {};
  ~Node() { delete nodeMutex_; };
  int value_ = 0;
  Node *next_ = nullptr;
  std::mutex *nodeMutex_ = new std::mutex {};
};

class FineGrainedQueue
{
  Node *head_ = nullptr;
  std::mutex *queueMutex_ = new std::mutex {};
  int &nodeCount_;
public:
  FineGrainedQueue(int &nodeCount_);
  ~FineGrainedQueue();
  void PushBack(int val);
  void PrintNodes();
  void Insert(int val, int pos);
  void Restore();
};
