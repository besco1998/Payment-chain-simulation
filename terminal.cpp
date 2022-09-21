#include "terminal.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>

using namespace std;

bool compareDates(char* cardDate, char* TransDate) {

	int cardMM, CardYY;
	int transDD, transMM, transYYYY;

	sscanf_s(cardDate, "%d/%d", &cardMM, &CardYY); //reads the numbers
	CardYY += 2000;
	sscanf_s(TransDate, "%d/%d/%d", &transDD, &transMM, &transYYYY); //from the string

	if (CardYY < transYYYY || cardMM < transMM) //compares 2 dates
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool checkTransDateFormat(char* date) {
	regex slashed_ddmmyy("^(3[01]|[12][0-9]|0?[1-9])/(1[0-2]|0?[1-9])/(?:[0-9]{2})?[0-9]{2}$"); // dd/mm/yyyy format
	return regex_match(date, slashed_ddmmyy);
}

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	char transaction_date[20]; // to save user input

	//Print "Enter Transaction Date" on the screen
	cout << "Enter Transaction Date(with with format DD/MM/YYYY):\n";

	//Accept User Data
	cin.getline(transaction_date, 20);
	//Check for users' input validation
	if (transaction_date == NULL || strlen(transaction_date) > 10 || strlen(transaction_date) < 10 || !checkTransDateFormat(transaction_date)) {
		printf("Error: Invalid Transaction Date");
		return WRONG_DATE;
	}
	else
	{
		for (int i = 0; i < sizeof(termData->transactionDate); ++i)
			termData->transactionDate[i] = transaction_date[i];
		//printf("ok");
		return EN_terminalError_t::Ok;
	}
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
	if (compareDates((char*) cardData.cardExpirationDate, (char*) termData.transactionDate)) {
		//printf("ok");
		return EN_terminalError_t::Ok;
	}
	else {
		//printf("Error: Expired Card");
		return INVALID_CARD;
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	float trans_amount; // to save user input

	//Print "Enter Transaction Date" on the screen
	cout << "Enter Transaction Amount:\n";

	//Accept User Data
	cin >> trans_amount;

	//Check for users' input validation
	if (trans_amount<=0) {
		printf("Error: Invalid Transaction Amount");
		return INVALID_AMOUNT;
	}
	else
	{
		termData->transAmount = trans_amount;
		//printf("ok");
		return EN_terminalError_t::Ok;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) {
		//printf("Error: Transaction amount Exceeds the Terminal Maximum Amount");
		return EXCEED_MAX_AMOUNT;
	}
	else { 
		//printf("ok");
		return Ok;
	}

}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	float max_trans_amount; // to save user input

	//Print "Enter Transaction Date" on the screen
	cout << "Enter Terminal Maximum Amount:\n";

	//Accept User Data
	cin >> max_trans_amount;
	cin.ignore(1024, '\n');
	// Check for users' input validation
	if (max_trans_amount <= 0) {
		printf("Error: Invalid Terminal Maximum Amount");
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		termData->maxTransAmount = max_trans_amount;
		//printf("ok");
		return EN_terminalError_t::Ok;
	}
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {
	return Ok;
}
