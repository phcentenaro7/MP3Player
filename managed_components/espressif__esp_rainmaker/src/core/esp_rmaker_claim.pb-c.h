/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: esp_rmaker_claim.proto */

#ifndef PROTOBUF_C_esp_5frmaker_5fclaim_2eproto__INCLUDED
#define PROTOBUF_C_esp_5frmaker_5fclaim_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _RmakerClaim__PayloadBuf RmakerClaim__PayloadBuf;
typedef struct _RmakerClaim__RespPayload RmakerClaim__RespPayload;
typedef struct _RmakerClaim__RMakerClaimPayload RmakerClaim__RMakerClaimPayload;


/* --- enums --- */

typedef enum _RmakerClaim__RMakerClaimStatus {
  RMAKER_CLAIM__RMAKER_CLAIM_STATUS__Success = 0,
  RMAKER_CLAIM__RMAKER_CLAIM_STATUS__Fail = 1,
  RMAKER_CLAIM__RMAKER_CLAIM_STATUS__InvalidParam = 2,
  RMAKER_CLAIM__RMAKER_CLAIM_STATUS__InvalidState = 3,
  RMAKER_CLAIM__RMAKER_CLAIM_STATUS__NoMemory = 4
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RMAKER_CLAIM__RMAKER_CLAIM_STATUS)
} RmakerClaim__RMakerClaimStatus;
typedef enum _RmakerClaim__RMakerClaimMsgType {
  RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE__TypeCmdClaimStart = 0,
  RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE__TypeRespClaimStart = 1,
  RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE__TypeCmdClaimInit = 2,
  RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE__TypeRespClaimInit = 3,
  RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE__TypeCmdClaimVerify = 4,
  RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE__TypeRespClaimVerify = 5,
  RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE__TypeCmdClaimAbort = 6,
  RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE__TypeRespClaimAbort = 7
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE)
} RmakerClaim__RMakerClaimMsgType;

/* --- messages --- */

struct  _RmakerClaim__PayloadBuf
{
  ProtobufCMessage base;
  uint32_t offset;
  ProtobufCBinaryData payload;
  uint32_t totallen;
};
#define RMAKER_CLAIM__PAYLOAD_BUF__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rmaker_claim__payload_buf__descriptor) \
    , 0, {0,NULL}, 0 }


struct  _RmakerClaim__RespPayload
{
  ProtobufCMessage base;
  RmakerClaim__RMakerClaimStatus status;
  RmakerClaim__PayloadBuf *buf;
};
#define RMAKER_CLAIM__RESP_PAYLOAD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rmaker_claim__resp_payload__descriptor) \
    , RMAKER_CLAIM__RMAKER_CLAIM_STATUS__Success, NULL }


typedef enum {
  RMAKER_CLAIM__RMAKER_CLAIM_PAYLOAD__PAYLOAD__NOT_SET = 0,
  RMAKER_CLAIM__RMAKER_CLAIM_PAYLOAD__PAYLOAD_CMD_PAYLOAD = 10,
  RMAKER_CLAIM__RMAKER_CLAIM_PAYLOAD__PAYLOAD_RESP_PAYLOAD = 11
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RMAKER_CLAIM__RMAKER_CLAIM_PAYLOAD__PAYLOAD)
} RmakerClaim__RMakerClaimPayload__PayloadCase;

struct  _RmakerClaim__RMakerClaimPayload
{
  ProtobufCMessage base;
  RmakerClaim__RMakerClaimMsgType msg;
  RmakerClaim__RMakerClaimPayload__PayloadCase payload_case;
  union {
    RmakerClaim__PayloadBuf *cmdpayload;
    RmakerClaim__RespPayload *resppayload;
  };
};
#define RMAKER_CLAIM__RMAKER_CLAIM_PAYLOAD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rmaker_claim__rmaker_claim_payload__descriptor) \
    , RMAKER_CLAIM__RMAKER_CLAIM_MSG_TYPE__TypeCmdClaimStart, RMAKER_CLAIM__RMAKER_CLAIM_PAYLOAD__PAYLOAD__NOT_SET, {0} }


/* RmakerClaim__PayloadBuf methods */
void   rmaker_claim__payload_buf__init
                     (RmakerClaim__PayloadBuf         *message);
size_t rmaker_claim__payload_buf__get_packed_size
                     (const RmakerClaim__PayloadBuf   *message);
size_t rmaker_claim__payload_buf__pack
                     (const RmakerClaim__PayloadBuf   *message,
                      uint8_t             *out);
size_t rmaker_claim__payload_buf__pack_to_buffer
                     (const RmakerClaim__PayloadBuf   *message,
                      ProtobufCBuffer     *buffer);
RmakerClaim__PayloadBuf *
       rmaker_claim__payload_buf__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rmaker_claim__payload_buf__free_unpacked
                     (RmakerClaim__PayloadBuf *message,
                      ProtobufCAllocator *allocator);
/* RmakerClaim__RespPayload methods */
void   rmaker_claim__resp_payload__init
                     (RmakerClaim__RespPayload         *message);
size_t rmaker_claim__resp_payload__get_packed_size
                     (const RmakerClaim__RespPayload   *message);
size_t rmaker_claim__resp_payload__pack
                     (const RmakerClaim__RespPayload   *message,
                      uint8_t             *out);
size_t rmaker_claim__resp_payload__pack_to_buffer
                     (const RmakerClaim__RespPayload   *message,
                      ProtobufCBuffer     *buffer);
RmakerClaim__RespPayload *
       rmaker_claim__resp_payload__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rmaker_claim__resp_payload__free_unpacked
                     (RmakerClaim__RespPayload *message,
                      ProtobufCAllocator *allocator);
/* RmakerClaim__RMakerClaimPayload methods */
void   rmaker_claim__rmaker_claim_payload__init
                     (RmakerClaim__RMakerClaimPayload         *message);
size_t rmaker_claim__rmaker_claim_payload__get_packed_size
                     (const RmakerClaim__RMakerClaimPayload   *message);
size_t rmaker_claim__rmaker_claim_payload__pack
                     (const RmakerClaim__RMakerClaimPayload   *message,
                      uint8_t             *out);
size_t rmaker_claim__rmaker_claim_payload__pack_to_buffer
                     (const RmakerClaim__RMakerClaimPayload   *message,
                      ProtobufCBuffer     *buffer);
RmakerClaim__RMakerClaimPayload *
       rmaker_claim__rmaker_claim_payload__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rmaker_claim__rmaker_claim_payload__free_unpacked
                     (RmakerClaim__RMakerClaimPayload *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*RmakerClaim__PayloadBuf_Closure)
                 (const RmakerClaim__PayloadBuf *message,
                  void *closure_data);
typedef void (*RmakerClaim__RespPayload_Closure)
                 (const RmakerClaim__RespPayload *message,
                  void *closure_data);
typedef void (*RmakerClaim__RMakerClaimPayload_Closure)
                 (const RmakerClaim__RMakerClaimPayload *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    rmaker_claim__rmaker_claim_status__descriptor;
extern const ProtobufCEnumDescriptor    rmaker_claim__rmaker_claim_msg_type__descriptor;
extern const ProtobufCMessageDescriptor rmaker_claim__payload_buf__descriptor;
extern const ProtobufCMessageDescriptor rmaker_claim__resp_payload__descriptor;
extern const ProtobufCMessageDescriptor rmaker_claim__rmaker_claim_payload__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_esp_5frmaker_5fclaim_2eproto__INCLUDED */
