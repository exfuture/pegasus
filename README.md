pegasus
=======

Powerful digital telecommunication channels simulator.

Description
-----------

pegasus helps researcher to evaluate bit error rate and symbol error rate in case of different modulations, channel types and forward error correction codes.

pegasus supports the following types of modulation:

* ASK
* FSK
* BPSK
* QPSK
* 8PSK
* 16PSK
* 32PSK
* 64PSK
* 256PSK
* 1024PSK
* 16QAM
* 32QAM (with non-optimal Gray-coding scheme)
* 64QAM
* 256QAM
* 1024QAM

Also the following FEC codes are supported:

* Hamming (7, 4)
* CRC (8, 5)
* BCH (15, 5, 7)
* BCH (15, 7, 5)

There are also several channel models:

* AWGN
* Rayleigh

Compiling
---------

### Prerequisites

* cmake (tested with 2.8.12.2)
* make (tested with GNU Make 4.0)
* one of the following compilers:
	* gcc (tested with 4.8.2)
	* clang (tested with 3.4)
	* icc (tested with 14.0.2)

Also it's OK to have `tcmalloc_minimal` library from
[Google Performance Tools](https://code.google.com/p/gperftools/)
or [jemalloc](http://www.canonware.com/jemalloc/) installed
as they ensure the simulation to take less time than with standard
malloc().

If gcc or icc is used, OpenMP support is activated automatically.

### Compiling

In source tree root create `build` folder, enter it and type
the following command to generate cmake files:

`cmake -DCMAKE_C_COMPILER=/usr/bin/cc ..`

Replace `/usr/bin/cc` to the desired compiler. As a rule, you
don't have to specify compiler at all to use gcc compiler.

Then type `make`. After finishing `pegasus` executable should be
produced.

Usage
-----

Running pegasus without arguments ensures the following simulation parameters:

* random source
* ASK
* no FEC
* AWGN channel
* 200 000 iterations per each step
* h<sup>2</sup> (or E<sub>s</sub>/N<sub>0</sub>) range from 0 to 15 decibels with 0.5 step
* BER evaluation

However, it's possible to change everything with following arguments:

* --source=&lt;source type&gt; — specifies information source type:
	* random — completely random bits
	* predefined — linear sequences

* --fec=&lt;fec code name&gt; — specifies FEC code:
	* none — no FEC applies
	* hamming74 — Hamming (7, 4) code is used
	* bch1557 — BCH (15, 5, 7) code is used
	* bch1575 — BCH (15, 7, 5) code is used

* --modulation=&lt;modulation type&gt; — desired modulation to use in channel:
	* ask — amplitude-shift keying
	* fsk — frequency-shift keying
	* bpsk — binary phase-shift keying
	* qpsk — phase-shift keying with 2 bits per symbol
	* 8psk — phase-shift keying with 3 bits per symbol
	* 16psk — phase-shift keying with 4 bits per symbol
	* 32psk — phase-shift keying with 5 bits per symbol
	* 64psk — phase-shift keying with 6 bits per symbol
	* 256psk — phase-shift keying with 8 bits per symbol
	* 1024psk — phase-shift keying with 10 bits per symbol
	* 16qam — quadrature-amplitude modulation with 4 bits per symbol
	* 32qam — quadrature-amplitude modulation with 5 bits per symbol
	* 64qam — quadrature-amplitude modulation with 6 bits per symbol
	* 256qam — quadrature-amplitude modulation with 8 bits per symbol
	* 1024qam — quadrature-amplitude modulation with 10 bits per symbol

* --channel=&lt;channel type&gt; — channel model to use:
	* awgn — with additive white Gaussian noise
	* rayleigh — with Rayleigh fading

* --error=&lt;error type&gt; — error type to evaluate:
	* ber — bit error rate
	* ser — symbol error rate

* --hsquare-start=&lt;value&gt; — E<sub>s</sub>/N<sub>0</sub> start value

* --hsquare-end=&lt;value&gt; — E<sub>s</sub>/N<sub>0</sub> end value

* --hsquare-step=&lt;value&gt; — E<sub>s</sub>/N<sub>0</sub> value increment step

* --units=&lt;measurement units&gt; — units of E<sub>s</sub>/N<sub>0</sub> value:
	* dbs — decibels
	* times — no comments

* --iterations=&lt;value&gt; — number of iterations per each step

* --quiet — do not produce additional information about simulation

* --self-test — perform various self-tests

Typical usage is shown below:

`./pegasus --modulation=16psk --fec=bch1575`

Distribution and Contribution
-----------------------------

pegasus is provided under terms and conditions of GPLv3.
See file "COPYING" for details.
Mail any suggestions, bugreports and comments to me: oleksandr@natalenko.name.
