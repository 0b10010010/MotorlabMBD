# MotorlabMBD

# TODO:
-[] set the baudrate to 460800 on STM32CubeMX and MATLAB function reading serialport


## SIL and PIL with Simulink
### Hardware Support Packages needed:
* Embedded Coder Support Package for STMicroelectronics Discovery Boards
* Embedded Coder Support Package for ARM Cortex-M Processors
* Embedded Coder Interface to QEMU Emulator

Using QEMU with Simulation: Set the Hardware Implementation under Configuration Parameter to use QEMU as hardware. Set the Verification under Code Generation to create PIL block within Advanced parameters. Click apply and close. Set the PID controller to be discrete. Right click on the controller block -> C/C++ Code -> Deploy this Subsystem to Hardware. Click Build button on the new window. This should build the model and open a new Simulink window containing the PIL block. Setting is PIL but placing the created PIL block and runnning the simulation will bring up QEMU emulation window which emulates the board. Although it emulates ARM Cortex M3&copy;, there should not be a noticeable performance difference doing a simple PID loop.

Using PIL with STM32F4 Discovery: Similar to above QEMU but change the hardware to STM32F4 Discovery. Deploy the subsystem to hardware which then builds the new block to be placed in the Simulink model. Run the simulation which will execute OpenOCD to communicate with the Discovery board. Make sure to use FTDI USB to Serial in order to avoid bottleneck in ST-LINK (It is extremely slow with ST-LINK even for 0.5 second simulation).

### Position Control Simulation SIL and PIL VS HIL

HIL PWM TIM3 block was configured to use ARR value of only 2 (which set the PWM freq. to 14MHz). Changing the ARR value to 140 correctly sets the output freq. to 200kHz.
Also, the difference in position control of 3000 deg was induced from Simulink's PID block N coefficient which was set at 100. Disabling this coefficient produced the step response using PD control to very similar to the one from motorlabGUI.
HIL plot shows an initial delay of about 0.05 seconds



## External Mode with STM32-MAT/Target

When editing the project within IDE, make sure to change the source code used in Simulink to generate code. Simulink code generation will overwrite any changes made!

DO NOT COPY THE SIMULINK FILE AND BUILD WITH DIFFERENT IOC FILE.
IT WILL GIVE YOU THE stm32_make_rtw_hook(): error with copyModelFiles() ERROR
Also, name of .ioc file will be the name of the generated project.

In STM32CubeMX, check generate under root to avoid project creation within a project.

When STM32CubeIDE throws error stating cannot open linker script file, change the Linker Script to "Z:\path\to\linker\script\STM32F407VGTX_FLASH.ld" instead of "//mne-stokes/path/to/script/.ld". 

For testing PIL demos, use run button. Unlike external mode, demo works wth run button.

For using the external mode with STM32-MAT Target, do not deploy the Simulink model. Instead, build the model, open the IDE, then run the debugger. And then connect to the Simulink model using the external mode control panel within Simulink. External mode is over UART5(PD2,PC12). 
