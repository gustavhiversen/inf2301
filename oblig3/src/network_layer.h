/// @file
/// <summary>
/// Contains the externally visible API for the network layer in the OSI stack.<br/>
/// <strong>Students are not required to understand the details of the declarations within this file, nor the implementation within the associated <see cref="network_layer.c"/> file.</strong>
/// </summary>

#pragma once
#ifndef NETWORK_LAYER_H
/// <summary>Header inclusion macro.</summary>
#define NETWORK_LAYER_H 1

#include "osi.h"
#include "transport_package.h"

/// <summary>Pushes a received transport package from the receiving network layer to the transport layer above it in the OSI-stack.</summary>
/// <param name="osi_stack">A reference to the OSI-stack the calling network layer belongs to.</param>
/// <param name="tp_pkg">A reference to the transport package that was received by the network layer and which is pushed to the transport layer.</param>
/// <remarks>
/// If <paramref name="osi_stack"/> is <c><see cref="NULL"/></c>, this function has no effect.
/// <para>This function is implemented by the precode. <strong>Students are neither required to implement nor to understand the details of this function.</strong></para>
/// </remarks>
void osi_nw2tp(osi_stack_t* osi_stack, transport_package_t* tp_pkg);

/// <summary>Type representing the implementation of the network layer in an OSI-stack.
typedef struct network_layer network_layer_t;

/// <summary>Creates a new network layer instance for the specified OSI-stack.</summary>
/// <param name="osi_stack">A reference to the OSI stack for which the layer will be created.</param>
/// <returns>A reference to the created network layer instance. It must be deallocated with a call to <see cref="network_layer_destroy"/> when it is no longer needed.</returns>
/// <remarks>
/// <see cref="malloc"/> is used to allocate memory on the system.
/// <para>This function is implemented by the precode. <strong>Students are neither required to implement nor to understand the details of this function.</strong></para>
/// </remarks>
/// <exception cref="NULL"><paramref name="osi_stack"/> is <c><see cref="NULL"/></c>.</exception>
/// <exception cref="NULL">There is not enough memory available on the system to allocate a new network layer. <c><see cref="errno"/></c> is set to an appropiate error value.</exception>
network_layer_t* network_layer_create(osi_stack_t* osi_stack);

/// <summary>Frees the memory occupied by the specified network layer.</summary>
/// <param name="nw_layer">A reference to the network layer to destroy.</param>
/// <remarks>
/// If <paramref name="nw_layer"/> is <c><see cref="NULL"/></c>, this function has no effect.
/// <para>This function is implemented by the precode. <strong>Students are neither required to implement nor to understand the details of this function.</strong></para>
/// </remarks>
void network_layer_destroy(network_layer_t* nw_layer);

/// <summary>Initializes the specified network layer instance after all components in the OSI-stack have been created.</summary>
/// <param name="nw_layer">A reference to the network layer instance to initialize.</param>
/// <remarks>
/// If <paramref name="nw_layer"/> is <c><see cref="NULL"/></c>, this function has no effect.
/// <para>This function is implemented by the precode. <strong>Students are neither required to implement nor to understand the details of this function.</strong></para>
/// </remarks>
void network_layer_init(network_layer_t* nw_layer);

/// <summary>Called when the network layer is being torn down as part of the OSI-stack being torn down.</summary>
/// <param name="nw_layer">A reference to the network layer that is being torn down.</param>
/// <remarks>
/// If <paramref name="nw_layer"/> is <c><see cref="NULL"/></c>, this function has no effect.
/// <para>This function is implemented by the precode. <strong>Students are neither required to implement nor to understand the details of this function.</strong></para>
/// <para>Calling this function, prevents the specified network layer instance from transmitting any subsequent packages.</para>
/// </remarks>
void network_layer_onTeardown(network_layer_t* nw_layer);

/// <summary>Called when the network layer receives a transport package to transmit from the transport layer above.</summary>
/// <param name="nw_layer">A reference to the network layer that receives the data.</param>
/// <param name="tp_pkg">A reference to the transport package being sent.</param>
/// If either <paramref name="nw_layer"/> or <paramref name="tp_pkg"/> is <c><see cref="NULL"/></c>, this function has no effect.
/// <para>This function is implemented by the precode. <strong>Students are neither required to implement nor to understand the details of this function.</strong></para>
/// <para>Calling this function, prevents the specified network layer instance from transmitting any subsequent packages.</para>
/// </remarks>
void network_layer_onTpSend(network_layer_t* nw_layer, transport_package_t* tp_pkg);

#endif // !NETWORK_LAYER_H
