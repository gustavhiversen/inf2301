/// @file
/// <summary>
/// Contains the diagnostic API to provide diagnostic printing to the standard output stream.<br/>
/// <strong>Students are not required to understand the implementation details of these utilities.</strong>
/// </summary>
/// <remarks>
/// The declarations within this file provide developers with a utility function <see cref="DIAG_PRINTF"/>
/// that provides a pre-formatted <see cref="printf"/> call with optional severity masking.
/// </remarks>

#pragma once
#ifndef DIAGNOSTICS_H
/// <summary>Header inclusion macro.</summary>
#define DIAGNOSTICS_H 1

#include "osi.h"

/// <summary>Enumeration type representing the Trace level (aka. the severity) of diagnostic message</summary>
/// <remarks>
/// The enumeration value increases the more trivial a diagnostic message becomes. To filter out certain messages, 
/// one checks whether the severity of the diagnostic message is less than or equal to the maximum output level.
/// </remarks>
typedef enum diag_trace_lvl
{
	/// <summary>Critical, non-recoverable Severity, aka. Fatal severity.</summary>
	DIAG_CRITICAL = 0,
	/// <summary>Error Severity</summary>
	DIAG_ERROR = 1,
	/// <summary>Warning Severity</summary>
	DIAG_WARNING = 2,
	/// <summary>Informational Severity</summary>
	DIAG_INFO = 3,
	/// <summary>Verbose Severity</summary>
	DIAG_VERBOSE = 4,
	/// <summary>Debugging Severity</summary>
	DIAG_DEBUG = 5
} diag_trace_lvl_t;

/// <summary>Enumeration type representing the source of a diagnostic message.</summary>
typedef enum diag_trace_src
{
	/// <summary>The source of the diagnostic message is unknown, or otherwise not specified.</summary>
	LAYER_NONE = 0,
	/// <summary>The diagnostic message was triggered in the Network layer of the application.</summary>
	LAYER_NW = 3,
	/// <summary>The diagnostic message was triggered in the Transport layer of the application.</summary>
	LAYER_TP = 4,
	/// <summary>The diagnostic message was triggered in the Application layer of the application.</summary>
	LAYER_APP = 5
} diag_trace_src_t;

/// <summary>Application-global variable that specifies the maximum severity level of printed diagnostic messages.</summary>
/// <remarks>
/// The <see cref="DIAG_PRINTF"/> function is designed in such a way, that diagnostic messages whose severity value 
/// is higher than the <see cref="TRACE_LEVEL"/> global value will not be printed to the output stream.
/// </remarks>
extern diag_trace_lvl_t TRACE_LEVEL;

/// <summary>Submits a pre-formatted <see cref="printf"/> call to the tracing system. The <see cref="TRACE_LEVEL"/> value decides whether the message is printed or not.</summary>
/// <param name="severity">A <see cref="diag_trace_lvl_t"/> enumeration value specifying the severity of the message.</param>
/// <param name="osi_stack">A reference to the <see cref="ost_stack_t"/> that triggered the diagnostic message. This parameter can be <see cref="NULL"/>.</param>
/// <param name="layer">A <see cref="diag_trace_src_t"/> enumeration value specifying the source of the diagnostic message.</param>
/// <param name="fmt">A <a href="https://msdn.microsoft.com/en-us/library/wc7014hz.aspx">printf</a>-like format string specifying the diagnostic message. This parameter is passed as a null-terminated string of single-byte characters. The value must not be <see cref="NULL"/>.</param>
/// <returns>
/// <c>0</c> (zero) if the printing of the message was skipped because of the <paramref name="severity"/> value; otherwise, 
/// the number of characters that were printed to the standard output stream.
/// </returns>
/// <remarks>
/// The <see cref="DIAG_PRINTF"/> function first evaluates whether the <paramref name="severity"/> against the value of the global
/// <see cref="TRACE_LEVEL"/> variable value. If <paramref name="severity"/> is greater than <see cref="TRACE_LEVEL"/> the diagnostic
/// message will not be printed to the standard output stream. Only if it is below or equal, the message will be printed.
/// </remarks>
int DIAG_PRINTF(diag_trace_lvl_t severity, osi_stack_t* osi_stack, diag_trace_src_t layer, const char fmt[], ...);

#endif // !DIAGNOSTICS_H
