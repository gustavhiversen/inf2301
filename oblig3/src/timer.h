/// @file
/// <summary>Contains the API declarations for the timer objects.</summary>
/// <remarks>Students will feel the need to include this header in their transport layer implementation.</remarks>

#pragma once
#ifndef TIMER_H
/// <summary>Header inclusion macro.</summary>
#define TIMER_H 1

/// <summary>The type representing a callback function that is invoked when a timer object reaches its targeted number of ticks.</summary>
/// <param name="ctx">
/// A generic reference to an object representing the context of the expired timer.
/// <para>The value of this parameter will be equal to the value of the <em>ctx</em> argument passed on to the original call to the <see cref="timer_set"/> function.</para>
/// </param>
/// <remarks>
/// It is up to the implementation of the function whose signature matches the <see cref="tick_timer_callback_t"/> to provide meaning to the <paramref name="ctx"/> parameter.
/// Explicit casts to a more specific reference type will be necessary in most cases.
/// <para>If the value of <paramref name="ctx"/> is to be used in the callback, the caller of the orignal call to the <see cref="timer_set"/> function will need 
/// to ensure that the memory location referred to by <paramref name="ctx"/> is still valid when the callback function is invoked.</para>
/// <para>Often it will be necessary that the callback function will perform cleanup actions for the <paramref name="ctx"/> parameter value.</para>
/// </remarks>
typedef void(*tick_timer_callback_t)(void* ctx);

/// <summary>Type representing a tick-timer object.</summary>
/// <remarks>
/// A timer object has expired when its <see cref="tick_count"/> value has become equal to or greater than its <see cref="target_ticks"/> value.
/// </remarks>
typedef struct tick_timer
{
	/// <summary>The number of ticks that the timer object has ticked since it has been set.</summary>
	/// <value>A value between <c>0</c> (zero) and <see cref="target_ticks"/>.</value>
	/// <remarks>
	/// A timer object has expired when its <see cref="tick_count"/> value has become equal to or greater than its <see cref="target_ticks"/> value.
	/// </remarks>
	int tick_count;
	/// <summary>The targeted number of ticks that is required for this timer to expire.</summary>
	/// <value>A 32-bit signed integer value greater than <c>0</c> (zero).</value>
	/// <remarks>
	/// A timer object has expired when its <see cref="tick_count"/> value has become equal to or greater than its <see cref="target_ticks"/> value.
	/// <para>Setting this field to <c>0</c> (zero) or less, will effectively cause the timer object to expire immediately.</para>
	/// </remarks>
	int target_ticks;
	/// <summary>The callback function to invoke when the timer object expires.</summary>
	/// <value>A reference to the callback function that is invoked when the timer object expires, or <c>NULL</c> if no callback is specified.</value>
	/// <remarks>The callback function is invoked with the <em>ctx</em> set to the <see cref="ctx"/> field value of the timer object that expired.</remarks>
	tick_timer_callback_t callback;
	/// <summary>The context reference for the timer object.</summary>
	/// <value>A generic reference that is passed on to the <see cref="callback"/> function when the timer object expires.</value>
	/// <remarks>It is up to the implementation of the <see cref="callback"/> function to decide the actual value of this reference.</remarks>
	void* ctx;
} tick_timer_t;

/// <summary>Sets the specified timer object and starts its countdown towards expiration.</summary>
/// <param name="timer">A reference to the timer object to set.</param>
/// <param name="ticks">The number of ticks until the timer object should expire.</param>
/// <param name="callback">A reference to the callback function to invoke when the timer object expires.</param>
/// <param name="ctx">A generic reference to pass on to the <paramref name="callback"/> function when the timer object expires.</param>
/// <remarks>
/// If <paramref name="timer"/> is <c><see cref="NULL"/></c>, this function has no effect.
/// <para><paramref name="ticks"/> should be a positive value. Specifying a value of <c>0</c> (zero) for the <paramref name="ticks"/> parameter will cause the timer object to expire immediately.</para>
/// <para>The meaning of the <paramref name="ctx"/> parameter value is definied by the specific implementational details of the specified <paramref name="callback"/> function.</para>
/// <para>If the caller specifies <c><see cref="NULL"/></c> as a value for <paramref name="callback"/>, the timer objects will still expire after <paramref name="ticks"/> ticks, but no callback function will be invoked.</para>
/// <para>This function overwrites all values of the specified <paramref name="timer"/> object. The <see cref="tick_timer_t::tick_count"/> is reset to <c>0</c>, and the other fields are set to the their respective parameter values passed as arguments to this function.</para>
/// </remarks>
void timer_set(tick_timer_t* timer, int ticks, tick_timer_callback_t callback, void* ctx);

/// <summary>Unsets the specified timer objects and removes it from the list of active timer objects.</summary>
/// <param name="timer">A reference to the timer to remove from the list of active timer objects.</param>
/// <returns>The number of times <paramref name="timer"/> was found and removed in the list of active timer objects.</returns>
/// <remarks>If this function returns <c>0</c> (zero), the specified timer was not set and could therefore not be removed.</remarks>
int timer_unset(tick_timer_t* timer);

/// <summary>Advances all active timer objects one tick ahead and invokes the callback function for expired timers.</summary>
/// <returns>The number of timer objects that expired during the function call.</returns>
int timer_tickall(void);

#endif // !TIMER_H
