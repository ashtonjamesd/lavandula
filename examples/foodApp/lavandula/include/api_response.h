#ifndef api_response_h
#define api_response_h

#include "lavandula.h"

/*
** Provides a standardized API response in JSON format.
*/

JsonBuilder *apiResponse(char *message, bool success);

HttpResponse apiSuccess();
HttpResponse apiFailure(char *message);

#endif