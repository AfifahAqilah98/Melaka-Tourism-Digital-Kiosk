#ifndef ATTRACTION_H
#define ATTRACTION_H

#include <string>
using namespace std;

struct node //for linked list
{
	string data;
	node* next;
};

class linkedList
{

public:
	node *head, *tail;
	int item;
	string attrList = "";
	linkedList();
	void addNode(string);
	void display();
	void deleteNode();
};

#endif