/// @file
/// <summary>
/// Contains the implementation details of the OSI-stack.<br/>
/// <strong>Students are not required to understand the details of the declarations within this file, nor the implementations in the associated <see cref="osi.c"/> file.</strong>
/// </summary>

#pragma once
#ifndef OSI_IMPL_H
/// <summary>Header inclusion macro.</summary>
#define OSI_IMPL_H 1

#include "osi.h"
#include "application_layer.h"
#include "transport_layer.h"
#include "network_layer.h"

/// <summary>Implements the <see cref="osi_stack_t"/> data type.</summary>
struct osi_stack
{
	/// <summary>The application layer of the OSI-stack.</summary>
	application_layer_t* app_layer;
	/// <summary>The transport layer of the OSI-stack.</summary>
	transport_layer_t* tp_layer;
	/// <summary>The network layer of the OSI-stack.</summary>
	network_layer_t* nw_layer;
};

#endif // !OSI_IMPL_H
