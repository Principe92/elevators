//
//    elevators.C
//
//    Function stubs for the missing parts of elevators.
//
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include "building.h"
#include "elevators.h"
#include "producer-consumer.C"
#include <queue>
#include <string.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

PCQueue passengers;
int peeps = 0;
int or0;
int onboard = 0;
//pthread_mutex_t plock;
//pthread_mutex_init(&plock, NULL);

//vector<PCQueue> passengers;
//vector<int> peeps;

struct Rider{
  const Person *name; int origin, destination;
  sem_t semwait;
};

PCQueue riders[11];
PCQueue waitup[11];
PCQueue waitdown[11];
sem_t waiters;

//
// Elevator constructor
//   Called once for each elevator before the thread is created.
//
Elevator::Elevator()
{ 
  // This is the place to perform any one-time initializations of
  // per-elevator data.
}

//
// Elevator::display_passengers()
//
//  Call display() for each Person on the elevator.
//  Return the number of riders.
//
//  Beware: calling message() from this function will hang the simulation.
//
int Elevator::display_passengers()
{
// for each floor, cycle through the passengers that are supposed to get off there
  for (int i = 0; i < 11; i++){
    for (int j = 0; j < riders[i].size(); j++){
      Rider* temp = riders[i].dequeue();
      temp->name->display();
      riders[i].enqueue(temp);
    }
  }
  return onboard;
}

//
// Elevator::run()
//
//   Main thread for an elevator.
//   Will be called at the beginning of the simulation, to put the
//   Elevator into operation.  run() should pick up and deliver Persons,
//   coordinating with other Elevators for efficient service.
//   run should never return.
//   
void Elevator::run()
{
int current_floor = 1;
int current_destination = 1;
sem_init(&waiters,0,0);
  while(1){
  
   // wait until there are people waiting
    sem_wait(&waiters); 

   // first passenger, go get em
    move_to_floor(or0);
    current_floor = or0;
    open_door();
    Rider* pass;

    if (!waitup[or0].empty())
      pass = waitup[or0].dequeue();
    else if (!waitdown[or0].empty())
      pass = waitdown[or0].dequeue();
    onboard++;

    int nfloor = pass->destination;
    cerr <<"Heading to floor: " << nfloor << endl;
    riders[nfloor].enqueue(pass);
    close_door();
    
    int direction;
    if (nfloor > current_floor){
      direction = 1;
      if (nfloor > current_destination)
        current_destination = nfloor;
    }
    else{
      direction = -1;
      if (nfloor < current_destination)
        current_destination = nfloor;
    }
    for (current_floor; current_floor != current_destination; current_floor += direction){
      while (!riders[current_floor].empty()){
        cerr << "Letting Riders Out" << endl;
        sleep(1);
        open_door();
        Rider* temp = riders[current_floor].dequeue();
        sem_post(&temp->semwait);
        onboard--;
     }

      if (direction == 1){
        while (!waitup[current_floor].empty()){
          cerr << "Letting Riders In" << endl;
          sleep(1);
          if (!door_is_open()) open_door();
          Rider* temp = waitup[current_floor].dequeue();
          int dest = temp->destination;
          riders[dest].enqueue(temp);
          peeps--;
          onboard++;
          //if (peeps == 0) sem_wait(&waiters);
          sem_wait(&waiters);
        }
        if (door_is_open()) close_door();
        move_up();
      }
      else if (direction == -1){
        while (!waitdown[current_floor].empty()){
          cerr << "Letting Riders In" << endl;
          sleep(1);
          if (!door_is_open()) open_door();
          Rider* temp = waitdown[current_floor].dequeue();
          int dest = temp->destination;
          riders[dest].enqueue(temp);
          peeps--;
          onboard++;
          //if (peeps == 0) sem_wait(&waiters);
          sem_wait(&waiters);
        }
        if (door_is_open()) close_door();
        move_down();
      }
    }
  }
}

//
//  take_elevator
//
//    A Person (who) calls this function to take an elevator from their
//    current floor (origin) to a different floor (destination).
//
void take_elevator(const Person *who, int origin, int destination)
{
  // Should not return until the person has taken an elevator
  // to their destination floor.
    
  //PSEUDOCODE
  Rider me;
  me.name = who;
  me.origin = origin;
  me.destination = destination;
  bool up = (destination > origin);
  sem_init(&me.semwait,0,0);

  if (peeps == 0){
   or0 = origin;
   //sem_post(&waiters);
  }
  sem_post(&waiters);
  
  if (up)
    waitup[origin].enqueue(&me);
  else
    waitdown[origin].enqueue(&me);

  passengers.enqueue(&me);
  sem_wait(&me.semwait);
  peeps++;
}
