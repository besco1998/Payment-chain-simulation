#include"card.h"
#include"terminal.h"
#include"server.h"
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

void printTransState(EN_transState_t transState) {
	if (transState==APPROVED)
	{
		cout << "STATUS:Approved" << endl;
	}
	else if (transState == DECLINED_STOLEN_CARD)
	{
		cout << "STATUS:Declined Stolen Card" << endl;
	}
	if (transState == DECLINED_INSUFFECIENT_FUND)
	{
		cout << "STATUS:Declined Insuffecient Fund" << endl;
	}
	if (transState == INTERNAL_SERVER_ERROR)
	{
		cout << "STATUS:Internal Server Error" << endl;
	}
}


void appStart() {
	ST_cardData_t Card; //Card object
	ST_terminalData_t Terminal; //Terminal object
	ST_transaction_t Transaction; //Transaction object

	setMaxAmount(&Terminal); //Asks the user to enter simulation paramerter (Max Amount of the Terminal)

	//Flow Chart Implementation
	if (getCardHolderName(&Card)==OK) {
		if (getCardExpiryDate(&Card) == OK) {
			if (getCardPAN(&Card) == OK) {
				if (getTransactionDate(&Terminal) == Ok) {
					if (isCardExpired(Card, Terminal) == Ok) {
						if (getTransactionAmount(&Terminal) == Ok){
							if (isBelowMaxAmount(&Terminal) == Ok){
								Transaction.cardHolderData = Card;
								Transaction.terminalData = Terminal;
								Transaction.transState = recieveTransactionData(&Transaction);
								printTransState(Transaction.transState);
								saveTransaction(&Transaction);
							}
							else{
								printf("Declined Amount Exceed Limit\n");
							}
						}
					}
					else{
						printf("Declined Expired Card\n");
					}
				}
			}
		}
	}

}

int main() {
	appStart();
	return 0;
}
