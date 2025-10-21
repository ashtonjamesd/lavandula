#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/http.h"

#define MAX_BODY_SIZE (10 * 1024 * 1024) // 10 MiB

static HttpMethod toHttpMethod(char *s) {
    if (strcmp(s, "GET") == 0) {
        return HTTP_GET;
    }
    if (strcmp(s, "POST") == 0) {
        return HTTP_POST;
    }
    if (strcmp(s, "PUT") == 0) {
        return HTTP_PUT;
    }
    if (strcmp(s, "PATCH") == 0) {
        return HTTP_PATCH;
    }
    if (strcmp(s, "DELETE") == 0) {
        return HTTP_DELETE;
    }

    return HTTP_GET;
}

const char* httpMethodToStr(HttpMethod method) {
    switch (method) {
        case HTTP_GET: {
            return "GET";
        }
        case HTTP_POST: {
            return "POST";
        }
        case HTTP_PUT: {
            return "PUT";
        }
        case HTTP_PATCH: {
            return "PATCH";
        }
        case HTTP_DELETE: {
            return "DELETE";
        }
        case HTTP_OPTIONS: {
            return "OPTIONS";
        }
        default: {
            return "UNKNOWN";
        }
    }
}

const char* httpStatusCodeToStr(HttpStatusCode code) {
    switch (code) {
        case HTTP_CONTINUE: {
            return "Continue";
        }
        case HTTP_SWITCHING_PROTOCOLS: {
            return "Switching Protocols";
        }
        case HTTP_PROCESSING: {
            return "Processing";
        }
        case HTTP_EARLY_HINTS: {
            return "Early Hints";
        }

        case HTTP_OK: {
            return "OK";
        }
        case HTTP_CREATED: {
            return "Created";
        }
        case HTTP_ACCEPTED: {
            return "Accepted";
        }
        case HTTP_NON_AUTHORITATIVE_INFORMATION: {
            return "Non-Authoritative Information";
        }
        case HTTP_NO_CONTENT: {
            return "No Content";
        }
        case HTTP_RESET_CONTENT: {
            return "Reset Content";
        }
        case HTTP_PARTIAL_CONTENT: {
            return "Partial Content";
        }
        case HTTP_MULTI_STATUS: {
            return "Multi-Status";
        }
        case HTTP_ALREADY_REPORTED: {
            return "Already Reported";
        }
        case HTTP_IM_USED: {
            return "IM Used";
        }

        case HTTP_MULTIPLE_CHOICES: {
            return "Multiple Choices";
        }
        case HTTP_MOVED_PERMANENTLY: {
            return "Moved Permanently";
        }
        case HTTP_FOUND: {
            return "Found";
        }
        case HTTP_SEE_OTHER: {
            return "See Other";
        }
        case HTTP_NOT_MODIFIED: {
            return "Not Modified";
        }
        case HTTP_USE_PROXY: {
            return "Use Proxy";
        }
        case HTTP_TEMPORARY_REDIRECT: {
            return "Temporary Redirect";
        }
        case HTTP_PERMANENT_REDIRECT: {
            return "Permanent Redirect";
        }

        case HTTP_BAD_REQUEST: {
            return "Bad Request";
        }
        case HTTP_UNAUTHORIZED: {
            return "Unauthorized";
        }
        case HTTP_PAYMENT_REQUIRED: {
            return "Payment Required";
        }
        case HTTP_FORBIDDEN: {
            return "Forbidden";
        }
        case HTTP_NOT_FOUND: {
            return "Not Found";
        }
        case HTTP_METHOD_NOT_ALLOWED: {
            return "Method Not Allowed";
        }
        case HTTP_NOT_ACCEPTABLE: {
            return "Not Acceptable";
        }
        case HTTP_PROXY_AUTHENTICATION_REQUIRED: {
            return "Proxy Authentication Required";
        }
        case HTTP_REQUEST_TIMEOUT: {
            return "Request Timeout";
        }
        case HTTP_CONFLICT: {
            return "Conflict";
        }
        case HTTP_GONE: {
            return "Gone";
        }
        case HTTP_LENGTH_REQUIRED: {
            return "Length Required";
        }
        case HTTP_PRECONDITION_FAILED: {
            return "Precondition Failed";
        }
        case HTTP_PAYLOAD_TOO_LARGE: {
            return "Payload Too Large";
        }
        case HTTP_URI_TOO_LONG: {
            return "URI Too Long";
        }
        case HTTP_UNSUPPORTED_MEDIA_TYPE: {
            return "Unsupported Media Type";
        }
        case HTTP_RANGE_NOT_SATISFIABLE: {
            return "Range Not Satisfiable";
        }
        case HTTP_EXPECTATION_FAILED: {
            return "Expectation Failed";
        }
        case HTTP_IM_A_TEAPOT: {
            return "I'm a teapot";
        }
        case HTTP_MISDIRECTED_REQUEST: {
            return "Misdirected Request";
        }
        case HTTP_UNPROCESSABLE_ENTITY: {
            return "Unprocessable Entity";
        }
        case HTTP_LOCKED: {
            return "Locked";
        }
        case HTTP_FAILED_DEPENDENCY: {
            return "Failed Dependency";
        }
        case HTTP_TOO_EARLY: {
            return "Too Early";
        }
        case HTTP_UPGRADE_REQUIRED: {
            return "Upgrade Required";
        }
        case HTTP_PRECONDITION_REQUIRED: {
            return "Precondition Required";
        }
        case HTTP_TOO_MANY_REQUESTS: {
            return "Too Many Requests";
        }
        case HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE: {
            return "Request Header Fields Too Large";
        }
        case HTTP_UNAVAILABLE_FOR_LEGAL_REASONS: {
            return "Unavailable For Legal Reasons";
        }

        case HTTP_INTERNAL_SERVER_ERROR: {
            return "Internal Server Error";
        }
        case HTTP_NOT_IMPLEMENTED: {
            return "Not Implemented";
        }
        case HTTP_BAD_GATEWAY: {
            return "Bad Gateway";
        }
        case HTTP_SERVICE_UNAVAILABLE: {
            return "Service Unavailable";
        }
        case HTTP_GATEWAY_TIMEOUT: {
            return "Gateway Timeout";
        }
        case HTTP_HTTP_VERSION_NOT_SUPPORTED: {
            return "HTTP Version Not Supported";
        }
        case HTTP_VARIANT_ALSO_NEGOTIATES: {
            return "Variant Also Negotiates";
        }
        case HTTP_INSUFFICIENT_STORAGE: {
            return "Insufficient Storage";
        }
        case HTTP_LOOP_DETECTED: {
            return "Loop Detected";
        }
        case HTTP_NOT_EXTENDED: {
            return "Not Extended";
        }
        case HTTP_NETWORK_AUTHENTICATION_REQUIRED: {
            return "Network Authentication Required";
        }

        default: {
            return "Unknown HTTP Status Code";
        }
    }
}

static inline char currentChar(HttpParser *parser) {
    return parser->requestBuffer[parser->position];
}

static inline void advance(HttpParser *parser) {
    parser->position++;
}

static inline bool isEnd(HttpParser parser) {
    return parser.position >= parser.requestLength;
}

static void skipWhitespace(HttpParser *parser) {
    while (!isEnd(*parser) && (currentChar(parser) == ' ' || currentChar(parser) == '\r' || currentChar(parser) == '\n')) {
        advance(parser);
    }
}

static char *parseTokenUntil(HttpParser *parser, char delimiter) {
    int capacity = 32;
    int length = 0;
    char *token = malloc(capacity);

    if (!token) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    while (!isEnd(*parser) && currentChar(parser) != delimiter && currentChar(parser) != '\r' && currentChar(parser) != '\n') {
        if (length + 1 >= capacity) {
            capacity *= 2;
            token = realloc(token, capacity);

            if (!token) {
                fprintf(stderr, "Fatal: out of memory\n");
                exit(EXIT_FAILURE);
            }
        }
        token[length++] = currentChar(parser);
        advance(parser);
    }
    token[length] = '\0';
    return token;
}

static void parseHeaders(HttpParser *parser) {
    parser->request.headerCapacity = 8;
    parser->request.headerCount = 0;
    parser->request.headers = malloc(sizeof(Header) * parser->request.headerCapacity);

    if (!parser->request.headers) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    while (!isEnd(*parser)) {
        if (currentChar(parser) == '\r' && parser->position + 1 < parser->requestLength && parser->requestBuffer[parser->position + 1] == '\n') {
            advance(parser);
            advance(parser);
            break;
        }

        char *name = parseTokenUntil(parser, ':');
        if (!isEnd(*parser) && currentChar(parser) == ':') advance(parser);
        skipWhitespace(parser);
        char *value = parseTokenUntil(parser, '\r');

        if (!isEnd(*parser) && currentChar(parser) == '\r') advance(parser);
        if (!isEnd(*parser) && currentChar(parser) == '\n') advance(parser);

        if (parser->request.headerCount >= parser->request.headerCapacity) {
            parser->request.headerCapacity *= 2;
            parser->request.headers = realloc(parser->request.headers, sizeof(Header) * parser->request.headerCapacity);
        }

        if (!parser->request.headers) {
            fprintf(stderr, "Fatal: out of memory\n");
            exit(EXIT_FAILURE);
        }

        int headerCount = parser->request.headerCount;

        strncpy(parser->request.headers[headerCount].name, name, MAX_HEADER_NAME - 1);
        parser->request.headers[headerCount].name[MAX_HEADER_NAME - 1] = '\0';
        strncpy(parser->request.headers[headerCount].value, value, MAX_HEADER_VALUE - 1);
        parser->request.headers[headerCount].value[MAX_HEADER_VALUE - 1] = '\0';

        parser->request.headerCount++;

        free(name);
        free(value);
    }
}

void printHeaders(HttpParser *parser) {
    printf("\nRequest:\n");
    printf("  Method: %s\n", httpMethodToStr(parser->request.method));
    printf("  Resource: %s\n", parser->request.resource);

    printf("Headers (%ld):\n", parser->request.headerCount);
    for (size_t i = 0; i < parser->request.headerCount; i++) {
        printf("  HEADER '%s':  %s\n", parser->request.headers[i].name, parser->request.headers[i].value);
    }
}

HttpParser parseRequest(char *request) {
    HttpParser parser = {
        .isValid = true,
        .requestBuffer = strdup(request),
        .requestLength = strlen(request),
        .position = 0,
    };

    char method[16];
    int methodIndex = 0;
    while (!isEnd(parser) && currentChar(&parser) != ' ' && methodIndex < (int)sizeof(method) - 1) {
        method[methodIndex++] = currentChar(&parser);
        advance(&parser);
    }
    method[methodIndex] = '\0';

    if (!isEnd(parser)) advance(&parser);

    char *url = parseTokenUntil(&parser, ' ');
    if (!isEnd(parser)) advance(&parser);

    parser.request.method = toHttpMethod(method);
    parser.request.resource = url;

    char *version = parseTokenUntil(&parser, '\r');
    strncpy(parser.request.version, version, sizeof(parser.request.version) - 1);
    parser.request.version[sizeof(parser.request.version) - 1] = '\0';
    free(version);

    if (!isEnd(parser) && currentChar(&parser) == '\r') advance(&parser);
    if (!isEnd(parser) && currentChar(&parser) == '\n') advance(&parser);

    parseHeaders(&parser);

    // printf("Received request:\n%s\n", parser.requestBuffer);

    bool hasContentLength = false;
    long long contentLengthVal = 0;

    parser.request.bodyLength = 0;

    for (size_t i = 0; i < parser.request.headerCount; i++) {
        Header header = parser.request.headers[i];

        if (strcasecmp(header.name, "Content-Length") == 0) {
            char *endptr = NULL;
            long long val = strtoll(header.value, &endptr, 10);

            if (val < 0 || endptr == header.value) {
                parser.isValid = false;
                return parser;
            }

            hasContentLength = true;
            contentLengthVal = val;

            break;
        }
    }

    if (hasContentLength) {
        if (contentLengthVal > MAX_BODY_SIZE) {
            parser.isValid = false;
            return parser;
        }

        if ((unsigned long long)contentLengthVal > (size_t) -1) {
            parser.isValid = false;
            return parser;
        }
        size_t contentLength = (size_t)contentLengthVal;

        if (parser.position + contentLength > parser.requestLength) {
            parser.isValid = false;
            return parser;
        }

        size_t alloc_size = contentLength + 1;
        parser.request.body = malloc(alloc_size);

        if (!parser.request.body) {
            parser.isValid = false;
            return parser;
        }

        memcpy(parser.request.body, parser.requestBuffer + parser.position, contentLength);
        parser.request.body[contentLength] = '\0';

        parser.request.bodyLength = contentLength;

        parser.position += contentLength;
    }

    // for (int i = parser.position; i < parser.position + parser.request.bodyLength; i++) {
    //     parser.request.body[i - parser.position] = parser.requestBuffer[i];
    // }

    // printHeaders(&parser);

    return parser;
}

void freeParser(HttpParser *parser) {
    if (!parser) return;

    free(parser->requestBuffer);
    free(parser->request.resource);

    free(parser->request.headers);

    free(parser->request.body);
}