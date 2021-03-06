Servitron client-server protocol
================================

Server responsibilities:
* Track quadrature encoders
* Maintain information about position and velocity of all axes
* Execute positioning/velocity commands
* Maintain PID loops and current position
* Maintain brake

Types of interaction
--------------------

* Hello: client should connect to server and get a greeting message including server version
* Status: client can request status from the server and get current positions, speeds, and queue status.
* Calibration: set PID settings, find index pulse, confirm directionality, find physical range
* Configuration persistence.
* Non-blocking motion commands: clients should be able to place motions on a queue of sequentially interpreted commands

General command/response framework
----------------------------------

Most commands are fixed-length and determined by a simple struct. Commands begin with a one-byte command identifier.
Responses are also generally fixed-length and delivered as soon as possible. They begin with a one-byte response code.

All command-response transactions are sequential; a second command should not be sent before a reply is received unless the response has timed out. Responses delivered more than half a second after the command is sent are considered to have timed out.

Any command or response that is not fixed-length must end in a zero-terminated string.

Response codes
--------------
0x80 - OK
0x81 - generic error

Hello interaction
-----------------

The HELLO interaction is generally sent when a client connects. It is not required.

Command code: 0x01
Command payload:
    uint16_t clientVersion;
    char* clientString;

Response payload:
    uint16_t serverVersion;
    char* serverString;

Status interactions
-------------------

*Mechanical status*
Command code: 0x02
Command payload: None
Response payload:
    {
        int32_t position
        int32_t velocity
        uint8_t flags _axis enabled, axis calibrated, etc_
    }[6]
    uint8_t flags _brake enabled, system initialized_

*Queue status*
Command code: 0x03
Command payload: None
Response payload:
    int16_t remainingMoves

Calibration interactions
------------------------
Calibration and configuration commands begin at 0x10

*Find directionality*
Command code: 0x10
Command payload:
    uint8_t axis
Response payload:
    uint8_t directionality
Directionality response:
* 0x00: directionality is positive (+motor is +pos)
* 0x01: directionality is negative (+motor is -pos)
* 0x02: directionality seek failed (unable to move motor within window)

*Start range of motion seek*
Command code: 0x11
Command payload:
    uint8_t axis
Response payload: None

*Complete range of motion seek*
Command code: 0x12
Command payload: None
Response payload:
    int32_t min_pos
    int32_t max_pos
    // index pulse frequency information??? TODO

*Set PID*
Command code: 0x13
Command payload: 
    uint8_t axis
    // PID data??? TODO
Response payload: None
    

Persistence interactions
------------------------

*Load config*

*Save config*

