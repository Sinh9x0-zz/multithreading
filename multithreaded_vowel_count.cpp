//This program counts all the vowels in specified text files
//TODO: take files as parameters instead of hard coding

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <pthread.h>

using namespace std;

/*
TODO: Set the number of threads equal to the number of text files passed in by the user 
*/
//equivalent to number of text files to be read
#define NUM_THREADS 20

int a = 0, e = 0, i = 0, o = 0, u = 0;

//ensures only one file is open at a time
pthread_mutex_t myMutex;

void * vowelCount(void *args) {
  ifstream inFile;
  string *p = (string *)args;
  string file = *p;
  delete p;
  pthread_mutex_lock(&myMutex);
  inFile.open(file.c_str());
  if (!inFile){
	cout << "Unable to open " << file << endl;
	exit(1);
  }
  
  //check each char for vowels
  char vowel;
  while (inFile >> noskipws >> vowel){
  	if (vowel == 'a' || vowel == 'A')
  	  a++;
  	else if (vowel == 'e' || vowel == 'E')
  	  e++;
  	else if (vowel == 'i' || vowel == 'I')
  	  i++;
  	else if (vowel == 'o' || vowel == 'O')
  	  o++;
  	else if (vowel == 'u' || vowel == 'U')
  	  u++;
  }
  inFile.close(); 
  pthread_mutex_unlock(&myMutex);
  pthread_exit(NULL);
}

int main(){
  pthread_t thread[NUM_THREADS];
  stringstream fileName;
  string *str_p;
  str_p = new string;
  pthread_mutex_init(&myMutex, 0);
  
  //creates a thread for each text file
  for (int n = 0; n < NUM_THREADS; n++){
    /*
    TODO: Currently, this is hard coded in, files should be given by the user.
  	fileName << "/directory/file";
  	fileName << n + 1 << ".txt";
    */ 
    str_p = new string(fileName.str());
  	if(pthread_create(&thread[n], NULL, vowelCount,
  					  (void *)str_p)){
  	  cout << "Error with thread creation" << endl;
  	  return -1;
  	}
  	fileName.str("");
  	fileName.clear();
  }
  
  for (int n = 0; n < NUM_THREADS; n++){
  	if(pthread_join(thread[n], NULL)){
  	  cout << "Error joining thread" << endl;
  	  return -1;
  	}
  }
  pthread_mutex_destroy(&myMutex);
  cout << "A: " << a << endl;
  cout << "E: " << e << endl;
  cout << "I: " << i << endl;
  cout << "O: " << o << endl;
  cout << "U: " << u << endl;
  return 0;
}
