1. "The Vector Calculator program is designed for calculations based on 
amplitude and phase values that were measured by a vibration analyzer. 

To use the calculator, you need to run: ./vc
Then sequentially enter the start amplitude and phase values, 
and then the final amplitude and phase values, for example:

217 24 41 316

The program will calculate the sub | div | add | mul of amplitudes (weight)
and phases, then output the result:

sub_amp_phase | div_amp_phase | add_amp_phase | mul_amp_phase
205.19 34.68 | 5.29 68.00 | 235.45 14.71 | 8897.00 340.00

2. In practice, for balancing calculations, you need sub 
and div values, which are calculated following the algorithm.

If you run the program with the 'b' option: ./vc -b
And then after entering the start and final amplitude and phase values,
add the values of the test weight (in grams) and its 
installation phase (angle), for example:

217 24 41 316 310 288

The program will calculate the dynamic impact factor, as well as 
the weight (in grams) and phase (angle) of the installation 
of the correcting weight for single-plane balancing with 
the removal of the test weight:

                    weight | phase
dynamic impact factor: 0.66 86.71
correction weight:     327.84 297.29

3. The 't' option is provided for testing the program, which used 
by the script (./testvc.sh) with test values.

4. The 'h' option is intended for reference: ./vc -h"
