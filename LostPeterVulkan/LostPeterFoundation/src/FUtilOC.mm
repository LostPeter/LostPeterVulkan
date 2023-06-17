/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#import <Foundation/Foundation.h>
#include <mach-o/dyld.h>
#include "../include/FUtilOC.h"

std::string F_OCGetPathExecute()
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

CFBundleRef F_OCMac_LoadExeBundle(const char* name)
{
    CFBundleRef baseBundle = CFBundleGetBundleWithIdentifier(CFSTR("com.lostpeter.vulkan"));
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFStringRef nameRef = CFStringCreateWithCString(nullptr, name, kCFStringEncodingASCII);
    CFURLRef bundleURL = 0; //URL of bundle to load
    CFBundleRef bundle = 0; //bundle to load
    
    //cut off .bundle if present
    if(CFStringHasSuffix(nameRef, CFSTR(".bundle"))) 
    {
        CFStringRef nameTempRef = nameRef;
        long end = CFStringGetLength(nameTempRef) - CFStringGetLength(CFSTR(".bundle"));
        nameRef = CFStringCreateWithSubstring(nullptr, nameTempRef, CFRangeMake(0, end));
        CFRelease(nameTempRef);
    }
            
    //assume relative to Resources/ directory of Main bundle
    bundleURL = CFBundleCopyResourceURL(mainBundle, nameRef, CFSTR("bundle"), nullptr);
    if(bundleURL) 
    {
        bundle = CFBundleCreate(nullptr, bundleURL);
        CFRelease(bundleURL);
    }
    
    //otherwise, try Resources/ directory of DeepFun Framework bundle
    if(!bundle) 
    {
        bundleURL = CFBundleCopyResourceURL(baseBundle, nameRef, CFSTR("bundle"), nullptr);
        if(bundleURL) 
        {
            bundle = CFBundleCreate(nullptr, bundleURL);
            CFRelease(bundleURL);
        }
    }
    CFRelease(nameRef);
    
    if(bundle) 
    {
        if(CFBundleLoadExecutable(bundle)) 
        {
            return bundle;
        }
        else 
        {
            CFRelease(bundle);
        }
    }
    
    return 0;
}

void* F_OCMac_GetBundleSym(CFBundleRef bundle, const char* name) 
{
    CFStringRef nameRef = CFStringCreateWithCString(nullptr, name, kCFStringEncodingASCII);
    void *sym = CFBundleGetFunctionPointerForName(bundle, nameRef);
    CFRelease(nameRef);
    return sym;
}

bool F_OCMac_UnloadExeBundle(CFBundleRef bundle) 
{
    if(bundle) 
    {
        //no-op, can't unload Obj-C bundles without crashing
        return 0;
    }
    return 1;
}

void* F_OCMac_LoadDynamicLib(const char* name)
{
    std::string fullPath=name;
    if(name[0] != '/')
        fullPath = F_OCGetMacPluginPath() + "/" + fullPath;

    void* lib = dlopen(fullPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if( !lib )
    {
        // Try loading it by using the run-path (@rpath)
        lib = dlopen(name, RTLD_LAZY | RTLD_GLOBAL);
    }
    
    return lib;
}

void* F_OCMac_LoadFramework(std::string name)
{
    std::string fullPath;
    if (name[0] != '/') 
    {
        // just framework name: path/RendererMetal.framework/RendererMetal 
        fullPath = F_OCGetMacFrameworksPath() + "/" + name + ".framework/" + name;
    }
    else 
    { 
        // absolute path: "/Library/Frameworks/RendererMetal.framework"
        size_t lastSlashPos = name.find_last_of('/');
        size_t extensionPos = name.rfind(".framework");

        if (lastSlashPos != std::string::npos && extensionPos != std::string::npos) 
        {
            std::string realName = name.substr(lastSlashPos + 1, extensionPos - lastSlashPos - 1);
            fullPath = name + "/" + realName; 
        }
        else 
        {
            fullPath = name;
        }
    }

    return dlopen(fullPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
}

std::string F_OCGetMacBundlePath()
{
    char path[PATH_MAX];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    F_Assert(mainBundle)
    
    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    F_Assert(mainBundleURL)
    
    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    F_Assert(cfStringRef)
    
    CFStringGetFileSystemRepresentation(cfStringRef, path, PATH_MAX);
    
    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);
    
    return std::string(path);
}

std::string F_OCGetMacPluginPath()
{
    return F_OCGetMacBundlePath() + "/Contents/Plugins/";
}

std::string F_OCGetMacFrameworksPath()
{
    return F_OCGetMacBundlePath() + "/Contents/Frameworks/";
}

std::string F_OCGetMacCachePath()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDirectory = [paths objectAtIndex:0];
    NSString *bundleId = [[NSBundle mainBundle] bundleIdentifier];

    return [[cachesDirectory stringByAppendingPathComponent:bundleId] fileSystemRepresentation];
}

std::string F_OCGetMacTempFileName()
{
    NSString *tempFilePath;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    for (;;) 
    {
        NSString *baseName = [NSString stringWithFormat:@"tmp-%x", arc4random()];
        tempFilePath = [NSTemporaryDirectory() stringByAppendingPathComponent:baseName];
        if (![fileManager fileExistsAtPath:tempFilePath])
            break;
    }
    return std::string([tempFilePath fileSystemRepresentation]);
}

std::string F_OCGetMacResourcesPath()
{
    return std::string(NSBundle.mainBundle.resourceURL.path.UTF8String) + "/";
}

std::string F_OCGetMacLogPath()
{
    NSURL* libURL = [NSFileManager.defaultManager
        URLForDirectory: NSLibraryDirectory
        inDomain: NSUserDomainMask
        appropriateForURL: nil
        create: YES
        error: nil];
    NSURL* logURL = [libURL URLByAppendingPathComponent: @"Logs" isDirectory: YES];
    return std::string( logURL.absoluteURL.path.UTF8String ) + "/";
}