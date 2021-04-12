//
// Created by thierry on 11/04/2021.
//

#include "../headers/MPPLibrary.h"

MPPSession* MPPSessionCreate(MPPError* error) {
    MPPSession* mppSession = NULL;

    assert(error != NULL);

    mppSession = (MPPSession*) malloc(sizeof(MPPSession));
    if(mppSession) {
        uuid_generate(mppSession->uuid);
        // check that the generate uuid is not null
        if(uuid_is_null(mppSession->uuid)) {
            error->errorCode = MPP_ERROR_FAILED_TO_GENERATE_UUID;
            strcpy(error->message,"Unable to generate an UUID for new session. Out of memory ?");
            free(mppSession);
            mppSession = NULL;
            return mppSession;
        } else {
            mppSession->jsonBuffer = NULL;
            mppSession->jsonBufferSize = 0;
            mppSession->hostname[0] = 0;
            mppSession->port = MPP_DEFAULT_NETWORK_PORT;
            mppSession->callback = NULL;
        }
    } else {
        error->errorCode = MPP_ERROR_FAILED_TO_ALLOCATE_MEMORY;
        strcpy(error->message,"Unable to allocate a new session. Out of memory ?");
    }
    return mppSession;
}

void MPPSessionDumpJsonNode(json_t *pJson) {
    // TODO: implement this in a near future.
    assert(pJson != NULL);

}

bool MPPSessionIsValidJsonBuffer(const char* jsonBuffer, size_t jsonBufferLength) {
    assert(jsonBuffer != NULL);
    assert(jsonBufferLength > 0);

    json_error_t jsonError;

    json_t * jsonRoot = json_loadb(jsonBuffer,jsonBufferLength,JSON_REJECT_DUPLICATES | JSON_DISABLE_EOF_CHECK,&jsonError);
    if(jsonRoot == NULL) {
        printf("[ERROR] While parsing jsonBuffer: at line [%d] column [%d] position [%d] source: [%s] error: [%s]\n",jsonError.line,jsonError.column,jsonError.position,jsonError.source,jsonError.text);
        return false;
    } else {
        MPPSessionDumpJsonNode(jsonRoot);
    }
    return true;
}

bool MPPSessionSetJsonBuffer(MPPSession* mppSession, const char* jsonBuffer, size_t jsonBufferLength, MPPError* error) {
    assert(mppSession != NULL);
    assert(error != NULL);
    assert(jsonBufferLength > 0);
    assert(jsonBuffer != NULL);
    assert(mppSession->jsonBuffer == 0);
    assert(mppSession->jsonBufferSize == 0);

    size_t allocatedSize = jsonBufferLength+1;
    mppSession->jsonBuffer = malloc(allocatedSize);
    if(mppSession->jsonBuffer == NULL) {
        error->errorCode = MPP_ERROR_FAILED_TO_ALLOCATE_MEMORY;
        strcpy(error->message,"Unable to allocate a jsonBuffer. Out of memory ?");
    } else {
        mppSession->jsonBufferSize = allocatedSize;
        mppSession->jsonBuffer[allocatedSize - 1] = 0;
        memcpy(mppSession->jsonBuffer,jsonBuffer,jsonBufferLength);
        if(MPPSessionIsValidJsonBuffer(mppSession->jsonBuffer,mppSession->jsonBufferSize)) {
            return true;
        } else {
            error->errorCode = MPP_ERROR_INVALID_JSON_CONTENT;
            strcpy(error->message,"Provided content in jsonBuffer is not valid JSON text.");
        }
    }
    return false;
}

void MPPSessionRelease(MPPSession* mppSession) {
    assert(mppSession != NULL);

    if(mppSession->jsonBufferSize > 0 && mppSession->jsonBuffer != 0) {
        free(mppSession->jsonBuffer);
        mppSession->jsonBuffer = 0;
        mppSession->jsonBufferSize = 0;
    }

    free(mppSession);
}

bool MPPSessionIsValidHostname(const char* hostname) {
    assert(hostname != NULL);
    assert(strlen(hostname) <= 253);

    // TODO: implement this.

    return true;
}

bool MPPSessionSetDestinationHost(MPPSession* mppSession, const char* destinationHostname, unsigned int destinationPort, MPPError* error) {
    assert(error != NULL);
    assert(mppSession != NULL);
    assert(destinationPort <= 65536);
    assert(destinationHostname != NULL);
    assert(strlen(destinationHostname) <= 253);

    if(destinationPort > 0) {
        mppSession->port = destinationPort;
    } else {
        mppSession->port = MPP_DEFAULT_NETWORK_PORT;
    }

    if(!MPPSessionIsValidHostname(destinationHostname)) {
        error->errorCode = MPP_ERROR_INVALID_HOSTNAME;
        strcpy(error->message,"The hostname provided is invalid, not conform.");
        return false;
    } else {
        strcpy(mppSession->hostname, destinationHostname);
    }

    return true;
}

void MPPSessionSetReceiveCallback(MPPSession* mppSession, MPPSessionCallbackReceiveNetworkFrameFunction callbackFunction) {
    assert(mppSession != NULL);

    mppSession->callback = callbackFunction;
}