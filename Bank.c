#include "Account.h"

int main(int argc, char *argv[])
{
    int continueflag = 0, i = 0;

    if (argc != 3)
    {
        printf("You need to give two arguments(Name/surname)");
        return 1;
    }

    // we put the arguments into the variables surname and name which will becaome the fullname
    surname = argv[1];
    name = argv[2];

    // Validate input strings
    if (surname[0] == '\0' || name[0] == '\0')
    {
        printf("Surname and name must not be empty\n");
        return 1;
    }

    strlwr(name);
    strlwr(surname);

    fullname= malloc(sizeof(name)+sizeof(surname)+1);

    strcpy(fullname, strcat(surname, name));
    filename = malloc(strlen(fullname) + 5); // ".csv" + null terminator
    if (filename == NULL)
    {
        perror("Error allocating memory");
        return 1;
    }
    strcpy(filename, fullname);
    // we concatenate the fullname and .csv in order to create the csv file and access it by the fullname
    strcat(filename, ".csv");

    while (continueflag == 0)
    {
        // checks if file exists.if it does then we choose what we want to do with it. If it doesn't then we create it and then choose what we want to do with it
        if (access(filename, F_OK) != -1)
        {
            int option;
            if (i == 0)
            {
                printf("\n");
                printf("=========================\n");
                printf("Welcome to the BANK!\n\nWhat do you want to do?\n1.Edit account\n2.Delete account\n3.View account\n4.Perform transaction\n5.Create a new account\n6.Exit\n");
                printf("Choose:");
            }
            else
            {
                printf("Do you want to do something else?\nWhat do you want to do?\n1.Edit account\n2.Delete account\n3.View account\n4.Perform transaction\n5.Create a new account\n6.Exit\n");
                printf("Choose:");
            }

            // Read the option (in case it is not an integer it will print an error)
            while (scanf(" %d", &option) != 1)
            {
                printf("\nInvalid input. Please enter a number from 1 to 6!.\n");

                // Clear input buffer
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    continue; // Restart the loop
                
                if(c=='\n')
                {
                    printf("Choose:");
                }
                //return 1;
            }
            printf("\n");

            // Menu
            switch (option)
            {
            // Edit account
            case 1:

                Edit();
                i++;
                break;

            // Delete account
            case 2:

                Delete();
                i++;
                break;

            // View Account
            case 3:

                View();
                i=1;
                break;

            // Perform account transactions
            case 4:

                Transaction();
                i=1;
                break;

            // Create a new account
            case 5:
                Add_New_Account();
                break;
            // Exit
            case 6:
                continueflag = 1;
                printf("\n=========================\n");
                break;

            // Error
            default:

                printf("Error\n");
                printf("\n=========================\n");
                i = 1;
                break;
            }
        }
        else
        {
            if(IbanExists==1)
            {
                printf("Iban already exists! Try again!");
                Create();
            }
            Create();
        }
    }

    free(filename);
    free(name);
    free(surname);
    free(fullname);

    return 0;
}