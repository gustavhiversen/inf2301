/// @file
/// <summary>Contains the externally visible declarations for the transport packages abstraction.</summary>

#pragma once
#ifndef TRANSPORT_PACKAGE_H
/// <summary>Header inclusion macro.</summary>
#define TRANSPORT_PACKAGE_H 1

#include <stdlib.h>

/// <summary>Type that represents a package containing data that is to be sent over a network channel.</summary>
typedef struct transport_package transport_package_t;

/// <summary>Creates a new transport package that contains the specified data.</summary>
/// <param name="data">A generic reference to a memory buffer that is at least <paramref name="size"/> bytes long.</param>
/// <param name="size">The number of valid bytes contained within the memory buffer specified by <paramref name="data"/>.</param>
/// <returns>A reference to the created transport package, or <c><see cref="NULL"/></c> if an error occurred.</returns>
/// <remarks>
/// This function does not make a copy of the specified data.
/// <para>The returned transport package is expected to be deallocated using the <see cref="transport_pkg_destroy"/> function.</para>
/// </remarks>
/// <exception cref="NULL"><paramref name="data"/> is <c><see cref="NULL"/></c>.<br/>-<em>or</em>-<br/>There is not enough memory available on the system to allocate a new transport package. <see cref="errno"/> is set to an appropiate error value.</exception>
transport_package_t* transport_pkg_create(void* data, size_t size, int hash, int id);

/// <summary>Frees all memory occupeid by the specified transport package.</summary>
/// <param name="tp_pkg">A reference to the transport package to destroy.</param>
/// <remarks>
/// If <paramref name="tp_pkg"/> is <c><see cref="NULL"/></c> this function has no effect.
/// <para>If the specified transport package is a copied transport package that was obtained through a call to <see cref="transport_pkg_copy"/>, this function will also automatically free the copy of the payload data.
/// </remarks>
void transport_pkg_destroy(transport_package_t* tp_pkg);

/// <summary>Creates a complete copy of the specified transport package.</summary>
/// <param name="src"></param>
/// <returns>A reference to a newly created transport package that is an exact copy of <paramref name="src"/>, 
/// or <c><see cref="NULL"/></c> if an error occurred.</returns>
/// <remarks>
/// The returned copy of <paramref name="src"/> is placed into one continuous memeory buffer containing both the transport package structure
/// as well as a copy of the payload data contained within <paramref name="src"/>. This allows all copied data to be deallocated as a single operation.
/// <para>The returned transport package is expected to be deallocated using the <see cref="transport_pkg_destroy"/> function.
/// </remarks>
/// <exception cref="NULL"><paramref name="src"/> is <c><see cref="NULL"/></c>.<br/>-<em>or</em>-<br/>There is not enough memory available on the system to allocate a new transport package. <see cref="errno"/> is set to an appropiate error value.</exception>
transport_package_t* transport_pkg_copy(transport_package_t* src);

/// <summary>Gets the size of the payload contained within a transport package.</summary>
/// <param name="pkg">A reference to the transport package to get the size from.</param>
/// <returns>The number of bytes of the payload data contained within <paramref name="pkg"/>.</returns>
/// <remarks>Returns <c>0U</c> (zero) if <paramref name="pkg"/> is <c><see cref="NULL"/></c>.</remarks>
size_t transport_pkg_size(transport_package_t* pkg);

/// <summary>Gets a reference to the payload data contained within a transport package.</summary>
/// <param name="pkg">A reference to the transport package to get the data from.</param>
/// <returns>A generic reference to the payload data contained within <paramref name="pkg"/>.</returns>
/// <remarks>Returns <c><see cref="NULL"/></c> if <paramref name="pkg"/> is <c><see cref="NULL"/></c>.</remarks>
void* transport_pkg_data(transport_package_t* pkg);

#endif // !TRANSPORT_PACKAGE_H
