/*
 * Serial_Coder.c
 *
 *  Created on: Aug 28, 2017
 *      Author: steven
 */

#include "Serial_Coder.h"




/**
 * Function for receiving serial data.
 * Only receives serial data that is between the start and end markers. Discards all other data.
 * Stores the received data in _tempBuffer, and after decodes the high bytes and copies the final
 * message to the corresponding message in _messages.
 */
void SerialCoderClass::getSerialData(){
	if(_myStream->available()){
		_varByte = _myStream->read();
		//Serial.println(_varByte);
		if (_varByte == START_MARKER){
			_bytesRecvd = 0;
			_inProgress = true;
			//Serial.println("start marker");
		}
		if (_inProgress){
			_tempBuffer[_bytesRecvd] = _varByte;
			_bytesRecvd++;
			//Serial.println("in progress");
		}
		if (_varByte == END_MARKER){
			//Serial.println("end marker");
			_inProgress = false;

			decodeHighBytes();
		}
	 }

	/*
	if (_myStream->available()){
		_varByte = _myStream->read();
		Serial.println(_varByte);
		if (_varByte == START_MARKER){
			_bytesRecvd = 0;
			_inProgress = true;
			Serial.println("start marker");
		}

		if (_inProgress){
			_tempBuffer[_bytesRecvd] = _varByte;
			_bytesRecvd++;
			Serial.println("in progress");
		}

		if (_varByte == END_MARKER){
			Serial.println("end marker");
			_inProgress = false;
			_allReceived = true;

			decodeHighBytes();
		}
	}
	*/

}

/**
 * Function for decoding the high bytes of received serial data and saving the message.
 * Since the start and end marker could also be regular payload bytes (since they are simply the values
 * 254 and 255, which could also be payload data) the payload values 254 and 255 have been encoded
 * as byte pairs 253 1 and 253 2 respectively. Value 253 itself is encoded as 253 0.
 *  This function will decode these back into values the original payload values.
 */
void SerialCoderClass::decodeHighBytes(){
	_dataRecvCount = 0;
	byte msgFrom = _tempBuffer[1];
	byte msgTo = _tempBuffer[2];
	byte msgType = _tempBuffer[3];
	byte msgLength = _tempBuffer [4];
	byte tempByte;
	/*
	Serial.println("Preamble:");
	Serial.print(msgFrom);
	Serial.print(msgTo);
	Serial.print(msgType);
	Serial.print(msgLength);
	Serial.println("End preamble");*/



	if (msgTo != _address )
		return;
	for (uint8_t i = 5; i<_bytesRecvd-1; i++){ // Skip the begin marker (0), message from (1), message type (2), and end marker (_bytesRecvd-1)
		tempByte = _tempBuffer[i];
		if (tempByte == SPECIAL_BYTE){
			i++;
			tempByte = SPECIAL_BYTE + _tempBuffer[i];
		}
		_recvBuffer[_dataRecvCount] = tempByte;
		//Serial.print(F("Stored character is: "));
		//Serial.println(_varByte);
		_dataRecvCount++;
	}
	_messageHandlers[msgType](_recvBuffer,_dataRecvCount);
}


/**
 * Function to send an array of bytes across the serial. The message will first have 5 preamble bytes,
 * 	Consisting of the start marker, the from address (this address), the to address, the msg type, and the msg length
 */
void SerialCoderClass::sendBytes(byte msgto, byte msgtype, byte* tosend, byte n){
	if (n>MAX_MESSAGE)
		return;
	encodeHighBytes(tosend, n);
	/*
	if(!_softwareSerialBool){
		Serial.write(START_MARKER);
		Serial.write(_address);
		Serial.write(msgto);
		Serial.write(msgtype);
		Serial.write(_dataTotalSend);
		Serial.write(_tempBuffer,_dataTotalSend);
		Serial.write(END_MARKER);
	}
	else{
		_mySerial->write(START_MARKER);
		_mySerial->write(_address);
		_mySerial->write(msgto);
		_mySerial->write(msgtype);
		_mySerial->write(_dataTotalSend);
		_mySerial->write(_tempBuffer,_dataTotalSend);
		_mySerial->write(END_MARKER);
	}*/
	_myStream->write(START_MARKER);
	_myStream->write(_address);
	_myStream->write(msgto);
	_myStream->write(msgtype);
	_myStream->write(_dataTotalSend);
	_myStream->write(_tempBuffer,_dataTotalSend);
	_myStream->write(END_MARKER);

}

/**
 * Function that will send a float over serial. The actual message that will be sent will have
 * a start marker, the from address, the message type, 4 bytes for the float, and the end marker.
 */
void SerialCoderClass::sendFloat(byte msgfrom,byte msgtype, float outfloat){
	byte floatbyte[4];
	memcpy(floatbyte,&outfloat,4);
	encodeHighBytes(floatbyte,4);
	_myStream->write(START_MARKER);
	_myStream->write(msgfrom);
	_myStream->write(msgtype);
	_myStream->write(_tempBuffer,_dataTotalSend);
	_myStream->write(END_MARKER);

}

/**
 * Function that encodes the high bytes of the serial data to be sent.
 * Start and end markers are reserved values 254 and 255. In order to be able to send these values,
 * the payload values 253, 254, and 255 are encoded as 2 bytes, respectively 253 0, 253 1, and 253 2.
 */
void SerialCoderClass::encodeHighBytes(byte* sendData, uint8_t msgSize){
	_dataSendCount = msgSize;
	_dataTotalSend = 0;
	for (uint8_t i = 0; i < _dataSendCount; i++){
		if (sendData[i] >= SPECIAL_BYTE){
			_tempBuffer[_dataTotalSend] = SPECIAL_BYTE;
			_dataTotalSend++;
			_tempBuffer[_dataTotalSend] = sendData[i] - SPECIAL_BYTE;
		}
		else{
			_tempBuffer[_dataTotalSend] = sendData[i];
		}
		_dataTotalSend++;
	}
}


