/*! \mainpage Reliable Transport Protocol Network Simulator
 * \brief Welcome to the documentation pages for the RTP network simulator. These documentation pages were generated purely out of the documentation comments
 * found in the source code using the <a href="http://www.stack.nl/~dimitri/doxygen/index.html">Doxygen</a> documentation generator.
 *
 * \tableofcontents
 *
 * \section Overview
 *
 * In this assignment you are to implement the transport layer that is found between the application and the network layer in an OSI-stack networking
 * architecture. Since the transport layer is already implemented in the core of the networking stack embedded in the operating system kernel, it is rather
 * hard to actually implement the transport layer functionality for a real network connection. Because of the you are presented with the RTP network simulator
 * that simulates a network connection. This also has the added benefit that we can directly control how many packages will be lost in transmission,
 * how many packages get corrupted and how long a transmission takes.
 *
 * \section Compilation
 *
 * The source code is written entirely using the C programming language and it is expected of your that you implement the missing parts of the transport layer using
 * the appropoate function entry points in the source code.
 *
 * Since the network simulator does not depend on any external library besides the C-runtime library, this application should be compilable on most modern
 * platforms, but during development particular emphasis has been on making the code compilable on moder Linux-distributions as well as modern Windows operating systems.
 *
 * \section doc Documentation
 *
 * If you click on the <c>Files</c> button in the navigation bar on the top of this page, you will be presented with a list of files containing documentation.
 * You are welcome to look at all documentation pages, but the following pages will be the most interesting for you:
 * <list type="bullet">
 * <item><see cref="diagnostics.h"/></item>
 * <item><see cref="timer.h"/></item>
 * <item><see cref="transport_layer.h"/></item>
 * <item><see cref="transport_layer.c"/></item>
 * <item><see cref="transport_package.h"/></item>
 * <item><see cref="transport_package_impl.h"/></item>
 * </list>
 * 
 * \section where Where should I start?
 *
 * For this assignment, you are required to fully implement the functions within the <see cref="transport_layer.c"/>, <see cref="transport_package_impl.h"/> and
 * <see cref="transport_package.c"/> files. Changing any other part of the simulator should not be necessary for this assignment. You are welcome to look at the 
 * implementation of the network simulator if you like, but do not worry if you cannot make heads or tails of it, in-depth knowledge of the implementational details
 * of the rest of the network simulator is not required. Nevertheless, all functions that are available through header files in this code-base are documented
 * in these documentation pages.
 *
 * It is recommended to first start with the assumption of a perfect network environment, i.e. an environment without loss, corruption, or delay variation.
 * Gradually extend your code to deal with tranmission timeouts, corrupted packages, and out-of-sequence packages.
 *
 * The <see cref="osi_tp2app"/> and <see cref="osi_tp2nw"/> are the functions you must use to push data upwards and downwards in the OSI-stack.
 *
 * \subsection make GNU-Make
 *
 * In the <c>src</c> subfolder, you will find the source code for the application, together with a <c>Makefile</c> containing instructions for GNU-make utility
 * to compile the source code using the GCC compiler. Navigate to the <c>src</c> subfolder and execute the following command:
 * \code{.sh}
 * make
 * \endcode
 * You may also re-generate the Doxygen documentation pages on your own, if you feel the need, or employ documentation comments in your code that are recognized
 * by the doxygen generator. For that purpose invoke the special build target for the documentation in using the same Makefile.
 * \code{.sh}
 * make doc
 * \endcode
 * 
 * \subsection vs Visual Studio
 *
 * The source code also contains a solution file together with a Visual C/C++ project file. You can open up the Solution file found in the root folder of the hand-out
 * in Visual Studio. In the Menu bar select <c>Build</c> and then <c>Build Solution</c> to compile your program. Note that Visual Studio will provide you with
 * first class IntelliSense support as well as very advanced Debugging capability. It is however, up to you whether you want to make use of these features or not.
 *
 * <strong>Note</strong> that the Visual Studio Build process (MSBuild) will create a <c>bin</c> folder next to the <c>src</c> folder and place the binaries there. 
 * Visual Studio will automatically attempt to run the doxygen command to produce the documentation pages as well, 
 * and will issue a warning message in case you do not have doxygen installed.
 *
 * \section The Network simulator application
 *
 * When executed the network simulator application will ask a series of questions to set up the simulated network environment.
 *
 * \subsection srand Random Number Generator Seeding
 *
 * The C-runtime library includes a simple pseudo-random number generator. This PRNG generates random numbers based on a configurable seed value.
 * It is customary to seed the PRNG with the current time or some other relatively random number to get different random numbers each time the
 * application is run.
 *
 * In your case, however, there might be situation where you might want to replay the exact same sequence of events you encountered in a previous run. Therefore
 * you are presented with the option to enter a specific seed value that you want. Entering nothing will seed with <c>0U</c> (zero) and entering <c>t</c> will
 * seed the PRNG with the current local time on your computer. When time is used, the calculated seed value is displayed for convenience.
 *
 * \subsection message_count Number of Generated Messages
 *
 * The simulator will terminate after the specified amount of messages has been sent and received from each application layer. In a single-directional environment
 * the application layer in endpoint A will send the sepcified amount to endpoint B and B will wait to receive the specifed amount of messages. In a bi-directional
 * environment, both application layers will send the specified amount of messages to the other endpoint.
 *
 * \subsection bidirectional Bidirectional
 *
 * While you are required to implement a bi-directional communication channel, it might be easier to start with a solution that reliably transmits data
 * only in one direction. Enter <c>y</c> for yes, and <c>n</c> for no.
 *
 * \subsection loss_corrupt_probs Loss- and Corruption-probability
 *
 * The network simulator will at the network layer decide whether a package to transmit should be lost or corrupted during transmission. The simulator will
 * prompt your to specify the probability of both loss and corruption of a package. You have to implement a reliable transport protocol that can deal with both,
 * but as a start it is easier to be able to turn either loss or corruption on or off (by specifying <c>0.0</c> to turn it off).
 *
 * Note that probabilities are expressed as relative probabilities, meaning a value between <c>0.0</c> and <c>1.0</c>. E.g. A value of <c>0.1</c> will result in a
 * 10% probability, whereas <c>0.97</c> will result in a 97% probability, and <c>1.0</c> will result in certainty.
 *
 * \subsection nw_delay Average Network Transmission Delay
 *
 * Specify the mean value in ticks for how long a transmission should take on average. Whenever a transport package is transmitted on the network layer,
 * the network layer calculates the amount of time it should take for the package to be transmitted. The actual calculated value is normally distributed around
 * the mean value you specify. In essence this means that the actual transmission time will vary, but very likely be close to the average you specify.
 * If you like, you may have a look at <a href="https://en.wikipedia.org/wiki/Normal_distribution">Normal Distribution</a> for more information on normal 
 * distributions, but this is a topic best suited for your Statistics class and not of much relevance here, as this functionality already is completely implemented.
 *
 * \subsection nw_stddev Network Transmission Delay Relative Variance
 *
 * Specify the relative variance from the previously specified average value that you want in your normal distribution. The less the value you specify, the
 * less variance around your mean value you will get. If you specify <c>0.0</c>, the normal distribution will always return your specified mean value.
 *
 * If the mean value is set to <c>100</c>, specifying a relative variance of <c>0.1</c> (or 10%) will in most cases yield a random value between <c>90</c> and <c>110</c>.
 * For a mean value of <c>200</c> and a relative variance of <c>0.15</c>, the resulting values will in most cases lie between <c>170</c> and <c>230</c>.
 *
 * \subsection gen_delay Application Level Data Generation Delay
 *
 * The application layer will at random intervals decice to send more data through the network channel. As with the network transmission delay, the application
 * layer delay is also a normally distributed random value. The application calculates a value that is optimal in your situation based on the specified loss and
 * corruption probabilities as well as the network transmission delay. Please contact the TAs, if this calculation leads to problems.
 *
 * For reference, the Application Layer Data Generation Delay uses a normal distribution with the announced mean value and a relative variance of 10%.
 *
 * \subsection trace Trace Level
 *
 * You can specify the verbosity of the application. The application will only print diagnost message that on or above the level that you specify.
 *
 * Specifying the Warning level, will cause Warning, Error and Critical messages to be printed, but Informational, Verbose and Debug messages will be supressed.
 * Therefore, specifying the Debug level will always cause every diagnostic message to be printed, but you might feel that using this level will
 * unnecessarily clutter your output window.
 *
 * \section app_layer Application Layer Functionality
 *
 * A sending application layer will at normally distributed random intervals decide to generate a message containing random binary data.
 * It will then push that data down to the underlying transport layer and expects that data to be transmitted in sequence to the other endpoint.
 * For verification puposes, the application layers also share their data directly which in this case is only possible becuase both application layers
 * live inside the same process in the application and share the same memory address space, this would not be possible a real networked situation.
 *
 * A receiving application layer will receive data from its underlying transport layer as the transport layer pushes the data upwards, and the application layer
 * will expect the data to arrive in sequence. The received bytes are byte-wise compared to the expected data, and an error will be generated if they do not
 * match.
 *
 * \section nw_layer Network Layer Functionality
 *
 * The network layer will wait for the transport layer to push transport packages downwards in the OSI-stack. It will then use its configured probabilities
 * to decide whether the package should be lost or corrupted. After that, it will use its normal distribution for the network delay to decide how many ticks
 * should pass before the package arrives and the remote network layer and sets a timer accordingly.
 * 
 * When a transport package is received at the network layer, it is simply pushed upwards in the OSI-stack and handed over to the Transport Layer.
 *
 * \section timer_objects Timer Objects
 *
 * In order to make the application behave the same if the same configuration values are specified, a decision was made to write the entire code-base around
 * a tick-based system instead of an actual time-based system. All timeouts and delay values are specified as ticks.
 *
 * The main function of the network simulator, features a continous loop that for the most part only calls the <see cref="timer_tickall"/> function. Each iteration
 * through that loop can be considered as one tick.
 * 
 * This also implies that every entity in the simulator, i.e. a transport package, or a layer in one of the OSI-stacks, gets to perform exactly one operation
 * during a single tick.
 *
 * For you as an implementor, this is really good news. Dealing with time in computer science is in most cases fairly complex and hard to do deterministically.
 * This tick-based system should be fairly easy to use and is guaranteed to behave deterministically and without influence from what else you are running on your
 * computer.
 */
