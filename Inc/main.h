/*
 * main.h
 *
 *  Created on: 2 de oct. de 2023
 *      Author: Alfonso Fernández Simón
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

// Default Values
#define DEFAULT_PRES 		5
#define DEFAULT_TEMP 		22

// Range Values
#define MIN_TEMP 			18
#define MAX_TEMP 			30
#define MIN_TEMP_VARIATION 	4
#define MED_TEMP_VARIATION 	8
#define MIN_PRES 			0.5
#define MAX_PRES 			25
#define MIN_INC 			1
#define MAX_INC 			5
#define MIN_DEC 			0.25
#define MAX_DEC 			0.75
#define MIN_COND 			0
#define MAX_COND 			100
#define MIN_WAT				0
#define MAX_WAT 			100
#define MIN_EV 				10
#define MAX_EV 				100
#define MIN_INC_EV 			5
#define MAX_INC_EV 			9
#define MIN_CYCLES 			4
#define MAX_CYCLES 			18
#define MAX_EV_DIST			90
#define MAX_INC_DIST 		20
#define MAX_DEC_DIST 		4.5

typedef enum {
    OFF,
    MIN,
    MED,
    MAX,
} state_t;

// System State Structure
typedef struct {
    float desired_temp;
    float actual_temp;
    float pressure_high;
    float pressure_low;
    state_t compressor_state;
    uint32_t fan_condensing;
    uint32_t water_pump;
    state_t resistance_state;
    uint32_t fan_evaporation;
    float user_desired_temperature;
} system_t;

void init_system(system_t *system);
void control_system(system_t *system, float user_desired_temperature);
float compare_values(float a, float b, float c);
const char *stateToString(state_t state);
void saveSystemToCsv(const system_t *system, const char *filename);

#endif /* MAIN_H_ */
