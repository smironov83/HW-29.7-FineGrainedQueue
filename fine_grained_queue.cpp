#include <iostream>
#include <iomanip>
#include "fine_grained_queue.h"

//Конструктор.
FineGrainedQueue::FineGrainedQueue(int &nodeCount): nodeCount_(nodeCount)
{
}

//Деструктор. Последовательно удаляет узлы списка. Очищает память.
FineGrainedQueue::~FineGrainedQueue()
{
  Node *node = head_;
  while (node != nullptr)
  {
    node = head_->next_;
    delete head_;
    head_ = node;
  }
  delete queueMutex_;
}

/*Помещает новый элемент в конец списка. Если список пуст - размещает первым
элементом. При вставке в уже готовый список вариант пустого списка не
рассматривается, исходя из условий задания.*/
void FineGrainedQueue::PushBack(int val)
{
  Node *newNode = new Node(val);
  newNode->nodeMutex_->lock();
  if (head_ == nullptr)
  {
    head_ = newNode;
    newNode->nodeMutex_->unlock();
    return;
  }
  queueMutex_->lock();
  Node *curNode = this->head_;
  curNode->nodeMutex_->lock();
  queueMutex_->unlock();
  while (curNode->next_ != nullptr)
  {
    Node *nextNode = curNode->next_;
    nextNode->nodeMutex_->lock();
    curNode->nodeMutex_->unlock();
    curNode = nextNode;
  }
  curNode->next_ = newNode;
  curNode->nodeMutex_->unlock();
  curNode->next_->nodeMutex_->unlock();
}

//Выводит список значений узлов в консоль.
void FineGrainedQueue::PrintNodes()
{
  std::cout << std::setiosflags(std::ios::left);
  std::cout << "Текущий односвязный список:" << std::endl;
  Node *curNode = head_;
  while (curNode != nullptr)
  {
    std::cout << std::setw(5) << curNode->value_;
    curNode = curNode->next_;
  }
  std::cout << std::endl;
}

/*Осуществляет вставку нового узла со значением val в позицию pos. Т.к.
по условию задания "очередь не пустая" и "вставку в начало списка не
нужно рассматривать", то варианты пустого списка и вставки в начало
списка исключены.*/
void FineGrainedQueue::Insert(int val, int pos)
{
  if (pos > nodeCount_ - 1)
    PushBack(val);
  else
  {
    Node *newNode = new Node(val);
    newNode->nodeMutex_->lock();
    queueMutex_->lock();
    Node *curNode = head_;
    curNode->nodeMutex_->lock();
    queueMutex_->unlock();
    int curPos = 0;
    while (curPos < pos - 1)
    {
      Node *nextNode = curNode->next_;
      nextNode->nodeMutex_->lock();
      curNode->nodeMutex_->unlock();
      curNode = nextNode;
      ++curPos;
    }
    Node *nextNode = curNode->next_;
    nextNode->nodeMutex_->lock();
    curNode->next_ = newNode;
    newNode->next_ = nextNode;
    curNode->nodeMutex_->unlock();
    curNode->next_->nodeMutex_->unlock();
    newNode->next_->nodeMutex_->unlock();
  }
  queueMutex_->lock();
  ++nodeCount_;
  queueMutex_->unlock();
}
/*Восстанавливает исходный список узлов путем удаления вставок - узлы со
значением >999. Т.к. вставки в начало списка нет по условиям задания, то
вариант удаления первого узла не рассматривается.*/
void FineGrainedQueue::Restore()
{
  Node *tempNode = head_, *prevNode = nullptr;
  while (tempNode != nullptr)
    if (tempNode->value_ > 999 && prevNode)
    {
      prevNode->next_ = tempNode->next_;
      delete tempNode;
      tempNode = prevNode->next_;
      --nodeCount_;
    }
    else
    {
      prevNode = tempNode;
      tempNode = tempNode->next_;
    }
}