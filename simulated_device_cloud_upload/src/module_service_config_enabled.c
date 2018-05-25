// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "azure_c_shared_utility/xlogging.h"
#include <unistd.h>

int configureAsAService(void)
{
    int returnValue;
    int functionReturn;
    
    printf("configureAsAService\n");

    returnValue = chdir("/");
    
    if (returnValue != 0)
    {
        printf("failed to change directory\n");
        LogError("Could not change working dir. Error Code: %d.", returnValue);
        functionReturn = __LINE__;
    }
    else
    {
        printf("Success directory\n");
        returnValue = daemon(0, 0);
        printf("Post daemon create\n");
        if (returnValue != 0)
        {
            printf("failed to create daemon\n");
            LogError("Could not detach program from terminal. Error Code: %d.", returnValue);
            functionReturn = __LINE__;
        }
        else
        {
            printf("success creating daemon\n");
            functionReturn = 0;
        }
    }

    return functionReturn;
}

void waitForUserInput(void)
{
    while(1)
    {
       sleep(1000); //Stays here till the service is stopped. 
    }       
}
