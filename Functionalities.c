#include "Functionalities.h"

int IbanExists = 0;

void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int check_Iban(char *iban)
{
    int IbanExists;                            // Initialize the variable
    FILE *Test_Iban = fopen("Ibans.csv", "r"); // Open file in read mode

    if (Test_Iban == NULL)
    {
        perror("Error opening file");
        return -1; // Return error code if file cannot be opened
    }

    char line[256];

    while (fgets(line, sizeof(line), Test_Iban) != NULL)
    {
        // Remove newline character from the line if it exists
        line[strcspn(line, "\n")] = 0;

        // Check if Iban exists already
        if (strcmp(line, iban) == 0)
        {
            IbanExists = 1;
            break;
        }
    }

    fclose(Test_Iban); // Close the file

    if (IbanExists == 0)
    {
        // Open file in append mode to write the new Iban
        Test_Iban = fopen("Ibans.csv", "a");
        if (Test_Iban != NULL)
        {
            fprintf(Test_Iban, "%s\n", iban);
            if (fflush(Test_Iban) != 0)
            {
                perror("Error flushing file");
                fclose(Test_Iban);
                return -1;
            }
            fclose(Test_Iban);
        }
        else
        {
            perror("Error opening file for appending");
            return -1;
        }
    }

    return IbanExists;
}

char *GetIban()
{
    char *Iban = malloc(35);
    int exist = 1;
    do
    {
        printf("Enter IBAN:");

        // reading the spaces too
        scanf(" %[^\n]%*c", Iban);

        // Remove trailing newline character, if any
        size_t len = strlen(Iban);
        if (len > 0 && Iban[len - 1] == '\n')
        {
            Iban[len - 1] = '\0';
        }

        // Check if IBAN contains spaces
        int hasSpace = 0;
        for (int i = 0; i < strlen(Iban); i++)
        {
            if (Iban[i] == ' ')
            {
                hasSpace = 1;
                break;
            }
        }

        // If IBAN contains spaces, prompt again
        if (hasSpace)
        {
            printf("Please enter your IBAN without spaces!\n");
            continue;
        }

        while (strlen(Iban) > 34)
        {
            printf("Error...Iban too long\n");
            continue;
        }
        // checks if Iban already was included
        IbanExists = check_Iban(Iban);
        if (IbanExists == 1)
        {
            printf("Iban already exists! Try again!");
            continue;
        }
        exist = 0;

    } while (exist == 1);
    return Iban;
}

char *GetCoin()
{
    char *Coin = malloc(7);
    do
    {
        int flag = 0;
        printf("Enter Coin:");
        scanf("%s", Coin);

        // Check the size
        if (strlen(Coin) > 6)
        {
            printf("Error...The type of coin doesn't exist\n");
            continue;
        }

        // Validate that Coin contains only alphabetic characters
        for (int i = 0; Coin[i] != '\0'; i++)
        {
            if (!isalpha(Coin[i]))
            {
                printf("Error...Coin must contain only alphabetic characters\n");
                // if the coin contains numbers we will flag this thing and go to the next loop
                flag = 1;
                break;
            }
        }
        if (flag == 1)
        {
            continue;
        }
        strupr(Coin);
        break;
    } while (1);
    return Coin;
}

int GetAmount()
{
    unsigned long long int Amounts;
    char term;
    // reading the amount and checking the size
    do
    {
        printf("Enter Amount:");
        // check if the Amount was introduced correctly
        if (scanf("%lld%c", &Amounts, &term) != 2 || term != '\n')
        {
            printf("Failure!The AMOUNT must contain only digits\n");
        }
        // if it was then we check if it is less than the biggest lld value and if it is bigger than 0
        else
        {
            if (Amounts < ULLONG_MAX && Amounts >= 0)
                break;
            else
                printf("Amount too big! Try another one\n");

            if (Amounts < 0)
            {
                printf("The amount can't be less than 0");
                continue;
            }
        }
    } while (1);
    return Amounts;
}

// initialize the User
user User;

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

    User.Owner = Owner;
    User.Iban = GetIban();
    User.Coin = GetCoin();
    User.Amount = GetAmount();

    // Print the user values inside the .csv file
    fprintf(fp, "%s,%s,%s,%llu", User.Owner, User.Iban, User.Coin, User.Amount);

    // we close the file
    fclose(fp);
    clearConsole();
}

void Create_new_account(char Filename[], char Owner[])
{
    FILE *fp = fopen(Filename, "r");
    fprintf(fp, "\n");

    char line[256], *token;
    int row = 0;

    rewind(fp);

    // getting the main values (The Owner's name and the Iban) from the respective Account .csv file and assigning them to the User
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        row++;
        if (row == 1)
        {
            token = strtok(line, ",");
            User.Owner = token;
            printf("%s,%s", token, User.Owner);
            token = strtok(NULL, ",");
            User.Iban = token;
            printf("%s,%s", token, User.Owner);
            break;
        }
    }

    User.Coin = GetCoin();
    User.Amount = GetAmount();

    // printing the values
    fprintf(fp, "%s,%s,%s,%llu", User.Owner, User.Iban, User.Coin, User.Amount);
    fclose(fp);
    clearConsole();
}

// Function for viewing an account
void View_account(char Filename[])
{
    clearConsole();
    FILE *file = fopen(Filename, "r");
    // check if the file exists
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];

    //printing the lines from the .csv file
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
    printf("\n");
    fclose(file);
}

// Function for deleting an account
void deleteAccount(char Filename[])
{
    // We attempt to remove the file
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
    printf("You want to create a new one?(y/n)\n");
    scanf(" %c", &yn);

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
    // File has been successfully removed, no need to close it
    clearConsole();
}

// Transaction between two accounts owned by the same person
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

    char *amounts1, *amounts2;
    char line[256];
    int currentRow1, currentRow2;
    unsigned long long int amount1 = 0, amount2 = 0, transaction = 0;
    const char *Header[] = {"owner", "iban", "coin", "amount"};

    currentRow1 = 0;
    currentRow2 = 0;

    while (fgets(line, sizeof(line), originalFile1) != NULL)
    {
        // If it is not the requested row we skip the iteration
        if (currentRow1 == 0)
        {
            currentRow1++;
            continue;
        }

        // we duplicate the requested row into amounts1
        amounts1 = strdup(line);
        break;
    }

    // We tokenize the row so we can modify the specific column we want to modify
    char *token1 = strtok(amounts1, ",");

    // Iterate over the number of elements in Header
    for (int i = 0; i < 4; i++)
    { // Iterate over the number of elements in Header

        // We extract the amount variable from the file
        if (strcmp(Header[i], "amount") != 0)
        {
            token1 = strtok(NULL, ",");
            continue;
        }

        amount1 = strtoull(token1, NULL, 10); // Convert token to integer
        break;
    }

    while (fgets(line, sizeof(line), originalFile2) != NULL)
    {
        // If it is not the requested row we skip the iteration
        if (currentRow2 == 0)
        {
            currentRow2++;
            continue;
        }

        // we duplicate the requested row into amounts2
        amounts2 = strdup(line);
        break;
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
                // Handle error (e.g., exit program)
            }
        }

        if (tempFile2 != NULL)
        {
            fclose(tempFile2);
            if (remove("temp2.csv") != 0)
            {
                perror("Error removing temp2.csv");
                // Handle error (e.g., exit program)
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

    currentRow1 = 0;

    // rewind the OriginalFile1 so that it begins the reading from the beginning again
    rewind(originalFile1);

    while (fgets(line, sizeof(line), originalFile1) != NULL)
    {
        // until we reach the row where the amount we want to edit is we will put every row into the temp file
        if (currentRow1 == 0)
        {
            currentRow1++;
            // we put the rows in temp file
            fprintf(tempFile1, "%s", line);
            continue;
        }

        // we duplicate the line that we want into amounts1
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
    }

    currentRow2 = 0;

    // rewind the OriginalFile2 so that it begins the reading from the beginning again
    rewind(originalFile2);

    while (fgets(line, sizeof(line), originalFile2) != NULL)
    {
        if (currentRow2 == 0)
        {
            currentRow2++;

            fprintf(tempFile2, "%s", line);
            continue;
        }

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

void Edit_account(char ColumnToEdit[], char Filename[])
{
    FILE *originalFile = fopen(Filename, "r");
    FILE *tempFile = fopen("temp.csv", "w");

    if (originalFile == NULL || tempFile == NULL)
    {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    ColumnToEdit = strlwr(ColumnToEdit);

    int currentRow = 0;
    char *token, line[256]; //*copy;

    while (fgets(line, sizeof(line), originalFile) != NULL)
    {
        if (currentRow == 1)
        {
            // copy = strdup(line);
            token = strtok(line, ",");
            if (token == NULL)
            {
                perror("Tokenizing error");
                exit(EXIT_FAILURE);
            }

            // print Owner
            fprintf(tempFile, "%s,", token);
            token = strtok(NULL, ",");
            // Check if the column we want to edit is the Iban column and then update the token so that the next prints in the .csv file will have the correct column element
            if (strcmp(ColumnToEdit, "iban") == 0)
            {
                User.Iban = GetIban();
                fprintf(tempFile, "%s,", User.Iban);
                token = strtok(NULL, ",");
            }
            else
            {

                fprintf(tempFile, "%s,", token);
                token = strtok(NULL, ",");
            }

            // Check if the column we want to edit is the coin column and then update the token so that the next prints in the .csv file will have the correct column element
            if (strcmp(ColumnToEdit, "coin") == 0)
            {
                User.Coin = GetCoin();
                fprintf(tempFile, "%s,", User.Coin);
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
                User.Amount = GetAmount();
                fprintf(tempFile, "%llu", User.Amount);
                token = strtok(NULL, ",");
            }
            else
            {
                fprintf(tempFile, "%s", token);
                token = strtok(NULL, ",");
            }
            break;
        }
        currentRow++;
        fprintf(tempFile, "%s", line);
    }

    fclose(originalFile);
    fclose(tempFile);

    remove(Filename);
    rename("temp.csv", Filename);
    clearConsole();
}