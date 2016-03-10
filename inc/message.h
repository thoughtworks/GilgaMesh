#pragma once

typedef enum {
	QueryVersion = 0,
	StartDFU = 1,
	SetFamilyID = 2
} BleMessageType;

// This is a message head.all request message struct must declare this head first.
typedef struct {
	BleMessageType messageType;	// Message Type is used to identify a request
	uint8_t password[6];		// Used for checking customer's role,whether the customer can config the device.
}__attribute__ ((packed)) BleMessageHead;

// This is the result of response,every response must declare this result first
typedef struct {
	uint8_t errorCode;
}__attribute__ ((packed))  BleMessageResult;

// Query Version Message
typedef struct {
	BleMessageHead head;
}__attribute__ ((packed))  BleMessageQueryVersionReq;

typedef struct {
	BleMessageResult result;
	uint8_t version;
}__attribute__ ((packed))  BleMessageQueryVersionRsp;

// DFU Update Message
typedef struct {
	BleMessageHead head;
}__attribute__ ((packed))  BleMessageStartDFUReq;

typedef struct {
	BleMessageResult result;
}__attribute__ ((packed))  BleMessageStartDFURsp;

typedef struct {
	BleMessageHead head;
	uint32_t familyID;
}__attribute__ ((packed))  BleMessageSetFamilyIDReq;
