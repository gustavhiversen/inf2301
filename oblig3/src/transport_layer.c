/// @file
/// <summary>
/// Contains the code implementation for the transport layer abstraction in the OSI-stack.<br/>
/// <strong>Students are required to implement the provided stubs within this file according to their own understanding of the transport layer.</strong>
/// </summary>

#include <stdlib.h>
#include <stdio.h>

#include "transport_layer.h"
#include "transport_package_impl.h"
#define WINDOW_SIZE 10
#define SEQUENCE_MAX 20
/// <summary>Sets a tick timer object for the specified transport layer.</summary>
/// <param name="tp_layer">A reference to the transport layer instance that sets the timer.</param>
/// <param name="timer">A reference to the timer object to set.</param>
/// <param name="ticks">The number of ticks to set the timer for.</param>
/// <remarks>
/// This serves as a simple convenience function that provides easy access to the more general <see cref="timer_set"/> function.
/// <para>This function will set the timer object with <see cref="transport_layer_onLayerTimeout"/> as the callback function, and
/// <paramref name="tp_layer"/> as the context argument. This will <see cref="transport_layer_onLayerTimeout"/> to be invoked correctly
/// with the right argument value when the timer expires.</para>
/// </remarks>
static void transport_layer_timer_set(transport_layer_t* tp_layer, tick_timer_t* timer, int ticks);

/// <summary>Data structure implementing the <see cref="transport_layer_t"/> type.</summary>
/// <remarks>
/// <strong>Students should modify/add fields declared in this structure according to their own understanding of the transport layer.</strong>
/// </remarks>
struct transport_layer
{
	/// <summary>A reference to the OSI-stack the transport layer belong to.</summary>
	osi_stack_t* osi_stack;

	// STUDENTS BEGIN:
	// Extend this structure with your own status variables for the transport layer.
	int window_size;
	int request_number;
	int sequence_number;
	int sequence_base;
	int sequence_end;
	int size;
	int amount;
	transport_package_t* tp_pkgs[SEQUENCE_MAX];
	tick_timer_t *timer;
	int ticks;
	// STUDENTS END
};

const unsigned int MOD_ADLER = 65521;

unsigned int adler32(unsigned char *data, size_t len) 
/* 
    where data is the location of the data in physical memory and 
    len is the length of the data in bytes 
*/
{
    unsigned int a = 1, b = 0;
    size_t index;
    
    // Process each byte of the data in order
    for (index = 0; index < len; ++index)
    {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    
    return (b << 16) | a;
}


// STUDENTS BEGIN:
// Implementation for the transport layer

transport_layer_t* transport_layer_create(osi_stack_t* osi_stack)
{

	// Remember to assign the osi_stack parameter to the new transport layer object you create in this function.
	if(!osi_stack)
		return NULL;
	transport_layer_t* tp_layer = malloc(sizeof(transport_layer_t));
	if (!tp_layer)
	{
		printf("Not enough memory");
		return NULL;
	}

	tp_layer->osi_stack = osi_stack;
	//tp_layer->window_size = WINDOW_SIZE;
	tp_layer->request_number = 0;//INDEX
	tp_layer->sequence_base = 0; //start of window
	tp_layer->sequence_end = tp_layer->sequence_base + WINDOW_SIZE; //end of window
	tp_layer->size = 0;
	tp_layer->timer = malloc(sizeof(tick_timer_t));
	tp_layer->ticks = 10;
	tp_layer->amount = 0;
	//tp_layer->sequence_max = SEQUENCE_MAX;

	return tp_layer;
}

void transport_layer_destroy(transport_layer_t* tp_layer)
{
	// transport_pkg_destroy(tp_layer->tp_pkgs);
	// free(tp_layer);'
	for (int i = 0; i > SEQUENCE_MAX; i++)
	{
		if(tp_layer->tp_pkgs[i])
		{
			transport_pkg_destroy(tp_layer->tp_pkgs[i]);
		}
	}
	free(tp_layer);
}

void print_data(transport_package_t* tp_pkg)
{

	char* buffer = (char*)tp_pkg->data;
	printf("data: 0x");
	for(int i = 0; i < tp_pkg->size; i++)
	{
		printf("%02X", (unsigned int)(unsigned char)buffer[i]);
	}
	printf("\n");

}

void transport_layer_onAppSend(transport_layer_t* tp_layer, void* data, size_t size)
{
	int hash = adler32(data, size);
	int id = tp_layer->size;
	//int id = tp_layer->request_number;
	transport_package_t* tp_pkg = transport_pkg_create(data, size, hash, id);
	tp_layer->tp_pkgs[id] = transport_pkg_copy(tp_pkg);
	tp_layer->tp_pkgs[id]->id = id;
	tp_layer->amount++;


	transport_package_t* pkg = tp_layer->tp_pkgs[tp_layer->request_number];
	tp_layer->request_number = tp_layer->request_number > WINDOW_SIZE + tp_layer->sequence_base ? tp_layer->sequence_base: tp_layer->request_number;

	printf("APP_SEND\n");
	printf("id= %d\n", pkg->id);
	printf("tp_layer->size %d\n", tp_layer->size);
	print_data(tp_pkg);
	printf("BASE %d\n", tp_layer->sequence_base);
	printf("REQUEST %d\n", tp_layer->request_number);
	printf("AMOUNT = %d\n", tp_layer->amount);
	tp_layer->request_number++;

	tp_layer->request_number = tp_layer->request_number % SEQUENCE_MAX;
	if(pkg->id == tp_layer->sequence_base)
	{
		printf("timer start\n");
		transport_layer_timer_set(tp_layer, tp_layer->timer,tp_layer->ticks);
	}
	osi_tp2nw(tp_layer->osi_stack, pkg);

	
	tp_layer->size++;
	tp_layer->size = tp_layer->size % SEQUENCE_MAX;
}


void transport_layer2_Receive(transport_layer_t* tp_layer, transport_package_t* tp_pkg)
{
	printf("\n\n");
	printf("TP_LAYER2\n");
	printf("sequence base = %d\n", tp_layer->sequence_base);
	printf("tp_pkg->id = %d\n", tp_pkg->id);
	print_data(tp_pkg);
	if(tp_pkg->id == tp_layer->sequence_base)
	{
		int hash = adler32(tp_pkg->data, tp_pkg->size);
		if(hash == tp_pkg->hash) // Package NOT corrupted
		{		
			//printf("sending ack\n");
			transport_package_t* ack_pkg = transport_pkg_copy(tp_pkg);
			// tp_layer->tp_pkgs[tp_layer->sequence_base] = ack_pkg;
			printf("onReceive\n");
			ack_pkg->ack = 1;
			tp_layer->sequence_base++;
			tp_layer->sequence_base= tp_layer->sequence_base%SEQUENCE_MAX;
			osi_tp2nw(tp_layer->osi_stack, ack_pkg);
			osi_tp2app(tp_layer->osi_stack,tp_pkg->data, tp_pkg->size);
		}
		else // Package corrupted
		{
			printf("Corrupted package\n");
			print_data(tp_pkg);
			transport_package_t* cor_pkg = transport_pkg_create(NULL, 0, 0, tp_pkg->id);
			cor_pkg->ack = 1;
			osi_tp2nw(tp_layer->osi_stack, cor_pkg);
		}
	}
	else if(tp_pkg->id > tp_layer->sequence_base + WINDOW_SIZE)
	{
		printf("wrong id package\n");
		print_data(tp_pkg);
		transport_package_t* cor_pkg = transport_pkg_create(NULL, 0, 0,tp_layer->sequence_base);
		cor_pkg->ack = 1;
		osi_tp2nw(tp_layer->osi_stack, cor_pkg);
	}
}

void transport_layer1_Receive(transport_layer_t* tp_layer, transport_package_t* tp_pkg)
{
	printf("\n\n");
	printf("TP_LAYER1\n");
	printf("sequence base = %d\n", tp_layer->sequence_base);
	printf("tp_pkg->id = %d\n", tp_pkg->id);
	printf("tp_pkg->size = %ld\n", tp_pkg->size);
	if (!tp_pkg->size) // Package corrupted
	{
		transport_package_t*tmp = transport_pkg_copy(tp_layer->tp_pkgs[tp_pkg->id]);
		//printf("FIXED id= %d\n", tmp->id);
		print_data(tmp);
		//transport_pkg_destroy(tp_pkg);
		tmp->ack = 0;
		osi_tp2nw(tp_layer->osi_stack, tmp);
	}
	else if(tp_pkg->id == tp_layer->tp_pkgs[tp_layer->sequence_base]->id) // Package NOT corrupted
	{
		printf("ack received\nid = %d\n", tp_pkg->id);
		tp_layer->sequence_base++;
		tp_layer->sequence_base = tp_layer->sequence_base % SEQUENCE_MAX;
	}
}


void transport_layer_onNwReceive(transport_layer_t* tp_layer, transport_package_t* tp_pkg)
{
	//nw recieve
	if(!tp_pkg->ack)
	{
		transport_layer2_Receive(tp_layer, tp_pkg);
	}
	else//ack = 1
	{
		transport_layer1_Receive(tp_layer, tp_pkg);	
	}
	//printf("------E-------\n\n\n\n");
}

void transport_layer_onLayerTimeout(transport_layer_t* tp_layer)
{
	printf("\n\n");
	printf("onLAYERTIMEOUTaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa-----------------------------------------------------------\n");
	transport_layer_timer_set(tp_layer, tp_layer->timer, tp_layer->ticks);
	for(int i = tp_layer->sequence_base; i <= tp_layer->sequence_base + WINDOW_SIZE; i++)
	{
		printf("index = %d\n", i);
		if(tp_layer->tp_pkgs[i%SEQUENCE_MAX])
		{
			printf("resending id = %d\n",tp_layer->tp_pkgs[i % SEQUENCE_MAX]->id);
			osi_tp2nw(tp_layer->osi_stack, tp_layer->tp_pkgs[i % SEQUENCE_MAX]);
		}
	}
}


// EXTRA PROBLEM:
// Establish connection by syncing the transmission (e.g. TCP SYN-packets)
// Shutdown connection when the application tears down the OSI stack

void transport_layer_init(transport_layer_t* tp_layer)
{
	if(!tp_layer)
	{
		return;
	}
}

void transport_layer_onTeardown(transport_layer_t* tp_layer)
{

}

// The following code defines convenience functions for you to use for timers.
// Feel free to look through it, but do not worry if you cannot make heads or tails of it.

static void transport_layer_timer_set(transport_layer_t* tp_layer, tick_timer_t* timer, int ticks)
{
	timer_set(timer, ticks, (tick_timer_callback_t)transport_layer_onLayerTimeout, tp_layer);
}
