// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>

//Windows Service not yet supported. 
int configureAsAService(void)
{
    return 0;
}


void waitForUserInput(void)
{
    while(1)
    {
       sleep(1000); //Stays here till the service is stopped. 
    } 
    // Interactive original: 
    //(void)printf("Press return to exit the application. \r\n");
    //(void)getchar();
}
