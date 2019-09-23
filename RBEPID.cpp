/*
 * Messages.h
 *
 *  Created on: 10/1/16
 *      Author: joest
 */
#include "Arduino.h"
#include "RBEPID.h"

//Class constructor
RBEPID::RBEPID() {

}

//Function to set PID gain values
void RBEPID::setpid(float P, float I, float D) {
	kp = P;
	ki = I;
	kd = D;
}

/**
 * calc the PID control signel
 *
 * @param setPoint is the setpoint of the PID system
 * @param curPosition the current position of the plan
 * @return a value from -1.0 to 1.0 representing the PID control signel
 */
float RBEPID::calc(double setPoint, double curPosition) {

	// calculate error
	float err = setPoint - curPosition;
	// calculate derivative of error
	float dterm = ((err-last_error)/sampleRateMs)*kd;
	last_error=err;
	// calculate integral error. Running average is best but hard to implement
	integralTotal-=inteegralBuffer[integralIndex];
	inteegralBuffer[integralIndex]=err;
	integralTotal+=inteegralBuffer[integralIndex];
	integralIndex++;
	if(integralIndex==(int)numIntegralValues)
		integralIndex=0;
	float iterm = (integralTotal/numIntegralValues)*ki * sampleRateMs;

	// sum up the error value to send to the motor based off gain values.

	float out = err * kp + iterm+dterm;	// simple P controller
	//return the control signal from -1 to 1
	if (out > 1)
		out = 1;
	if (out < -1)
		out = -1;
	return out;
}

/**
 * Clear the internal representation fo the integral term.
 *
 */
void RBEPID::clearIntegralBuffer() {
	//TODO implement this when implementing the integral term
	integralTotal=0;
	for(int i=0;i<numIntegralValues;i++)
		inteegralBuffer[i]=0;
	integralIndex=0;
}
