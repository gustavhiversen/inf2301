#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <time.h>

#include "randomlib.h"
#include "diagnostics.h"
#include "timer.h"
#include "osi_impl.h"
#include "network_layer_impl.h"
#include "application_layer_impl.h"

diag_trace_lvl_t TRACE_LEVEL;
int num_packets, bidirectional;
double loss_prob;
double corrupt_prob;
int gen_delay, nw_delay;
double nw_stddev;

static osi_stack_t a_stack;
static osi_stack_t b_stack;

static void osi_nw_layer_onDatagramTick(void* ctx[3])
{
	osi_stack_t* target;
	transport_package_t* pkg;

	if (!ctx)
		return;

	target = (osi_stack_t*)ctx[0];

	DIAG_PRINTF(DIAG_DEBUG, target, LAYER_NW, "Freeing network layer transmission timer %p", ctx[1]);
	free(ctx[1]);

	pkg = (transport_package_t*)ctx[2];
	DIAG_PRINTF(DIAG_INFO, target, LAYER_NW, "Incoming network datagram. Handover to transport layer.");
	osi_nw2tp(target, pkg);
	DIAG_PRINTF(DIAG_DEBUG, target, LAYER_NW, "Freeing transport package %p", ctx[2]);
	transport_pkg_destroy(pkg);

	DIAG_PRINTF(DIAG_DEBUG, target, LAYER_NW, "Freeing event context %p", ctx);
	free(ctx);
}

static void osi_app_layer_onDataGenerateTick(osi_stack_t* osi_stack)
{
	application_data_node_t* dataNode;
	application_layer_t* remote_app_layer;
	char data[MAX_APP_DATA_SIZE];
	size_t i, size;
	int ticks;

	if (!osi_stack)
		return;

	size = (size_t)(rand_in_range(1, MAX_APP_DATA_SIZE + 1));
	DIAG_PRINTF(DIAG_INFO, osi_stack, LAYER_APP, "Generating app data: %Iu bytes", size);
	for (i = 0U; i < size; i++)
	{
		data[i] = (char)rand_in_range(0, 256);
	}

	remote_app_layer = osi_stack->nw_layer->remote_stack->app_layer;
	dataNode = (application_data_node_t*)malloc(sizeof(application_data_node_t));
	if (dataNode)
	{
		DIAG_PRINTF(DIAG_DEBUG, osi_stack, LAYER_APP, "Initializing app data node %p", dataNode);
		dataNode->idx = 0;
		dataNode->size = size;
		memcpy(dataNode->data, data, size);
		dataNode->next = NULL;

		if (remote_app_layer->dataLast)
			remote_app_layer->dataLast->next = dataNode;
		else
			remote_app_layer->dataFirst = dataNode;
		remote_app_layer->dataLast = dataNode;
	}

	osi_app2tp(osi_stack, data, size);
	osi_stack->app_layer->dataGenerateCount += 1;

	if (osi_stack->app_layer->dataGenerateCount < num_packets)
	{
		ticks = rand_normal_expect_stddev((int)gen_delay, 0.05 * gen_delay);
		DIAG_PRINTF(DIAG_VERBOSE, osi_stack, LAYER_APP, "Scheduling additional app data generation after %d ticks.", ticks);
		timer_set(&(osi_stack->app_layer->dataGenerateTimer), ticks, (tick_timer_callback_t)osi_app_layer_onDataGenerateTick, osi_stack);
	}
	else
	{
		DIAG_PRINTF(DIAG_INFO, osi_stack, LAYER_APP, "Data generation limit reached, not generating any more data.");
	}
}

static int shouldContinue(void)
{
	int aContinue, bContinue;
	aContinue = a_stack.app_layer->dataGenerateCount < num_packets || b_stack.app_layer->dataFirst;
	if (!bidirectional)
		return aContinue;
	bContinue = b_stack.app_layer->dataGenerateCount < num_packets || a_stack.app_layer->dataFirst;
	return aContinue || bContinue;
}

int main()
{
	int loopFlag;
	int trace_level;
	char yesNoFlag;
	char inputBuffer[128];
	unsigned int rand_seed;

	printf("-----  Reliable Transfer Protocol Simulator -------- \n\n");
	loopFlag = 0;
	do
	{
		printf("Seed random number generator? [Enter t to use the current time, or an integer value]: ");
		if (!fgets(inputBuffer, sizeof(inputBuffer), stdin))
			return EXIT_FAILURE;
		if (inputBuffer[0] == '\0' || inputBuffer[0] == '\n')
		{
			rand_seed = 0U;
			printf("No value specified, seeding random number generator with: %u\n", 0U);
			loopFlag = 1;
		}
		else if (inputBuffer[0] == 't' || inputBuffer[0] == 'T')
		{
			rand_seed = (unsigned int)time(NULL);
			printf("Random number genenator seed calculated to: %u\n", rand_seed);

			loopFlag = 1;
		}
		else if (sscanf(inputBuffer, "%u", &rand_seed))
			loopFlag = 1;

	} while (!loopFlag);

	loopFlag = 0;
	do
	{
		printf("Perform bidirectional package transmissions [y/n]: ");
		if (!fgets(inputBuffer, sizeof(inputBuffer), stdin))
			return EXIT_FAILURE;
		yesNoFlag = inputBuffer[0];
		switch (yesNoFlag)
		{
			case 'y':
			case 'Y':
				bidirectional = 1;
				loopFlag = 1;
				break;
			case 'n':
			case 'N':
				bidirectional = 0;
				loopFlag = 1;
				break;
			default:
				continue;
		}
	} while (!loopFlag);

	printf("Enter the number of messages to simulate: ");
	if (!fgets(inputBuffer, sizeof(inputBuffer), stdin))
		return EXIT_FAILURE;
	if (!sscanf(inputBuffer, "%d", &num_packets))
		num_packets = 0;

	printf("Enter packet loss probability [enter 0.0 for no loss]: ");
	if (!fgets(inputBuffer, sizeof(inputBuffer), stdin))
		return EXIT_FAILURE;
	if (!sscanf(inputBuffer, "%lf", &loss_prob))
		loss_prob = 0.0;

	printf("Enter packet corruption probability [0.0 for no corruption]: ");
	if (!fgets(inputBuffer, sizeof(inputBuffer), stdin))
		return EXIT_FAILURE;
	if (!sscanf(inputBuffer, "%lf", &corrupt_prob))
		corrupt_prob = 0.0;

	/*do
	{
		printf("Enter average time between messages from sender's application layer [ >= 0 ticks]: ");
		if (!fgets(inputBuffer, sizeof(inputBuffer), stdin))
			return EXIT_FAILURE;
		if (!sscanf(inputBuffer, "%u", &gen_delay))
			gen_delay = 0;
	} while (gen_delay < 0);*/

	do
	{
		printf("Enter average network layer transmission delay [ >= 0 ticks]: ");
		if (!fgets(inputBuffer, sizeof(inputBuffer), stdin))
			return EXIT_FAILURE;
		if (!sscanf(inputBuffer, "%u", &nw_delay))
			nw_delay = 0;
	} while (nw_delay < 0);

	printf("Enter relative network layer transmission delay variance [e.g. 0.2]: ");
	if (!fgets(inputBuffer, sizeof(inputBuffer), stdin))
		return EXIT_FAILURE;
	if (!sscanf(inputBuffer, "%lf", &nw_stddev))
		nw_stddev = 0.2;
	nw_stddev *= 0.5 * nw_delay;
	printf("Network layer transmission standard deviation calculated to %u ticks\n", (unsigned int)nw_stddev);
	double max_nw_delay = nw_delay + 2 * nw_stddev;
	gen_delay = (int)(1.03 * (max_nw_delay * (1 + (loss_prob > corrupt_prob ? loss_prob : corrupt_prob))));
	printf("Optimal average time between messages from sender's application layer calculated to: %u ticks\n", gen_delay);
	printf("Specify TRACING level:\n * 0 - CRITICAL\n * 1 - ERROR\n * 2 - WARNING\n * 3 - INFO\n * 4 - VERBOSE\n * 5 - DEBUG\nTracing level: ");
	if (!fgets(inputBuffer, sizeof(inputBuffer), stdin))
		return EXIT_FAILURE;
	if (!sscanf(inputBuffer, "%d", &trace_level))
		trace_level = 0;
	TRACE_LEVEL = (diag_trace_lvl_t)trace_level;

	puts("");

	// Initialise random number generator
	srand(rand_seed);

	DIAG_PRINTF(DIAG_INFO, &a_stack, LAYER_NONE, "Labeling endpint A as %p.", &a_stack);
	DIAG_PRINTF(DIAG_INFO, &b_stack, LAYER_NONE, "Labeling endpint B as %p.", &b_stack);

	a_stack.nw_layer = network_layer_create(&a_stack);
	a_stack.nw_layer->remote_stack = &b_stack;
	a_stack.nw_layer->transmit_callback = osi_nw_layer_onDatagramTick;
	a_stack.nw_layer->loss_prob = loss_prob;
	a_stack.nw_layer->corrupt_prob = corrupt_prob;
	a_stack.nw_layer->nw_delay = nw_delay;
	a_stack.nw_layer->nw_stddev = nw_stddev;

	b_stack.nw_layer = network_layer_create(&b_stack);
	b_stack.nw_layer->remote_stack = &a_stack;
	b_stack.nw_layer->transmit_callback = osi_nw_layer_onDatagramTick;
	b_stack.nw_layer->loss_prob = loss_prob;
	b_stack.nw_layer->corrupt_prob = corrupt_prob;
	b_stack.nw_layer->nw_delay = nw_delay;
	b_stack.nw_layer->nw_stddev = nw_stddev;

	a_stack.tp_layer = transport_layer_create(&a_stack);
	b_stack.tp_layer = transport_layer_create(&b_stack);

	a_stack.app_layer = application_layer_create(&a_stack);
	b_stack.app_layer = application_layer_create(&b_stack);

	DIAG_PRINTF(DIAG_VERBOSE, &a_stack, LAYER_NONE, "Initializing OSI stack.");
	osi_stack_init(&a_stack);
	DIAG_PRINTF(DIAG_VERBOSE, &b_stack, LAYER_NONE, "Initializing OSI stack.");
	osi_stack_init(&b_stack);

	osi_app_layer_onDataGenerateTick(&a_stack);
	if (bidirectional)
		osi_app_layer_onDataGenerateTick(&b_stack);

	while (shouldContinue())
	{
		timer_tickall();
	}

	osi_stack_teardown(&a_stack);

	application_layer_destroy(a_stack.app_layer);
	application_layer_destroy(b_stack.app_layer);

	transport_layer_destroy(a_stack.tp_layer);
	transport_layer_destroy(b_stack.tp_layer);

	network_layer_destroy(a_stack.nw_layer);
	network_layer_destroy(b_stack.nw_layer);

	return 0;
}
