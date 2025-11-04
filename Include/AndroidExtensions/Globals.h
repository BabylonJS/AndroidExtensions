#pragma once

#include <jni.h>
#include <android/asset_manager_jni.h>
#include <arcana/containers/ticketed_collection.h>
#include "JavaWrappers.h"

namespace android::global
{
    void Initialize(JavaVM* javaVM, jobject context);
    // Passing nullptr for appContext/assetManager clears cached globals so embedders can
    // explicitly tear down between reloads, or supply only an asset manager when no stable
    // Java Context is available (e.g., headless/unit-test harnesses).
    void Initialize(JavaVM* javaVM, jobject appContext, jobject assetManager);

    JNIEnv* GetEnvForCurrentThread();

    android::content::Context GetAppContext();

    void SetAssetManager(jobject assetManager);
    AAssetManager* GetAssetManager();

    android::app::Activity GetCurrentActivity();
    void SetCurrentActivity(jobject currentActivity);
    using AppStateChangedCallback = std::function<void()>;
    using AppStateChangedCallbackTicket = arcana::ticketed_collection<AppStateChangedCallback>::ticket;

    void Pause();
    AppStateChangedCallbackTicket AddPauseCallback(std::function<void()>&&);

    void Resume();
    AppStateChangedCallbackTicket AddResumeCallback(std::function<void()>&&);

    using RequestPermissionsResultCallback = std::function<void(int32_t, const std::vector<std::string>&, const std::vector<int32_t>&)>;
    using RequestPermissionsResultCallbackTicket = arcana::ticketed_collection<RequestPermissionsResultCallback>::ticket;

    void RequestPermissionsResult(int32_t, const std::vector<std::string>&, const std::vector<int32_t>&);
    RequestPermissionsResultCallbackTicket AddRequestPermissionsResultCallback(RequestPermissionsResultCallback&&);
}
