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

//PCQueue passengers;
//int peeps = 0;
vector<PCQueue> passengers;
vector<int> peeps;

struct Rider{
  const Person *name; int origin, destination;
  sem_t wait;
};

//queue<Rider *> lines;


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
  for (int i = 0; i < peeps; i++){
    Rider* temp = passengers.dequeue();
    temp->name->display();
    passengers.enqueue(temp);
  }
  return peeps;
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
//  peeps = 0;
  while(1){
  
    open_door();
  
    Rider *pass = passengers.dequeue();
    peeps++;
    int nfloor = pass->origin;
    cerr <<"Heading to floor: " << nfloor << endl;
    
    close_door();
    
    move_to_floor(nfloor);
    
    
    // Pick up and drop off passengers.
    sem_post(&pass->wait);
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
  sem_init(&me.wait,0,0);
  
  passengers.enqueue(&me);
  sem_wait(&me.wait);

}
