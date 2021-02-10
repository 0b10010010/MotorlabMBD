# MotorlabMBD

When editing the project within IDE, make sure to change the source code used in Simulink to generate code. Simulink code generation will overwrite any changes made!

DO NOT COPY THE SIMULINK FILE AND BUILD WITH DIFFERENT IOC FILE.
IT WILL GIVE YOU THE stm32_make_rtw_hook(): error with copyModelFiles() ERROR

For testing PIL demos, use run button. Unlike external mode, demo works wth run button.

For using the external mode with STM32-MAT Target, do not deploy the Simulink model. Instead, build the model, open the IDE, then run the debugger. And then connect to the Simulink model using the external mode control panel within Simulink.




Using QEMU with Simulation: Set the Hardware Implementation under Configuration Parameter to use QEMU as hardware. Set the Verification under Code Generation to create PIL block within Advanced parameters. Click apply and close. Set the PID controller to be discrete. Right click on the controller block -> C/C++ Code -> Deploy this Subsystem to Hardware. This should build the model and open a new Simulink window containing the PIL block. Setting is PIL but placing the created PIL block and runnning the simulation will bring up QEMU emulation window which emulates the board. Although it emulates ARM Cortex M3&copy;, there should not be a noticeable performance difference doing a simple PID loop.
