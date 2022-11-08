
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

//for most recent error's number
extern int errno;

//Defining  Constansts First//
#define DISCOUNT_MAX_AGE 25
#define DISCOUNT_MIN_AGE 22
#define DISCOUNT_PERCENTAGE 0.23f 
#define MAXIMUM_SALES 200
#define CSV_FILE "SalesData.csv"
#define MENU_OPTION_VIEW_CARS 'a'
#define MENU_OPTION_BUY_CARS 'b'
#define MENU_OPTION_VIEW_SALES 'c' 
#define MENU_OPTION_VIEW_SALES_BETWEEN 'd' 
#define MENU_OPTION_EXIT 'x' 
#define TRUE 1 
#define FALSE 0 

// it can accept unsigned char type values.
#define bool unsigned char

// Defining the Variables//
unsigned short CarsAvailable = 200, numberOfSales = 0, CarAmountPerSale[MAXIMUM_SALES], TypeOfCarPerSale[MAXIMUM_SALES];
bool DiscountGivenPerSale[MAXIMUM_SALES];

//holding name of customer for each sale, assuming name only upto 70 charcters
//along with it the array holding the types of Car
char customerNames[MAXIMUM_SALES][71];
char CarTypes[][10] = { "TESLA", "MERCEDES", "NISSAN","FERRARI" };


//defining the price of car sequentially
float carPrices[] = { 82900, 35800, 21500, 27050 };

#define FILE_OPENED 0
#define FILE_CLOSED 1
#define FILE_ERROR 2

FILE* file;
unsigned char fileStatus = FILE_CLOSED;

//!! Functions Used in Program !!//
void clearScreen()
{
	system("cls"); 
}

void pauseProgram(char userChoice)
{
	if (userChoice == MENU_OPTION_EXIT)
	{
		printf("\n\n     . . .   Please Press Enter to Exit  .  .   .  ! ");
	}
	else
	{
		printf("\n\n      . .  . Please Press Enter to Return the Main Menu . . . ");
	}

	// this is because if user Input is underscore, 
	// it is used in pausing after file error occured
	if (userChoice != '_') 
	{
		getchar();
	}

	getchar();
}

char getCharFromConsole(char message[201])
{
	char userInput;
	printf(message);
	scanf("\n%c", &userInput);
	return userInput;
}

unsigned short getUnsignedShortFromConsole(char message[201])
{
	unsigned short userInput;
	printf(message);
	scanf("%hd", &userInput);
	return userInput;
}

//here *str is used because if any changes occurr in varibale it will also get modified in the existing string variable.
void getStringFromConsole(char message[201], char* str)
{
	printf(message);
	scanf("\n%[^\n]s", str);
}

void swapUnsignedShort(unsigned short* a, unsigned short* b)
{
	unsigned short temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void swapBool(bool* a, bool* b) {
	bool temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

//-- FILE FUNCTIONS --
FILE* createFile(char fileName[201]) {
	file = fopen(fileName, "w");                               //The mode W represents writing
	if (file != NULL) {
		fclose(file);
	}
	return file;
}


void openFile(char fileName[201], char mode[4]) {
	file = fopen(fileName, mode);

	if (file == NULL)
	{
		//if there is no error it would manage to open a file and incase of error the error is displayed
		if (createFile(fileName) == NULL)
		{
			fileStatus = FILE_ERROR;
			printf("Error opening %s: %s", fileName, strerror(errno));
		}
		else
		{
			openFile(fileName, mode);
		}

	}
	else
	{
		fileStatus = FILE_OPENED;
	}
}

void closeFile()
{
	if (fileStatus == FILE_OPENED) {
		fclose(file);
		fileStatus = FILE_CLOSED;
	}
}


void readDataFromFile() {
	int lineCounter = 0;
	int TotalSales = 0;
	while (1) {    // this is being used to make the loop infinite times. it would be later closed manually
		unsigned short CarAmountPerSaleValue = 0, TypeOfCarPerSaleValue = 0, DiscountGivenPerSaleValue = 0;
		char CustomerNameValue[201] = "";
		int scanResult = fscanf (file,"%hd,%hd,%hd,%[^\n]s",
			&CarAmountPerSaleValue,
			&TypeOfCarPerSaleValue,
			&DiscountGivenPerSaleValue,
			&CustomerNameValue 
		);
		TotalSales += CarAmountPerSaleValue;
		if (scanResult == EOF)
		{
			break;
		}

		CarAmountPerSale[lineCounter] = CarAmountPerSaleValue;
		TypeOfCarPerSale[lineCounter] = TypeOfCarPerSaleValue;
		DiscountGivenPerSale[lineCounter] = (bool)DiscountGivenPerSaleValue;
		strcpy(customerNames[lineCounter], CustomerNameValue);
		lineCounter++;
	}
	numberOfSales = lineCounter;
	CarsAvailable = MAXIMUM_SALES - TotalSales;
}

void getDataFromFile() {
	openFile(CSV_FILE, "r");  

	if (fileStatus == FILE_OPENED) {
		readDataFromFile();
	}
	else if (fileStatus == FILE_ERROR) {
		printf(" \n\nSorry ! An error occured while trying to read from the file %s.", CSV_FILE);
		pauseProgram('_');
	}
	closeFile();
}

void writeDataToFile() {
	for (int j = 0; j < numberOfSales; j++) {

		char line[201];
		char data[10];

		//Converting the values into String and adding commas to separate the values from each other
		_itoa((int)CarAmountPerSale[j], data, 10);
		strcpy(line, data);
		strcat(line, ",");

		_itoa((int)TypeOfCarPerSale[j], data, 10);
		strcat(line, data);
		strcat(line, ",");

		_itoa((int)DiscountGivenPerSale[j], data, 10);
		strcat(line, data);
		strcat(line, ",");

		strcat(line, customerNames[j]);
		fprintf(file, line);

		if (j < numberOfSales - 1) {
			fprintf(file, "\n");            //adding the newline character at the very last line
		}
	}
}

void saveDataToFile() {
	openFile(CSV_FILE, "w");

	if (fileStatus == FILE_OPENED) {
		writeDataToFile();
	}
	else if (fileStatus == FILE_ERROR) {
		printf("\n\n Sorry ! An error occured while trying to write to the file %s.", CSV_FILE);
		pauseProgram('_');
	}

	closeFile();
}

void menu_greetCustomer() { 
	printf("\n\n                                HELLO          \n");
	printf("              !  Welcome to the EVEREST AUTOMOBILES TRADES  !\n\n");
}

void menu_showMenu()
{

	printf(" \n                             || Menu ||\n\n");
	printf(" %c.     View Car Models          \n ", MENU_OPTION_VIEW_CARS);
	printf("%c.     Buy Cars                 \n", MENU_OPTION_BUY_CARS);
	printf(" %c.     View Sales Status        \n", MENU_OPTION_VIEW_SALES);
	printf(" %c.     View Specific Sales Data \n", MENU_OPTION_VIEW_SALES_BETWEEN);
	printf(" %c.     Exit                     \n\n", MENU_OPTION_EXIT);
}

void menu_showCarTypes() {
	int numberOfCars = sizeof(carPrices) / sizeof(float);
	printf("\n  || Cars Types || \n");
	for (int i = 0 ; i < numberOfCars; i++)
	{
		printf("\n\n   %d -         %s\n", i+1, CarTypes[i]);
	}
}

float menu_applyDiscount(float currentPrice)
{
	return currentPrice * (1 - DISCOUNT_PERCENTAGE);
}

bool menu_checkIfDiscountIsNeeded(unsigned short userAge)
{
	if (userAge >= DISCOUNT_MIN_AGE && userAge <= DISCOUNT_MAX_AGE)
	{
		char StartupEnterpreneurDiscount = getCharFromConsole("\n GREAT ! We have a discount condition for your age group."
			"\n\n     Are you an entrepreneur ?\n  "
			"           Answer 'y' or 'n' : ");
		if (StartupEnterpreneurDiscount == 'y')
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		printf("\n     Sorry ! You are not Eligible for discount  ");
	}
	return FALSE;
}

void menu_printDiscountOutcome(bool giveDiscount)
{
	switch (giveDiscount)
	{
	case TRUE:
		printf(" \n\n\n <<<  Congratulations ! You get a discount of 23 Percent !! >>> ");
		break;

	case FALSE:
		break;
	}
}

void menu_ViewCars()
{
	if (CarsAvailable > 0) {
		printf("\n \n   [ Currently, we have the following cars in our showroom ] \n\n\n");
		printf("           MODELS:                         PRICE: (IN GBP)   \n");
		printf("     1.  TESLA MODEL S                        82,900          \n");
		printf("     2.  Mercedes Z class                     35,800          \n");
		printf("     3.  NISSAN 3rd Gen Versa                 21,500          \n");
		printf("     4.  FERRARI Mondial 8                    27,050          \n");
	}

}

bool checkIfCustomerNameIsValid() {
	char costumerName[201];
	strcpy(costumerName, customerNames[numberOfSales]);

	for (unsigned short i = 0; i < costumerName[i] != '\0'; i++) {
		if (!(costumerName[i] >= 'A' && costumerName[i] <= 'Z') && !(costumerName[i] >= 'a' && costumerName[i] <= 'z')) {
			return FALSE;
		}
	}

	return TRUE;
}

void menu_buyCars()
{
	printf("     \n                <<<<  You are on BUYING SECTION now!  >>>>     \n\n");
	if (CarsAvailable > 0) {		
		printf("  \n\n\n         There are %hd cars available in total     \n\n", CarsAvailable);
	}
	else {
		printf("   \nSorry, there are no more Cars available.");
		return;
	}

	unsigned short carsNeeded = 0, userAge = 0, carType;
	bool giveDiscount = FALSE;
	float totalPrice = 0;
	
	//to enter user information and validate if user has enetred the correct information.
	do {
		getStringFromConsole("           Lets Start.  \n\n For that we need your information. What is your name? Name: ", &customerNames[numberOfSales]);
		if (checkIfCustomerNameIsValid() == FALSE) {
			printf(" \n\n\n\n !! Invalid Input. Please write your name coorectly!!! \n\n\n");
			continue;
		}
	
		break;
	} while (1);

	carsNeeded = getUnsignedShortFromConsole("\n How many cars do you need? Amount: ");

	if (CarsAvailable < carsNeeded) {
		printf("\n\n\n\n        Sorry, there are fewer Cars remaining than you require.");
		return;
	}

	CarAmountPerSale[numberOfSales] = carsNeeded;
	menu_showCarTypes();
	carType = getUnsignedShortFromConsole("\n\n\n     Which Car are you intrested in buying? Please Choose from above:  ");

	TypeOfCarPerSale[numberOfSales] = carType;
	totalPrice = carsNeeded * carPrices[carType];
	CarsAvailable -= carsNeeded;

	userAge = getUnsignedShortFromConsole("\n\n\n            How old are you? Age: ");
	giveDiscount = menu_checkIfDiscountIsNeeded(userAge);
	



	if (giveDiscount == TRUE) {
		totalPrice = menu_applyDiscount(totalPrice);
	}

	DiscountGivenPerSale[numberOfSales] = giveDiscount;
	time_t CurrentTime;
	time(&CurrentTime);
	menu_printDiscountOutcome(giveDiscount);

	printf("\n\n     Thank you for Choosing US.\n\n");
	printf("       You have bought %hd cars .\n", carsNeeded);
	printf("       The Total Cost is %0.2f GBP.\n", totalPrice);
	printf("       You have bought this Car at : %s \n\n ", ctime(&CurrentTime));
	printf("\n     There are %hd Cars remaining.", CarsAvailable);

	numberOfSales++;
}

void sortArraysByNumberOfCarsSoldPerSale()
{

	for (int i = 0; i < numberOfSales - 1; i++)
	{
		for (int j = i + 1; j < numberOfSales; j++)
		{
			if (CarAmountPerSale[i] < CarAmountPerSale[j])
			{
				swapUnsignedShort(&CarAmountPerSale[i], &CarAmountPerSale[j]);

				swapUnsignedShort(&TypeOfCarPerSale[i], &TypeOfCarPerSale[j]);

				swapBool(&DiscountGivenPerSale[i], &DiscountGivenPerSale[j]);

				char temp[201];
				strcpy(temp, customerNames[i]);
				strcpy(customerNames[i], customerNames[j]);
				strcpy(customerNames[j], temp);

			}

		}

	}

}

void printSalesDataAtPosition(int position) {

	int typeOfCar = TypeOfCarPerSale[position];
	float price = CarAmountPerSale[position] * carPrices[typeOfCar];

	char discountGivenText[4];
	if (DiscountGivenPerSale[position] == TRUE) {
		strcpy(discountGivenText, "Yes");
		price *= (1 - DISCOUNT_PERCENTAGE);
	}
	else {
		strcpy(discountGivenText, "No");
	}

	
	printf("\n  Customer Name: %s  \n  Type of Car: %s   \n  Sale Index: %d    \n  Total Price: %0.2f   \n  "
	   	"Car Price: %0.2f     \n  Number of Cars: %hd   \n  Discount Given: %s   \n        \n\n ",
		customerNames[position], CarTypes[typeOfCar], position, price, 
		carPrices[typeOfCar], CarAmountPerSale[position], discountGivenText );
}

void menu_viewSales()
{
	sortArraysByNumberOfCarsSoldPerSale();

	float totalSalesValue = 0;
	unsigned int carsSold = 0;

	printf(" \n\n ||    All Sales Data:   ||       \n\n");

	for (int i = 0; i < numberOfSales; i++)
	{
		int typeOfCar = TypeOfCarPerSale[i];
		float price = CarAmountPerSale[i] * carPrices[typeOfCar];
		printSalesDataAtPosition(i);
		totalSalesValue += price;
		carsSold += CarAmountPerSale[i];

	}
	printf("\n\n  %hd Cars have been sold with a total value of %0.2f GBP. \n   There are %hd Cars unsold. \n",
		carsSold, totalSalesValue, CarsAvailable);
}

void printSalesDataBasedOnCarsSoldBetweenMinMax(unsigned short min, unsigned short max)
{
	printf(" \n   Sales Data containing between %hd and %hd Cars sold:   \n\n", min, max);
	for (int i = 0; i < numberOfSales; i++)
	{
		if (min <= CarAmountPerSale[i] && CarAmountPerSale[i] <= max)
		{
			printSalesDataAtPosition(i);
		}

	}

}

void menu_viewSalesBetween()
{
	sortArraysByNumberOfCarsSoldPerSale();
	unsigned short minCarsSold = getUnsignedShortFromConsole("\n\n     What's the minimum number of Cars sold you want to look at? Number = \n");
	unsigned short maxCarsSold = getUnsignedShortFromConsole("\n\n     What's the maximum number of Cars sold you want to look at? Number = \n");
	printSalesDataBasedOnCarsSoldBetweenMinMax(minCarsSold, maxCarsSold);
}

void menu_exit()
{
	printf("Hope you got your Desired Car. Thank you for using EVEREST AUTOMOBILE TRADES program !");
	saveDataToFile();
}

void main()
{
	getDataFromFile();
	char userChoice;
	do
	{
		clearScreen();
		menu_greetCustomer();
		menu_showMenu();
		userChoice = getCharFromConsole(" \n Please choose the Menu Options : ");
		clearScreen();

		switch (userChoice) {
		case MENU_OPTION_VIEW_CARS:
			menu_ViewCars();
			break;

		case MENU_OPTION_BUY_CARS:
			menu_buyCars();
			break;

		case MENU_OPTION_VIEW_SALES:
			menu_viewSales();
			break;

		case MENU_OPTION_VIEW_SALES_BETWEEN:
			menu_viewSalesBetween();
			break;

		case MENU_OPTION_EXIT:
			menu_exit();
			break;
		}

		pauseProgram(userChoice);

	} while (userChoice != MENU_OPTION_EXIT);

	clearScreen();
	printf("\n\n\n  <<  Have a good day!  >>  \n\n\n");
}