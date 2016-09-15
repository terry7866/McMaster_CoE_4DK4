
/*
 *
 * Simulation of Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#include <stdio.h>
#include "simlib.h"
#include <stdlib.h>

/*******************************************************************************/

/*
 * Simulation Parameters
 */

#define RANDOM_SEED 744927	
#define NUMBER_TO_SERVE 50e6

#define SERVICE_TIME 10
#define ARRIVAL_RATE 0.09

#define BLIP_RATE 10000

/*******************************************************************************/

/*
 * main() uses various simulation parameters and creates a clock variable to
 * simulate real time. A loop repeatedly determines if the next event to occur
 * is a customer arrival or customer departure. In either case the state of the
 * system is updated and statistics are collected before the next
 * iteration. When it finally reaches NUMBER_TO_SERVE customers, the program
 * outputs some statistics such as mean delay.
 */

int main()
{
  double clock = 0; /* Clock keeps track of simulation time. */

  /* System state variables. */
  int number_in_system = 0;
  double next_arrival_time = 0;
  double next_departure_time = 0;

  /* Data collection variables. */
  long int total_served = 0;
  long int total_arrived = 0;

  double total_busy_time = 0;
  double integral_of_n = 0;
  double last_event_time = 0;
  double arrival_rate_val = 0.01;
  int random_seed_val = 0;
  double avg_mean_delay = 0;

  int a = 0;

  for (arrival_rate_val = 0.09; arrival_rate_val < 0.1; arrival_rate_val = arrival_rate_val + 0.001) {

	  for (a = 0; a < 10; a = a + 1) {

		  //printf("Current Arrival Rate: %f\n", arrival_rate_val);
		  printf("value of a: %d\r", a);

		  random_seed_val = rand() % 100000;
		  //printf("value of Random Seed: %d\n", random_seed_val);

		  /* Set the seed of the random number generator. */
		  random_generator_initialize(random_seed_val);

		  /* Process customers until we are finished. */
		  while (total_served < NUMBER_TO_SERVE) {

			  /* Test if the next event is a customer arrival or departure. */
			  if (number_in_system == 0 || next_arrival_time < next_departure_time) {

				  /*
				  * A new arrival is occurring.
				  */

				  clock = next_arrival_time;
				  next_arrival_time = clock + exponential_generator((double)1 / arrival_rate_val);

				  /* Update our statistics. */
				  integral_of_n += number_in_system * (clock - last_event_time);
				  last_event_time = clock;

				  number_in_system++;
				  total_arrived++;

				  /* If this customer has arrived to an empty system, start its
				  service right away. */
				  if (number_in_system == 1) next_departure_time = clock + exponential_generator((double)SERVICE_TIME);

			  }
			  else {

				  /*
				  * A customer departure is occuring.
				  */

				  clock = next_departure_time;

				  /* Update our statistics. */
				  integral_of_n += number_in_system * (clock - last_event_time);
				  last_event_time = clock;

				  number_in_system--;
				  total_served++;
				  total_busy_time += exponential_generator((double)SERVICE_TIME);

				  /*
				  * If there are other customers waiting, start one in service
				  * right away.
				  */

				  if (number_in_system > 0) next_departure_time = clock + exponential_generator((double)SERVICE_TIME);

				  /*
				  * Every so often, print an activity message to show we are active.
				  */

				  if (total_served % BLIP_RATE == 0)
					  printf("Customers served = %ld (Total arrived = %ld)\r",
						  total_served, total_arrived);
			  }

		  }

		  /* Output final results. */
		  //printf("\nUtilization = %f\t", total_busy_time / clock);
		  //printf("Fraction served = %f\t", (double)total_served / total_arrived);
		  //printf("Mean number in system = %f\t", integral_of_n / clock);
		  //printf("Mean delay = %f\n", integral_of_n / total_served);

		  avg_mean_delay += (integral_of_n / total_served);

		  number_in_system = 0;
		  next_arrival_time = 0;
		  next_departure_time = 0;

		  clock = 0;
		  total_served = 0;
		  total_arrived = 0;

		  total_busy_time = 0;
		  integral_of_n = 0;
		  last_event_time = 0;

	  }

	  printf("\n %f \t %f \n", arrival_rate_val, avg_mean_delay / a);
	  avg_mean_delay = 0;

  }
  
  /* Halt the program before exiting. */
  printf("Hit Enter to finish ... \n");
  getchar();

  return 0;

}






