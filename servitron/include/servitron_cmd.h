#ifndef SERVITRON_CMD_H
#define SERVITRON_CMD_H

#include <stdint.h>
// This defines the command structures for servitron 6.
namespace servitron {
  typedef uint8_t CmdCode;
  typedef uint8_t RspCode;

  const RspCode RspOK = 0x80;
  const RspCode RspError = 0x81;

  const CmdCode Hello = 0x01;

  typedef struct {
    uint16_t clientVersion;
    char* clientStr;
  } __attribute__((packed)) HelloCmd;

  typedef struct {
    uint16_t serverVersion;
    char* serverStr;
  } __attribute__((packed)) HelloRsp;

  const CmdCode MechStatus = 0x02;

  typedef struct {
    int32_t position;
    int32_t velocity;
    uint8_t flags; // powered, etc
  } __attribute__((packed)) MechAxisStatus;

  typedef struct {
    MechAxisStatus axis[6];
    uint8_t flags; // brake, etc
  } __attribute__((packed)) MechStatusRsp;

  const CmdCode QueueStatus = 0x03;
  
  typedef struct {
    int16_t remainingMoves;
  } __attribute__((packed)) QueueStatusRsp;
};

#endif // SERVITRON_CMD_H
