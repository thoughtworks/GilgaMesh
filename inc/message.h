#pragma once
#define BROADCAST_SIZE 12

typedef enum {
	SetFamilyID = 0,
	StartDFU = 1,
	Broadcast = 2
} BleMessageType;

// This is a message head.all request message struct must declare this head first.
typedef struct {
	BleMessageType messageType;	// Message Type is used to identify a request
	uint8_t password[6];		// Used for checking customer's role,whether the customer can config the device.
}__attribute__ ((packed)) BleMessageHead;

// DFU Update Message
typedef struct {
	BleMessageHead head;
}__attribute__ ((packed))  BleMessageStartDFUReq;

typedef struct {
	BleMessageHead head;
	uint32_t familyID;
}__attribute__ ((packed))  BleMessageSetFamilyIDReq;

typedef struct {
	BleMessageHead head;
	uint8_t message[BROADCAST_SIZE];
}__attribute__ ((packed)) BleMessageBroadcastReq;
