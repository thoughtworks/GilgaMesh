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
  Custom = 4
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

