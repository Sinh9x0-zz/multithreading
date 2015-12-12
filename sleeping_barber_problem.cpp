//This program solves the 'sleeping barber' problem using semaphores
//TODO: replace sem_t mutex with pthread mutex

#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

int customers;
int chairs;

//only a specified number of people in the waiting room at a time
sem_t waitroom;

//only a single person can have their hair cut at a time
sem_t mutex;

//no more than one activity should be printed to console at a time
sem_t activity; 

void * custThread(void *args){
  int *p = (int *) args;
  int custNum = *p;
  sem_wait(&activity);
  cout << "Customer " << custNum << " leaving for barber shop" << endl;
  sem_post(&activity);
  sem_wait(&activity);
  cout << "Customer " << custNum << " arrived at barber shop." << endl;
  sem_post(&activity);
  sem_wait(&waitroom);
  sem_wait(&activity);
  cout << "Customer " << custNum << " entering waiting room." << endl;
  sem_post(&activity);
  sem_wait(&mutex);
  sem_post(&waitroom);
  sem_wait(&activity);
  cout << "Customer " << custNum << " waking the barber." << endl;
  sem_post(&activity);
  sem_wait(&activity);
  cout << "The barber is cutting hair." << endl;
  sem_post(&activity);
  sem_wait(&activity);
  cout << "The barber has finished cutting hair." << endl;
  sem_post(&activity);
  sem_wait(&activity);
  cout << "The barber is sleeping." << endl;
  sem_post(&activity);
  sem_post(&mutex);
  sem_wait(&activity);
  cout << "Customer " << custNum << " leaving the barber shop." << endl;
  sem_post(&activity);
  return 0;
}

int main(){
  cout << "Maximum number of customers can only be 25. ";
  cout << "Enter number of customers and chairs." << endl;
  cin >> customers >> chairs;
  while (cin.fail()){
  	cout << "Input must be two integers separated by a space" << endl;
  	cin.clear();
  	cin.ignore(256, '\n');
  	cin >> customers >> chairs;
  }
  cout << "A solution to the sleeping barber ";
  cout << "problem using semaphores." << endl;
  pthread_t thread[customers];
  sem_init(&waitroom, 0, chairs);
  sem_init(&mutex, 0, 1);
  sem_init(&activity, 0, 1);
  int *custNum;
  cout << "The barber is sleeping" << endl;
  
  //creates a thread for each customer
  for (int n = 0; n < customers; n++){
  	custNum  = new int(n);
  	if(pthread_create(&thread[n], NULL, custThread, (void *) custNum)){
  	  cout << "Error with thread creation" << endl;
  	  return -1;
  	}
  }

  for (int n = 0; n < customers; n++){
  	if(pthread_join(thread[n], NULL)){
  	  cout << "Error joining thread" << endl;
  	  return -1;
  	}
  }
  cout << "The barber is going home for the day." << endl;
  return 0;
}
