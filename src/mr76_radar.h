#ifndef __MR76_RADAR_H__
#define __MR76_RADAR_H__

#include <stdint.h>


#define ARRAY_SIZE 32


class MR76 {



    private :
    
    struct mr76_data{
    uint8_t id;     
    float distance_long;
    float distance_lat;
    float velocity_long;
    float velocity_lat;
    uint8_t obj_section;
    uint8_t obj_state;
    float rcs;
    };
    mr76_data _object_data;
    int is_ready = 2;  
    
    void update_data();
    int _total_objects = 0;
    int _cycles = 0;
    
    int object_counter = 0;
    mr76_data _object[ARRAY_SIZE];

// -------------------------------------------------------
    public :
    void parse_data(int id, int len, unsigned char buffer[8]);      //input data from can periodically
    void configure(unsigned long int *id, unsigned char _buffer[8],
                    int previous_id, int max_distance, int sensor_id, int output_type, int radar_power, int sort_index);

    int skip_cycle = 0;                                             //some microcontroller is not fast enough to process all data
                                                                        // use this to skip some frame
    int total_objects;                                              // How many objects reported by header
    int object_detected;                                            // How many objects actually recieved by the program
    int cycles;                                                     // How many cycles/frame passed
                                                                        // not affected by skip
    int is_object_complete;                                         // To check if the total objects match                   
    mr76_data object[ARRAY_SIZE];                                   // Container of each object data
    int isready();                                                  // Check if current frame data is available

};

#endif