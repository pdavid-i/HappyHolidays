#pragma once

typedef struct
{
	char* type;
	char* destination;
	char* date;
	int price;
} Offer;

Offer* createOffer(char* type, char* destination, char* date, int price);
void destroyOffer(Offer* o);
Offer* copyOffer(Offer* o);
void changeOffer(Offer* o, char* new_type, char* new_destination, char* date, int price);

char* getType(Offer* o);
char* getDest(Offer* o);
char* getDate(Offer* o);
int	  getPrice(Offer* o);


void printIt(Offer* o);
