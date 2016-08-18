#pragma once

typedef enum {
  DoNotPropagate,
  PropagateToAll,
  PropagateToCentral
} MessagePropagationType;

typedef enum {
  Broadcast = 1,
  Handshake = 2,
  Heartbeat = 3,
} BleMessageType;

// This is a message head.all request message struct must declare this head first.
typedef struct {
  BleMessageType messageType;  // Message Type is used to identify a request
}__attribute__ ((packed)) BleMessageHead;

//typedef struct {
//  BleMessageHead head;
//  uint32_t deviceId;
//  userVote vote;
//}__attribute__ ((packed)) BleMessageVoteReq;
//
//typedef struct {
//  BleMessageHead head;
//  uint32_t deviceId;
//  userVoteAck voteAck;
//}__attribute__ ((packed)) BleMessageVoteAckReq;
//
//typedef struct {
//  BleMessageHead head;
//  uint32_t deviceId;
//  bool setGroup;
//  uint8_t newGroup;
//  bool setValue;
//  uint8_t newValue;
//}__attribute__ ((packed)) BleMessageGroupValueReq;
//
