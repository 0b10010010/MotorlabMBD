# MotorlabMBD

When editing the project within IDE, make sure to change the source code used in Simulink to generate code. Simulink code generation will overwrite any changes made!

DO NOT COPY THE SIMULINK FILE AND BUILD WITH DIFFERENT IOC FILE.
IT WILL GIVE YOU THE stm32_make_rtw_hook(): error with copyModelFiles() ERROR

For testing PIL demos, use run button. Unlike external mode, demo works wth run button.

For using the external mode with STM32-MAT Target, do not deploy the Simulink model. Instead, build the model, open the IDE, then run the debugger. And then connect to the Simulink model using the external mode control panel within Simulink.
