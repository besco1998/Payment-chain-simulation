#include "card.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>

using namespace std;

bool checkDateFormat(char* date) {
	regex slashed_ddmmyy("^(((0)[0-9])|((1)[0-2]))\/\\d{2}$"); // mm/yy format
	return regex_match(date, slashed_ddmmyy);
}

bool containsOnlyNumbers(std::string const& str) {
	return str.find_first_not_of("0123456789") == std::string::npos;
}
 
EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	char card_holder_name[100]; // to save user input
	
	//Print "Enter Card Holder Name" on the screen
	cout << "Enter Card Holder Name(with 20 characters minimum and 24 characters maximum):\n";
	//Accept User Data
	cin.getline(card_holder_name, 100);
	//Check for users' input validation
	if (card_holder_name == NULL || strlen(card_holder_name) > 25 || strlen(card_holder_name) < 21) {
		printf("Error: Invalid card holder name");
		return WRONG_NAME;
	}
	else
	{
		for (int i = 0; i < sizeof(cardData->cardHolderName); ++i)
			cardData->cardHolderName[i] = card_holder_name[i];

		//printf("ok");
		return OK;
	}
	
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	char card_expiration_date[20]; // to save user input

	//Print "Enter Card Expiry Date" on the screen
	cout << "Enter Card Expiry Date(with with format MM/YY):\n";

	//Accept User Data
	cin.getline(card_expiration_date, 20);

	//Check for users' input validation
	if (card_expiration_date == NULL || strlen(card_expiration_date) > 5 || strlen(card_expiration_date) < 5 || !checkDateFormat(card_expiration_date)) {
		printf("Error: Invalid Card Expiry Date");
		return WRONG_EXP_DATE;
	}
	else
	{
		for (int i = 0; i < sizeof(cardData->cardExpirationDate); ++i)
			cardData->cardExpirationDate[i] = card_expiration_date[i];
		//printf("ok");
		return OK;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	char primary_account_number[30]; // to save user input

	//Print "Enter Card Expiry Date" on the screen
	cout << "Enter Card Primary Account Number (PAN):\n";

	//Accept User Data
	cin.getline(primary_account_number, 30);

	//Check for users' input validation
	if (primary_account_number == NULL || strlen(primary_account_number) > 19 || strlen(primary_account_number) < 16 || !containsOnlyNumbers(primary_account_number)) {
		printf("Error: Invalid Primary Account Number (PAN)");
		return WRONG_PAN;
	}
	else
	{
		for (int i = 0; i < sizeof(cardData->primaryAccountNumber); ++i)
			cardData->primaryAccountNumber[i] = primary_account_number[i];
		//printf("ok");
		return OK;
	}
}

