# Accelerating Gustavson-based SpMM on Embedded FPGAs with Element-wise Parallelism and Access Pattern-aware Caches

Shield: [![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg

We implement and evaluate the design on the Xilinx ZCU106 platform under 100MHz. We synthesis and generate the bitstream using the Xilinx Vitis toolchain v2022.1.

+ The ***Preprocessing*** includes the python code of converting .mtx file to binaries.
+ The ***HLS*** folder includes the source code of SpMM design. 
+ The ***SDK*** folder includes the driver code for the FPGA design in the Vitis IDE.  

Currently, two cases are not supported: 1) empty rows exist; 2) the number of partial results exceed 1000. We will support these cases in the future.

For now, benchmarks we use are as follows:

```markdown
| Benchmark  | Cycles   |
|------------|----------|
| poisson3Da | 19878983 |
| raefsky1   | 27458639 |
| crystk01   | 21885313 |
| s3rmt3m3   | 8628745  |
| t2dah_a    | 5582450  |
| nasa2910   | 12797230 |
| bcsstk24   | 7588259  |
| cavity26   | 6632677  |
| ex9        | 4011290  |
| af23560    | 15408694 |
```