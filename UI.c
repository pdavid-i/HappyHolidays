#include "UI.h"
#include <stdio.h>
#include <stdlib.h>

UI* createUI(Controller* c)
{
	UI* ui = (UI*)malloc(sizeof(UI));
	ui->ctrl = c;

	return ui;
}

void destroyUI(UI * ui)
{
	// first destroy the controller
	destroyController(ui->ctrl);
	// free the UI memory
	free(ui);
}

/*
	Prints the available menu for the problem
	Input: -
	Output: the menu is printed at the console
*/
void printMenu()
{
	printf("%s","\n**********************************************************\n");
	printf("%s", "1 - add an offer.\n");
	printf("2 - delete an offer\n");
	printf("3 - update an offer\n");
	printf("4 - list all offers.\n");
	printf("5 - undo.\n");
	printf("6 - list offers with specific destination\n");
	printf("7 - list offers with a specific type and later than a date\n");
	printf("8 - redo\n");
	printf("9 - list offers with minimum price\n");
	printf("0 - to exit.\n");
	printf("%s","\n**********************************************************\n");
}


/*
	Verifies if the given command is valid (is between 0-9)
	Input: command - integer
	Output: 1 - if the command is valid
	0 - otherwise
*/
int validCommand(int command)
{
	if (command >= 0 && command <= 9)
		return 1;
	return 0;
}

/*
	Reads an integer number from the keyboard. Asks for number while read errors encoutered.
	Input: the message to be displayed when asking the user for input.
	Returns the number.
*/
int readIntegerNumber(const char* message)
{
	char s[16];
	int res = 0;
	int flag = 0;
	int r = 0;

	while (flag == 0)
	{
		printf("%s",message);
		scanf("%s", s);

		r = sscanf(s, "%d", &res);	// reads data from s and stores them as integer, if possible; returns 1 if successful
		flag = (r == 1);
		if (flag == 0)
			printf("Error reading number!\n");
	}
	return res;
}

/*
	Reads a string with spaces from the standard input.
	Input:	message - string, message to be shown to the user.
			maxStrSize - the maximum size for the string to be read
	Output: the string that was read.
*/
void readStringWithSpaces(const char* message, int maxStrSize, char str[])
{
	printf("%s",message);
	fgets(str, maxStrSize, stdin);
	// the newline is also read so we must eliminate it from the string
	int size = strlen(str) - 1;
	if (str[size] == '\n')
		str[size] = '\0';
}

int addOfferUI(UI* ui)
{
	char destination[50], type[50], date[20];
	int price;
	
	fgetc(stdin); // read the newline, to prevent it from going further to fgets
	readStringWithSpaces("Please input the type: ", 50, type);
	readStringWithSpaces("Please input the destination: ", 50, destination);
	readStringWithSpaces("Please input the departure date: ", 50, date);
	printf("%s","Please input the price:");
	scanf("%d", &price);

	return addOfferCtrl(ui->ctrl, type, destination, date, price);
}

int deleteOfferUI(UI* ui)
{
	char destination[50];
	char date[20];
	printf("Please input the destination:");
	scanf("%s", destination);
	printf("Please input date:");
	scanf("%s", date);

	return removeOfferCtrl(ui->ctrl, destination, date);
}

int updateOfferUI(UI* ui)
{
	char old_destination[50];
	char old_date[20];
	char new_type[20];
	char new_destination[50];
	char new_date[20];
	int new_price;

	fgetc(stdin);
	readStringWithSpaces("Please put the old destination: ", 50, old_destination);
	readStringWithSpaces("Please input the old date: ", 20, old_date);
	readStringWithSpaces("Please input the new type: ", 20, new_type);
	readStringWithSpaces("Please input the new destination: ", 50, new_destination);
	readStringWithSpaces("Please input the new date: ", 20, new_date);
	printf("Please input the new price: ");
	scanf("%d", &new_price);
	updateOfferCtrl(ui->ctrl, old_destination, old_date, new_type, new_destination, new_date, new_price);
	return (0);
}

void listAllOffers(UI* ui)
{
	Repo* repo = getRepo(ui->ctrl);
	int length = getRepoLength(repo);

	if (length == 0)
	{
		char* str = "There are no stored offers.";
		printf("%s \n", str);
	}

	for (int i = 0; i < getRepoLength(repo); i++)
	{
		printIt(getOfferOnPos(repo, i));
	}
}

void listOffersLaterThanDate(UI* ui)
{
	Repo* repo = getRepo(ui->ctrl);
	int length = getRepoLength(repo);
	char type[20];
	char date[20];
	int was_printed = 0;
	printf("Enter the type of the offer! seaside/mountain/city break\n");
	scanf("%s", type);
	printf("Enter the date! A valid date looks like this: yy/mm/dd\n");
	scanf("%s", date);
	
	for (int i = 0; i<length; i++)
	{
		if ((is_later(getDate(getOfferOnPos(repo, i)), date) > 0) && (strcmp(type, getType(getOfferOnPos(repo, i))) == 0))
		{
			printIt(getOfferOnPos(repo, i));
			was_printed++;
		}
	}
	if (was_printed == 0)
		printf("There are no offers that correspond to the criteria!");
}





void listOffersAtDestination(UI* ui)
{
		Repo* repo = getRepo(ui->ctrl);
		int length = getRepoLength(repo);
		char destination[50];


		printf("%s", "Enter the destination");
		scanf("%s", destination);
		if (length == 0)
		{
			char* str = "There are no stored offers.";
			printf("%s \n", str);
			return;
		}

		sortByPrice(ui->ctrl);

		for (int i = 0; i < getRepoLength(repo); i++)
		{
			if (strcmp(getDest(getOfferOnPos(repo, i)), destination) == 0)
				printIt(getOfferOnPos(repo, i));								
		}

}

void listOffersWithMinimumPrice(UI* ui)
{
	Repo* repo = getRepo(ui->ctrl);
	int length = getRepoLength(repo);
	int price;

	if (length == 0)
	{
		char* str= "There are no stored offers.";
		printf("%s \n", str);
		return;
	}	
	

	printf("%s", "Enter the minimum price");
	scanf("%d", &price);

	sortByPrice(ui->ctrl);

	for (int i = 0; i < getRepoLength(repo); i++)
	{
		if (getPrice(getOfferOnPos(repo, i)) >= price)
			printIt(getOfferOnPos(repo, i));
	}
	
}

void startUI(UI* ui)
{
	while (1)
	{
		printMenu();
		int res;
		int command = readIntegerNumber("Input command: ");
		while (validCommand(command) == 0)
		{
			printf("%s","Please input a valid command!\n");
			command = readIntegerNumber("Input command: ");
		}
		if (command == 0)
			break;
		switch (command)
		{
		case 1:
		{
			res = addOfferUI(ui);
			if (res == 1)
				printf("%s","Planet successfully added.\n");
			else
				printf("%s","Error! Planet could not be added, as there is another planet with the same symbol combination!\n");
			break;
		}
		case 2:
			res = deleteOfferUI(ui);
			if (res == 1)
				printf("%s", "Offer succesfully deleted.\n");
			else
				printf("%s", "Error! Offer could not be deleted, as it does not exist!\n");

		case 3:
			updateOfferUI(ui);
		case 4:
		{
			listAllOffers(ui);
			break;
		}
		case 5:
		{
			int res = undo(ui->ctrl);
			if (res == 1)
				printf("%s", "Undo was successful.\n");
			else
				printf("%s","No more undos to be made.\n");
			break;
		}
		case 6:
			listOffersAtDestination(ui);
		case 7:
			listOffersLaterThanDate(ui);
		case 8:
			{
				int res = redo(ui->ctrl);
				if (res == 1)
					printf("%s", "Redo was succesful.\n");
				else
					printf("%s", "No redos to be done.\n");
				break;
			}
		case 9:
			listOffersWithMinimumPrice(ui);
		}
	}
}
