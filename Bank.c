#include "Account.h"

int main(int argc, char *argv[])
{
    int continueflag = 0, i = 0;

    char *person;

    if (argc != 3)
    {
        printf("You need to give two arguments(Name/surname)");
        return 1;
    }

    person = person_init(argv[1], argv[2]);

    if (person != "error")
    {
        while (continueflag == 0)
        {
            // checks if file exists.if it does then we choose what we want to do with it. If it doesn't then we create it and then choose what we want to do with it
            if (access(person, F_OK) != -1)
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

                option = check_option(option);

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
                    i = 1;
                    break;

                // Perform account transactions
                case 4:

                    Transaction();
                    i = 1;
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
                if (IbanExists == 1)
                {
                    printf("Iban already exists! Try again!");
                    Create();
                }
                else
                {
                    Create();
                }
            }
        }
    }

    free(filename);
    free(name);
    free(surname);
    free(fullname);

    return 0;
}