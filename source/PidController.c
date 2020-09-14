float pid(float Kp, float Ki, float Kd,
         float error, float integral,
         float DT, float Umax)      // pid controller returns integral outputs U
{
 static float lastError;
 float U, USat;
 
  integral = integral + Ki*DT*error;     // Integral portion of PID
  USat = Umax;
  if (integral > USat) integral = USat;    // stopgap limits on integral
  if (integral < -USat) integral = -USat;  // to limit integral windup 

  U = integral + Kp*error + Kd*(error-lastError)/DT;   // PID 
  USat = Umax;
  if (U > USat) U = USat;               // saturation limits
  if (U < -USat) U = -USat;

  lastError = error;      // remember last error
  return(U);
}     