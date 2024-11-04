#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_PASSWORD_LENGTH 28

//Adding employee structure
typedef struct {
    int id;
    char name[30];
    char address[100];
    char email[25];
    char position[20];
    float salary;
    char joiningdate[15];
} Employee;

//Adding menu structure
typedef struct {
    int itemtype;
    int itemnumber;
    char itemname[30];
    int price;
} MenuItem;

//Adding orders structure
typedef struct {
    int orderNo;
    char customer[30];
    char address[200];
    int number;
    char remarks[50];    
    int total;
    int items[50];
} Order;

// Function declarations
void mainmenu();
void administrator();
void employee();
void customer();
int password_check(int caseValue);

//Declaring addministrative functions
void employee_addrecord();
void employee_deleterecord();
void employee_searchrecord();
void order_viewrecord();
void changePassword(int caseValue);

//Declaring employee functions
void addmenu();
void deletemenu();

//Declaring customer functions
Order addcart(Order currentOrder);
Order checkout(Order currentOrder);
int getLastOrderNumber();

//Declaring password related functions
void readPasswords();
void writePasswords();

//main menu
void mainmenu() {
    system("cls");
    int choice = 0;

    while(choice != 5){
        printf("\n----- Foodies Main Menu -----\n");
        printf("\t1. Administrator\n");
        printf("\t2. Employee\n");
        printf("\t3. Customer\n");
        printf("\t4. About us\n");
        printf("\t5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (password_check(1)) {
                    administrator();
                } else {
                    printf("Incorrect password. Access denied.\n");
                }
                break;
            case 2:
                if (password_check(0)) {
                    employee();
                } else {
                    printf("Incorrect password. Access denied.\n");
                }
                break;
            case 3:
                customer();
                break;
            case 4:
                printf("-----About us-----");
                break;
            case 5:
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

//Defining all administrative functions
void administrator() {
    system("cls");
    int Achoice = 0;

    while(Achoice != 6){
        printf("Welcome to the Administratitive Section\n");
        printf("\t1. Add Employee Record\n");
        printf("\t2. Delete Employee Record\n");
        printf("\t3. Search Employee Record\n");
        printf("\t4. View Order Record\n");
        printf("\t5. Change passwords\n");
        printf("\t6. Return to main menu\n");
        printf("\tEnter your choice: ");
        scanf("%d", &Achoice);

        // Perform actions based on the user's choice
        switch (Achoice) {
            case 1:
                // Call the function to add employee record
                employee_addrecord();
                break;
            case 2:
                // Call the function to delete employee record
                employee_deleterecord();
                break;
            case 3:
                // Call the function to search employee record
                employee_searchrecord();
                break;
            case 4:
                // Call the function to view order record
                order_viewrecord();
                break;
            case 5:
                //Take input of which password to change
                int caseValue;
                printf("Which password do you want to change?\n\t1. Administrator\n\t2.Employee");
                scanf("%d", &caseValue);
                fflush(stdin);
                //call the password change function
                if(caseValue == 1)
                    changePassword(1);
                else if(caseValue == 2)
                    changePassword(0);
                else
                    printf("Invalid input");
                    break;
            case 6:
                // Call the main menu function to return
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
}

void employee_addrecord() {
    Employee newEmployee;

    // Prompt for input
    printf("Enter employee details:\n");
    printf("\tID: ");
    scanf("%d", &newEmployee.id);
    fflush(stdin);
    printf("\tName: ");
    gets(newEmployee.name);
    fflush(stdin);
    printf("\tAddress: ");
    gets(newEmployee.address);
    fflush(stdin);
    printf("\tEmail: ");
    gets(newEmployee.email);
    fflush(stdin);
    printf("\tPosition: ");
    gets(newEmployee.position);
    fflush(stdin);
    printf("\tSalary: ");
    scanf("%f", &newEmployee.salary);
    fflush(stdin);
    printf("\tJoining Date: ");
    gets(newEmployee.joiningdate);
    fflush(stdin);

    // Open the file in append mode
    FILE* file = fopen("employee.txt", "a+");
    if (file == NULL) {
        printf("Error opening file employee.\n");
        return;
    }

    // Write the employee record to the file
    fprintf(file, "%d~%s~%s~%s~%s~%.2f~%s\n",
            newEmployee.id,
            newEmployee.name,
            newEmployee.address,
            newEmployee.email,
            newEmployee.position,
            newEmployee.salary,
            newEmployee.joiningdate);

    fclose(file);

    printf("Add another employee? y/n");
    char another;
    scanf("%c", &another);
    if(another == 'y')
        employee_addrecord();
}

void employee_deleterecord() {
    int employeeId;
    printf("Enter the ID of the employee to delete: ");
    scanf("%d", &employeeId);
    fflush(stdin);

    // Open the input and output files
    FILE* inputFile = fopen("employee.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening file employee.\n");
        return;
    }
    FILE* outputFile = fopen("temp.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening file temp.\n");
        fclose(inputFile);
        return;
    }

    int found = 0;
    Employee currentEmployee;
    fflush(stdin);

    // Read the records from the input file and copy them to the output file, except for the employee to be deleted
    while (fscanf(inputFile, "%d~%[^~]~%[^~]~%[^~]~%[^~]~%f~%[^~]", &currentEmployee.id, currentEmployee.name, currentEmployee.address, currentEmployee.email, currentEmployee.position, &currentEmployee.salary, currentEmployee.joiningdate) == 7) {
        if (currentEmployee.id == employeeId) {
            found = 1;
        } else {
            fprintf(outputFile, "%d~%s~%s~%s~%s~%.2f~%s\n",
                    currentEmployee.id,
                    currentEmployee.name,
                    currentEmployee.address,
                    currentEmployee.email,
                    currentEmployee.position,
                    currentEmployee.salary,
                    currentEmployee.joiningdate);
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    if (found) {
        // Delete the original file
        remove("employee.txt");

        // Rename the temporary file to the original file
        rename("temp.txt", "employee.txt");

        printf("Employee record deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("Employee record not found.\n");
    }
    fflush(stdin);
    printf("Delete another employee? y/n");
    char another;
    scanf("%c", &another);
    if(another == 'y')
        employee_deleterecord();
}

void employee_searchrecord() {
    printf("\t1. Id\n\t2. Name\nEnter method to search by: ");
    fflush(stdin);
    int option;
    scanf("%d", &option);
    fflush(stdin);
    int employeeId;
    char employeeName[30];
    switch(option){
        case 1:
            printf("Enter the ID of the employee to search: ");
            scanf("%d", &employeeId);
            fflush(stdin);
            break;
        case 2: 
            printf("Enter the name of the employee to search: ");
            gets(employeeName);
            fflush(stdin);
            break;
        default:
            printf("Invalid choice.\n");
            break;

    }

    // Open the file for reading
    FILE* file = fopen("employee.txt", "r");
    if (file == NULL) {
        printf("Error opening file employee.\n");
        return;
    }

    Employee currentEmployee;
    int found = 0;
    fflush(stdin);

    // Search for the employee record
    if(option == 1){
        while (fscanf(file, "%d~%[^~]~%[^~]~%[^~]~%[^~]~%f~%[^~]", &currentEmployee.id, currentEmployee.name, currentEmployee.address, currentEmployee.email, currentEmployee.position, &currentEmployee.salary, currentEmployee.joiningdate) == 7) {
            if (currentEmployee.id == employeeId) {
                found = 1;
                break;
            }
        }
    }
    else if (option == 2){
           while (fscanf(file, "%d~%[^~]~%[^~]~%[^~]~%[^~]~%f~%[^~]", &currentEmployee.id, currentEmployee.name, currentEmployee.address, currentEmployee.email, currentEmployee.position, &currentEmployee.salary, currentEmployee.joiningdate) == 7) {
            if (strcmp(currentEmployee.name, employeeName) == 0) {
                found = 1;
                break;
            }
        } 
    }

    fclose(file);

    if (found) {
        // Display the employee details
        printf("Employee found:\n");
        printf("ID: %d\n", currentEmployee.id);
        printf("Name: %s\n", currentEmployee.name);
        printf("Address: %s\n", currentEmployee.address);
        printf("Email: %s\n", currentEmployee.email);
        printf("Position: %s\n", currentEmployee.position);
        printf("Salary: %.2f\n", currentEmployee.salary);
        printf("Joining Date: %s\n", currentEmployee.joiningdate);
    } else {
        printf("Employee not found.\n");
    }
    fflush(stdin);
    printf("Search another employee? y/n");
    char another;
    scanf("%c", &another);
    if(another == 'y')
        employee_searchrecord();
}

void order_viewrecord() {
    // Open the order file for reading
    fflush(stdin);
    FILE* orderFile = fopen("order.txt", "r");
    if (orderFile == NULL) {
        printf("Error opening file order.\n");
        return;
    }

    // Open the menu file for reading
    FILE* menuFile = fopen("menu.txt", "r");
    if (menuFile == NULL) {
        printf("Error opening file menu.\n");
        fclose(orderFile);
        return;
    }

    Order currentOrder;

    // Read and display the order records
    while (fscanf(orderFile, "%d~%[^~]~%[^~]~%d~%[^~]~%d~",
                  &currentOrder.orderNo, currentOrder.customer, currentOrder.address,
                  &currentOrder.number, currentOrder.remarks, &currentOrder.total) == 6) {
        for (int i = 0; i < 50; i++) {
            fscanf(orderFile, "%d", &currentOrder.items[i]);
        }
        printf("Order No: %d\n", currentOrder.orderNo);
        printf("Customer: %s\n", currentOrder.customer);
        printf("Address: %s\n", currentOrder.address);
        printf("Number: %d\n", currentOrder.number);
        printf("Remarks: %s\n", currentOrder.remarks);
        printf("Total: %d\n", currentOrder.total);

        printf("Items:\n");

        MenuItem currentMenu;
        int i = 0;

        // Rewind the menu file to the beginning
        rewind(menuFile);

        while (fscanf(menuFile, "%d~%d~%[^~]~%d",
                      &currentMenu.itemtype, &currentMenu.itemnumber, currentMenu.itemname, &currentMenu.price) == 4) {
            if (currentOrder.items[i] != 0) {
                printf("Item: %s, Quantity: %d\n", currentMenu.itemname, currentOrder.items[i]);
            }
            i++;
        }

        printf("\n");
    }

    fclose(menuFile);
    fclose(orderFile);
}


//Defining all employee functions
void employee() {
    system("cls");
    fflush(stdin);
    int Echoice;

    printf("Employee Section\n");
    printf("\t1. Add Menu Item\n");
    printf("\t2. Delete Menu Item\n");
    printf("\t3. View Order Record\n");
    printf("\t4. Return to main menu\n");
    printf("Enter your choice: ");
    scanf("%d", &Echoice);
    system("cls");
    fflush(stdin);

    // Perform actions based on the user's choice
    switch (Echoice) {
        case 1:
            // Call the function to add a menu item
            addmenu();
            break;
        case 2:
            // Call the function to delete a menu item
            deletemenu();
            break;
        case 3:
            // Call the function to view the order record
            order_viewrecord();
            break;
        case 4:
            // Return to main menu
            mainmenu();
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
    printf("Run again? y/n");
    char another;
    scanf("%c", &another);
    if(another == 'y')
        employee();
}

void addmenu() {
    MenuItem newItem;

    // Prompt for input
    printf("Enter menu item details:\n");
    printf("Item type (1. Starter, 2. Platter, 3. Beverages, 4. Desserts): \n");
    scanf("%d", &newItem.itemtype);
    fflush(stdin);
    printf("Item Number: ");
    scanf("%d", &newItem.itemnumber);
    fflush(stdin);
    printf("Item Name: ");
    fflush(stdin);
    gets(newItem.itemname);
    printf("Price: ");
    scanf("%d", &newItem.price);
    fflush(stdin);

    // Open the file in append mode
    FILE* file = fopen("menu.txt", "a+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write the menu item to the file
    fprintf(file, "%d~%d~%s~%d\n",newItem.itemtype, newItem.itemnumber, newItem.itemname, newItem.price);

    fclose(file);
    fflush(stdin);
    //Loop to return
    printf("Add another item? y/n");
    char another;
    scanf("%c", &another);
    if(another == 'y')
        addmenu();
}

void deletemenu() {
    int itemNumber;
    printf("Enter the item number to delete: ");
    scanf("%d", &itemNumber);
    fflush(stdin);

    // Open the input and output files
    FILE* inputFile = fopen("menu.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening file menu.\n");
        return;
    }
    FILE* outputFile = fopen("temp.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening file temp.\n");
        fclose(inputFile);
        return;
    }
    fflush(stdin);

    int found = 0;
    MenuItem currentItem;

    // Read the records from the input file and copy them to the output file, except for the item to be deleted
    while (fscanf(inputFile, "%d~%d~%[^~]~%d", &currentItem.itemtype, &currentItem.itemnumber, currentItem.itemname, &currentItem.price) == 4) {
        int i = 1;
        if (currentItem.itemnumber == itemNumber) {
            found = 1;
        } else {
            fprintf(outputFile, "%d~%d~%s~%d\n", i, currentItem.itemname, currentItem.price);
            i++;
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    fflush(stdin);

    if (found) {
        // Delete the original file
        remove("menu.txt");

        // Rename the temporary file to the original file
        rename("temp.txt", "menu.txt");

        printf("Menu item deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("Menu item not found.\n");
    }
    fflush(stdin);

    //Loop to return
    printf("Delete another item? y/n");
    char another;
    scanf("%c", &another);
    if(another == 'y')
        deletemenu();
    else 
        employee();
}

//Defining all customer choices
void customer(){
    system("cls");
    int Cchoice = 0;
    char another;
    Order currentOrder;
    currentOrder.orderNo = getLastOrderNumber() + 1;

    while (Cchoice != 4){
        printf("Welcome to the Customer Section\n");
        printf("\t1. Add items to the cart\n");
        printf("\t2. Checkout or place order\n");
        printf("\t3. Cancel order\n");
        printf("\t4. Return to main menu\n");
        printf("\tEnter your choice: ");
        scanf("%d", &Cchoice);

        // Perform actions based on the user's choice
        switch (Cchoice) {
            case 1:
                // Adding items to your cart
                currentOrder = addcart(currentOrder);
                printf("Proceed to checkout? y/n");
                scanf("%c", &another);
                if(another == 'y')
                    currentOrder = checkout(currentOrder);
                break;
            case 2:
                // Placing order via delivery or dine-in
                checkout(currentOrder);
                printf("Place another order? y/n");
                scanf("%c", &another);
                if(another == 'y')
                    customer();
                break;
            case 3:
                // canceling order
                printf("Order canceled!!");
                printf("Place another order? y/n");
                scanf("%c", &another);
                if(another == 'y')
                    customer();
                break;
            case 4:
                // Call the main menu function to return
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
        }
}

Order addcart(Order currentOrder) {
    // Open and display the starter menu
    FILE* file = fopen("menu.txt", "r");
    if (file == NULL) {
        printf("Error opening file menu.\n");
    }

    MenuItem currentItem;
    int itemNumber = 500;
    int quantity;

    printf("Menu:\n");

    // Read and display the starter menu MenuItem
    while (fscanf(file, "%d~%d~%[^~]~%d\n", &currentItem.itemtype, &currentItem.itemnumber, currentItem.itemname, &currentItem.price) == 4) {
        printf("%d. %s - $%dtk\n", currentItem.itemnumber, currentItem.itemname, currentItem.price);
    }

    fclose(file);

    // Process the customer's choices
    while (itemNumber != 0) {
        // Prompt the customer for their choice
        printf("Enter the item number to add to the cart (0 to exit): ");
        scanf("%d", &itemNumber);

        if (itemNumber == 0) {
            break;
        }

        // Prompt for quantity
        printf("Enter the quantity: ");
        scanf("%d", &quantity);

        // Check if the item number is valid
        int itemFound = 0;
        file = fopen("menu.txt", "r");
        while (fscanf(file, "%d~%d~%[^~]~%d\n", &currentItem.itemtype, &currentItem.itemnumber, currentItem.itemname, &currentItem.price) == 4) {
            if (currentItem.itemnumber == itemNumber) {
                itemFound = 1;
                break;
            }
        }
        fclose(file);

        if (itemFound) {
            // Add the item to the cart and update the total
            currentOrder.items[itemNumber - 1] += quantity;
            currentOrder.total += currentItem.price * quantity;
        } else {
            printf("Invalid item number.\n");
        }
    }
    return currentOrder;
}



int getLastOrderNumber() {
    FILE* file = fopen("order.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    int lastOrderNumber = 0;
    char hudai[300];

    while (fscanf(file, "%d~%[^\n]", &lastOrderNumber, hudai) == 2) {}

    fclose(file);

    return lastOrderNumber;
}

Order checkout(Order currentOrder) {
    char deliveryChoice;
    
    // Prompt for delivery or dine-in
    printf("Checkout\n");
    printf("Select an option:\n");
    printf("1. Delivery\n");
    printf("2. Dine-in\n");
    printf("Enter your choice (1 or 2): ");
    fflush(stdin);
    scanf(" %c", &deliveryChoice);
    fflush(stdin);
    
    if (deliveryChoice == '1') {
        // Delivery option selected
        printf("\nDelivery Details\n");
        printf("Enter your name: ");
        gets(currentOrder.customer);
        fflush(stdin);
        
        printf("Enter your address: ");
        gets(currentOrder.address);
        fflush(stdin);
        
        printf("Enter your contact number: ");
        scanf("%d", &currentOrder.number);
        fflush(stdin);
        
        printf("Enter any remarks (if any): ");
        gets(currentOrder.remarks);
        fflush(stdin);

    } else if (deliveryChoice == '2') {
        // Dine-in option selected
        printf("\nDine-in Details\n");
        printf("Enter your name: ");
        gets(currentOrder.customer);
        fflush(stdin);
    } else {
        printf("Invalid choice. Please try again.\n");
    }
    
    // Save the order to the "order.txt" file
    FILE* file = fopen("order.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
    }
    fflush(stdin);

    fprintf(file, "%d~%s~%s~%d~%s~%d~", currentOrder.orderNo, currentOrder.customer, currentOrder.address, currentOrder.number, currentOrder.remarks, currentOrder.total);
    for(int i = 0; i < 50; i++){fprintf(file, "%d ", currentOrder.items[i]);}
    fprintf(file, "\n");
    fclose(file);
    
    printf("\nOrder placed successfully!!!\n your grand total is %d\n", currentOrder.total);
    return currentOrder;
}

//Defining all password functions
char employeePassword[MAX_PASSWORD_LENGTH];
char adminPassword[MAX_PASSWORD_LENGTH];

void readPasswords() {
    FILE* file = fopen("password.txt", "r");
    if (file == NULL) {
        printf("Error opening password file.\n");
        exit(1);
    }

    // Read the employee password
    fscanf(file, "%s", employeePassword);

    // Read the administrator password
    fscanf(file, "%s", adminPassword);

    fclose(file);
}

void writePasswords() {
    FILE* file = fopen("password.txt", "w+");
    if (file == NULL) {
        printf("Error opening password file.\n");
        exit(1);
    }

    // Write the employee password
    fprintf(file, "%s\n", employeePassword);

    // Write the administrator password
    fprintf(file, "%s\n", adminPassword);

    fclose(file);
}

int password_check(int caseValue) {
    readPasswords();

    char store_pass[MAX_PASSWORD_LENGTH];
    int match = 1, i = 0;
    
    printf("Enter the password: ");
    
    while (1) {
        char ch = getch();
        if (ch == '\r')  // Check for Enter key (carriage return)
            break;
        
        fflush(stdin);
        printf("*");
        
        if (i < MAX_PASSWORD_LENGTH - 1) {
            store_pass[i] = ch;
            if (caseValue == 0 && store_pass[i] != employeePassword[i])
                match = 0;
            else if (caseValue == 1 && store_pass[i] != adminPassword[i])
                match = 0;
            i++;
        } else {
            printf("\nMaximum password length exceeded.\n");
            return 0;
        }
    }
    
    return match;
}

void changePassword(int caseValue) {
    char newPass[MAX_PASSWORD_LENGTH];
    
    printf("Enter the new password: ");
    scanf("%s", newPass);
    
    if (caseValue == 0) {
        strcpy(employeePassword, newPass);
        printf("Employee password changed successfully.\n");
    } else if (caseValue == 1) {
        strcpy(adminPassword, newPass);
        printf("Administrator password changed successfully.\n");
    }

    writePasswords();
}


//Finally the main function
    int main(void){
        mainmenu();
        return 0;
    }