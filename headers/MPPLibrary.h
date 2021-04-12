#ifndef MPP_MPPLIBRARY_H
#define MPP_MPPLIBRARY_H

#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <uuid.h>
#include <jansson.h>
#include <MPPLibraryConfig.h>


static const uint32_t MPP_NETWORK_FRAME_HEADER_SIGNATURE = 0x2bedad4;
static const uint32_t MPP_NETWORK_FRAME_HEADER_VERSION = 0x01;

static const uint8_t MPP_NETWORK_FRAME_TYPE_BUFFER_SIZE_NEGOTIATION = 0;
static const uint8_t MPP_NETWORK_FRAME_TYPE_HEADER = 1;
static const uint8_t MPP_NETWORK_FRAME_TYPE_DATA = 2;

static const unsigned int MPP_DEFAULT_NETWORK_PORT = 12567;

typedef struct mpp_network_frame_header_negotiation {
    uint16_t bufferSize;
} mpp_network_frame_header_negotiation_t;

typedef struct mpp_network_frame_header_header {
    uint16_t headerChunks;
    uint16_t chunkNumber;
} mpp_network_frame_header_header_t;

typedef struct mpp_network_frame_header_data {
    uint16_t dataChunks;
    uint16_t chunkNumber;
} mpp_network_frame_header_data_t;

typedef struct mpp_error {
    uint32_t errorCode;
    char message[1024];
} mpp_error_t;
typedef struct mpp_error MPPError;

typedef struct mpp_network_frame_header {
    uint32_t signature;
    uint8_t version;
    uuid_t uuid;
    uint16_t frameSize;
    uint8_t type;
    union headerKind {
        mpp_network_frame_header_negotiation_t negotiation;
        mpp_network_frame_header_header_t header;
        mpp_network_frame_header_data_t data;
    } kind;
} mpp_network_frame_header_t ;
typedef struct mpp_network_frame_header MPPNetworkFrameHeader;

typedef struct mpp_network_frame {

} mpp_network_frame_t;

typedef struct mpp_network_frame MPPNetworkFrame;

typedef struct mpp_session MPPSession;

typedef void (*MPPSessionCallbackReceiveNetworkFrameFunction) (const MPPSession* mppSession, const MPPNetworkFrame* networkFrame);

typedef struct mpp_session {
    uuid_t uuid;
    char* jsonBuffer;
    size_t jsonBufferSize;
    char hostname[255];
    unsigned int port;
    MPPSessionCallbackReceiveNetworkFrameFunction callback;
} mpp_session_t;



static const int MPP_NETWORK_FRAME_HEADER_SIZE = sizeof(MPPNetworkFrameHeader);

// Session utilities functions
MPPSession* MPPSessionCreate(MPPError* error);
bool MPPSessionSetJsonBuffer(MPPSession* mppSession, const char* jsonBuffer, size_t jsonBufferLength, MPPError* error);
void MPPSessionRelease(MPPSession* mppSession);
bool MPPSessionSetDestinationHost(MPPSession* mppSession, const char* destinationHostname, unsigned int destinationPort, MPPError* error);
void MPPSessionSetReceiveCallback(MPPSession* mppSession, MPPSessionCallbackReceiveNetworkFrameFunction callbackFunction);

// NetworkFrame utilities functions
bool MPPNetworkFrameHeaderInit(MPPNetworkFrameHeader* mppNetworkFrameHeader);

// General's Library functions
bool isMPPLibraryInitialized(void);
bool MPPLibraryStart(void);
bool MPPLibraryStop(void);
bool isMPPLibraryNetworkListenerStarted(void);
void MPPLibraryStartNetworkListener(MPPError* mppError);
void MPPLibraryStopNetworkListener(MPPError* mppError);

// Errors
static const int MPP_ERROR_FAILED_TO_ALLOCATE_MEMORY = -1;
static const int MPP_ERROR_FAILED_TO_GENERATE_UUID = -2;
static const int MPP_ERROR_INVALID_JSON_CONTENT = -3;
static const int MPP_ERROR_INVALID_HOSTNAME = -4;

#endif //MPP_MPPLIBRARY_H
