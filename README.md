# MotorlabMBD
This readme is for my list of features to complete and to keep track of bugs, workarounds, and etc.

PIL simulation running at 1kHz matches better with 10kHz HIL response to 200 deg step input.

The board is setup to write to GPIO to check the loop frequency and it is exactly at 10kHz.

Limitations of Simulink compiler: does not catch syntax errors so one have to wait untill the generated code is ported into the working IDE for debug.

Follow ![this](https://electronics.stackexchange.com/questions/179546/getting-pwm-to-work-on-stm32f4-using-sts-hal-libraries) to setup PWM. Everything seems ok except I have to manually call `HAL_TIM_PWM_Start()` in my pwmInit function to start PWM output (see the `HIL_EXT.c` file where it calls `HIL_EXT_initialize()` to setup PWM output.

# TODO

  &#10060; plant bandwidth?</br>
  &#10060; What if the current control loop is not '1'? -> add a current control loop in my model?</br>
  &#9989; Check the difference between single tasking mode vs multi tasking mode (overhead with multi tasking might be affecting the response) -> unstable with Kp = 0.01 without multi tasking</br>
  &#10060; Implement circular buffer to compare the performance of position control using unsaturating Kp and step input.</br>
  &#10060; Try generating code as C++ by replacing the source code to C++ and setting the code generation to C++</br>
  
  <h2>STM32:</h2>
  
  &#10060; TIM3 block for PWM generation does not set the other CCR to zero when dutycycle is set to input. (when CCR1 is set to dutycycle, CCR2 is not set)</br>
  &#10060; One must generate code using CubeMX first before building and generating code from Simulink</br>
  &#10060; when using external mode with receive interrupts in my function, there is a <code>multiple definition of 'HAL_UART_RxCpltCallback'</code> error</br>
  &#10060; setup software interrupt on USART2 RX to trigger</br>
  &#9989; set objects as global to add Watch Expressions during debug</br>
  &#10060; when the motor is turned manually while debugging and when it slips, encoder shows the correct 200 deg. <- troubleshoot</br>
  &#9989; set the baudrate to 921600 on STM32CubeMX and MATLAB function reading serialport</br>
    &emsp; &#8627; Along with 921600, oversample is set to 8 instead of 16 but still no difference</br>
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
  &#9989; give the encoder signal priority in order to get the data from zero</br>
    &emsp; &#8627; Now plot starts from zero (initial) value</br>
  &#9989; implement 2nd order LPF for speed control</br>
  &#9989; switch in the dashboard to alternate between position and speed control</br>
    &emsp; &#8627; Dashboard blocks do not work with real time external mode. Simulink Real Time Desktop toolbox is required</br>
  &#9989; amp switch is connected to TX block so that turning on the amp will start sending data back to host PC to plot</br>
  &#9989; displays to show pos and vel data</br>
    &emsp; &#8627; Did not work. Same reason as Simulink Real Time Desktop toolbox is not available</br>
  &#9989; edit encoder block to be able to set it back to zero</br>
  &#9989; edit HIL Simulink model to send the data (all 2048 by n fields containing position, velocity, etc.) back to host PC when a button is pressed.</br>
  
## Simulink Data Collection Workflow
First, make sure amp switch is off and set the block to zero. Then run the MATLAB script reading and ploting serial data. Back in the diagram, set the input to 200. Click on empty space in the diagram and turn on the amp switch. After the first run, set the input to zero. Motor should go back to zero. Turn off the amp. Reset the encoder, clear the device in the workspace reading in the serial data, and repeat the process. If not following the steps above, either data will not start from the zero (initial value), or control loop will not be optimal.

## MATLAB Data Handle
When the Run Wave Autosave button is used to collect the whole data, motorlabGUI `runAutosaveButton_Callback` function sends the &mu;controller parameter object to start Autosave. Then the gui waits for the &mu;controller which sends a single instance at a time, total of 2048 times. Each instance contains all 9 floatval data. See function `receiveAllObjects` in `GloTalkClass.m`. Within the function `receiveAllObjects`, it will compare the available bytes in the buffer: `serialPort.BytesAvailable >= totNumBytes` to decide if it should read in the data or not. `totNumBytes` is `(numBytes+9)*numInstance` where `numBytes` is the `sizeof(logDataDataType)` which is a `float` array with length of 9. `numInstance` is 2048 for the `logDataDataType`. `+9` is the other bytes in the buffer: start (+3), objID (+1), instance (+2), numBytes (+1), endBytes (+2). In the `getObject`, which contains the `receiveAllObjects`, `objID` is 0 in `bytes` which is a request to &mu;controller to send back the data. See the following from the description:
```
gloObjects consist of

typedef struct{
  uint8_t id;
  uint8_t numBytes;
  uint16_t numInstance;
  uint8_t junk;
  void *dataPtr;
  void  (* onReceiveFuncPtr)(uint16_t instance);
} gloMetaDataType;

objID number 0 is not an object it is merely a request for an object to be sent.  It has a one byte body that is the objID.  The instance
number (two bytes) is in the message header. If the instance number is zero, then it is a request for all instances.

objID number 255 is reserved for ack/nack (TODO)

gloObjectPtrs[objID]; is a pointer the the meta data structure for the obj
'objname'.dataPtr & gloObjectPtrs[objID]->dataPtr; pointers to the first instance of the obj
```
`numInstance` is 2 bytes because the number 2048 is 12 bit.
For the status update in motorlabGUI.m to display the status of the motor, such as the position at an instance, the function `getObject(handles.status,1)` is used to get a single instance. This instance contains 9 floatvals of status of the motor which is similar to `handles.logData`. If the instance is 0 it is a request for all instances; for `getObject(handles.logData, 0)` it will receive all 2048 `floatvals[9]`.

`objID` is the following: `TrapProf` is 1, `LogData` is 2, `Parameters` is 3, and `Status` is 4. Only `LogData` has length of 2048 and the others are 1.

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


## Analysing the Position Control Data

With the step size of 200 and Kp 0f 0.01 amp/deg, the motor amp is not saturating (3 amp saturation).

![equation](https://latex.codecogs.com/svg.latex?w_n%20%3D%20%5Csqrt%7B%5Cfrac%7BK_pK_tK_d_r%7D%7BJ%7D%7D)

Buadrate for each position value transmission is found with the following:

each char array of ASCII numerical values are 64bit each -> 64bit/115200bits/sec = 0.0005556 seconds for each transmission which is not sufficient for 0.0001s steps.


Meeting wiht Dr. White:

Thesis -> Comaprison to current motrolab. RTOS on the current device. I am doing MBC and there will be a difference in effor there as far as comparison to RTOS.
Problems with motorlab, if you want to do something different, equipemnt on motorlab (software) is designed to do only specific things. Instead of writing a new software, i could illustrate the merrits of MBD by demonstratng students could simply replace blocks to do different lab exercises that origianl motorlab was not able to do so.

Outline of my thesis.
Talk to Dr. Brockhoff -> see any ideas of my MBD application to his problems.


## Thesis Outline

Objective of the thesis: to enable MBD approach to an existing Controls I laboratory appartus in order to demonstrate the capabilities
