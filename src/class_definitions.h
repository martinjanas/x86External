#pragma once

class CGlobalVarsBase
{
public:
	float     realtime;                       // 0x0000
	int       framecount;                     // 0x0004
	float     absoluteframetime;              // 0x0008
	float     absoluteframe_starttime_stddev; // 0x000C
	float     curtime;                        // 0x0010
	float     frametime;                      // 0x0014
	int       max_clients;                    // 0x0018
	int       tickcount;                      // 0x001C
	float     interval_per_tick;              // 0x0020
	float     interpolation_amount;           // 0x0024
	int       sim_ticks_this_frame;           // 0x0028
	int       network_protocol;               // 0x002C
	void*     save_data;                      // 0x0030
	bool      is_client;                      // 0x0031
	bool      is_remote_client;               // 0x0032
private:
	int       timestamp_networking_base;
	int       timestamp_randomize_window;
};

class glow_object_t {
public:
	void*   entity;
	float   r;
	float   g;
	float   b;
	float   a;                                 //0x0010
	uint8_t some_pad01[4];                     //0x0014
	float   some_float;                        //0x0018
	uint8_t some_pad02[4];                     //0x001C
	float   bloom_amount;                      //0x0020
	bool    render_when_occluded;              //0x0024
	bool    render_when_unoccluded;            //0x0025
	bool    full_bloom_render;                 //0x0026
	uint8_t some_pad03[5];                     //0x0027
	int32_t glow_style;                        //0x002C
	int32_t split_screen_slot;                 //0x0030
	int32_t next_free_slot;                    //0x0034
};
