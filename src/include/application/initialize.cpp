#include "globalFunctions.h"
#include <filesystem>
#include "optimization/handleError.h"
#include "gameInfo.h"
#include "../sound/soundHandler2D.h"

std::filesystem::path workingDirectory;

#if onWindows
#include "windowsIncluder.h"
#include <corecrt.h>
#include <shlobj.h>
#elif onAndroid
stdPath androidCacheDir;

// Since we want to get the native activity from SFML, we'll have to use an
// extra header here:
#include <SFML/System/NativeActivity.hpp>
#include <android/native_activity.h>
#include <jni.h>
// NDK/JNI sub example - call Java code from native code
// int vibrate(sf::Time duration)
//{
//    // First we'll need the native activity handle
//    ANativeActivity* activity = sf::getNativeActivity();
//
//    // Retrieve the JVM and JNI environment
//    JavaVM* vm  = activity->vm;
//    JNIEnv* env = activity->env;
//
//    // First, attach this thread to the main thread
//    JavaVMAttachArgs attachargs;
//    attachargs.version = JNI_VERSION_1_6;
//    attachargs.name    = "NativeThread";
//    attachargs.group   = nullptr;
//    jint res           = vm->AttachCurrentThread(&env, &attachargs);
//
//    if (res == JNI_ERR)
//        return EXIT_FAILURE;
//
//    // Retrieve class information
//    jclass natact  = env->FindClass("android/app/NativeActivity");
//    jclass context = env->FindClass("android/content/Context");
//
//    // Get the value of a constant
//    jfieldID fid    = env->GetStaticFieldID(context, "VIBRATOR_SERVICE", "Ljava/lang/String;");
//    jobject  svcstr = env->GetStaticObjectField(context, fid);
//
//    // Get the method 'getSystemService' and call it
//    jmethodID getss   = env->GetMethodID(natact, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
//    jobject   vib_obj = env->CallObjectMethod(activity->clazz, getss, svcstr);
//
//    // Get the object's class and retrieve the member name
//    jclass    vib_cls = env->GetObjectClass(vib_obj);
//    jmethodID vibrate = env->GetMethodID(vib_cls, "vibrate", "(J)V");
//
//    // Determine the timeframe
//    jlong length = duration.asMilliseconds();
//
//    // Bzzz!
//    env->CallVoidMethod(vib_obj, vibrate, length);
//
//    // Free references
//    env->DeleteLocalRef(vib_obj);
//    env->DeleteLocalRef(vib_cls);
//    env->DeleteLocalRef(svcstr);
//    env->DeleteLocalRef(context);
//    env->DeleteLocalRef(natact);
//
//    // Detach thread again
//    vm->DetachCurrentThread();
//    return 0;
//}
#endif

std::filesystem::path getCommonAppDataFolder()
{
#if onWindows
    TCHAR szPath[MAX_PATH];
    // https://stackoverflow.com/questions/2899013/how-do-i-get-the-application-data-path-in-windows-using-c
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
    {
        // to make this work, #define UNICODE is defined in "windowsIncluder.h"
        return std::filesystem::path(&szPath[0]);
    }
#elif onAndroid

    // First we'll need the native activity handle
    ANativeActivity *activity = sf::getNativeActivity();

    JNIEnv *env = activity->env;
    JavaVM *vm = activity->vm;

    // First, attach this thread to the main thread
    JavaVMAttachArgs attachargs;
    attachargs.version = JNI_VERSION_1_6;
    attachargs.name = "NativeThread";
    attachargs.group = nullptr;
    jint res = vm->AttachCurrentThread(&env, &attachargs);
    if (res == JNI_ERR)
    {
        handleError(L"couldn't attach to current thread");
        return L"";
    }

    // https://stackoverflow.com/questions/7701801/obtaining-the-name-of-an-android-apk-using-c-and-the-nativeactivity-class
    jobject obj = activity->clazz;
    jclass nativeActivityClass = env->GetObjectClass(obj);

    jmethodID getApplicationContextMethod = env->GetMethodID(nativeActivityClass,
                                                             "getApplicationContext",
                                                             "()Landroid/content/Context;");
    jobject applicationContext = env->CallObjectMethod(activity->clazz,
                                                       getApplicationContextMethod);

    jmethodID getClassLoader = env->GetMethodID(nativeActivityClass, "getClassLoader",
                                                "()Ljava/lang/ClassLoader;");
    jobject classLoader = env->CallObjectMethod(activity->clazz, getClassLoader);
    jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
    jmethodID findClass = env->GetMethodID(classLoaderClass, "loadClass",
                                           "(Ljava/lang/String;)Ljava/lang/Class;");
    jstring strClassName = env->NewStringUTF("com.johnheikens.medievalsurvival.JavaHelper");
    jclass javaHelperClass = (jclass)(env->CallObjectMethod(classLoader, findClass, strClassName));

    // jclass javaHelperClass = env->FindClass("com/johnheikens/medievalsurvival/JavaHelper");
    jmethodID initializeContextMethod = env->GetStaticMethodID(javaHelperClass, "initializeContext",
                                                               "(Landroid/app/Activity;)Ljava/lang/String;");
    jobject jExtractedAssetsDir = env->CallStaticObjectMethod(javaHelperClass,
                                                              initializeContextMethod,
                                                              activity->clazz);
    const char *assetsDir = env->GetStringUTFChars((jstring)jExtractedAssetsDir, nullptr);
    androidCacheDir = assetsDir;
    androidCacheDir = androidCacheDir.parent_path(); // remove the 'data/'

    env->ReleaseStringUTFChars((jstring)jExtractedAssetsDir, assetsDir);

    env->DeleteLocalRef(javaHelperClass);
    env->DeleteLocalRef(strClassName);
    env->DeleteLocalRef(classLoaderClass);
    env->DeleteLocalRef(classLoader);
    env->DeleteLocalRef(applicationContext);
    env->DeleteLocalRef(nativeActivityClass);

    vm->DetachCurrentThread();

    return androidCacheDir;
#elif onLinux
    return L"/user/share";

#elif onMac
    return L"/Library/Application Support/";
#endif

    handleError(L"common appdata folder not found");
    return std::filesystem::path();
}

void loadPath()
{
    if (onAndroid || !std::filesystem::exists(L"data"))
    { // this application is not deployed
        // change data folder to %appdata%/JohnHeikens/Medieval Survival/data
        //
        // TODO: get common appdata folder on every system
        std::filesystem::path commonAppDataFolder = getCommonAppDataFolder();
        std::filesystem::path newWorkingDirectory = onAndroid ? commonAppDataFolder : commonAppDataFolder / authorName / gameName;
        if (std::filesystem::exists(newWorkingDirectory))
        {
            std::filesystem::current_path(newWorkingDirectory); // setting path
#if onAndroid
            // copy files from APK to working directory

#endif
        }
        else
        {
            handleError(newWorkingDirectory.wstring() + L" not found");
        }
        //
        // check if it worked
        if (!std::filesystem::exists(L"data"))
            handleError(L"data folder not found in " + commonAppDataFolder.wstring());
    }
    workingDirectory = std::filesystem::current_path();
}
void initialize()
{
    loadPath();
}
