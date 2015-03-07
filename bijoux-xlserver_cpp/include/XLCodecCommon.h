

#include "XLCall.h"

#include <cstdio>

#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#define STREAM_BUF_SIZE 4096

#define XL_CODEC_TYPE_NUM 0x1
#define XL_CODEC_TYPE_STR 0x2
#define XL_CODEC_TYPE_BOOL 0x3
#define XL_CODEC_TYPE_ERR 0x4
#define XL_CODEC_TYPE_MULTI 0x5
#define XL_CODEC_TYPE_MISSING 0x6
#define XL_CODEC_TYPE_NIL 0x7
#define XL_CODEC_TYPE_INT 0x8
#define XL_CODEC_TYPE_SREF 0x9
#define XL_CODEC_TYPE_BIGDATA 0xA

