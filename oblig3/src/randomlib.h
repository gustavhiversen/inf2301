/// @file
/// <summary>Contains the API declarations for advanced random number generation.</summary>

#pragma once
#ifndef RANDOMLIB_H
/// <summary>Header inclusion macro.</summary>
#define RANDOMLIB_H 1

/// <summary>Generates a uniformly distributed random value between <c>0.0</c> (zero) and <c>1.0</c> (inclusive).</summary>
/// <returns>A <see cref="double"/> floating point value between and including <c>0.0</c> (zero) and <c>1.0</c>.</returns>
/// <remarks>This function makes use of the <see cref="rand"/> function in the C-runtime library. <see cref="srand"/> may be used to seed the random number generator.</remarks>
double rand_0_1();

/// <summary>Generates a uniformly distributed random value within the given range.</summary>
/// <param name="lower">The lower inclusive bounds of the range.</param>
/// <param name="upper">The upper exclusive bounds of the range.</param>
/// <returns>A randomized integer value whose value is within the interval starting from and including <paramref name="lower"/> and ranging up to but excluding <paramref name="upper"/>.</returns>
/// <remarks>This function makes use of the <see cref="rand"/> function in the C-runtime library. <see cref="srand"/> may be used to seed the random number generator.</remarks>
int rand_in_range(int lower, int upper);

/// <summary>Generates a normally distributed random value value centered around <c>0.0</c> (zero).</summary>
/// <returns>A <see cref="double"/> floating point value.</returns>
/// <remarks>
/// The default random number generator included in the C-runtime library generates uniformly distributed random values.
/// The Box-Muller transformation algorthim is used to transform these uniformly distributed values to a normal distribution.
/// <para>This function alternates between calling the Cosinus and the Sinus variant of the Box-Muller Transformation.</para>
/// <para>This function makes use of the <see cref="rand"/> function in the C-runtime library. <see cref="srand"/> may be used to seed the random number generator.</para>
/// <para>This function generates the deafult normal distribution. I.e. a normal distribution with a mean value of <c>0</c> (zero) and a standard deviation of <c>1.0</c>.</para>
/// </remarks>
/// <seealso cref="rand_normal_boxmuller_cos"/>
/// <seealso cref="rand_normal_boxmuller_sin"/>
double rand_normal_boxmuller();

/// <summary>Generates a normally distributed random value value centered around <c>0.0</c> (zero).</summary>
/// <returns>A <see cref="double"/> floating point value.</returns>
/// <remarks>
/// The default random number generator included in the C-runtime library generates uniformly distributed random values.
/// The Box-Muller transformation algorthim is used to transform these uniformly distributed values to a normal distribution.
/// <para>This function uses the Cosinus variant of the Box-Muller Transformation.</para>
/// <para>This function makes use of the <see cref="rand"/> function in the C-runtime library. <see cref="srand"/> may be used to seed the random number generator.</para>
/// <para>This function generates the deafult normal distribution. I.e. a normal distribution with a mean value of <c>0</c> (zero) and a standard deviation of <c>1.0</c>.</para>
/// </remarks>
/// <seealso cref="rand_normal_boxmuller"/>
/// <seealso cref="rand_normal_boxmuller_sin"/>
double rand_normal_boxmuller_cos();

/// <summary>Generates a normally distributed random value value centered around <c>0.0</c> (zero).</summary>
/// <returns>A <see cref="double"/> floating point value.</returns>
/// <remarks>
/// The default random number generator included in the C-runtime library generates uniformly distributed random values.
/// The Box-Muller transformation algorthim is used to transform these uniformly distributed values to a normal distribution.
/// <para>This function uses the Sinus variant of the Box-Muller Transformation.</para>
/// <para>This function makes use of the <see cref="rand"/> function in the C-runtime library. <see cref="srand"/> may be used to seed the random number generator.</para>
/// <para>This function generates the deafult normal distribution. I.e. a normal distribution with a mean value of <c>0</c> (zero) and a standard deviation of <c>1.0</c>.</para>
/// </remarks>
/// <seealso cref="rand_normal_boxmuller"/>
/// <seealso cref="rand_normal_boxmuller_cos"/>
double rand_normal_boxmuller_sin();

/// <summary>Generates a normally distributed random value centered around the specified mean with the specified standard deviation.</summary>
/// <param name="expectation">The mean or expectation value.</param>
/// <param name="stddev">The standard deviation from the mean value as a <see cref="double"/> floating point number.</param> 
/// <returns>A normally distributed random integer value from a normal distribution with a mean value of <paramref name="expectation"/> and a standard deviation of <paramref name="stddev"/>.</returns>
/// <remarks>
/// The default random number generator included in the C-runtime library generates uniformly distributed random values.
/// The Box-Muller transformation algorthim is used to transform these uniformly distributed values to a normal distribution.
/// <para>This function makes use of the <see cref="rand"/> function in the C-runtime library. <see cref="srand"/> may be used to seed the random number generator.</para>
/// </remarks>
/// <seealso cref="rand_normal_boxmuller"/>
int rand_normal_expect_stddev(int expectation, double stddev);

#endif // !RANDOMLIB_H

