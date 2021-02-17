# MotorlabMBD
This readme is for my list of features to complete and to keep track of bugs, workarounds, and etc.

# TODO

  &#10060; Try generating code as C++ by replacing the source code to C++ and setting the code generation to C++</br>
  
  <h2>STM32:</h2>
  
  &#9989; set objects as global to add Watch Expressions during debug</br>
  &#10060; when the motor is turned manually while debugging and when it slips, encoder shows the correct 200 deg. <- troubleshoot</br>
  &#10060; set the baudrate to 460800 on STM32CubeMX and MATLAB function reading serialport</br>
  &#10060; setup ADC with DMA to output current sensing from the amp to the buffer on STM32F4</br>
  &#9989; check if position control of 200 deg works without the UART TX block</br>
    &emsp; &#8627; Did not work. Motor turns to about 30 deg and just sits there</br>

  <h2>MATLAB:</h2>
  
  &#9989; use <code>data = uint8(fread(this.serialPort,totNumBytes)');</code> <- See `GloTalkClass.m`</br>
  &#9989; create callback function to call when 2048 bytes of data is received from the &mu; and plot it</br>
  &#9989; create a new Simulink diagram for Simulink_Motorlab to remove PIL bug</br>
    &emsp; &#8627; Did not work. PIL config window bug persists</br>
  &#9989; change <code>\inc</code> and <code>\src</code> directories in block source m files</br>
  &#10060; for MATLAB system blocks that need variable sample time, such as the sampling freq for data, edit the source m files to use <code>createSampleTime</code> and <code>setNumTicksUnitlNextHit</code>. See <a href="https://www.mathworks.com/help/simulink/ug/single-rate-sample-time-matlab-system-block.html">Here</a></br>

  <h2>Simulink:</h2>
  
  &#10060; Convert to multi-tasking and multi-rate diagram</br>
  &#10060; give the encoder signal priority in order to get the data from zero</br>
  &#9989; implement 2nd order LPF for speed control</br>
  &#9989; switch in the dashboard to alternate between position and speed control</br>
    &emsp; &#8627; Dashboard blocks do not work with real time external mode. Simulink Real Time Desktop toolbox is required</br>
  &#9989; amp switch is connected to TX block so that turning on the amp will start sending data back to host PC to plot</br>
  &#9989; displays to show pos and vel data</br>
    &emsp; &#8627; Did not work. Same reason as Simulink Real Time Desktop toolbox is not available</br>
  &#9989; edit encoder block to be able to set it back to zero</br>
  &#9989; edit HIL Simulink model to send the data (all 2048 by n fields containing position, velocity, etc.) back to host PC when a button is pressed.</br>

## MATLAB Data Handle
When the Run Wave Autosave button is used to collect the whole data, motorlabGUI `runAutosaveButton_Callback` function sends the &mu;controller parameter object to start Autosave. Then the gui waits for the &mu;controller which sends a single instance at a time, total of 2048 times. Each instance contains all 9 floatval data. See function `receiveAllObjects` in `GloTalkClass.m`. `totNumBytes` is `(numBytes+9)*numInstance` where `numBytes` is the `sizeof(logDataDataType)`, `+9` is the other bytes in the buffer: start (+3), objID (+1), instance (+2), numBytes (+1), endBytes (+2), and `numInstance` is 2048 for the `logDataDataType`. 

For the status update in motorlabGUI.m to display the status of the motor, such as the position at an instance, the function `getObject(handles.status,1)` is used to get a single instance. This instance contains 9 floatvals of status of the motor. If the instance is 0 it is a request for all instances which is 2049.

## SIL and PIL with Simulink
### Hardware Support Packages needed:
* `Embedded Coder Support Package for STMicroelectronics Discovery Boards`
* `Embedded Coder Support Package for ARM Cortex-M Processors`
* `Embedded Coder Interface to QEMU Emulator`</br>
  &#8627; In my case I had to run MATLAB as admin in order to successfully install the package.

Using QEMU with Simulation: Set the Hardware Implementation under Configuration Parameter to use QEMU as hardware. Set the Verification under Code Generation to create PIL block within Advanced parameters. Click apply and close. Set the PID controller to be discrete. Right click on the controller block -> C/C++ Code -> Deploy this Subsystem to Hardware. Click Build button on the new window. This should build the model and open a new Simulink window containing the PIL block. Setting is PIL but placing the created PIL block and runnning the simulation will bring up QEMU emulation window which emulates the board. Although it emulates ARM Cortex M3&copy;, there should not be a noticeable performance difference doing a simple PID loop.

Using PIL with STM32F4 Discovery: Similar to above QEMU but change the hardware to STM32F4 Discovery. Deploy the subsystem to hardware which then builds the new block to be placed in the Simulink model. Run the simulation which will execute OpenOCD to communicate with the Discovery board. Make sure to use FTDI USB to Serial in order to avoid bottleneck in ST-LINK (It is extremely slow with ST-LINK even for 0.5 second simulation).

### Position Control Simulation SIL and PIL VS HIL

HIL PWM TIM3 block was configured to use ARR value of only 2 (which set the PWM freq. to 14MHz). Changing the ARR value to 140 correctly sets the output freq. to 200kHz.
Also, the difference in position control of 3000 deg was induced from Simulink's PID block N coefficient which was set at 100. Disabling this coefficient produced the step response using PD control to very similar to the one from motorlabGUI.
HIL plot shows an initial delay of about 0.05 seconds



## External Mode with STM32-MAT/Target

When editing the project within IDE, make sure to change the source code used in Simulink to generate code. Simulink code generation will overwrite any changes made!

DO NOT COPY THE SIMULINK FILE AND BUILD WITH DIFFERENT IOC FILE.
IT WILL GIVE YOU THE `stm32_make_rtw_hook(): error with copyModelFiles() ERROR`
Also, name of `.ioc` file will be the name of the generated project.

In STM32CubeMX, check generate under root to avoid project creation within a project.

When STM32CubeIDE throws error stating cannot open linker script file, change the Linker Script to `Z:\path\to\linker\script\STM32F407VGTX_FLASH.ld` instead of `//mne-stokes/path/to/script/.ld`.

For testing PIL demos, use run button. Unlike external mode, demo works wth run button.

For using the external mode with STM32-MAT Target, do not deploy the Simulink model. Instead, build the model, open the IDE, then run the debugger. And then connect to the Simulink model using the external mode control panel within Simulink. External mode is over UART5(PD2,PC12). 
