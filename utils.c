#include "utils.h"

void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int IbanExists = 0;
// initialize the User
user User;

int isEmpty(const char *str)
{
    char ch;

    do
    {
        ch = *(str++);

        // Check non whitespace character
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            return 0;

    } while (ch != '\0');

    return 1;
}

int check_last_line(char Filename[])
{
    FILE *fp = fopen(Filename, "r");
    int counter = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        counter++;
    }

    fclose(fp);

    return counter;
}

int check_Iban(char *Iban, char *Owner, int flag)
{
    int IbanExists;                            // Initialize the variable
    FILE *Test_Iban = fopen("Ibans.csv", "r"); // Open file in read mode

    if (Test_Iban == NULL)
    {
        perror("Error opening file");
        return -1; // Return error code if file cannot be opened
    }

    char line[256], *IbanToken;

    while (fgets(line, sizeof(line), Test_Iban) != NULL)
    {
        // Remove newline character from the line if it exists
        line[strcspn(line, "\n")] = 0;
        IbanToken = strtok(line, ",");
        IbanToken = strtok(NULL, ",");
        // Check if Iban exists already
        if (IbanToken != NULL && strcmp(IbanToken, Iban) == 0)
        {
            IbanExists = 1;
            break;
        }
    }

    fclose(Test_Iban); // Close the file

    if (IbanExists == 0 && flag == 0)
    {
        // Open file in append mode to write the new Iban
        Test_Iban = fopen("Ibans.csv", "a");
        if (Test_Iban != NULL)
        {
            fprintf(Test_Iban, "%s,%s\n", Owner, Iban);
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
            fprintf(Test_Iban, "%s,%s", Owner, Iban);
            if (fflush(Test_Iban) != 0)
            {
                perror("Error flushing file");
                fclose(Test_Iban);
                return -1;
            }
            fclose(Test_Iban);
        }
    }
    return IbanExists;
}

// the flag variable is for the check_Iban function. If it's 0 then it's going to append the Iban to the Ibans.csv file if not then it will just check if it was used or not
char *GetIban(int flag, char *Owner)
{
    char *Iban = malloc(35);

    // Allocate memory for Owner if necessary
    // Allocate memory for Owner
    User.Owner = malloc(100 * sizeof(char));
    if (User.Owner == NULL)
    {
        perror("Failed to allocate memory for Owner");
        // Handle error appropriately, possibly return NULL or exit
    }
    strcpy(User.Owner, Owner);
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
        IbanExists = check_Iban(Iban, User.Owner, flag);
        if (IbanExists == 1)
        {
            printf("Iban already exists! Try again!\n");
            continue;
        }
        exist = 0;

    } while (exist == 1);
    free(User.Owner);
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

unsigned long long int GetAmount()
{
    unsigned long long int Amounts;
    char input[256];
    char *endptr;

    while (getchar() != '\n')
        ;

    // Reading the amount and checking the size
    while (1)
    {
        printf("Enter Amount: ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Failure! The AMOUNT must contain only digits\n");
            continue;
        }

        // Remove newline character from the input
        input[strcspn(input, "\n")] = '\0';

        // Check if the input is empty
        if (input[0] == '\0')
        {
            printf("Failure! The AMOUNT must contain only digits higher than 0\n");
            continue;
        }

        // Check if the input contains any non-digit characters
        int isValid = 1;
        for (int i = 0; i < strlen(input); i++)
        {
            if (!isdigit(input[i]))
            {
                isValid = 0;
                break;
            }
        }

        if (!isValid)
        {
            printf("Failure! The AMOUNT must contain only digits higher than 0\n");
            continue;
        }

        // Convert string to unsigned long long int
        Amounts = strtoull(input, &endptr, 10);

        // Check if the conversion was successful
        if (*endptr == '\0')
        {
            if (Amounts <= ULLONG_MAX)
                break;
            else
                printf("Amount too big! Try another one\n");
        }
        else
        {
            printf("Failure! The AMOUNT must contain only digits higher than 0\n");
        }
    }

    return Amounts;
}

char *loadUserOwner(char Filename[])
{
    char line[256], *UserToken;
    int row = -1;
    FILE *fp = fopen(Filename, "r");

    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        row++;
        if (row == 1) // Assuming the owner is on the second row (index 1)
        {
            UserToken = strtok(line, ",");
            if (UserToken != NULL)
            {
                // Allocate memory and copy the string
                User.Owner = malloc(strlen(UserToken) + 1);
                if (User.Owner == NULL)
                {
                    perror("Error allocating memory");
                    exit(EXIT_FAILURE);
                }
                strcpy(User.Owner, UserToken);
            }
            break;
        }
    }

    fclose(fp);

    if (row < 1) // Check if we read at least two lines
    {
        fprintf(stderr, "File does not contain enough lines\n");
        exit(EXIT_FAILURE);
    }

    return User.Owner;
}