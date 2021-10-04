#include <iostream>
#include <string>
using namespace std;
#include "Attraction.h"

linkedList::linkedList()
{
	head = NULL;
	tail = NULL;
	item = 0;
}

void linkedList::addNode(string attr)
{

	node* tmp = new node;
	tmp->data = attr;
	tmp->next = NULL;

	if (head == NULL)
	{
		head = tmp;
		tail = tmp;
	}
	else
	{
		tail->next = tmp;
		tail = tail->next;
	}

}

void linkedList::display()
{
	node* pDisplay = head;

	while (pDisplay != NULL)
	{
		attrList = attrList + pDisplay->data + " ";
		pDisplay = pDisplay->next;

		if (pDisplay != NULL)
			attrList = attrList + ", ";
	}
}

void linkedList::deleteNode()
{
	node* pTemp = head;

	while (pTemp != NULL)
	{
		head->next = pTemp->next;
		pTemp->next = NULL;
		head = head->next;
		pTemp = head;
	}
}