#include "OperationStack.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Operation* createOperation(Offer* off, char* operationType, char* old_type, char* old_dest, char* old_date, int old_price)
{
	Operation* o = (Operation*)malloc(sizeof(Operation));
	o->offer = copyOffer(off);

	if (operationType != NULL)
	{
		o->operationType = (char*)malloc(sizeof(char) * (strlen(operationType) + 1));
		strcpy(o->operationType, operationType);
	}
	else
		o->operationType = NULL;
	o->old_type = (char*)malloc(sizeof(char) * (strlen(old_type) +1));
	strcpy(o->old_type, old_type);

	o->old_dest = (char*)malloc(sizeof(char) * (strlen(old_dest) + 1));
	strcpy(o->old_dest, old_dest);

	o->old_date = (char*)malloc(sizeof(char) * (strlen(old_date) + 1));
	strcpy(o->old_date, old_date);

	o->old_price = old_price;
	return o;
}

char* getOldType(Operation *o)
{
	return o->old_type;
}

char* getOldDate(Operation* o)
{
	return o->old_date;
}

char* getOldDest(Operation* o)
{
	return o->old_dest;	
}

int getOldPrice(Operation* o)
{
	return o->old_price;
}

void destroyOperation(Operation* o)
{
	if (o == NULL)
		return;

	// first destroy the planet
	destroyOffer(o->offer);
	// then the operationType
	free(o->operationType);
	// then free the operation
	free(o);
}

Operation* copyOperation(Operation * o)
{
	if (o == NULL)
		return NULL;

	Operation* newOp = createOperation(o->offer, o->operationType, getOldType(o), getOldDest(o), getOldDate(o), getOldPrice(o));
	return newOp;
}

char* getOperationType(Operation* o)
{
	return o->operationType;
}

Offer* getOffer(Operation* o)
{
	return o->offer;
}

// ---------------------------------------------------------------

OperationsStack* createStack()
{
	OperationsStack* s = (OperationsStack*)malloc(sizeof(OperationsStack));
	s->length = 0;
	s->redo_length = 0;
	return s;
}

void destroyStack(OperationsStack* s)
{
	if (s == NULL)
		return;

	// first deallocate memory of operations (this is allocated when a new operation is pushed onto the stack)
	for (int i = 0; i < s->length; i++)
		destroyOperation(s->operations[i]);

	// then free the stack
	free(s);
}

void push(OperationsStack* s, Operation* o)
{
	if (isFull(s))
		return;

	s->operations[s->length++] = copyOperation(o);	// copies of operations are added, such that the stask manages its own operations
}

void redo_push(OperationsStack* s, Operation* o)
{
	if (isFull(s))
		return ;
	s->redo_operations[s->redo_length++] = copyOperation(o);
}

Operation* pop(OperationsStack* s)
{
	if (isEmpty(s))
		return NULL;
	s->length--;
	redo_push(s, s->operations[s->length]);
	return s->operations[s->length];
}

Operation* redo_pop(OperationsStack* s)
{
	if (isRedoEmpty(s))
		return NULL;
	s->redo_length--;
	return s->redo_operations[s->redo_length];
}

int isRedoEmpty(OperationsStack* s)
{
	return (s->redo_length == 0);
}

int isRedoFull(OperationsStack* s)
{
	return (s->redo_length = 100);
}

int isEmpty(OperationsStack* s)
{
	return (s->length == 0);
}

int isFull(OperationsStack* s)
{
	return s->length == 100;
}

// Tests
void testsStack()
{
	OperationsStack* s = createStack();
	
	Offer* p1 = createOffer("seaside", "vama", "01.05.18", 400);
	Offer* p2 = createOffer("mountain", "Cheile Turzii", "31.03.18", 450);
	Operation* o1 = createOperation(p1, "add", "1", "2", "3", 4);
	Operation* o2 = createOperation(p2, "add", "1", "2", "3", 4);
	Operation* o3 = createOperation(p1, "remove", "1", "2", "3", 4);

	// the planets may be destroyed, as the operations contain copies of these planets
	destroyOffer(p1);
	destroyOffer(p2);

	push(s, o1);
	push(s, o2);
	push(s, o3);

	// the operations may be destroyed, as the stack contains copies of these operations
	destroyOperation(o1);
	destroyOperation(o2);
	destroyOperation(o3);

	assert(isFull(s) == 0);
	assert(isEmpty(s) == 0);
	Operation* o = pop(s);
	assert(strcmp(o->operationType, "remove") == 0);
	// after each pop, the operations must be destroyed
	destroyOperation(o);

	o = pop(s);
	assert(strcmp(o->operationType, "add") == 0);
	destroyOperation(o);

	o = pop(s);
	assert(strcmp(o->operationType, "add") == 0);
	destroyOperation(o);

	assert(isEmpty(s) == 1);

	// destroy the stack
	destroyStack(s);
}
