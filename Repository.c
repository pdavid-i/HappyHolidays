#include "Repository.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

Repo* createRepo()
{
	Repo* v = (Repo*)malloc(sizeof(Repo));
	v->offers = createDynamicArray(CAPACITY);

	return v;
}

void destroyRepo(Repo* v)
{
	if (v == NULL)
		return;

	// !!! If the dynamic vector takes responsibility of these planets, then the memory can be deallocated in the function "destroy" in DynamicArray.c.
	// Otherwise, the repository should deallocate the memory for the planets.
	/*for (int i = 0; i < getLength(v->planets); i++)
	{
		Planet* p = get(v->planets, i);
		destroyPlanet(p);
	}*/

	// then destroy the dynamic array
	destroy(v->offers);
	free(v);
}

int findPosOfOffer(Repo * v, char * destination, char* date)
{
	if (v == NULL)
		return -1;

	for (int i = 0; i < getLength(v->offers); i++)
	{
		Offer* o = get(v->offers, i);
		if ((strcmp(o->destination, destination) == 0) && (strcmp(o->date, date) == 0))
			return i;
	}
	return -1;
}

Offer* find(Repo* v, char* destination, char* date)
{
	if (v == NULL)
		return NULL;

	int pos = findPosOfOffer(v, destination, date);
	if (pos == -1)
		return NULL;
	return get(v->offers, pos);
}

int addOffer(Repo* v, Offer* o)
{
	if (v == NULL)
		return 0;

	// first search for a planet with the same name as the one that is to be added and do not add it if it already exists
	if (find(v, o->destination, o->date) != NULL)
		return 0;

	// a copy of the planet which was passed will be stored, such that the memory for the planet can be deallocated where it was allocated (in Controller) 
	// and the Repository will handle its own memory
	Offer* copy = copyOffer(o);
	add(v->offers, copy);

	return 1;
}

void updateOffer(Repo*v, char* old_destination, char* old_date, char* new_type, char* new_destination, char* new_date, int new_price)
{
	deleteOffer(v, old_destination, old_date);
	Offer* off = createOffer(new_type, new_destination, new_date, new_price);
	addOffer(v, off);
}

int deleteOffer(Repo* v, char* destination, char* date)
{
	if (v == NULL)
		return 0;

	// find the position of the planet in the repository
	int pos = findPosOfOffer(v, destination, date);
	if (pos == -1)
		return 0;

	// destroy the planet
	Offer* o = get(v->offers, pos);
	destroyOffer(o);

	// delete the pointer from the dynamic array
	delete(v->offers, pos);
	return (1);
}

int getRepoLength(Repo* v)
{
	if (v == NULL)
		return -1;

	return getLength(v->offers);
}

Offer* getOfferOnPos(Repo* v, int pos)
{
	if (v == NULL)
		return NULL;

	if (pos < 0 || pos >= getLength(v->offers))
		return NULL;

	return get(v->offers, pos);
}




// Tests
void testAdd()
{
	Repo* v = createRepo();

	Offer* o1 = createOffer("seaside", "vama", "23.01.15", 350);

	addOffer(v, o1);
	assert(getRepoLength(v) == 1);
	assert(strcmp(getDate(getOfferOnPos(v, 0)), "23.01.15") == 0);

	Offer* o2 = createOffer("mountain", "Neptune-like", "24.01.15", 450);
	assert(addOffer(v, o2) == 1);
	assert(getRepoLength(v) == 2);

	// now try to add the same planet again -> add must return 0
	assert(addOffer(v, o2) == 0);

	// destroy the test repository
	destroyRepo(v);

	// now the memory allocated for the planets must be freed
	destroyOffer(o1);
	destroyOffer(o2);
}


void testsPlanetRepo()
{
	testAdd();
}


