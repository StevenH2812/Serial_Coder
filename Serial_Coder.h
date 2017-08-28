/*
 * Serial_Coder.h
 *
 *  Created on: Aug 28, 2017
 *      Author: steven
 */

#ifndef SERIAL_CODER_H_
#define SERIAL_CODER_H_

#include <Arduino.h>


#define MAX_MESSAGE 5
#define END_MARKER 255
#define SPECIAL_BYTE 253
#define START_MARKER 254
#define MESSAGES 3

struct Message{
	uint8_t type;
	byte msg[MAX_MESSAGE];
};

/**
 * This class is used for encoding, decoding, sending, and receiving messages that will be sent over Arduino's serial communication.
 * The purpose is to make this sending more robust by implementing features like start and end characters to make sure messages are sent and received in whole.
 * Furthermore it will dedicate a byte to indicate the type of message, such that multiple different messages can be sent depending on the indicator byte.
 * A message will therefore look like {<start character> <message type> <message payload> <end character>}
 */
class SerialCoderClass{

public:
	SerialCoderClass();
	static void testFun();

protected:
	static byte _bytesRecvd;
	static byte _dataSentNum;
	static byte _dataRecvCount;

	static byte _dataRecvvd[MAX_MESSAGE];
	static byte _dataSend[MAX_MESSAGE];
	static byte _tempBuffer[MAX_MESSAGE];

	static byte _dataSendCount;
	static byte _dataTotalSend;

	static boolean _inProgress;
	static boolean _startFound;
	static boolean _allReceived;

	Message messages[MESSAGES];

};

extern SerialCoderClass SerialCoder;

#endif /* SERIAL_CODER_H_ */
