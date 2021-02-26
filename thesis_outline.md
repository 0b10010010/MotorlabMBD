# Master's Thesis Outline

## Abstract
  Problem statement of traditional coding practices for prototype development in contrast to the benefits of MBD approach. Include industrial practices especially in automotive and aerospace industries where hardware testing is much more difficult to do. Also what students can take away from doing MBD lab exercises. Less intricate coding practice.

## Table of Contents
  1. List of figures
  2. List of Tables
  3. Acknowledgements
  4. Nomenclature (MIL, PIL, HIL, PID, etc.).
  5. Chapters

## I. Introduction
  1. A statement of the purpose of the thesis. Abstract of MBD and it's benefits for students as well as the application in industries utilizing such development technique.
  2. Description of other chapters.
 
## II. Hardware
  1. Motor plant including the amplifier and reference schematic
  2. Microcontroller used
  3. MBD setup which is slightly different from the original device with FTDI converter
  4. Host PC equipped with MathWorks products

## III. Code Generation using MBD (Software)
  1. Overview of other options and limitations for each available software that implements MBD wihtin Simulink and MATLAB
  2. Software requirements and other programs such as ST-Link Utility (Simulink, STM32 Mat/Target, IDE, CubeMX)</br>
    2.1. Minor setup within CubeIDE such as printf float setting and virtual environment path problems
  3. Basic Simulink model configuration for running the motorlab</br>
    3.1. MATLAB system blocks: sink and source
    3.2. Peripheral setups using CubeMX
  5. Methods of MIL, SIL, PIL, and HIL and how to generate code for each</br>
    5.1. How to connect to Simulink using workaround for external mode generation (build fails when generate and connect)

## III. Modeling of plant
  1. Details of the plant (motorlab) model development
  2. Validate the frequency of the control loop is at least 10x than the crossover frequency of the plant model using MATLAB
  3. Position control
  4. Velocity control</br>
    3.1 Speed filter

## IV. Verification of HIL (Results)
  1. Initial verification using STM32 Mat/Target -> Position response compared to the origianl lab device
  2. Using external mode to tune or change any parameters

## V. Adaptations for possible Arduino or Eeva targets
  1. limitations compared to RTOS running on the original motorlab appartus
  2. Arduino robot car from ME570 for prof. Spaulding
  3. Consult Dr. Brockhoff on his opinion with MBD approach for students

## VI. Conclusions

## References

## Appendix
