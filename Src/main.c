/*
 * main.c
 *
 *  Created on: 2 de oct. de 2023
 *      Author: Alfonso Fernández Simón
 */

#include "../Inc/main.h"

// Initialize system with default values
void init_system(system_t *system)
{
    system->desired_temp = DEFAULT_TEMP;
    system->actual_temp = DEFAULT_TEMP;
    system->pressure_high = DEFAULT_PRES;
    system->pressure_low = DEFAULT_PRES;
    system->compressor_state = OFF;
    system->fan_condensing = MIN_COND;
    system->water_pump = MIN_WAT;
    system->resistance_state = OFF;
    system->fan_evaporation = MIN_EV;
    system->user_desired_temperature = DEFAULT_TEMP;
}

#ifdef USE_PID
	// Use a PID Controller for choosing the desired temperature
	float update_pid(pid_controller_t *pid, float setpoint, float process_variable){
		// Current error
	    pid->error = setpoint - process_variable;
	    // Integral term
	    pid->integral += pid->error * DT;
	    // Derivative term
	    pid->derivative = (pid->error - pid->previous_error) / DT;
	    // Output of PID
	    float output = KP * pid->error + KI * pid->integral + KD * pid->derivative;
	    // Update previous error
	    pid->previous_error = pid->error;

	    return output;
	}
#endif

// Control the system
void control_system(system_t *system, float user_desired_temperature)
{
	#ifdef USE_PID
		pid_controller_t pid;
		init_pid_controller(&pid);
		system->user_desired_temperature = user_desired_temperature;
		system->fan_evaporation = MAX_EV;
		system->pressure_high = DEFAULT_PRES;
		system->pressure_low = DEFAULT_PRES;
		system->desired_temp = user_desired_temperature;
		float max_temp_variation = 0.0;
		float pressure_increment = 0.0;
		float pressure_decrement = 0.0;
		float fan_evaporation_decrement = 0.0;
		float max_cycles = 0.0;
	#else
		system->user_desired_temperature = user_desired_temperature;
		system->fan_evaporation = MAX_EV;
		system->pressure_high = DEFAULT_PRES;
		system->pressure_low = DEFAULT_PRES;
		system->desired_temp = user_desired_temperature;
		float max_temp_variation = 0.0;
		float pressure_increment = 0.0;
		float pressure_decrement = 0.0;
		float fan_evaporation_decrement = 0.0;
		float max_cycles = 0.0;
	#endif

    while (system->fan_evaporation > MIN_EV)
    {
    	sleep(1);
		#ifdef USE_PID
			fan_evaporation_decrement = (float)(MIN_INC_EV) + ((float)rand() / RAND_MAX) * (MAX_INC_EV - MIN_INC_EV);
			pressure_increment = (float)(MIN_INC) + ((float)rand() / RAND_MAX) * (MAX_INC - MIN_INC);
			pressure_decrement = (float)(MIN_DEC) + ((float)rand() / RAND_MAX) * (MAX_DEC - MIN_DEC);
			float pid_output = update_pid(&pid, user_desired_temperature, system->actual_temp);
			system->fan_evaporation -= fan_evaporation_decrement + pid_output;
		#else
			// Simulate updating the evaporator fan
			fan_evaporation_decrement = (float)(MIN_INC_EV) + ((float)rand() / RAND_MAX) * (MAX_INC_EV - MIN_INC_EV);
			pressure_increment = (float)(MIN_INC) + ((float)rand() / RAND_MAX) * (MAX_INC - MIN_INC);
			pressure_decrement = (float)(MIN_DEC) + ((float)rand() / RAND_MAX) * (MAX_DEC - MIN_DEC);
			system->fan_evaporation -= fan_evaporation_decrement;
		#endif
		// Check if cooling is needed
		if (system->actual_temp > system->desired_temp)
		{
			// Simulate updating the pressures
			system->pressure_high += pressure_increment;
			system->pressure_low -= pressure_decrement;
	    	max_cycles = compare_values((float)MAX_EV_DIST/fan_evaporation_decrement, (float)MAX_INC_DIST/pressure_increment, (float)MAX_DEC_DIST/pressure_decrement);
			max_temp_variation = (system->actual_temp - system->desired_temp);
			if (max_temp_variation <= MIN_TEMP_VARIATION)
			{
				// Activate the compressor
				system->compressor_state = MIN;
				system->fan_condensing = MIN_COND;
				// Turn off the water pump and electric resistance
				system->water_pump = MIN_WAT;
				system->resistance_state = OFF;
				// Simulate updating the temperatures
				system->actual_temp -= (max_temp_variation/max_cycles);
			}
			else if (max_temp_variation <= MED_TEMP_VARIATION)
			{
				// Activate the compressor
				system->compressor_state = MED;
				system->fan_condensing = MIN_COND;
				// Turn off the water pump and electric resistance
				system->water_pump = MIN_WAT;
				system->resistance_state = OFF;
				// Simulate updating the temperatures
				system->actual_temp -= (max_temp_variation/max_cycles);
			}
			else
			{
				// Activate the compressor and adjust the condensing fan
				system->compressor_state = MAX;
				system->fan_condensing = MAX_COND;
				// Turn off the water pump and electric resistance
				system->water_pump = MIN_WAT;
				system->resistance_state = OFF;
				// Simulate updating the temperatures
				system->actual_temp -= (max_temp_variation/max_cycles);
			}
		// Check if heating is needed
		}
		else if (system->actual_temp < system->desired_temp)
		{
	    	max_cycles = compare_values((float)MAX_EV_DIST/fan_evaporation_decrement, (float)MAX_INC_DIST/pressure_increment, (float)MAX_DEC_DIST/pressure_decrement);
			max_temp_variation = (system->desired_temp - system->actual_temp);
			if (max_temp_variation <= MIN_TEMP_VARIATION)
			{
				// Activate the compressor
				system->resistance_state = MIN;
				system->water_pump = MIN_WAT;
				// Turn off the compressor and condensing fan
				system->compressor_state = OFF;
				system->fan_condensing = MIN_COND;
				// Simulate updating the temperatures
				system->actual_temp += (max_temp_variation/max_cycles);
			}
			else if (max_temp_variation <= MED_TEMP_VARIATION)
			{
				// Activate the compressor
				system->resistance_state = MED;
				system->water_pump = MIN_WAT;
				// Turn off the compressor and condensing fan
				system->compressor_state = OFF;
				system->fan_condensing = MIN_COND;
				// Simulate updating the temperatures
				system->actual_temp += (max_temp_variation/max_cycles);
			}
			else
			{
				// Activate the compressor and adjust the condensing fan
				system->resistance_state = MAX;
				system->water_pump = MAX_WAT;
				// Turn off the compressor and condensing fan
				system->compressor_state = OFF;
				system->fan_condensing = MIN_COND;
				// Simulate updating the temperatures
				system->actual_temp += (max_temp_variation/max_cycles);
			}
		}
		else
		{
			// Desired temperature has been reached
			system->compressor_state = OFF;
			system->fan_condensing = MIN_COND;
			system->water_pump = MIN_WAT;
			system->resistance_state = OFF;
			system->actual_temp = user_desired_temperature;
		}
    	if (system->pressure_high >= MAX_PRES)
    	{
    		printf("Max Pressure Reached\n");
    		system->pressure_high = MAX_PRES;
    		system->pressure_low = MIN_PRES;
    		break;
    	}
    	else if (system->pressure_high <= MIN_PRES)
    	{
    		printf("Min Pressure Reached\n");
    		system->pressure_high = MAX_PRES;
    		system->pressure_low = MIN_PRES;
    		break;
    	}
        // Print the system's state
    	printf("\n");
    	printf("Desired temperature: %.2f °C\n", user_desired_temperature);
    	printf("Actual temperature: %.2f ºC\n", system->actual_temp);
    	printf("High pressure: %.2f bars\n", system->pressure_high);
    	printf("Low pressure: %.2f bars\n", system->pressure_low);
    	printf("Compressor state: %s\n", stateToString(system->compressor_state));
    	printf("Condenser fan: %d\n", system->fan_condensing);
    	printf("Water pump speed: %d\n", system->water_pump);
    	printf("Electrical resistance state: %s\n", stateToString(system->resistance_state));
    	printf("Evaporator fan speed: %d\n", system->fan_evaporation);
    	printf("\n");
    }
	// Desired temperature has been reached
	system->compressor_state = OFF;
	system->fan_condensing = MIN_COND;
	system->water_pump = MIN_WAT;
	system->resistance_state = OFF;
    system->fan_evaporation = MIN_EV;
	system->actual_temp = user_desired_temperature;
    // Print the system's state
	printf("\n");
	printf("Desired temperature: %.2f °C\n", user_desired_temperature);
	printf("Actual temperature: %.2f ºC\n", system->actual_temp);
	printf("High pressure: %.2f bars\n", system->pressure_high);
	printf("Low pressure: %.2f bars\n", system->pressure_low);
	printf("Compressor state: %s\n", stateToString(system->compressor_state));
	printf("Condenser fan: %d\n", system->fan_condensing);
	printf("Water pump speed: %d\n", system->water_pump);
	printf("Electrical resistance state: %s\n", stateToString(system->resistance_state));
	printf("Evaporator fan speed: %d\n", system->fan_evaporation);
	printf("\n");
}

// Find the maximum value of 3 floats
float compare_values(float a, float b, float c)
{
	float max;
    if (a >= b && a >= c)
    {
        max = a;
    }
    else if (b >= a && b >= c)
    {
        max = b;
    }
    else
    {
        max = c;
    }
    return max;
}

// Obtains a readable string from struct state
const char *stateToString(state_t state)
{
    switch (state)
    {
        case OFF:
            return "OFF";
        case MIN:
            return "MIN";
        case MED:
            return "MED";
        case MAX:
            return "MAX";
        default:
            return "OFF"; // Manejar cualquier otro valor de manera segura
    }
}

// Save system variables to a csv file
void saveSystemToCsv(const system_t *system, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    // Column headers in the CSV file
    fprintf(file, "Desired Temperature: %.2f ºC\nCurrent Temperature: %.2f ºC\nHigh Pressure: %.2f bars\nLow Pressure: %.2f bars \nCompressor State: %s\nCondenser Fan Speed: %d\nWater Pump Speed: %d\nResistance State: %s\nEvaporator Fan Speed: %d\n\n",
            system->desired_temp,
            system->actual_temp,
            system->pressure_high,
            system->pressure_low,
			stateToString(system->compressor_state),
            system->fan_condensing,
            system->water_pump,
			stateToString(system->resistance_state),
            system->fan_evaporation
			);

    fclose(file);

    printf("CSV data saved to %s\n", filename);
}

int main()
{
    system_t system;
    // Initialize the system
    init_system(&system);
    // Create exit request status
    bool exit_requested = false;
    srand(time(NULL));

    while (!exit_requested)
    {
        // Simulate the system for a specific temperature
    	#ifdef USE_RANDOM_VALUES
    		srand(time(NULL)); // Seed the random number generator
    		float random_temperature_change = (rand() % 5) - 2; // Simulate temperature fluctuations
    		float user_desired_temperature = system.actual_temp + random_temperature_change;
            // Print the system's state
        	printf("\n");
        	printf("Desired temperature: %.2f °C\n", user_desired_temperature);
        	printf("Actual temperature: %.2f ºC\n", system.actual_temp);
        	printf("High pressure: %.2f bars\n", system.pressure_high);
        	printf("Low pressure: %.2f bars\n", system.pressure_low);
        	printf("Compressor state: %s\n", stateToString(system.compressor_state));
        	printf("Condenser fan: %d\n", system.fan_condensing);
        	printf("Water pump speed: %d\n", system.water_pump);
        	printf("Electrical resistance state: %s\n", stateToString(system.resistance_state));
        	printf("Evaporator fan speed: %d\n", system.fan_evaporation);
        	printf("\n");
        	control_system(&system, user_desired_temperature);
        	// Save the values to a CSV file
        	saveSystemToCsv(&system, "system_data.csv");
    	#else
    		#ifdef USE_SPECIFIC_VALUES
    			float user_desired_temperature = 24;
	            // Print the system's state
	        	printf("\n");
	        	printf("Desired temperature: %.2f °C\n", user_desired_temperature);
	        	printf("Actual temperature: %.2f ºC\n", system.actual_temp);
	        	printf("High pressure: %.2f bars\n", system.pressure_high);
	        	printf("Low pressure: %.2f bars\n", system.pressure_low);
	        	printf("Compressor state: %s\n", stateToString(system.compressor_state));
	        	printf("Condenser fan: %d\n", system.fan_condensing);
	        	printf("Water pump speed: %d\n", system.water_pump);
	        	printf("Electrical resistance state: %s\n", stateToString(system.resistance_state));
	        	printf("Evaporator fan speed: %d\n", system.fan_evaporation);
	        	printf("\n");
	        	control_system(&system, user_desired_temperature);
	        	// Save the values to a CSV file
	        	saveSystemToCsv(&system, "system_data.csv");
    		#else
    			float user_desired_temperature;
    			// Ask the user to enter the desired temperature
    			printf("Enter the desired temperature (between 18 and 30°C) or -0- to exit: ");
    			scanf("%f", &user_desired_temperature);
    			printf("\n");

    	        if (user_desired_temperature == 0)
    	        {
    	            exit_requested = true;
    			// Verify that the entered temperature is within the allowed range.
    	        }
    	        else if (user_desired_temperature < MIN_TEMP || user_desired_temperature > MAX_TEMP)
    	        {
    				printf("Error: The desired temperature should be between 18 and 30°C\n");
    				return 1;
    			}
    	        else
    			{
    	            // Print the system's state
    	        	printf("\n");
    	        	printf("Desired temperature: %.2f °C\n", user_desired_temperature);
    	        	printf("Actual temperature: %.2f ºC\n", system.actual_temp);
    	        	printf("High pressure: %.2f bars\n", system.pressure_high);
    	        	printf("Low pressure: %.2f bars\n", system.pressure_low);
    	        	printf("Compressor state: %s\n", stateToString(system.compressor_state));
    	        	printf("Condenser fan: %d\n", system.fan_condensing);
    	        	printf("Water pump speed: %d\n", system.water_pump);
    	        	printf("Electrical resistance state: %s\n", stateToString(system.resistance_state));
    	        	printf("Evaporator fan speed: %d\n", system.fan_evaporation);
    	        	printf("\n");
    	        	control_system(&system, user_desired_temperature);
    	        	// Save the values to a CSV file
    	        	saveSystemToCsv(&system, "system_data.csv");
    			}
    		#endif
    	#endif
    }
    return 0;
}