#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
using namespace std;


template <class T>
class Node
{
public:
	char senderID;
	string message;
	Node<T>* next;

	Node()
	{
		senderID = 0;
		message = "";
	}

	Node(char s, string m, Node<T>* n)
	{
		senderID = s;
		message = m;
		next = n;
	}

	Node(char s, string m)
	{
		senderID = s;
		message = m;
		next = NULL;
	}

};

template <class T>
class messageQueue
{
public:
	Node<T>* front;
	Node<T>* rear;

	messageQueue()
	{
		front = NULL;
		rear = NULL;
	}

	bool isEmpty()
	{
		if (front == NULL)
			return true;
		return false;
	}

	void enqueueMessage(char s, string m) {
		Node<T>* n = new Node<T>;
		n->senderID = s;
		n->message = m;
		n->next = NULL;
		if (front == NULL)
		{
			front = n;
			rear = n;
		}
		else
		{
			rear->next = n;
			rear = n;
		}
	}

	void Enqueue(Node<T>* t) {
		Node<T>* n = new Node<T>;
		n->senderID = t->senderID;
		n->message = t->message;
		n->next = NULL;
		if (front == NULL)
		{
			front = n;
			rear = n;
		}
		else
		{
			rear->next = n;
			rear = n;
		}
	}

	Node<T>* dequeueMessage()
	{
		Node <T>* temp = new Node<T>;
		temp = front;
		front = front->next;
		return temp;
	}


	void print()
	{
		Node <T>* temp = new Node<T>;
		temp = front;
		cout << "\n\nMessages in the conversation: \n";
		int i =1;
		while (temp)
		{
			cout << i << ". User " <<  temp->senderID << ": \"" << temp->message << "\"" << endl;
			temp = temp->next;
			i++;
		}
		cout << endl << endl;
	}
};

messageQueue<int> q;
sem_t lock1;

void* func(void* x)
{	
	sem_wait(&lock1);
	
	char id = 3;
	string i, m = "hello";
	cout << "\nEnter user ID: ";
	cin >> id;
	cout <<  "\nEnter massage: ";
	//cin.ignore();
	getline(cin >> ws, m); 
	
	q.enqueueMessage(id, m);
	q.print();
	sem_post(&lock1);
	pthread_exit(NULL);
}

void* manage(void* x)
{
	pthread_t tid1, tid2, tid3;
	pthread_create(&tid1, NULL, func, NULL);
	pthread_create(&tid2, NULL, func, NULL);
	pthread_create(&tid3, NULL, func, NULL);
	
	pthread_exit(NULL);
}

int main()
{
	sem_init(&lock1, 0, 1);
	messageQueue<int> q;
	int min = 3;
	cout << "\nEnter the number of minutes the program should run: ";
	cin >> min;
	
	int i = 0;
	pthread_t tid;
	for (i=0; i<min; i++)
	{
		pthread_create(&tid, NULL, manage, NULL);
	}
	pthread_exit(NULL);
}
