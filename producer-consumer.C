/*
 * Producer-Consumer problem with an unbounded queue
 *   uses semaphores
 */
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

using namespace std;

class Rider;

class PCQueue {
public:
  PCQueue();
  void enqueue(Rider *it);
  Rider *dequeue();
private:
  queue<Rider *> q;
  pthread_mutex_t lock; // protects q
  sem_t Qsize;
};

PCQueue::PCQueue()
{
  pthread_mutex_init(&lock,NULL);
  sem_init(&Qsize,0,0);
}

// Add an Rider to the queue
void PCQueue::enqueue(Rider *it)
{
  pthread_mutex_lock(&lock);

  q.push(it);

  pthread_mutex_unlock(&lock);

  sem_post(&Qsize);
}

// Sleep until there is an Rider on the queue, then return it.
Rider *PCQueue::dequeue()
{
  Rider *it;

  sem_wait(&Qsize);

  pthread_mutex_lock(&lock);

  assert(!q.empty());
  it = q.front();
  q.pop();

  pthread_mutex_unlock(&lock);

  return it;
}

