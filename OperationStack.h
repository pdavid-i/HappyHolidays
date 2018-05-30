#pragma once
#include "Offer.h"
#include "DynamicArray.h"
#include <string.h>

typedef struct
{
	Offer* offer;
	char* operationType;
// these are all just so we can undo/redo modify operations, since other
// wise we would have no way of storing the old data

	char* old_type;
	char* old_dest;
	char* old_date;
	char* old_price;
} Operation;

Operation* createOperation(Offer* off, char* operationType, char* old_type, char* old_dest, char* old_date, int old_price);
void destroyOperation(Operation* o);
Operation* copyOperation(Operation* o);
char* getOperationType(Operation* o);
Offer* getOffer(Operation* o);


//this is only for update undo/redo
typedef struct
{
	Offer* offer;
	char* old_dest;
	char* old_date;
	char* old_type;
	int old_price;
	char* operationType;
}UpdateOperation;


// ---------------------------------------------------------------
typedef struct
{
	Operation* operations[100];
	Operation* redo_operations[100];
	int length;
	int redo_length;
} OperationsStack;


char* getOldType(Operation *o);
char* getOldDest(Operation *o);
char* getOldDate(Operation *o);
int  getOldPrice(Operation *o);
OperationsStack* createStack();
void destroyStack(OperationsStack* s);
void push(OperationsStack* s, Operation* o);
void redo_push(OperationsStack* s, Operation* o);
Operation* redo_pop(OperationsStack* s);
Operation* pop(OperationsStack* s);
int isEmpty(OperationsStack* s);
int isFull(OperationsStack* s);
int isRedoEmpty(OperationsStack* s);
int isRedoFull(OperationsStack* s);
void testsStack();
