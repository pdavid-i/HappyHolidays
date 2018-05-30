#pragma once
#include "Repository.h"
#include "OperationStack.h"

typedef struct
{
	Repo* repo;
	OperationsStack* undoStack;
} Controller;

Controller* createController(Repo* r, OperationsStack* undoS);
void destroyController(Controller* c);

/// <summary>
/// Adds a planet to the repository of planets.
/// </summary>
/// <param name="c">Pointer to theController.</param>
/// <param name = "name">A string, the name of the planet.</param>
/// <param name = "type">A string, the planet's type.</param>
/// <param name = "distanceFromEarth">Double, the distance from the planet to Earth, in light years.</param>
/// <returns>1 - if the planet was sucessfully added; 0 - if the planet could not be added, as another planet with the same symbol already exists.</returns>
int addOfferCtrl(Controller* c, char* type, char* destination, char* date, int price);
int updateOfferCtrl(Controller* c, char* old_destination, char* old_date, char* new_type, char* new_destination, char* new_date, int new_price);

void sortByPrice(Controller* c);

int removeOfferCtrl(Controller* c, char* destination, char* date);

Repo* getRepo(Controller* c);

/// <summary>
/// Undoes the last performed operation.
/// </summary>
/// <param name="c">Pointer to theController.</param>
/// <returns>1, if the undo operation could be done, 0 otherwise.</returns>
int undo(Controller* c);
int redo(Controller* c);
