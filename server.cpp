#include "server.h"
#include <algorithm>
#include <string>

using namespace std;

//Initialize accountsDB with 10 accounts {Balance,PAN} (PAN Incereses in the last two digits)
ST_accountsDB_t accountsDB[225] = { { 1500.0f, "1234567891234567" },
                                    { 1000.0f, "1234567891234568" },
                                    { 250.0f, "1234567891234569" },
                                    { 500.0f, "1234567891234570" },
                                    { 344.0f, "1234567891234571" },
                                    { 150.0f, "1234567891234572" },
                                    { 18000.0f, "1234567891234573" },
                                    { 1500.0f, "1234567891234574" },
                                    { 30.0f, "1234567891234575" },
                                    { 14.0f, "1234567891234576" } };




//Initialize transactionDB with 10 transactions initially with 0s.
ST_transaction_t transactionDB[225] = {
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
    {{"0000000000000000000","0000000000000000000","00/00"},{0.0f,0.0f,"00/00/0000"},EN_transStat_t::APPROVED,0},
};

//Automatically fill in the databases
void initDB() {
    for (int i = 0; i < 225; i++)
    {   
        accountsDB[i].balance = 1000;
        string tmp = to_string(1234567891234567 + i);
        for (int j = 0; j < 20; j++)
        {

        accountsDB[i].primaryAccountNumber[j] = tmp.c_str()[j];
        }
    }
    ST_cardData_t emptyCard;
    for (int i = 0; i < 6; i++)
    {
    emptyCard.cardExpirationDate[i] = '0';
    }
    for (int i = 0; i < 25; i++)
    {
    emptyCard.cardHolderName[i] = '0';
    }
    for (int i = 0; i < 20; i++)
    {
        emptyCard.primaryAccountNumber[i] = '0';
    }


    ST_terminalData_t emptyTerminal;
    emptyTerminal.maxTransAmount = 1000;
    emptyTerminal.transAmount = 0;
    for (int i = 0; i < 11; i++)
    {
        emptyTerminal.transactionDate[i] = '0';
    }



    for (int i = 0; i < 225; i++)
    {
        transactionDB[i].cardHolderData = emptyCard;
        transactionDB[i].terminalData = emptyTerminal;
        transactionDB[i].transactionSequenceNumber = 226;
        transactionDB[i].transState = INTERNAL_SERVER_ERROR;
    }
}

uint32_t trans_sequence_number=0;

int accIndex;

int arraySearch(uint8_t* key)
{
    int n = sizeof(accountsDB) / sizeof(*accountsDB);
    int ind = -1;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < strlen((char*)accountsDB[i].primaryAccountNumber)+1; j++)
        {
                if (accountsDB[i].primaryAccountNumber[j] != key[j])
                {
                    break;
                }
                if (j == strlen((char*)accountsDB[i].primaryAccountNumber))
                {
                    ind=i;
                }          
        }
    }
    return ind;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
    try
    {
        if (isValidAccount(&(transData->cardHolderData)) == ACCOUNT_NOT_FOUND) {
            printf("Declined Invalid Account\n");
            return DECLINED_STOLEN_CARD;
        }
        else if (isAmountAvailable(&(transData->terminalData))== LOW_BALANCE) {
            printf("Declined Insuffecient Funds\n");
            return DECLINED_INSUFFECIENT_FUND;
        }
        else
        {
            printf("Old Balance %f:\n", accountsDB[accIndex].balance);
            accountsDB[accIndex].balance -= transData->terminalData.transAmount;
            printf("New Balance %f:\n", accountsDB[accIndex].balance);
            return APPROVED;
        }
    }
    catch (const std::exception&)
    {
        printf("Error: Internal Server Error");
        return INTERNAL_SERVER_ERROR;
    }
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData) {
    accIndex = arraySearch(cardData->primaryAccountNumber);
    if (accIndex <0)
    {
        //printf("Error: Invalid Account");
        return ACCOUNT_NOT_FOUND;
    }
    else {
        //printf("ok");
        return oK;
    }

}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) {
    if (accIndex >= 0) {
    if (termData->transAmount > accountsDB[accIndex].balance)
    {
        //printf("Error: Insuffcient Credit");
        return LOW_BALANCE;
    }
    else {
        //printf("ok");
        return oK;
    }
    }
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
    try
    {
        //Assign a SequenceNumber to the Transaction
        transData->transactionSequenceNumber = trans_sequence_number;
        
        //Save the Transaction
        transactionDB[trans_sequence_number] = *transData;
        
        //Increment SequenceNumber
        trans_sequence_number += 1;
        
        printf("Transaction Saved!");
        return oK;
    }
    catch (const std::exception&)
    {
        return SAVING_FAILED;
    }

}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData) {
	return oK;
}
