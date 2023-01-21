#ifndef CONSTANTS_H
#define CONSTANTS_H

const int MS_TO_SECS = 1000;
const int BYTE_TO_KILOBYTE = 1000;
const int SERVER_TRIGGER_COMMAND_TIMER_INTERVAL = 60 * MS_TO_SECS;
const int SERVER_AUTHENTICATION_TIMEOUT = 24 * 60 * 60 * MS_TO_SECS;
const int FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE = 50000;

#endif  // CONSTANTS_H