/*
 * Serial_Coder.h
 *
 *  Created on: Aug 28, 2017
 *      Author: steven
 */

#ifndef SERIAL_CODER_H_
#define SERIAL_CODER_H_

#include <Arduino.h>
#include <Stream.h>

#define FLOAT_SIZE 4

#define MAX_MESSAGE 20
#define END_MARKER 255
#define SPECIAL_BYTE 253
#define START_MARKER 254
#define MESSAGES_IN 10







/**
 * This class is used for encoding, decoding, sending, and receiving messages that will be sent over Arduino's serial communication.
 * The purpose is to make this sending more robust by implementing features like start and end characters to make sure messages are sent and received in whole.
 * Furthermore it will dedicate a byte to indicate the type of message, such that multiple different messages can be sent depending on the indicator byte.
 * A message will therefore look like {<start character> <message type> <message payload> <end character>}
 */
class SerialCoderClass{

public:


	SerialCoderClass(byte thisaddress, Stream &myStream) : _address(thisaddress), _myStream(&myStream) {}
	SerialCoderClass(byte thisaddress, Stream *myStream) : _address(thisaddress), _myStream(myStream) {}

	void setSoftwareSerial(boolean setsoft);
	void getSerialData();
	void processData();
	void decodeHighBytes();

	float receiveFloat(byte msgtype);


	void sendFloat(byte msgfrom, byte msgtype, float outfloat);
	void sendBytes(byte msgto, byte msgtype, byte* tosend, byte n);

	void encodeHighBytes(byte* sendData, uint8_t msgSize);

	void attachMessageHandler(void (* handleMessage)(byte *message, byte n),uint8_t msgtype){_messageHandlers[msgtype] = handleMessage;}




protected:
	byte _bytesRecvd;
	byte _dataSentNum;
	byte _dataRecvCount;

	//static byte _dataRecvd[MAX_MESSAGE];
	byte _dataSend[MAX_MESSAGE*2];
	byte _tempBuffer[MAX_MESSAGE*2];
	byte _recvBuffer[MAX_MESSAGE*2];

	byte _dataSendCount;
	byte _dataTotalSend;

	boolean _inProgress;
	boolean _startFound;
	boolean _allReceived;


	byte _varByte;
	byte _varByte2;

	byte _address = 0;

	byte _inmessages;

	boolean _softwareSerialBool = false;

	Stream *_myStream;


	void (*_messageHandlers[MESSAGES_IN])(byte *message,byte n);

};

//extern SerialCoderClass SerialCoder;

#endif /* SERIAL_CODER_H_ */
