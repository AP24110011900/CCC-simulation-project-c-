#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TIME 60   // Simulation time in minutes
#define ARRIVAL_PROB 30 // % probability of a customer arriving per minute
#define MIN_SERVICE 2
#define MAX_SERVICE 6

// --------------------- LINKED LIST NODE ---------------------

typedef struct Customer {
    int id;
    int arrivalTime;
    int serviceTime;
    int startService;
    struct Customer *next;
} Customer;

Customer *front = NULL, *rear = NULL;
int customerID = 1;

// --------------------- QUEUE FUNCTIONS ----------------------

void enqueue(int arrival, int service) {
    Customer *c = malloc(sizeof(Customer));
    c->id = customerID++;
    c->arrivalTime = arrival;
    c->serviceTime = service;
    c->startService = -1;
    c->next = NULL;

    if (rear == NULL) {
        front = rear = c;
    } else {
        rear->next = c;
        rear = c;
    }
}

Customer *dequeue() {
    if (front == NULL)
        return NULL;

    Customer *c = front;
    front = front->next;
    if (front == NULL)
        rear = NULL;

    return c;
}

int queueLength() {
    int count = 0;
    Customer *temp = front;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

// ------------------------------------------------------------

int main() {
    srand(time(NULL));

    int currentTime = 0;
    int serverBusyUntil = 0;

    int totalServed = 0;
    int totalWaitTime = 0;

    printf("\n----- BANK QUEUE SIMULATION -----\n\n");

    for (currentTime = 0; currentTime < MAX_TIME; currentTime++) {
        // 1. Random arrivals
        int r = rand() % 100;
        if (r < ARRIVAL_PROB) {
            int serviceTime = MIN_SERVICE + rand() % (MAX_SERVICE - MIN_SERVICE + 1);
            enqueue(currentTime, serviceTime);

            printf("Time %2d: Customer %d arrived (service time %d)\n",
                    currentTime, customerID - 1, serviceTime);
        }

        // 2. Serve the next customer if server is free
        if (currentTime >= serverBusyUntil && front != NULL) {
            Customer *c = dequeue();
            c->startService = currentTime;

            int wait = c->startService - c->arrivalTime;
            totalWaitTime += wait;
            totalServed++;

            serverBusyUntil = currentTime + c->serviceTime;

            printf("Time %2d: Serving customer %d (waited %d minutes)\n",
                    currentTime, c->id, wait);

            free(c);
        }
    }

    // ---------------------------------------------------------
    // END OF SIMULATION
    // ---------------------------------------------------------

    printf("\n----- SIMULATION SUMMARY -----\n");
    printf("Total customers served: %d\n", totalServed);
    printf("Customers still in queue: %d\n", queueLength());
    
    if (totalServed > 0)
        printf("Average wait time: %.2f minutes\n",
               (float)totalWaitTime / totalServed);
    else
        printf("Average wait time: N/A\n");

    printf("--------------------------------\n\n");

    return 0;
}