#include "mr76_radar.h"

// MR76::MR76(){
//     _total_objects = 0;
//     _cycles = 0;
//     object_counter = 0;
// }

void MR76::parse_data(int id, int len, unsigned char rxBuf[8]){
    if(id & 0x60B == 0x60B and len == 8){
        int16_t data_stream[8]; // stream of extracted bits defining data
      
        data_stream[0] = rxBuf[0];                                                            // object id
        data_stream[1] = int16_t(((rxBuf[1] << 8) | (rxBuf[2] & 0xF8)) >> 3);                 // distance long
        data_stream[2] = int16_t((rxBuf[2] & 0x07) << 8 | rxBuf[3]);                          // distance lat
        data_stream[3] = int16_t(((rxBuf[4] << 8) | (rxBuf[5] & 0xC0)) >> 6);                 // Vrelative long
        data_stream[4] = int16_t(((((rxBuf[5] & 0x3F) << 8) | (rxBuf[6] & 0xE0))) >> 5);      // Vrelative lat
        data_stream[5] = int16_t((rxBuf[6] & 0x18) >> 3);                                     // Object class
        data_stream[6] = int16_t(rxBuf[6] & 0x07);                                            // Object dynamic prop
        data_stream[7] = rxBuf[7];


        _object_data.id = int(data_stream[0])+1;
        _object_data.distance_long= float(data_stream[1]*0.2 - 500);
        _object_data.distance_lat = float(data_stream[2]*0.2 - 204.2);
        _object_data.velocity_long = float(data_stream[3]*0.25 - 128.0);
        _object_data.velocity_lat = float(data_stream[4]*0.25 - 64.0);
        _object_data.obj_section = int(data_stream[5]);
        _object_data.obj_state = int(data_stream[6]);
        _object_data.rcs = float(data_stream[7]*0.5 - 64.0);
        _object[object_counter] = _object_data;
        object_counter ++;
    }
    else if (id & 0x60A == 0x60A and len == 8){
        update_data();
        object_counter = 0;
        _total_objects = rxBuf[0];
        _cycles = int16_t((rxBuf[1] << 8 )| (rxBuf[2]));
    }
}

void MR76::update_data(){
    is_object_complete = 0;
    if(object_counter == _total_objects) is_object_complete = 1;
    is_ready = is_object_complete;
    object_detected = object_counter;
    for (int iter = 0; iter < object_counter; iter++){
        object[iter]=_object[iter];
    }
    this->total_objects = _total_objects;
    this->cycles = _cycles;

}
void MR76::configure(unsigned long int *id, unsigned char _buffer[8] ,int previous_id, int max_distance, int sensor_id, int output_type, int radar_power, int sort_index){
    int set_distance = 0;
    int set_id = 0;
    int set_power = 0;
    int set_output = 0;
    int set_quality = 0;
    int set_ext = 0;
    int set_sort = 0;
    int set_nvm = 1;
    if (max_distance != 0) set_distance = 1;
    if (sensor_id != 0) set_id = 1;
    if (output_type != 0) set_output = 1;
    if (radar_power != 0) set_power = 1;
    if (sort_index != 0) set_sort = 1;
    *id = 0x200;
    _buffer[0] = (
        set_distance |
        set_id << 1 |
        set_power << 2 |
        set_output << 3 |
        set_quality << 4 |
        set_ext << 5 |
        set_sort << 6|
        set_nvm << 7 
        );

    _buffer[1] = (max_distance & 0x1FC) >> 2;
    _buffer[2] = (max_distance & 0x3) << 6;
    _buffer[4] = (radar_power << 5) | (output_type << 3) | (sensor_id);
    _buffer[5] = (0x01 << 7) | (sort_index << 4);
}
int MR76::isready(){
    if (is_ready != 2 and cycles%(skip_cycle + 1) == 0){
        is_ready = 2;
        return 1;
    }
    else {
        return 0;
    }
}