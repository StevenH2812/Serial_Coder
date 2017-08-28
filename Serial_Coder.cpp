/*
 * Serial_Coder.c
 *
 *  Created on: Aug 28, 2017
 *      Author: steven
 */

#include "Serial_Coder.h"

SerialCoderClass SerialCoder;

byte SerialCoderClass::_bytesRecvd = 0;
byte SerialCoderClass::_dataSentNum = 0;
byte SerialCoderClass::_dataRecvCount = 0;

byte SerialCoderClass::_dataSendCount = 0;
byte SerialCoderClass::_dataTotalSend = 0;

boolean SerialCoderClass::_inProgress = 0;
boolean SerialCoderClass::_startFound = 0;
boolean SerialCoderClass::_allReceived = 0;



SerialCoderClass::SerialCoderClass(){
	for (uint8_t i=0;i<MESSAGES;i++){
		messages[i].type = i;
	}
}

void SerialCoderClass::testFun(){
	Serial.println("Hello world");
	delay(1000);
}



