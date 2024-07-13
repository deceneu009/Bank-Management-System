#include "Functionalities.h"

user Account;

// function for creating an account
void Create_account(char Filename[], char Owner[])
{
    FILE *fp = fopen(Filename, "a");

    IbanExists = 0;

    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int rows;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // check if the file is new
        rows++;
        if (rows > 0)
        {
            break;
        }
    }
    if (rows == 0)
    {
        // printing the header if the file is new
        fprintf(fp, "Owner,IBAN,Coin,Amount\n");
    }

    Account.Owner = Owner;
    Account.Iban = GetIban(0, Account.Owner);
    Account.Coin = GetCoin();
    Account.Amount = GetAmount();

    // Print the user values inside the .csv file
    fprintf(fp, "%s,%s,%s,%llu", Account.Owner, Account.Iban, Account.Coin, Account.Amount);

    // we close the file
    fclose(fp);
    clearConsole();
}

void Create_new_account(char Filename[], char Owner[])
{
    FILE *fp = fopen(Filename, "r");
    fprintf(fp, "\n");

    char line[256], *token;
    int row = -1;

    // getting the main values (The Owner's name and the Iban) from the respective Account .csv file and assigning them to the User
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        row++;
        if (row == 1)
        {
            token = strtok(line, ",");
            Account.Owner = token;
            // printf("%s,%s", token, User.Owner);
            token = strtok(NULL, ",");
            Account.Iban = token;
            // printf("%s,%s", token, User.Owner);
            break;
        }
    }

    Account.Coin = GetCoin();
    Account.Amount = GetAmount();
    fclose(fp);

    // Opening the file in append mode to print the values inside the csv file
    FILE *fpAppend = fopen(Filename, "a");
    // printing the values
    fprintf(fpAppend, "\n%s,%s,%s,%llu", Account.Owner, Account.Iban, Account.Coin, Account.Amount);
    fclose(fpAppend);
    clearConsole();
}

// Function for viewing an account
void View_account(char Filename[])
{
    clearConsole();
    int number = -1;

    FILE *file = fopen(Filename, "r");

    // check if the file exists
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];

    // printing the lines from the .csv file
    while (fgets(line, sizeof(line), file) != NULL)
    {
        number++;
        if (number == 0)
        {
            printf("%s", line);
        }
        else
        {
            printf("%d.%s", number, line);
        }
    }
    printf("\n");
    fclose(file);
}

// I need to make it delete an account and print the others if any. and if the user remains without any account then delete the file and the Iban
void deleteAccount(char Filename[])
{
    View_account(Filename);
    int choice, accountToDelete, validInput = 0;

    do
    {
        printf("Do you want to delete all accounts (0) or just one (1)? ");
        if (scanf("%d", &choice) == 1)
        {
            // Check if the input is within the expected range (0 or 1)
            if (choice == 0 || choice == 1)
            {
                validInput = 1;
            }
            else
            {
                printf("Invalid choice. Please enter 0 or 1.\n");
            }
        }
        else
        {
            // Clear input buffer
            while (getchar() != '\n')
                ; // Clear until newline

            printf("Invalid input. Please enter a valid number.\n");
        }
    } while (!validInput);

    validInput = 0;

    if (choice == 0)
    { // We attempt to remove the file
        if (remove(Filename) == 0)
        {
            printf("Deleted successfully\n");
        }
        else
        {
            perror("Unable to delete the file");
            exit(EXIT_FAILURE);
        }

        // create a new account in case the user wants to
        char yn, *fullname;
        do
        {
            printf("You want to create a new one? (y/n)\n");
            if (scanf(" %c", &yn) == 1)
            {
                // Check if the input is within the expected characters ('y' or 'n')
                if (yn == 'y' || yn == 'n')
                {
                    validInput = 1;
                }
                else
                {
                    printf("Invalid choice. Please enter 'y' or 'n'.\n");
                }
            }
            else
            {
                // Clear input buffer
                while (getchar() != '\n')
                    ; // Clear until newline

                printf("Invalid input. Please enter a valid character.\n");
            }
        } while (!validInput);

        if (yn == 'y')
        {
            char *token = strtok(Filename, ".");
            fullname = malloc(sizeof(token));
            strcpy(fullname, token);
            strcat(Filename, ".csv");
            Create_account(Filename, fullname);
        }
        else if (yn != 'n' && yn != 'y')
        {
            printf("Invalid input!");
        }
        free(fullname);
    }

    validInput = 0;
    char line[256];

    if (choice == 1)
    {
        int row = -1, counter = check_last_line(Filename);
        FILE *fp = fopen(Filename, "r");
        FILE *tempFp = fopen("temp.csv", "w");

        do
        {
            printf("Which account do you want to delete? ");
            if (scanf("%d", &accountToDelete) == 1)
            {
                // Check if the input is within the valid range (1 to counter)
                if (accountToDelete >= 1 && accountToDelete <= counter - 1)
                {
                    validInput = 1;
                }
                else
                {
                    printf("Select an account from 1 to %d.\n", counter - 1);
                }
            }
            else
            {
                // Clear input buffer
                while (getchar() != '\n')
                    ; // Clear until newline

                printf("Invalid input. Please enter a valid integer.\n");
            }
        } while (!validInput);

        int lastNonEmptyLine = -1;

        // First pass: find the last non-empty line excluding the account to delete
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n')
            {
                line[len - 1] = '\0';
            }
            if (!isEmpty(line) && row != accountToDelete)
            {
                lastNonEmptyLine = row;
            }
            row++;
        }

        rewind(fp);
        row = 0;
        int writeCount = 0;

        // Second pass: copy lines, excluding the specified account and newlines after the last line
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n')
            {
                line[len - 1] = '\0';
            }
            if (row == accountToDelete || isEmpty(line))
            {
                row++;
                continue; // Skip the line to delete or empty lines
            }

            if (writeCount > 0)
            {
                fputc('\n', tempFp); // Add newline before every line except the first
            }
            fputs(line, tempFp);
            writeCount++;
            row++;
        }

        fclose(fp);
        fclose(tempFp);

        remove(Filename);
        rename("temp.csv", Filename);
    }
    // File has been successfully removed, no need to close it
    clearConsole();
}

// Transaction between two accounts
void Transactions(char Filename1[], char Filename2[])
{
    char *filenameT1, *filenameT2;

    filenameT1 = (char *)malloc(strlen(Filename1) + 5);
    strcpy(filenameT1, Filename1);
    strcat(filenameT1, ".csv");
    // we concatenate the fullname and .csv in order to create the csv file and access it by the fullname

    filenameT2 = (char *)malloc(strlen(Filename2) + 5);
    strcpy(filenameT2, Filename2);
    strcat(filenameT2, ".csv");

    FILE *originalFile1 = fopen(filenameT1, "r");
    FILE *tempFile1 = fopen("temp1.csv", "w");

    FILE *originalFile2 = fopen(filenameT2, "r");
    FILE *tempFile2 = fopen("temp2.csv", "w");

    // We check if both files open successfully. In case they don't, we exit the program with an error
    if (originalFile1 == NULL || originalFile2 == NULL)
    {
        printf("Error opening files");
        exit(EXIT_FAILURE);
    }

    char *amounts1, *amounts2, line[256];
    int currentRow1 = -1, currentRow2 = -1, wantedRow1 = 0, wantedRow2 = 0, counter1 = check_last_line(filenameT1), counter2 = check_last_line(filenameT2), validInput;
    unsigned long long int amount1 = 0, amount2 = 0, transaction = 0;
    const char *Header[] = {"owner", "iban", "coin", "amount"};

    View_account(filenameT1);

    // make sure the row is correct
    do
    {
        printf("What row do you want to edit?");
        validInput = scanf("%d", &wantedRow1);

        if (validInput != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input. Please enter an integer.\n");
        }
        else if (wantedRow1 < 1 || wantedRow1 > counter1 - 1)
        {
            printf("THe row must be between 1 and %d\n", counter2 - 1);
        }
    } while (validInput != 1 || wantedRow1 < 1 || wantedRow1 > counter1 - 1);

    while (fgets(line, sizeof(line), originalFile1) != NULL)
    {
        currentRow1++;

        // we duplicate the requested row into amounts1
        if (currentRow1 == wantedRow1)
        {
            amounts1 = strdup(line);
            break;
        }
    }

    // We tokenize the row so we can modify the specific column we want to modify
    char *token1 = strtok(amounts1, ",");

    // Iterate over the number of elements in Header
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(Header[i], "amount") != 0)
        {
            token1 = strtok(NULL, ",");
            continue;
        }

        amount1 = strtoull(token1, NULL, 10); // Convert token to integer
        break;
    }

    View_account(filenameT2);

    // we make sure that the row is correct
    do
    {
        printf("What row do you want to edit? ");
        validInput = scanf("%d", &wantedRow2);

        if (validInput != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input. Please enter an integer.\n");
        }
        else if (wantedRow2 < 1 || wantedRow2 > counter2 - 1)
        {
            printf("THe row must be between 1 and %d\n", counter2 - 1);
        }
    } while (validInput != 1 || wantedRow2 < 1 || wantedRow2 > counter2 - 1);

    while (fgets(line, sizeof(line), originalFile2) != NULL)
    {
        currentRow2++;

        // we duplicate the requested row into amounts2
        if (currentRow2 == wantedRow2)
        {
            amounts2 = strdup(line);
            break;
        }
    }

    char *token2 = strtok(amounts2, ",");

    // Iterate over the number of elements in Header
    for (int i = 0; i < 4; i++)
    {
        // We extract the amount variable from the file
        if (strcmp(Header[i], "amount") != 0)
        {
            token2 = strtok(NULL, ",");
            continue;
        }
        amount2 = strtoull(token2, NULL, 10);
        break;
    }

    // print the amounts before transaction
    printf("\n");
    printf("Amount from your account: %llu\n", amount1);
    printf("Amount from the other account: %llu\n", amount2);
    printf("\n");

    if (amount1 == ULLONG_MAX || amount2 == ULLONG_MAX)
    {
        printf("One or both accounts have the maximum amount of money!\n");
        free(filenameT1);
        free(filenameT2);
        free(amounts1);
        free(amounts2);
        fclose(originalFile1);
        fclose(originalFile2);

        // Close temporary files before removal
        if (tempFile1 != NULL)
        {
            fclose(tempFile1);
            if (remove("temp1.csv") != 0)
            {
                perror("Error removing temp1.csv");
            }
        }

        if (tempFile2 != NULL)
        {
            fclose(tempFile2);
            if (remove("temp2.csv") != 0)
            {
                perror("Error removing temp2.csv");
            }
        }
        return;
    }

    while (1)
    {
        char yn;
        printf("How much do you want to send to the other user?");
        scanf("%llu", &transaction);

        // in case the amount of money is <= than the amount of the current account then we perform the transaction
        if (transaction > amount1)
        {
            printf("Transaction too big. Do you want to send a smaller amount?(y/n)");
            scanf(" %c", &yn);
            if (yn == 'y')
            {
                continue;
            }
            else if (yn == 'n')
            {
                return;
            }
            else
            {
                printf("Input not accepted!\n");
                free(filenameT1);
                free(filenameT2);
                free(amounts1);
                free(amounts2);
                fclose(originalFile1);
                fclose(originalFile2);
                // Check if tempFile1 is not NULL before attempting to remove it
                if (tempFile1 != NULL)
                    remove("temp1.csv");

                // Check if tempFile2 is not NULL before attempting to remove it
                if (tempFile2 != NULL)
                    remove("temp2.csv");
                return;
            }
        }

        // most probably bad logic...may be something that I did when I encountered a bug, but now I am too afraid to delete it
        if ((amount2 + transaction) > transaction)
        {
            amount1 -= transaction;
            amount2 += transaction;
            printf("Transaction amount: %llu\n", transaction);
            break;
        }
    }
    // print the amounts after transaction
    printf("\n");
    printf("Amount from your account: %llu\n", amount1);
    printf("Amount from the other account: %llu\n", amount2);
    printf("\n");

    currentRow1 = -1;

    // rewind the OriginalFile1 so that it begins the reading from the beginning again
    rewind(originalFile1);

    int flag = 0;

    while (fgets(line, sizeof(line), originalFile1) != NULL)
    {
        currentRow1++;
        // until we reach the row where the amount we want to edit is we will put every row into the temp file
        if (currentRow1 != wantedRow1)
        {
            // we put the rows in temp file
            fprintf(tempFile1, "%s", line);
            continue;
        }

        // we duplicate the line that we want into amounts1
        if (flag != 1)
        {
            amounts1 = strdup(line);

            // We tokenize the row so we can modify the amount
            char *token1 = strtok(amounts1, ",");
            for (int i = 0; i < 4; i++)
            {
                // Iterate over the number of elements in Header
                if (strcmp(Header[i], "amount") != 0)
                {
                    fprintf(tempFile1, "%s,", token1);
                    token1 = strtok(NULL, ",");
                }

                // When we reach the amount column we edit in the temp file by puting the new value of the amount
                if (strcmp(Header[i], "amount") == 0)
                {
                    fprintf(tempFile1, "%llu", amount1);
                    token1 = strtok(NULL, ",");
                }
            }
            flag = 1;
            if (currentRow1 != counter1 - 1)
            {
                fprintf(tempFile1, "\n");
            }
            continue;
        }
        fprintf(tempFile1, "%s", line);
    }

    currentRow2 = -1;

    // rewind the OriginalFile2 so that it begins the reading from the beginning again
    rewind(originalFile2);

    flag = 0;

    while (fgets(line, sizeof(line), originalFile2) != NULL)
    {
        currentRow2++;
        if (currentRow2 != wantedRow2)
        {
            fprintf(tempFile2, "%s", line);
            continue;
        }
        if (flag != 1)
        {
            amounts2 = strdup(line);

            // We tokenize the row so we can modify the specific column we want to modify
            char *token2 = strtok(amounts2, ",");
            for (int i = 0; i < 4; i++)
            { // Iterate over the number of elements in Header
                if (strcmp(Header[i], "amount") != 0)
                {
                    fprintf(tempFile2, "%s,", token2);
                    token2 = strtok(NULL, ",");
                }

                // We check if the element we want to edit exists
                if (strcmp(Header[i], "amount") == 0)
                {
                    fprintf(tempFile2, "%llu", amount2);
                    token2 = strtok(NULL, ",");
                }
            }
            flag = 1;
            if (currentRow2 != counter2 - 1)
            {
                fprintf(tempFile2, "\n");
            }
            continue;
        }
        fprintf(tempFile2, "%s", line);
    }

    // Close files
    fclose(originalFile1);
    fclose(tempFile1);
    fclose(originalFile2);
    fclose(tempFile2);

    // Opening the temp files again, but in read mode so we can read the lines that got modified
    FILE *temp1 = fopen("temp1.csv", "r");
    FILE *temp2 = fopen("temp2.csv", "r");

    currentRow1 = 0;
    currentRow2 = 0;

    // printing the end results
    printf("Your account new amount:\n");
    while (fgets(line, sizeof(line), temp1) != NULL)
    {
        if (currentRow1 == 1)
        {
            printf("%s", line);
            break;
        }
        currentRow1++;
    }
    printf("\n");

    printf("The other account new amount:\n");
    while (fgets(line, sizeof(line), temp2) != NULL)
    {
        if (currentRow2 == 1)
        {
            printf("%s", line);
            break;
        }
        currentRow2++;
    }
    printf("\n");

    // closing them again
    fclose(temp1);
    fclose(temp2);

    // free memory
    free(amounts1);
    free(amounts2);

    // remove the original files and replace it with the updated files that contain the correct amounts after transaction
    remove(filenameT1);
    rename("temp1.csv", filenameT1);
    remove(filenameT2);
    rename("temp2.csv", filenameT2);

    free(filenameT1);
    free(filenameT2);
    clearConsole();
}

// TO DO: need to change it to work for multiple account for the same user
void Edit_account(char ColumnToEdit[], char Filename[])
{
    FILE *originalFile = fopen(Filename, "r");
    FILE *tempFile = fopen("temp.csv", "w");

    if (originalFile == NULL || tempFile == NULL)
    {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    // ColumnToEdit[255] = '\0';

    Account.Owner = loadUserOwner(Filename);

    strlwr(ColumnToEdit);

    int currentRow = -1, rowToEdit = 0;
    char *token, line[256];

    if (strcmp(ColumnToEdit, "iban") != 0)
    {
        int counter = check_last_line(Filename), validInput;
        do
        {
            printf("What row do you want to edit? ");
            validInput = scanf("%d", &rowToEdit);

            // Clear the input buffer if the input was not valid
            if (validInput != 1)
            {
                while (getchar() != '\n')
                    ; // discard invalid input
                printf("Invalid input. Please enter an integer.\n");
            }
            else if (rowToEdit < 1 || rowToEdit > counter - 1)
            {
                printf("The row must be between 1 and %d\n", counter - 1);
            }

        } while (validInput != 1 || rowToEdit < 1 || rowToEdit > counter - 1);

        while (fgets(line, sizeof(line), originalFile) != NULL)
        {
            currentRow++;

            if (currentRow == rowToEdit)
            {
                token = strtok(line, ",");
                if (token == NULL)
                {
                    perror("Tokenizing error");
                    exit(EXIT_FAILURE);
                }

                // print Owner
                fprintf(tempFile, "%s,", token);
                token = strtok(NULL, ",");

                // print Iban
                fprintf(tempFile, "%s,", token);
                token = strtok(NULL, ",");

                // Check if the column we want to edit is the coin column and then update the token so that the next prints in the .csv file will have the correct column element
                if (strcmp(ColumnToEdit, "coin") == 0)
                {
                    Account.Coin = GetCoin();
                    fprintf(tempFile, "%s,", Account.Coin);
                    token = strtok(NULL, ",");
                }
                else
                {
                    fprintf(tempFile, "%s,", token);
                    token = strtok(NULL, ",");
                }
                // Check if the column we want to edit is the amount column and then update the token so that the next prints in the .csv file will have the correct column element
                if (strcmp(ColumnToEdit, "amount") == 0)
                {
                    Account.Amount = GetAmount();
                    fprintf(tempFile, "%llu", Account.Amount);
                    if (currentRow != counter - 1)
                    {
                        fputs("\n", tempFile);
                    }
                }
                else
                {
                    fprintf(tempFile, "%s", token);
                }

                // if (currentRow != counter)
                // {
                //     fprintf(tempFile, "\n");
                // }

                continue;
            }
            fprintf(tempFile, "%s", line);
        }
        // if there are empty lines
        rewind(tempFile);
        int row = 0;
        int writeCount = 0;

        // Second pass: copy lines, excluding the specified account and newlines after the last line
        while (fgets(line, sizeof(line), tempFile) != NULL)
        {
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n')
            {
                line[len - 1] = '\0';
            }
            if (row == rowToEdit || isEmpty(line))
            {
                row++;
                continue; // Skip the line to delete or empty lines
            }

            if (writeCount > 0)
            {
                fputc('\n', tempFile); // Add newline before every line except the first
            }
            fputs(line, tempFile);
            writeCount++;
            row++;
        }
    }
    else
    {
        char lineIbans[256], lineCopy[256];
        int row = 0;
        Account.Iban = GetIban(1, Account.Owner);

        // change Iban inside of the Ibans.csv
        char *OwnerTokenIban;

        FILE *OriginalIbans = fopen("Ibans.csv", "r");
        FILE *tempIbans = fopen("tempIbans.csv", "w");

        int counter = check_last_line("Ibans.csv");

        if (OriginalIbans == NULL || tempIbans == NULL)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        // it's printing the same thing multiple times. Need to fix
        while (fgets(lineIbans, sizeof(lineIbans), OriginalIbans) != NULL)
        {
            strcpy(lineCopy, lineIbans);
            OwnerTokenIban = strtok(lineCopy, ",");
            row++;
            if (strcmp(Account.Owner, OwnerTokenIban) == 0)
            {
                fprintf(tempIbans, "%s,%s", Account.Owner, Account.Iban);
                if (row != counter)
                {
                    fprintf(tempIbans, "\n");
                }
                continue;
            }

            fprintf(tempIbans, "%s", lineIbans);
        }

        fclose(tempIbans);
        fclose(OriginalIbans);

        if (remove("Ibans.csv") != 0)
        {
            perror("Error deleting Ibans.csv");
            exit(EXIT_FAILURE);
        }
        if (rename("tempIbans.csv", "Ibans.csv") != 0)
        {
            perror("Error renaming tempIbans.csv to Ibans.csv");
            exit(EXIT_FAILURE);
        }

        int flag = 0;

        // second change the Ibans inside the User's .csv file
        while (fgets(line, sizeof(line), originalFile))
        {
            // printing the header
            if (flag == 0)
            {
                fprintf(tempFile, "%s", line);
                flag = 1;
                continue;
            }

            // printing the changes
            if (flag == 1)
            {
                token = strtok(line, ",");
                // printing the Owner
                fprintf(tempFile, "%s,", token);
                // printing the Iban
                fprintf(tempFile, "%s,", Account.Iban);
                token = strtok(NULL, ",");
                token = strtok(NULL, ",");
                // printing the Coin
                fprintf(tempFile, "%s,", token);
                token = strtok(NULL, ",");
                // printing the amount
                fprintf(tempFile, "%s", token);
            }
        }
    }
    fclose(originalFile);
    fclose(tempFile);

    free(Account.Owner);

    if (remove(Filename) != 0)
    {
        perror("Error deleting original file");
        exit(EXIT_FAILURE);
    }
    if (rename("temp.csv", Filename) != 0)
    {
        perror("Error renaming temp file to original file");
        exit(EXIT_FAILURE);
    }

    clearConsole();
}