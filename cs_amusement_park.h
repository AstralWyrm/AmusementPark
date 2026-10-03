// CS Amusement Park
// cs_amusement_park.h
// Written by <Yi Han>
// on <15/04/2025>

////////////////////////////////////////////////////////////////////////////////
// Provided Constants
////////////////////////////////////////////////////////////////////////////////
#define MAX_SIZE 100

////////////////////////////////////////////////////////////////////////////////
// Your constants
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Provided Enums
////////////////////////////////////////////////////////////////////////////////

enum ride_type { ROLLER_COASTER, CAROUSEL, FERRIS_WHEEL, BUMPER_CARS, INVALID };

////////////////////////////////////////////////////////////////////////////////
// Provided Structs
////////////////////////////////////////////////////////////////////////////////

struct park {
    // The name of the park
    char name[MAX_SIZE];
    // The total number of visitors in the park
    int total_visitors;
    // The list of rides in the park
    struct ride *rides;
    // The list of visitors in the park
    struct visitor *visitors;
};

struct ride {
    // The name of the ride
    char name[MAX_SIZE];
    // The type of the ride i.e. ROLLER_COASTER,
    // CAROUSEL, FERRIS_WHEEL, BUMPER_CARS, INVALID

    enum ride_type type;
    // The number of riders the ride can hold
    int rider_capacity;
    // The number of visitors that can be in the queue
    int queue_capacity;
    // The minimum height required to ride
    double min_height;
    // The queue of visitors waiting to ride
    struct visitor *queue;
    // The next ride in the list
    struct ride *next;
};

struct visitor {
    // The name of the visitor
    char name[MAX_SIZE];
    // The height of the visitor
    double height;
    // The next visitor in the list
    struct visitor *next;
};

////////////////////////////////////////////////////////////////////////////////
// Provided function prototypes
////////////////////////////////////////////////////////////////////////////////
void print_usage(void);
void scan_name(char name[MAX_SIZE]);
void print_ride(struct ride *ride);
enum ride_type scan_type(void);
void print_welcome_message(char name[MAX_SIZE]);

// Additional provided function prototypes
// You won't need to use these functions!
// We use just them to implement some of the provided helper functions.
int scan_token(char *buffer, int buffer_size);
char *type_to_string(enum ride_type type);
enum ride_type string_to_type(char *type_str);

////////////////////////////////////////////////////////////////////////////////
// Your function prototypes
////////////////////////////////////////////////////////////////////////////////

// Stage 1
struct park *initialise_park(char name[MAX_SIZE]);
struct visitor *create_visitor(char name[MAX_SIZE], double height);
struct ride *create_ride(char name[MAX_SIZE], enum ride_type type);
void command_loop(struct park *park);
void add_ride(struct park *park);
void add_visitor(struct park *park);
void print_park(struct park *park);
int visitor_exist(struct park *park, char name[MAX_SIZE]);
int ride_exist(struct park *park, char name[MAX_SIZE]);

// Stage 2
void insert_ride(struct park *park);
void join_queue(struct park *park);
void remove_from_queue(struct park *park);
void move_to_different_rides(struct park *park);
void count_visitors(struct park *park);
void count_range(struct park *park);

// Stage 3
void free_park(struct park *park);
void leave_park(struct park *park);
void operate_rides(struct park *park);
void shut_down_ride(struct park *park);

// Stage 4
void merge_rides(struct park *park, enum ride_type ride_type_to_merge);
int count_queue_length(struct visitor *queue_head);
void delete_ride_from_park(struct park *park, struct ride *ride_to_delete);
void split_ride(struct park *park);
