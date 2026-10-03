// CS Amusement Park
// cs_amusement_park.c
// Written by <Yi Han>, <z5614040>
// on <15/04/2025>

////////////////////////////////////////////////////////////////////////////////
// Provided Libraries
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cs_amusement_park.h"

////////////////////////////////////////////////////////////////////////////////
// Your libraries
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////
// Function Definitions
////////////////////////////////////////////////////////////////////////////////

// Stage 1.1
// Function to initialise the park
// Params:
//      name - the name of the park
// Returns: a pointer to the park
struct park *initialise_park(char name[MAX_SIZE]) {
  
    struct park *new_park = malloc(sizeof(struct park));
    
    if (new_park == NULL) {
        return NULL;
    }

    strcpy(new_park->name, name);
    new_park->total_visitors = 0;
    new_park->rides = NULL;
    new_park->visitors =NULL;

    return new_park;
}

// Stage 1.1
// Function to create a visitor
// Params:
//      name - the name of the visitor
//      height - the height of the visitor
// Returns: a pointer to the visitor
struct visitor *create_visitor(char name[MAX_SIZE], double height) {
  
    struct visitor *new_visitor = malloc(sizeof(struct visitor));

    if (new_visitor == NULL) {
        return NULL;
    }

    strcpy(new_visitor->name, name);
    new_visitor->height = height;
    new_visitor->next = NULL;

    return new_visitor;
}

// Stage 1.1
// Function to create a ride
// Params:
//      name - the name of the ride
//      type - the type of the ride
// Returns: a pointer to the ride
struct ride *create_ride(char name[MAX_SIZE], enum ride_type type) {
    

    struct ride *new_ride = malloc(sizeof(struct ride));
    if (new_ride == NULL) {
        return NULL;
    }

    strcpy(new_ride->name, name);

    new_ride->type = type;
    
    if (type == ROLLER_COASTER) {
        new_ride->rider_capacity = 4;
        new_ride->queue_capacity = 7;
        new_ride->min_height = 120.0;
    } else if (type == CAROUSEL) {
        new_ride->rider_capacity = 6;
        new_ride->queue_capacity = 9;
        new_ride->min_height = 60.0;
    } else if (type == FERRIS_WHEEL) {
        new_ride->rider_capacity = 8;
        new_ride->queue_capacity = 11;
        new_ride->min_height = 75.0;
    } else if (type == BUMPER_CARS) {
        new_ride->rider_capacity = 10;
        new_ride->queue_capacity = 13;
        new_ride->min_height = 100.0;
    } 

    new_ride->next = NULL;
    new_ride->queue = NULL;


    return new_ride;
}

// Stage 1.2
// Function to run the command loop
//
// Params:
//     park - a pointer to the park
// Returns: None
void command_loop(struct park *park) {    
    char command = ' ';
    while (1) {
        printf("Enter command: ");
        int result = scanf(" %c", &command);

        if (result == EOF || command == 'q') {
            free_park(park);
            break;
        }
        if (command == '?') {
            print_usage();
        } else if (command == 'a') {
            char sub_command;
            scanf(" %c", &sub_command);
            if (sub_command == 'r') {
                add_ride(park);
            } else if (sub_command == 'v') {
                add_visitor(park);
            }
        }
        else if (command == 'p') {
            print_park(park);
        } else if (command == 'i') {
            insert_ride(park);
        } else if (command == 'j') {
            join_queue(park);
        } else if (command == 'd') {
            remove_from_queue(park);
        } else if (command == 'm') {
            move_to_different_rides(park);
        } else if (command == 't') {
            count_visitors(park);
        } else if (command == 'c') {
            count_range(park);
        } else if (command == 'l') {
            leave_park(park);
        } else if (command == 'r') {
            operate_rides(park);
        } else if (command == 'S') {
            shut_down_ride(park);
        } else if (command == 'M') {
            enum ride_type type = scan_type();
            merge_rides(park, type);
        } else if (command == 's') {
            split_ride(park);
        } else {
            printf("ERROR: Invalid command.\n");
        }  
    }
}
// Stage 1.3
// Function to add a ride to the park
// Params:
//      park - a pointer to the park
// Returns: None
void add_ride(struct park *park) {
    
    char name[MAX_SIZE];
    scan_name(name);  

    enum ride_type type = scan_type();
    
    if (type == INVALID) {
        printf("ERROR: Invalid ride type.\n");
        return;
    }

    // Error check: name already exists (stage 1.5)
    if (ride_exist(park, name) == 1) {
        printf("ERROR: '%s' already exists.\n", name);
        return;
    }
    
    struct ride *new_ride = create_ride(name, type);

    if (park->rides == NULL) {
        park->rides = new_ride;
    } else {
        struct ride *current = park->rides;
        while (current->next != NULL) {
            current = current->next;
        }
       
        current->next = new_ride;
    }

    printf("Ride: '%s' added!\n", name);

}

// Stage 1.3
// Function to add a visitor to the park
// Params:
//      park - a pointer to the park
// Returns: None
void add_visitor(struct park *park) {
    
    char name[MAX_SIZE];
    double height;

    scan_name(name);              
    scanf(" %lf", &height);
    
    if (height < 50 || height > 250) {
        printf("ERROR: Height must be between 50 and 250.\n");
        return;
    }

    // Error check: name already exists(stage 1.5)
    if (visitor_exist(park, name) == 1) {
        printf("ERROR: '%s' already exists.\n", name);
        return;
    }

    if (park->total_visitors >= 40) {
        printf("ERROR: Cannot add another visitor to the park. The park is at capacity.\n");
        return;
    }

    struct visitor *new_visitor = create_visitor(name, height);

    if (park->visitors == NULL) {
        park->visitors = new_visitor;
    } else {
        struct visitor *current = park->visitors;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_visitor;
    }
    park->total_visitors++;

    printf("Visitor: '%s' has entered the amusement park!\n", name);
}


// Stage 1.4
// Function to print the park
// Params:
//      park - a pointer to the park
// Returns: None
void print_park(struct park *park) {
    
    // Print the title
    print_welcome_message(park->name);

    // If the park is completely empty
    if (park->rides == NULL && park->visitors == NULL) {
        printf("The amusement park is empty!\n\n");
        return;
    }

    printf("Rides:\n");
    if (park->rides == NULL) {
        printf("  No rides!\n");
    } else {
        struct ride *current_ride = park->rides;
        while (current_ride != NULL) {
            print_ride(current_ride);
            current_ride = current_ride->next;
        }
    }

    printf("Visitors:\n");
    if (park->visitors == NULL) {
        printf("  No visitors!\n");
    } else {
        struct visitor *current_visitor = park->visitors;
        while (current_visitor != NULL) {
            printf("  %s (%.2lfcm)\n", current_visitor->name,\
             current_visitor->height);
            current_visitor = current_visitor->next;
        }
    }
    printf("\n");  
}


// Stage 1.5
int visitor_exist(struct park *park, char name[MAX_SIZE]) {
    struct visitor *current = park->visitors;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int ride_exist(struct park *park, char name[MAX_SIZE]) {
    struct ride *current = park->rides;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// Stage 2.1
void insert_ride(struct park *park) {
    int n;
    char name[MAX_SIZE];

    scanf(" %d", &n);
    scan_name(name);
    enum ride_type type = scan_type();
    // Error 1: n < 1
    if (n < 1) {
        printf("ERROR: n must be at least 1.\n");
        return;
    }
    // Error 2: IBNVALID
    if (type == INVALID) {
        printf("ERROR: Invalid ride type.\n");
        return;
    }
    // Error 3: EXIST
    if (ride_exist(park, name)) {
        printf("ERROR: '%s' already exists.\n", name);
        return;
    }
    struct ride *new_ride = create_ride(name, type);
    if (n == 1 || park->rides == NULL) {
        new_ride->next = park->rides;
        park->rides = new_ride;
    } else {
        struct ride *current = park->rides;
        int count = 1;
        while (current->next != NULL && count < n - 1) {
            current = current->next;
            count++;
        }
        new_ride->next = current->next;
        current->next = new_ride;
    }

    printf("Ride: '%s' inserted!\n", name);
}

// Stage 2.2
void join_queue(struct park *park) {
    char ride_name[MAX_SIZE], visitor_name[MAX_SIZE];
    scan_name(ride_name); 
    scan_name(visitor_name);
    struct ride *ride_node = park->rides;
    while (ride_node != NULL && strcmp(ride_node->name, ride_name) != 0) {
        ride_node = ride_node->next;
    } 
    if (ride_node == NULL) {
        printf("ERROR: No ride exists with name '%s'.\n", ride_name);
        return;
    }
    struct visitor *previous = NULL;
    struct visitor *current = park->visitors;
    while (current != NULL && strcmp(current->name, visitor_name) != 0) {
        previous = current;
        current = current->next;
    } 
    if (current == NULL) {
        printf("ERROR: No visitor exists with name '%s'.\n", visitor_name);
        return;
    } else if (current->height < ride_node->min_height) {
        printf("ERROR: '%s' is not tall enough to ride '%s'.\n", \
             visitor_name, ride_name);
        return;
    }
    int queue_count = 0;
    struct visitor *que = ride_node->queue;
    while (que != NULL) {
        queue_count++;
        que = que->next;
    } 
    if (queue_count >= ride_node->queue_capacity) {
        printf("ERROR: The queue for '%s' is full. '%s' cannot join the queue.\n", \
                ride_name, visitor_name);
        return;
    } else if (previous == NULL) {
        park->visitors = current->next;
    } else {
        previous->next = current->next;
    }
    current->next = NULL;
    if (ride_node->queue == NULL) {
        ride_node->queue = current;
    } else {
        struct visitor *q_tail = ride_node->queue;
        while (q_tail->next != NULL) {
            q_tail = q_tail->next;
        }
        q_tail->next = current;
    }
    printf("Visitor: '%s' has entered the queue for '%s'.\n",visitor_name, ride_name);
} 

// Stage 2.3
void remove_from_queue(struct park *park) {
    char visitor_name[MAX_SIZE];
    scan_name(visitor_name);

    struct ride *ride_current = park->rides;

    while (ride_current != NULL) {
        struct visitor *previous = NULL;
        struct visitor *current = ride_current->queue;

        while (current != NULL) {
            if (strcmp(current->name, visitor_name) == 0) {
                if (previous == NULL) {
                    ride_current->queue = current->next;
                } else {
                    previous->next = current->next;
                }
                current->next = NULL;
                if (park->visitors == NULL) {
                    park->visitors = current;
                } else {
                    struct visitor *visitor_tail = park->visitors;
                    while (visitor_tail->next != NULL) {
                        visitor_tail = visitor_tail->next;
                    }
                    visitor_tail->next = current;
                }

                printf ("Visitor: '%s' has been removed from their ride queue and is now roaming the park.\n", \
                        visitor_name);
                return;
            }

            previous = current;
            current = current->next;
        }
        ride_current = ride_current->next;
    }
    printf("ERROR: Visitor '%s' not found in any queue.\n", visitor_name);
}

// Stage 2.4
void move_to_different_rides(struct park *park) {
    char visitor_name[MAX_SIZE];
    char ride_name[MAX_SIZE];

    scan_name(visitor_name);
    scan_name(ride_name);

    // Find the target ride
    struct ride *target_ride = park->rides;
    while (target_ride != NULL && strcmp(target_ride->name, ride_name) != 0) {
        target_ride = target_ride->next;
    }
    if (target_ride == NULL) {
        printf("ERROR: No ride exists with name '%s'.\n", ride_name);
        return;
    }

    // Find the visitor, possibly in a roaming list or a queue. Do not
    // detach it until the target ride has passed every validation.
    struct visitor *previous = NULL;
    struct visitor *visitors = NULL;

    // roaming (park->visitors)
    struct visitor *current = park->visitors;
    while (current != NULL) {
        if (strcmp(current->name, visitor_name) == 0) {
            visitors = current;
            break;
        }
        previous = current;
        current = current->next;
    }

    // queue
    struct ride *ride_a = park->rides;
    struct visitor *queue_previous = NULL;
    if (visitors == NULL) {
        while (ride_a != NULL) {
            struct visitor *queue_current = ride_a->queue;
            queue_previous = NULL;
            while (queue_current != NULL) {
                if (strcmp(queue_current->name, visitor_name) == 0) {
                    if (ride_a == target_ride) {
                        printf("ERROR: '%s' is already in the queue for '%s'.\n",\
                             visitor_name, ride_name);
                        return;
                    }
                    // Found visitor not in target ride,
                    //  ready to be removed from this queue.
                    visitors = queue_current;
                    break;
                }
                queue_previous = queue_current;
                queue_current = queue_current->next;
            }
            if (visitors != NULL) break;
            ride_a = ride_a->next;
        }
    }

    // No visitor found (which means it's not in
    // park->visitors or in any queue)
    if (visitors == NULL) {
        printf("ERROR: No visitor with name: '%s' exists.\n", visitor_name);
        return;
    }

    if (visitors->height < target_ride->min_height) {
        printf("ERROR: '%s' is not tall enough to ride '%s'.\n",\
             visitor_name, ride_name);
        return;
    }
    int queue_count = 0;
    struct visitor *que = target_ride->queue;
    while (que != NULL) {
        queue_count++;
        que = que->next;
    }
    if (queue_count >= target_ride->queue_capacity) {
        printf("ERROR: The queue for '%s' is full. '%s' cannot join the queue.\n", \
             ride_name, visitor_name);
        return;
    }

    // Now it is safe to remove the visitor from its current list.
    if (ride_a != NULL) {
        if (queue_previous == NULL) ride_a->queue = visitors->next;
        else queue_previous->next = visitors->next;
    } else if (previous == NULL) {
        park->visitors = visitors->next;
    } else {
        previous->next = visitors->next;
    }
    visitors->next = NULL;
    if (target_ride->queue == NULL) {
        target_ride->queue = visitors;
    } else {
        struct visitor *q_tail = target_ride->queue;
        while (q_tail->next != NULL) {
            q_tail = q_tail->next;
        }
        q_tail->next = visitors;
    }

    printf("Visitor: '%s' has been moved to the queue for '%s'.\n", \
         visitor_name, ride_name);
}

// Stage 2.5
void count_visitors(struct park *park) {
    int roaming = 0;
    struct visitor *current = park->visitors;
    while (current != NULL) {
        roaming++;
        current = current->next;
    }

    int queued = 0;
    struct ride *ride = park->rides;
    while (ride != NULL) {
        struct visitor *que = ride->queue;
        while (que != NULL) {
            queued++;
            que = que->next;
        }
        ride = ride->next;
    }

    int total = roaming + queued;
    printf("Total visitors: %d\n", total);
    printf("Visitors walking around: %d\n", roaming);
    printf("Visitors in queues: %d\n", queued);
}

void count_range(struct park *park) {
    char start_name[MAX_SIZE], end_name[MAX_SIZE];
    scan_name(start_name);
    scan_name(end_name);

    struct ride *start_ride = NULL;
    struct ride *end_ride = NULL;

    struct ride *ride_ptr = park->rides;
    while (ride_ptr != NULL) {
        if (strcmp(ride_ptr->name, start_name) == 0) {
            start_ride = ride_ptr;
        }
        if (strcmp(ride_ptr->name, end_name) == 0) {
            end_ride = ride_ptr;
        }
        ride_ptr = ride_ptr->next;
    }

    if (start_ride == NULL || end_ride == NULL) {
        printf("ERROR: One or both rides do not exist ('%s' or '%s').\n", \
               start_name, end_name);
        return;
    }

    int total_queue_count = 0;
    struct ride *current_ride = start_ride;
    while (1) {
        total_queue_count += count_queue_length(current_ride->queue);
        if (current_ride == end_ride) break;
        current_ride = current_ride->next;
        if (current_ride == NULL) current_ride = park->rides;
    }
    printf("Total visitors from '%s' to '%s': %d.\n",
           start_name, end_name, total_queue_count);
}

// Stage 3.1
void free_park(struct park *park) {

    struct ride *ride_pointer = park->rides;
    while (ride_pointer != NULL) {
    
        struct visitor *queue_pointer = ride_pointer->queue;
        while (queue_pointer != NULL) {
            struct visitor *visitor_to_free = queue_pointer;
            queue_pointer = queue_pointer->next;
            free(visitor_to_free);
        }

        struct ride *ride_to_free = ride_pointer;
        ride_pointer = ride_pointer->next;
        free(ride_to_free);
    }

    struct visitor *visitor_pointer = park->visitors;
    while (visitor_pointer != NULL) {
        struct visitor *visitor_to_free = visitor_pointer;
        visitor_pointer = visitor_pointer->next;
        free(visitor_to_free);
    }

    free(park);
}

// Stage 3.2
void leave_park(struct park *park) {
    char visitor_name[MAX_SIZE];
    scan_name(visitor_name);

    struct visitor *previous_visitor = NULL;
    struct visitor *current_visitor = park->visitors;
    while (current_visitor != NULL) {
        if (strcmp(current_visitor->name, visitor_name) == 0) {
            
            if (previous_visitor == NULL) {
                park->visitors = current_visitor->next;
            } else {
                previous_visitor->next = current_visitor->next;
            }
            free(current_visitor);
            park->total_visitors--;
            printf("Visitor: '%s' has left the park.\n", visitor_name);
            return;
        }
        previous_visitor = current_visitor;
        current_visitor = current_visitor->next;
    }

    struct ride *ride_pointer = park->rides;
    while (ride_pointer != NULL) {
        struct visitor *previous_in_queue = NULL;
        struct visitor *current_in_queue = ride_pointer->queue;

        while (current_in_queue != NULL) {
            if (strcmp(current_in_queue->name, visitor_name) == 0) {
            
                if (previous_in_queue == NULL) {
                    ride_pointer->queue = current_in_queue->next;
                } else {
                    previous_in_queue->next = current_in_queue->next;
                }
                free(current_in_queue);
                park->total_visitors--;
                printf("Visitor: '%s' has left the park.\n", visitor_name);
                return;
            }
            previous_in_queue = current_in_queue;
            current_in_queue = current_in_queue->next;
        }

        ride_pointer = ride_pointer->next;
    }

    printf("ERROR: Visitor '%s' not found in the park.\n",\
         visitor_name);
}

// Stage 3.3
void operate_rides(struct park *park) {
   
    struct ride *reversed = NULL;
    struct ride *current_ride = park->rides;

    while (current_ride != NULL) {
        struct ride *next_ride = current_ride->next;
        current_ride->next = reversed;
        reversed = current_ride;
        current_ride = next_ride;
    }
    
    current_ride = reversed;
    while (current_ride != NULL) {
        int count = 0;
        struct visitor *queue_head = current_ride->queue;
        struct visitor *next = NULL;

        while (queue_head != NULL && count < current_ride->rider_capacity) {
            next = queue_head->next;
            queue_head->next = NULL;

            
            if (park->visitors == NULL) {
                park->visitors = queue_head;
            } else {
                struct visitor *roaming_tail = park->visitors;
                while (roaming_tail->next != NULL) {
                    roaming_tail = roaming_tail->next;
                }
                roaming_tail->next = queue_head;
            }

            count++;
            queue_head = next;
        }
     
        current_ride->queue = queue_head;

        current_ride = current_ride->next;
    }

    struct ride *re_restored = NULL;
    current_ride = reversed;
    while (current_ride != NULL) {
        struct ride *next = current_ride->next;
        current_ride->next = re_restored;
        re_restored = current_ride;
        current_ride = next;
    }

    park->rides = re_restored;
}

// Stage 3.4
void shut_down_ride(struct park *park) {
    char ride_name[MAX_SIZE];
    scan_name(ride_name);

    struct ride *previous_ride = NULL;
    struct ride *ride_to_shut_down = park->rides;

    while (ride_to_shut_down != NULL && \
        strcmp(ride_to_shut_down->name, ride_name) != 0) {
        previous_ride = ride_to_shut_down;
        ride_to_shut_down = ride_to_shut_down->next;
    }

    if (ride_to_shut_down == NULL) {
        printf("ERROR: No ride exists with name '%s'.\n", ride_name);
        return;
    }

    enum ride_type type_of_shutting_ride = ride_to_shut_down->type;

    int total_available_capacity = 0;

    struct ride *ride_being_checked = park->rides;
    while (ride_being_checked != NULL) {
        if (ride_being_checked != ride_to_shut_down &&
            ride_being_checked->type == type_of_shutting_ride) {

            int current_queue_length = 0;
            struct visitor *visitor_in_queue = ride_being_checked->queue;
            while (visitor_in_queue != NULL) {
                current_queue_length++;
                visitor_in_queue = visitor_in_queue->next;
            }

            int remaining_capacity = ride_being_checked->queue_capacity
                 - current_queue_length;
            total_available_capacity += remaining_capacity;
        }
        ride_being_checked = ride_being_checked->next;
    }

    int shutting_ride_queue_length = 0;
    struct visitor *visitor_in_shutting_queue = ride_to_shut_down->queue;
    while (visitor_in_shutting_queue != NULL) {
        shutting_ride_queue_length++;
        visitor_in_shutting_queue = visitor_in_shutting_queue->next;
    }

    if (shutting_ride_queue_length > total_available_capacity) {
        printf("ERROR: Not enough capacity to redistribute all visitors from '%s'.\n", \
                ride_name);

        struct visitor *visitor_to_move = ride_to_shut_down->queue;
        while (visitor_to_move != NULL) {
            struct visitor *next_visitor = visitor_to_move->next;
            visitor_to_move->next = NULL;

            if (park->visitors == NULL) {
                park->visitors = visitor_to_move;
            } else {
                struct visitor *tail_of_roaming_visitors = park->visitors;
                while (tail_of_roaming_visitors->next != NULL) {
                    tail_of_roaming_visitors = tail_of_roaming_visitors->next;
                }
                tail_of_roaming_visitors->next = visitor_to_move;
            }

            visitor_to_move = next_visitor;
        }

    } else {
        struct visitor *visitor_to_move = ride_to_shut_down->queue;
        ride_to_shut_down->queue = NULL;

        ride_being_checked = park->rides;
        while (visitor_to_move != NULL && ride_being_checked != NULL) {
            if (ride_being_checked != ride_to_shut_down &&\
                ride_being_checked->type == type_of_shutting_ride) {

                int current_queue_length = 0;
                struct visitor *visitor_in_queue = ride_being_checked->queue;
                while (visitor_in_queue != NULL) {
                    current_queue_length++;
                    visitor_in_queue = visitor_in_queue->next;
                }

                int remaining_capacity = \
                    ride_being_checked->queue_capacity \
                        - current_queue_length;
                while (remaining_capacity > 0 && visitor_to_move != NULL) {
                    struct visitor *next_visitor = visitor_to_move->next;
                    visitor_to_move->next = NULL;

                    if (ride_being_checked->queue == NULL) {
                        ride_being_checked->queue = visitor_to_move;
                    } else {
                        struct visitor *tail_of_queue = \
                        ride_being_checked->queue;
                        while (tail_of_queue->next != NULL) {
                            tail_of_queue = tail_of_queue->next;
                        }
                        tail_of_queue->next = visitor_to_move;
                    }

                    visitor_to_move = next_visitor;
                    remaining_capacity--;
                }
            }
            ride_being_checked = ride_being_checked->next;
        }

        while (visitor_to_move != NULL) {
            struct visitor *next_visitor = visitor_to_move->next;
            visitor_to_move->next = NULL;

            if (park->visitors == NULL) {
                park->visitors = visitor_to_move;
            } else {
                struct visitor *tail_of_roaming_visitors = park->visitors;
                while (tail_of_roaming_visitors->next != NULL) {
                    tail_of_roaming_visitors = tail_of_roaming_visitors->next;
                }
                tail_of_roaming_visitors->next = visitor_to_move;
            }

            visitor_to_move = next_visitor;
        }
    }

    if (previous_ride == NULL) {
        park->rides = ride_to_shut_down->next;
    } else {
        previous_ride->next = ride_to_shut_down->next;
    }

    free(ride_to_shut_down);

    printf("Ride: '%s' shut down.\n", ride_name);
}

// Stage 4.1
void merge_rides(struct park *park, enum ride_type type) {
    struct ride *smallest = NULL;
    struct ride *next_smallest = NULL;
    for (struct ride *current = park->rides; current != NULL;
         current = current->next) {
        if (current->type != type) continue;
        if (smallest == NULL || count_queue_length(current->queue) <
            count_queue_length(smallest->queue)) {
            next_smallest = smallest;
            smallest = current;
        } else if (next_smallest == NULL || count_queue_length(current->queue) <
                   count_queue_length(next_smallest->queue)) {
            next_smallest = current;
        }
    }
    if (smallest == NULL || next_smallest == NULL) {
        printf("ERROR: Not enough rides of the specified type to merge.\n");
        return;
    }

    // Preserve the one earlier in the park list, as required by the spec.
    struct ride *keep = smallest;
    struct ride *remove = next_smallest;
    for (struct ride *current = park->rides; current != NULL;
         current = current->next) {
        if (current == next_smallest) { keep = next_smallest; remove = smallest; break; }
        if (current == smallest) break;
    }

    struct visitor *a = smallest->queue;
    struct visitor *b = next_smallest->queue;
    if (count_queue_length(b) > count_queue_length(a)) {
        struct visitor *temp = a; a = b; b = temp;
    }
    struct visitor *head = NULL;
    struct visitor **tail = &head;
    while (a != NULL || b != NULL) {
        if (a != NULL) { struct visitor *next = a->next; a->next = NULL; *tail = a; tail = &a->next; a = next; }
        if (b != NULL) { struct visitor *next = b->next; b->next = NULL; *tail = b; tail = &b->next; b = next; }
    }
    keep->queue = head;
    keep->rider_capacity = smallest->rider_capacity + next_smallest->rider_capacity;
    keep->queue_capacity = smallest->queue_capacity + next_smallest->queue_capacity;
    delete_ride_from_park(park, remove);

    printf("Merged the two smallest rides of type '%s'.\n",\
         type_to_string(type));
}

// Stage 4.2
void split_ride(struct park *park) {
    int n;
    char name[MAX_SIZE];
    scanf(" %d", &n);
    scan_name(name);
    struct ride *previous = NULL;
    struct ride *old = park->rides;
    while (old != NULL && strcmp(old->name, name) != 0) {
        previous = old;
        old = old->next;
    }
    if (old == NULL) {
        printf("ERROR: No ride exists with name: '%s'.\n", name);
        return;
    }
    if (n <= 1) {
        printf("ERROR: Cannot split '%s' into %d rides. n must be > 1.\n", name, n);
        return;
    }
    struct ride *new_head = NULL;
    struct ride *new_tail = NULL;
    int suffix = 1;
    for (int made = 0; made < n; made++) {
        char new_name[MAX_SIZE];
        do {
            snprintf(new_name, MAX_SIZE, "%.*s_%d", MAX_SIZE - 12, name, suffix++);
        } while (ride_exist(park, new_name));
        struct ride *ride = create_ride(new_name, old->type);
        if (new_head == NULL) new_head = ride;
        else new_tail->next = ride;
        new_tail = ride;
    }
    int total = count_queue_length(old->queue);
    int base = total / n;
    int extra = total % n;
    struct visitor *visitor = old->queue;
    struct ride *ride = new_head;
    for (int i = 0; i < n; i++, ride = ride->next) {
        int amount = base + (i < extra);
        struct visitor **tail = &ride->queue;
        while (amount-- > 0) {
            struct visitor *next = visitor->next;
            visitor->next = NULL;
            *tail = visitor;
            tail = &visitor->next;
            visitor = next;
        }
    }
    new_tail->next = old->next;
    if (previous == NULL) park->rides = new_head;
    else previous->next = new_head;
    free(old);
    printf("Ride '%s' split into %d new rides.\n", name, n);
}


int count_queue_length(struct visitor *queue_head) {
    int length = 0;
    while (queue_head != NULL) {
        length++;
        queue_head = queue_head->next;
    }
    return length;
}

void delete_ride_from_park(struct park *park, \
     struct ride *ride_to_delete) {
    if (park == NULL || ride_to_delete == NULL) {
        return;
    }

    struct ride *previous_ride = NULL;
    struct ride *current_ride = park->rides;

    while (current_ride != NULL) {
        if (current_ride == ride_to_delete) {
            if (previous_ride == NULL) {
                park->rides = current_ride->next;
            } else {
                previous_ride->next = current_ride->next;
            }

            free(current_ride);
            return;
        }
        previous_ride = current_ride;
        current_ride = current_ride->next;
    }
}



////////////////////////////////////////////////////////////////////////////////
// Providing function definitions
////////////////////////////////////////////////////////////////////////////////

// Function to print the usage of the program
// '?' command
// Params: None
// Returns: None
// Usage:
// ```
//      print_usage();
// ```
void print_usage(void) {
    printf(
        "======================[ CS Amusement Park ]======================\n"
        "      ===============[     Usage Info     ]===============       \n"
        "  a r [ride_name] [ride_type]                                    \n"
        "    Add a ride to the park.                                      \n"
        "  a v [visitor_name] [visitor_height]                            \n"
        "    Add a visitor to the park.                                   \n"
        "  i [index] [ride_name] [ride_type]                              \n"
        "    Insert a ride at a specific position in the park's ride list.\n"
        "  j [ride_name] [visitor_name]                                   \n"
        "    Add a visitor to the queue of a specific ride.               \n"
        "  m [visitor_name] [ride_name]                                   \n"
        "    Move a visitor from roaming to a ride's queue.               \n"
        "  d [visitor_name]                                               \n"
        "    Remove a visitor from any ride queue and return to roaming.  \n"
        "  p                                                              \n"
        "    Print the current state of the park, including rides and     \n"
        "    visitors.                                                    \n"
        "  t                                                              \n"
        "    Display the total number of visitors in the park, including  \n"
        "    those roaming and in ride queues.                            \n"
        "  c [start_ride] [end_ride]                                      \n"
        "    Count and display the number of visitors in queues between   \n"
        "    the specified start and end rides, inclusive.                \n"
        "  l [visitor_name]                                               \n"
        "    Remove a visitor entirely from the park.                     \n"
        "  r                                                              \n"
        "    Operate all rides, allowing visitors to enjoy the rides      \n"
        "    and moving them to roaming after their ride.                 \n"
        "  M [ride_type]                                                  \n"
        "    Merge the two smallest rides of the specified type.          \n"
        "  s [n] [ride_name]                                              \n"
        "    Split an existing ride into `n` smaller rides.               \n"
        "  q                                                              \n"
        "    Quit the program and free all allocated resources.           \n"
        "  T [n] [command]                                                \n"
        "    Scheduled the [command] to take place `n` ticks              \n"
        "    in the future.                                               \n"
        "  ~ [n]                                                          \n"
        "    Progress the schedule for `n` ticks.                         \n"
        "  ?                                                              \n"
        "    Show this help information.                                  \n"
        "=================================================================\n");
}

// Function to print a welcome message
// Params:
//      name - the name of the park
// Returns: None
// Usage:
// ```
//      print_welcome_message(name);
// ```
void print_welcome_message(char name[MAX_SIZE]) {
    printf("===================[ %s ]===================\n", name);
}

// Function to print a ride
// Params:
//      ride - the ride to print
// Returns: None
// Usage:
// ```
//      print_ride(ride);
// ```
void print_ride(struct ride *ride) {
    printf("  %s (%s)\n", ride->name, type_to_string(ride->type));
    printf("    Rider Capacity: %d\n", ride->rider_capacity);
    printf("    Queue Capacity: %d\n", ride->queue_capacity);
    printf("    Minimum Height: %.2lfcm\n", ride->min_height);
    printf("    Queue:\n");
    struct visitor *curr_visitor = ride->queue;
    if (curr_visitor == NULL) {
        printf("      No visitors\n");
    } else {
        while (curr_visitor != NULL) {
            printf(
                "      %s (%.2lfcm)\n", curr_visitor->name,
                curr_visitor->height);
            curr_visitor = curr_visitor->next;
        }
    }
}

// Scan in the a name string into the provided buffer, placing
// '\0' at the end.
//
// Params:
//      name - a char array of length MAX_SIZE, which will be used
//                  to store the name.
// Returns: None
// Usage:
// ```
//      char name[MAX_SIZE];
//      scan_name(name);
// ```
void scan_name(char name[MAX_SIZE]) {
    scan_token(name, MAX_SIZE);
}

// Scans a string and converts it to a ride_type.
//
// Params: None
// Returns:
//      The corresponding ride_type, if the string was valid,
//      Otherwise, returns INVALID.
//
// Usage:
// ```
//      enum ride_type type = scan_type();
// ```
//
enum ride_type scan_type(void) {
    char type[MAX_SIZE];
    scan_token(type, MAX_SIZE);
    return string_to_type(type);
}

////////////////////////////////////////////////////////////////////////////////
// Additional provided functions
////////////////////////////////////////////////////////////////////////////////

// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

enum ride_type string_to_type(char *type_str) {
    int len = strlen(type_str);

    if (strncasecmp(type_str, "roller_coaster", len) == 0) {
        return ROLLER_COASTER;
    }
    if (strncasecmp(type_str, "CAROUSEL", len) == 0) {
        return CAROUSEL;
    }
    if (strncasecmp(type_str, "FERRIS_WHEEL", len) == 0) {
        return FERRIS_WHEEL;
    }
    if (strncasecmp(type_str, "BUMPER_CARS", len) == 0) {
        return BUMPER_CARS;
    }

    return INVALID;
}

char *type_to_string(enum ride_type type) {
    if (type == ROLLER_COASTER) {
        return "ROLLER_COASTER";
    }
    if (type == CAROUSEL) {
        return "CAROUSEL";
    }
    if (type == FERRIS_WHEEL) {
        return "FERRIS_WHEEL";
    }
    if (type == BUMPER_CARS) {
        return "BUMPER_CARS";
    }
    return "INVALID";
}

int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    scanf(" ");

    while (i < buffer_size - 1 && (num_scanned = scanf("%c", &c)) == 1 &&
           !isspace(c)) {
        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}
