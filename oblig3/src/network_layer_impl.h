/// @file
/// <summary>
/// Contains the implementation details of the network layer in the OSI-stack.<br/>
/// <strong>Student are not required to understand the details of the declarations within this file, nor within the associated implementation in the <see cref="network_layer.c"/> file.</strong>
/// </summary>

#pragma once
#ifndef NETWORK_LAYER_IMPL_H
/// <summary>Header inclusion macro.</summary>
#define NETWORK_LAYER_IMPL_H 1

#include "network_layer.h"

/// <summary>Implements the <see cref="network_layer_t"/> data type.</summary>
struct network_layer
{
	/// <summary>A reference to the OSI-stack this network layer is part of.</summary>
	osi_stack_t* osi_stack;

	/// <summary>A reference to the OSI-stack that represents the remote endpoint this network layer is connected to.</summary>
	/// <value>
	/// The value of this field is either a reference to the OSI-stack on the other side of the communication channel, 
	/// or <c><see cref="NULL"/></c> if this network layer instance is not connected to any remote endpoint.
	/// </value>
	osi_stack_t* remote_stack;

	/// <summary>A reference to the callback function to call whenever a transport package is to be transmitted over the communcation channel.</summary>
	/// <value>
	/// A reference to a function that has no return value and takes an array-reference with 3 elements, where each element is a generic reference.
	/// <para>This field has a value of <c><see cref="NULL"/></c> if the network layer is not connected or if the connection has been torn down.</para>
	/// <para>
	/// The callback function will receive the following values in its <em>ctx</em> argument:
	/// <list type="table">
	/// <listheader><term>Index</term><description>Details</description></listheader>
	/// <item><term><c><em>ctx</em>[0]</c><br/><see cref="osi_stack_t"/>*</term><description>An <see cref="ost_stack_t"/> reference representing the destination OSI-stack.</description></item>
	/// <item><term><c><em>ctx</em>[1]</c><br/><see cref="tick_timer_t"/>*</term><description>A <see cref="tick_timer_t"/> reference to the timer that expired for the callback to be triggered.</description></item>
	/// <item><term><c><em>ctx</em>[2]</c><br/><see cref="transport_package_t"/>*</term><description>A <see cref="transport_package_t"/> reference to the transport package that is transmitted.</description></item>
	/// </list> The callback function is responsible for deallocating <c><em>ctx</em>[1]</c> using the <see cref="free"/> function,
	/// <c><em>ctx</em>[2]</c> using the <see cref="transport_pkg_destroy"/> function, as well as the actual <c><em>ctx</em></c> reference using the <see cref="free"/> function.
	/// </para>
	/// </value>
	/// <seealso cref="tick_timer_callback_t"/>
	void(*transmit_callback)(void* ctx[3]);

	/// <summary>Specifies the probability that the network layer instance will loose a package during transmission.</summary>
	/// <value>A 32-bit floating point number between <c>0.0f</c> and <c>1.0f</c>, where <c>0.0f</c> specifies no package loss
	/// and <c>1.0f</c> specifies certain package loss.</value>
	double loss_prob;
	/// <summary>Specifies the probability that the network layer instance will corrupt a package during transmission.</summary>
	/// <value>A 32-bit floating point number between <c>0.0f</c> and <c>1.0f</c>, where <c>0.0f</c> specifies no corruption
	/// and <c>1.0f</c> specifies certain data corruption.</value>
	double corrupt_prob;
	/// <summary>The mean package transmission delay of this network layer instance.</summary>
	int nw_delay;
	/// <summary>The standard deviation for the transmission delay.</summary>
	double nw_stddev;
};

#endif // !NETWORK_LAYER_IMPL_H
