#import <Foundation/Foundation.h>
#include <mach-o/dyld.h>
#include "../include/OCUtil.h"

std::string OCUtil_GetPathExecute()
{   
    char buf[0];
    uint32_t size = 0;
    int res = _NSGetExecutablePath(buf, &size);

    char* path = (char*)malloc(size + 1);
    path[size] = 0;
    res = _NSGetExecutablePath(path, &size);

    char* p = strrchr(path, '/');
    *p = 0;
    std::string pathTemp;
    pathTemp.append(path);
    free(path);
    return pathTemp;
}