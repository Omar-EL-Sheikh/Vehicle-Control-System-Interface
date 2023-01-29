/*
 ============================================================================
 Name        : Mini_Project_01.c
 Author      : Omar M. EL-Sheikh
 Description : Vehicle Control System
 Date        : 02/08/2022
 ============================================================================
 */

/* Pre-processing */
#include <stdio.h>
#include <stdlib.h>

/* Macros */
#define RED_SPEED 0
#define ORANGE_SPEED 30
#define GREEN_SPEED 100
#define WITH_ENGINE_TEMP_CONTROLLER 1

/* Enumeration Constants */
enum state {
	QUIT_PROGRAM, ENGINE_ON, ENGINE_OFF, INVALID_STATE,
	ON = 1, OFF = 0
}; // Enumeration constants to indicate the current state of the system


/* Customized Data Types Definition */
typedef enum {
	False = 0,
	True = 1
}bool; // Creating Boolean data type

typedef struct air_conditioner {
	bool state;
	double temp;
}AC; // Data type to define vehicle AC

typedef struct engine_temp_controller {
	bool state;
	double temp;
}ETC; // Data type to define vehicle engine temperature controller

/* Global Variables */
enum state systemState; // Variable to store current system state based on user's input
double roomTemp = 25; // Variable to store vehicle room temperature
double engineTemp = 115; // Variable to store engine temperature
int vehicleSpeed = 0; // Variable to store vehicle speed
AC vehicleAC = {OFF, 0}; // Variable to store AC information
ETC vehicleETC = {OFF, 0}; // Variable to store ETC information

/* Functions Prototypes */
void PromptState(char * const);// Function to take state input
enum state StateModify(const char * const); // Function to modify current state based on input
void StatePrint(const enum state * const); //Function to print current system state
void SystemStateUpdate(); // Function to take the state of the system from user, modifies it based on input, and prints current state
void PromptSetMenuState(char * const);// Function to apply current sensor mode based on user input
void SetSensorsMode(const char * const); // Function to set current mode and apply it
void SensorsSetMenuUpdate(); // Function to update current sensor state
void SetTrafficLight();// Function to set vehicle speed based on traffic light mode
void SetRoomTemp(); // Function to change AC temperature based on current room temperature
void SetEngineTemp(); // Function to change engine temperature controller based on current engine temperature
void UpdateTemp(const int * const); // Function to update room and engine temperature if the speed is 30
void CurrentVehicleStates(const enum state * const, const double * const, const double * const, const int * const, const AC * const,
		const ETC * const); // Function to display current vehicle states

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	while(1){
		SystemStateUpdate(); // Prompting Initial State

		/*Program Flow Control*/
		while(systemState != QUIT_PROGRAM){
			if (systemState == INVALID_STATE || systemState == ENGINE_OFF){
				SystemStateUpdate();// Updating state in case of invalid input
			}
			else {
				SensorsSetMenuUpdate(); // Selecting and applying current sensor mode if the engine is on
			}
		}

		if (systemState == QUIT_PROGRAM){ // Double checking that the state is to quit the program
			break; // Terminates the program
		}
	}
	return EXIT_SUCCESS;
}

/* Functions Definitions */
void PromptState(char*const inputPtr){
	printf("a. Turn on the vehicle engine\n");
	printf("b. Turn off the vehicle engine\n");
	printf("c. Quit the system\n\n");
	scanf(" %c", inputPtr); // Taking input state from user
	return;
};

enum state StateModify(const char * const input){
	switch(*input){
	case 'a':
		return ENGINE_ON;
		break;

	case 'b':
		return ENGINE_OFF;
		break;

	case 'c':
		return QUIT_PROGRAM;
		break;

	default:
		return INVALID_STATE; // Returns invalid state in case of user error
		break;
	}
};

void StatePrint(const enum state * const ptrState){
	switch(*ptrState){
	case QUIT_PROGRAM:
		puts("Quit the system !");
		break;

	case ENGINE_OFF:
		puts("Turn off the vehicle Engine !\n");
		break;

	case ENGINE_ON:
		puts("Turn on the vehicle Engine !\n");
		break;

	case INVALID_STATE:
		puts("Invalid state !\n\nPlease renter state !\n");
		break;
	}

};

void SystemStateUpdate(){
	char inputState; // Variable to store the current input state from user

	PromptState(&inputState); // State input from user
	systemState = StateModify(&inputState); // Modifying the state
	StatePrint(&systemState); // State display
};

void PromptSetMenuState(char * const inputPtr){
	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color\n");
	printf("c. Set the room temperature\n");
#if	(1 == WITH_ENGINE_TEMP_CONTROLLER)
	printf("d. Set the engine temperature\n\n");
#endif
	scanf(" %c", inputPtr);
};

void SetSensorsMode(const char * const input){
	void (*ptr_func[3])() = {SetTrafficLight, SetRoomTemp, SetEngineTemp};
	switch(*input){
	case 'a':
		systemState = ENGINE_OFF; // Setting engine off
		StatePrint(&systemState); // State print
		break;

	case 'b':
		(*ptr_func[0])();  // Traffic Light Mode
		break;

	case 'c':
		(*ptr_func[1])(); // Room Temperature Set Mode
		break;

#if(1 == WITH_ENGINE_TEMP_CONTROLLER)
	case 'd':
		(*ptr_func[2])(); // Engine Temperature Set Mode
		break;
#endif

	default:
		printf("Invalid mode !\n\nPlease renter !\n\n");// Handling error from user
		break ;
	}
};

void SensorsSetMenuUpdate(){
	char inputState;

	PromptSetMenuState(&inputState); // Scanning current sensor mode from user
	SetSensorsMode(&inputState); // Applying current mode to change vehicle features
	UpdateTemp(&vehicleSpeed); // Updating room and engine temperature if the speed is 30
	if (systemState != ENGINE_OFF)
		CurrentVehicleStates(&systemState, &roomTemp, &engineTemp, &vehicleSpeed, &vehicleAC, &vehicleETC);
};

void SetTrafficLight(){

	char trafficLight;

	puts("Enter traffic light data (R, O, G): ");
	scanf(" %c", &trafficLight); // Scanning traffic light data

	switch(trafficLight){ // Setting speed based on traffic light data
	case 'G':
		vehicleSpeed = GREEN_SPEED;
		break;

	case 'O':
		vehicleSpeed = ORANGE_SPEED;
		break;

	case 'R':
		vehicleSpeed = RED_SPEED;
		break;

	default:
		printf("Invalid traffic light input ! please renter !\n\n"); // Prompting error message in case of invalid input
		SetTrafficLight(); //  Calling the function again until the user enters a valid input
	}
};

void SetRoomTemp(){
	puts("Enter room temperature data: ");
	scanf("%lf", &roomTemp); // Scanning room temperature data

	if (roomTemp < 10 || roomTemp > 30){
		vehicleAC.state = ON;
		vehicleAC.temp = 20;
		roomTemp = vehicleAC.temp;
	}
	else {
		vehicleAC.state = OFF;
	}
};

void SetEngineTemp(){
	puts("Enter engine temperature data: ");
	scanf("%lf", &engineTemp); // Scanning engine temperature data

	if (engineTemp < 100 || engineTemp > 150){
		vehicleETC.state = ON;
		vehicleETC.temp = 125;
		engineTemp = vehicleETC.temp;
	}
	else {
		vehicleETC.state = OFF;
	}
};

void UpdateTemp(const int * const speedPtr){
	if (*speedPtr == 30 && systemState == ENGINE_ON){
		vehicleAC.state = ON;
		vehicleAC.temp = roomTemp*((double)5/4) + 1;
		roomTemp = vehicleAC.temp;

#if	(1 == WITH_ENGINE_TEMP_CONTROLLER)
		vehicleETC.state = ON;
		vehicleETC.temp = engineTemp*((double)5/4) + 1;
		engineTemp = vehicleETC.temp;
#endif
	}
};

void CurrentVehicleStates(const enum state * const systemStatePtr, const double * const roomTempPtr, const double * const engineTempPtr,
		const int * const vehicleSpeedPtr, const AC * const vehicleACptr, const ETC * const vehicleETCptr){
	if (*systemStatePtr == ENGINE_ON){
		printf("Engine State: ON\n");
	}
	else if (*systemStatePtr == ENGINE_OFF){
		printf("Engine State: OFF\n");
	}

	if(vehicleACptr->state == True){
		puts("AC State: ON");
	}
	else{
		puts("AC State: OFF");
	}

	printf("Vehicle Speed: %d km/hrs\n", *vehicleSpeedPtr);

	printf("Room Temperature: %.2lf C\n", *roomTempPtr);

#if	(1 == WITH_ENGINE_TEMP_CONTROLLER)
	if(vehicleETCptr->state == True){
		puts("Engine Temperature Controller State: ON");
	}
	else{
		puts("Engine Temperature Controller State: OFF");
	}

	printf("Engine Temperature: %.2lf C\n", *engineTempPtr);
#endif
	puts("");
};

/* Commit github */
