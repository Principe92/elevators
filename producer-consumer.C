/*
 * Producer-Consumer problem with an unbounded queue
 *   uses semaphores
 */
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

using namespace std;

class Object;

class PCQueue {
public:
  PCQueue();
  void enqueue(Object *it);
  Object *dequeue();
private:
  queue<Object *> q;
  pthread_mutex_t lock; // protects q
  sem_t Qsize;
};

PCQueue::PCQueue()
{
  pthread_mutex_init(&lock,NULL);
  sem_init(&Qsize,0,0);
}

// Add an Object to the queue
void PCQueue::enqueue(Object *it)
{
  pthread_mutex_lock(&lock);

  q.push(it);

  pthread_mutex_unlock(&lock);

  sem_post(&Qsize);
}

// Sleep until there is an Object on the queue, then return it.
Object *PCQueue::dequeue()
{
  Object *it;

  sem_wait(&Qsize);

  pthread_mutex_lock(&lock);

  assert(!q.empty());
  it = q.front();
  q.pop();

  pthread_mutex_unlock(&lock);

  return it;
}

