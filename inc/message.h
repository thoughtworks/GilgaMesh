#pragma once

#define NODE_NAME_SIZE                    8
#define BROADCAST_SIZE                    12


typedef enum {
  SetFamilyID = 0, // no longer used
  StartDFU = 1,
  Broadcast = 2,
  ConnectionInfo = 3,
  Heartbeat = 4
} BleMessageType;

// This is a message head.all request message struct must declare this head first.
typedef struct {
  BleMessageType messageType;  // Message Type is used to identify a request
}__attribute__ ((packed)) BleMessageHead;

// DFU Update Message
typedef struct {
  BleMessageHead head;
}__attribute__ ((packed)) BleMessageStartDFUReq;

typedef struct {
  BleMessageHead head;
  uint8_t message[BROADCAST_SIZE];
}__attribute__ ((packed)) BleMessageBroadcastReq;

typedef struct {
  BleMessageHead head;
  uint32_t deviceId;
  uint8_t majorVersion;
  uint8_t minorVersion;
}__attribute__ ((packed)) BleMessageConnectionInfoReq;

typedef struct {
  BleMessageHead head;
  uint32_t deviceId;
  uint32_t centralConnectionDeviceId;
  uint8_t majorVersion;
  uint8_t minorVersion;
}__attribute__ ((packed)) BleMessageHeartbeatReq;
