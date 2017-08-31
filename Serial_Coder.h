/*
 * Serial_Coder.h
 *
 *  Created on: Aug 28, 2017
 *      Author: steven
 */

#ifndef SERIAL_CODER_H_
#define SERIAL_CODER_H_

#include <Arduino.h>
//#include "ConnectedRanging.h"


#define MAX_MESSAGE 10
#define IN_MESSAGE_SIZE 4
#define OUT_MESSAGE_SIZE 7
#define END_MARKER 255
#define SPECIAL_BYTE 253
#define START_MARKER 254
#define IN_MESSAGES 3
#define OUT_MESSAGES 1

//#define STATE_SIZE 3

// Message in types
#define VX 0
#define VY 1
#define Z 2
#define R 3

/*
struct MessageIn{
	byte type;
	byte msg[IN_MESSAGE_SIZE];
};*/

/*
struct selfState{
	float vx;
	float vy;
	float z;
	boolean updated[STATE_SIZE];
};
*/

/**
 * This class is used for encoding, decoding, sending, and receiving messages that will be sent over Arduino's serial communication.
 * The purpose is to make this sending more robust by implementing features like start and end characters to make sure messages are sent and received in whole.
 * Furthermore it will dedicate a byte to indicate the type of message, such that multiple different messages can be sent depending on the indicator byte.
 * A message will therefore look like {<start character> <message type> <message payload> <end character>}
 */
class SerialCoderClass{

public:
	SerialCoderClass();
	static void getSerialData();
	static void processData();
	static void decodeHighBytes();

	static float receiveFloat(byte msgtype);


	static void sendFloat(byte msgfrom, byte msgtype, float outfloat);
	static void encodeHighBytes(byte* sendData, uint8_t msgSize);


	static void attachStateHandle(void (* handleNewSelfState)(void)) { _handleNewSelfState = handleNewSelfState; };
	static void updateStateVar(byte msgFrom,byte msgType);
	static boolean stateUpdated();



protected:
	static byte _bytesRecvd;
	static byte _dataSentNum;
	static byte _dataRecvCount;

	//static byte _dataRecvd[MAX_MESSAGE];
	static byte _dataSend[MAX_MESSAGE];
	static byte _tempBuffer[MAX_MESSAGE];
	static byte _recvBuffer[FLOAT_SIZE];

	static byte _dataSendCount;
	static byte _dataTotalSend;

	static boolean _inProgress;
	static boolean _startFound;
	static boolean _allReceived;

	static MessageIn _receiveMessages[IN_MESSAGES];
	static selfState _selfState;

	static byte _varByte;


	// Handlers
	static void (* _handleNewSelfState)(void);

};

extern SerialCoderClass SerialCoder;

#endif /* SERIAL_CODER_H_ */
