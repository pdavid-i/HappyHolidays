#include "Controller.h"
#include <stdlib.h>
#include <string.h>

Controller* createController(Repo* r, OperationsStack* undoS)
{
	Controller* c = (Controller*)malloc(sizeof(Controller));
	c->repo = r;
	c->undoStack = undoS;

	return c;
}

void destroyController(Controller * c)
{
	// first destroy the repository inside
	destroyRepo(c->repo);

	// then the operation stack
	destroyStack(c->undoStack);

	// then free the memory
	free(c);
}

int addOfferCtrl(Controller* c, char* type, char* destination, char* date, int price)
{
	Offer* o = createOffer(type, destination, date, price);
	
	int res = addOffer(c->repo, o);

	if (res == 1) // if the offer was successfully added - register the operation
	{
		Operation* op = createOperation(o, "add", "1", "coae", "3", 4);
		push(c->undoStack, op);
		// once added, the operation can be destroyed (a copy of the operation was added)
		destroyOperation(op);
	}

	// destroy the planet that was just created, as the repository stored a copy
	destroyOffer(o);

	return res;
}

int removeOfferCtrl(Controller* c, char* destination, char* date)
{
	int res = deleteOffer(c->repo, destination, date);

	Offer* o = createOffer("seaside", destination, date, 1);
	if (res == 1)
	{
		Operation* op = createOperation(o, "remove", "1", "2", "remu", 4);
		push(c->undoStack, op);
		destroyOperation(op);
	}
	
	return res;
}	

void sortByPrice(Controller* c)
{
	Repo* repo = getRepo(c);
	int n = getRepoLength(repo);
	int i, j;
	if (n == 0)
		return;
	Offer* temp = getOfferOnPos(repo, 0);
	Offer* on_j = getOfferOnPos(repo, 0);
	Offer* on_jp = getOfferOnPos(repo, 0);
	for (i = 1; i < n; i++)
		  for (j = 0; j < n - i; j++) 
		  {
			   if (getPrice(getOfferOnPos(repo, j)) > getPrice(getOfferOnPos(repo, j + 1))) 
			   {
					temp = c->repo->offers->elems[j];
					c->repo->offers->elems[j] = c->repo->offers->elems[j+1];
					c->repo->offers->elems[j+1] = temp;
				}
			}
}


int updateOfferCtrl(Controller* c, char* destination, char* date, char* new_type, char* new_destination, char* new_date, int new_price)
{
	Offer* old = find(c-> repo, destination, date);

	char type[20];
	char dest[50];
	char data[20];
	
	int price = getPrice(old);
	strcpy(type, getType(old));
	strcpy(dest, getDest(old));
	strcpy(data, getDate(old));

	int res = deleteOffer(c->repo, destination, date);
	Offer* o = createOffer(new_type, new_destination, new_date, new_price);
	Operation* op = createOperation(o, "update", type, dest, data, price);
	push(c->undoStack, op);
	res += addOffer(c->repo, o);

	return res;
}



Repo* getRepo(Controller* c)
{
	return c->repo;
}

int is_later(char* date1,char* date2)
{
	char yy1[2],yy2[2],mm1[2],mm2[2],dd1[2], dd2[2];
	strncpy(dd1, date1, 2);
	strncpy(dd2, date2, 2);
	strncpy(mm1, date1+3*sizeof(char), 2);
	strncpy(mm2, date2+3*sizeof(char), 2);
	strncpy(yy1, date1+6*sizeof(char), 2);
	strncpy(yy2, date2+6*sizeof(char), 2);
	if (strcmp(yy1, yy2) == 0)
	{
		if (strcmp(mm1, mm2) == 0)
		{
			if (strcmp(dd1, dd2) == 0)
			{
				return (1);
			}
			return strcmp(dd1, dd2);
		}
		return strcmp(mm1, mm2);
	}
	return strcmp(yy1, yy2);
}

int undo(Controller* c)
{
	if (isEmpty(c->undoStack))
	{
		return 0;
	}

	Operation* operation = pop(c->undoStack);
	redo_push(c->undoStack, operation);
	if (strcmp(getOperationType(operation), "add") == 0)
	{
		Offer* off = getOffer(operation);

		char destination[50];
		char date[50];
		strcpy(destination, getDest(off));
		strcpy(date, getDate(off));
		deleteOffer(c->repo, destination, date);
	}
	else if (strcmp(getOperationType(operation), "remove") == 0)
	{
		// TO DO!!!
		Offer* off = getOffer(operation);
		char type[20];
		char destination[50];
		char date[20];
		int price = getPrice(off);
		strcpy(destination, getDest(off));
		strcpy(date, getDate(off));
		strcpy(type, getType(off));
		Offer* off_new = createOffer(type, destination, date, price);
		addOffer(c->repo, off_new);
		destroyOffer(off_new);
	}
	else if(strcmp(getOperationType(operation), "update") == 0)
	{
		Offer* off = getOffer(operation);
		char type[20];
		char dest[50];
		char date[20];
		int price = getOldPrice(operation);
		strcpy(type, getOldType(operation));
		strcpy(dest, getOldDest(operation));
		strcpy(date, getOldDate(operation));

		deleteOffer(c->repo, getDest(off), getDate(off));

		Offer* o = createOffer(type, dest, date, price);
		addOffer(c->repo, o);
	}

	// the operation must be destroyed
	destroyOperation(operation);

	return 1;

}

int redo(Controller* c)
{
	if (isRedoEmpty(c->undoStack))
	{
			return 0;
	}

	Operation* op = redo_pop(c->undoStack);

	if(strcmp(getOperationType(op), "add") == 0)
	{
		Offer* off = getOffer(op);
		char type[20];
		char destination[50];
		char date[20];
		int price = getPrice(off);
		strcpy(type, getType(off));
		strcpy(destination, getDest(off));
		strcpy(date, getDate(off));
		Offer* off_new = createOffer(type, destination, date, price);
		addOffer(c->repo, off_new);
		destroyOffer(off_new);
	}
	else if (strcmp(getOperationType(op), "remove") == 0)
	{
		Offer* off = getOffer(op);
		
		char destination[50];
		char date[20];
		strcpy(destination, getDest(off));
		strcpy(date, getDate(off));
		deleteOffer(c->repo, destination, date);
	}
	else if (strcmp(getOperationType(op), "update") == 0)
	{
		Offer* off = getOffer(op);
		char type[20];
		char dest[50];
		char date[20];
		int price = getPrice(off);
		strcpy(type, getType(off));
		strcpy(dest, getDest(off));
		strcpy(date, getDate(off));

		deleteOffer(c->repo, getOldDest(op), getOldDate(op));

		Offer* o = createOffer(type, dest, date, price);
		addOffer(c->repo, o);
	}
return 1;
}
