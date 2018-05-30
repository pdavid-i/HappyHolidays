#include "Offer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Offer*	createOffer(char* type, char* destination, char* date, int price)
{
	Offer* o = (Offer*)malloc(sizeof(Offer));
	o->type = (char *)malloc(sizeof(char) * (strlen(type) + 1));
	strcpy(o->type, type);
	o->destination = (char *)malloc(sizeof(char) * (strlen(destination) + 1));
	strcpy(o->destination, destination);
	o->date = (char *)malloc(sizeof(char) * (strlen(date) + 1));
	strcpy(o->date, date);
	o->price = price;

	return o;
}

Offer* copyOffer(Offer* o)
{
	Offer* copy = createOffer(getType(o), getDest(o), getDate(o), getPrice(o));	
	return copy;
}

void changeOffer(Offer* o, char* new_type, char* new_destination, char* new_date, int new_price)
{
	strcpy(o->type, new_type);
	strcpy(o->destination, new_destination);
	strcpy(o->date, new_date);
	o->price = new_price;
}

void destroyOffer(Offer *o)
{
	if (o == NULL)
		return;

	free(o->type);
	free(o->destination);
	free(o->date);

	free(o);
}

char* getType(Offer *o)
{
	return o->type;
}

char* getDest(Offer *o)
{
	return o->destination;
}

char* getDate(Offer* o)
{
	return o->date;
}

int getPrice(Offer* o)
{
	return o->price;
}


void printIt(Offer *o)
{
	printf("Offer is of type: %s, with destination: %s, departure date: %s and price %d\n", o->type, o->destination, o->date, o->price);
}




















