#pragma once
#include "Offer.h"
#include "DynamicArray.h"

typedef struct
{
	DynamicArray* offers;
} Repo;

/// <summary>
/// Creates a PlanetRepo.
/// </summary>
Repo* createRepo();

/// <summary>
/// Destroys a given planet repository. The memory is freed.
/// </summary>
void destroyRepo(Repo* v);

/// <summary>
/// Finds the planet with the given name.
/// </summary>
/// <param name="v">Pointer to the PlanetRepo.</param>
/// <param name = "symbols">A string, the name to search for.</param>
/// <returns>The pointer to the planet with the given name, or null, if such a planet does not exist.</returns>
Offer* find(Repo* v, char* destination, char* date);

/*
	Finds the position of the planet with hte given name in the repository.
	Input:  - v - pointer to the PlanetRepo
			- name - a string, the name to search for.
	Output: If a planet with the given name exists in the repository, its position is returned.
			-1 - if such a planet does not exist.
*/
int findPosOfPlanet(Repo* v, char* destination, char* date);

/*
	Adds a planet to the repository of planets.
	Input:	- v - pointer to the PlanetRepo
			- p - planet
	Output: 1 - if the planet was sucessfully added
			0 - if the planet could not be added, as another planet with the same symbol already exists in the PlanetRepo.
*/
void updateOffer(Repo* v, char* old_destination, char* old_date, char* new_type, char* new_destination, char* new_date, int new_price);


int addOffer(Repo* v, Offer* o);

/*
	Deletes the planet with the given name from the repository.
	Input:  - v - pointer to the PlanetRepo
			- name - a string, the name which identifies the planet to be deleted.
	Output: If a planet with the given name exists in the repository, it is deleted.
*/
int deleteOffer(Repo* o, char* destination, char* date);

int getRepoLength(Repo* v);


/*
	Returns the planet on the given position in the planet vector.
	Input:	v - the planet repository;
			pos - integer, the position;
	Output: the planet on the given potision, or an "empty" planet.
*/
Offer* getOfferOnPos(Repo* v, int pos);

void testsPlanetRepo();
