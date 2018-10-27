#include <stdlib.h>

#include "timer.h"

/// <summary>Type representing a node within the linked list of timer objects within this application.</summary>
/// <remarks>This type definition is not part of the externally visible timer object API.</remarks>
typedef struct timer_node timer_node_t;

/// <summary>Data structure implementing the <see cref="timer_node_t"/> type.</summary>
/// <remarks>This type definition is not part of the externally visible timer object API.</remarks>
struct timer_node
{
	/// <summary>A reference to the <see cref="tick_timer_t"/> reference stored within the node.</summary>
	tick_timer_t* value;
	/// <summary>A reference to the node that follows the node in the linked list of timer objects.</summary>
	timer_node_t* next;
};

static timer_node_t* timer_lst_head = NULL;

void timer_set(tick_timer_t * timer, int ticks, tick_timer_callback_t callback, void * ctx)
{
	timer_node_t* nd;
	timer_node_t* prev;
	timer_node_t* curr;

	if (!timer)
		return;

	timer->tick_count = 0;
	timer->target_ticks = ticks;
	timer->callback = callback;
	timer->ctx = ctx;

	for (prev = NULL, curr = timer_lst_head; curr; prev = curr, curr = curr->next)
	{
		if (curr->value == timer) // Specified timer already set and active
			return;
	}

	nd = (timer_node_t*)malloc(sizeof(timer_node_t));
	if (!nd)
		return;

	nd->value = timer;
	nd->next = NULL;

	if (prev)
		prev->next = nd;
	else
		timer_lst_head = nd;
}

int timer_unset(tick_timer_t* timer)
{
	int count = 0;
	timer_node_t* prev;
	timer_node_t* curr;
	tick_timer_t* t;

	for (prev = NULL, curr = timer_lst_head; curr; prev = curr, curr = curr->next)
	{
		t = curr->value;
		if (t != timer)
			continue;
		count++;

		if (prev)
			prev->next = curr->next;
		else if (curr == timer_lst_head)
			timer_lst_head = curr->next;
		free(curr);
		curr = prev;

		if (!curr)
			break;
	}
	return count;
}

int timer_tickall(void)
{
	int count = 0;
	timer_node_t* prev;
	timer_node_t* curr;
	tick_timer_t* t;

	for (prev = NULL, curr = timer_lst_head; curr; prev = curr, curr = curr->next)
	{
		t = curr->value;
		if (!t)
			continue;

		t->tick_count += 1;
		if (t->tick_count < t->target_ticks)
			continue;
		count++;

		if (prev)
			prev->next = curr->next;
		else if (curr == timer_lst_head)
			timer_lst_head = curr->next;
		free(curr);
		curr = prev;

		if (t->callback)
			t->callback(t->ctx);

		if (!curr)
			break;
	}

	return count;
}
