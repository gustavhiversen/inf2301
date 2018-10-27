/// @file
/// <summary>
/// Contains the externally visible API of the OSI-stack.<br/>
/// <strong>Students are not required to understand the details of the declarations within this file, nor the implementations in the associated <see cref="osi.c"/> file.</strong>
/// </summary>

#pragma once
#ifndef OSI_H
/// <summary>Header inclusion macro.</summary>
#define OSI_H 1

/// <summary>Type representing of the network OSI-stack.</summary>
typedef struct osi_stack osi_stack_t;

/// <summary>Initializes the specified OSI-stack.</summary>
/// <param name="osi_stack">A reference to the OSI-stack to initialize.</param>
/// <remarks>This function will initialize the different layers of the OSI-stack.</remarks>
void osi_stack_init(osi_stack_t* osi_stack);

/// <summary>Tears down the specified network OSI-stack.</summary>
/// <param name="osi_stack">A reference to the OSI-stack to tear down.</param>
/// <remarks>This function will cause the different layers of the OSI-stack to be torn down.</remarks>
void osi_stack_teardown(osi_stack_t* osi_stack);

#endif // !OSI_H
