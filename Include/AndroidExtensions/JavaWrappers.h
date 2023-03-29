#pragma once

#include <jni.h>
#include <string>
#include <vector>
#include <cstddef>
#include <android/asset_manager.h>
#include <android/native_window.h>
#include <unordered_map>

// --------------------
// Forward Declarations
// --------------------

namespace java::lang
{
    class ByteArray;
    class Object;
    class String;
    class Throwable;
}

namespace java::websocket
{
    class WebSocketClient;
}

namespace java::io
{
    class ByteArrayOutputStream;
    class InputStream;
    class OutputStream;
    class OutputStreamWriter;
}

namespace java::net
{
    class HttpURLConnection;
    class URL;
    class URLConnection;
}

namespace android
{
    class ManifestPermission;
}

namespace android::app
{
    class Activity;
}

namespace android::content
{
    class Context;
}

namespace android::content::res
{
    class AssetManager;
    class Resources;
    class Configuration;
}

namespace android::net
{
    class Uri;
}

// ------------
// Declarations
// ------------

namespace java::lang
{
    class ByteArray
    {
    public:
        ByteArray(int size);
        ByteArray(jbyteArray byteArray);

        operator jbyteArray() const;

        operator std::vector<std::byte>() const;

    protected:
        JNIEnv* m_env;
        jbyteArray m_byteArray;
    };

    class Class final
    {
    public:
        Class(const char* className);
        Class(const jclass classObj);
        ~Class();

        Class(const Class&);
        Class& operator=(const Class&);

        Class(Class&&);
        Class& operator=(Class&&);

        operator jclass() const;

        bool IsAssignableFrom(Class otherClass);

    private:
        jclass JClass() const;
        void JClass(jclass classObj);

        JNIEnv* m_env;
        jclass m_class;
    };

    class Object
    {
    public:
        operator jobject() const;
        Class GetClass();
        ~Object();

    protected:
        Object(const char* className);
        Object(jclass classObj);
        Object(jobject object);

        Object(const Object&);
        Object& operator=(const Object&);

        Object(Object&&);
        Object& operator=(Object&&);

        jobject JObject() const;
        void JObject(jobject object);

        JNIEnv* m_env;
        Class m_class;

    private:
        jobject m_object;

    };

    class String
    {
    public:
        String(jstring string);
        String(const char* string);

        operator jstring() const;

        operator std::string() const;

    protected:
        JNIEnv* m_env;
        jstring m_string;
    };

    class Throwable : public Object, public std::exception
    {
    public:
        Throwable(jthrowable throwable);
        ~Throwable();

        String GetMessage() const;

        const char* what() const noexcept override;

    private:
        jobject m_throwableRef;
        std::string m_message;
    };
}

namespace java::websocket
{
    class WebSocketClient : public lang::Object
    {
    public:
        WebSocketClient(std::string url, std::function<void()> open_callback, std::function<void()> close_callback, std::function<void(std::string)> message_callback, std::function<void()> error_callback);
        void Send(std::string message);
        void Close();

    private:
        static void OnOpen(JNIEnv* env, jobject obj);
        static void OnMessage(JNIEnv* env, jobject obj, jstring message);
        static void OnClose(JNIEnv* env, jobject obj);
        static void OnError(JNIEnv* env, jobject obj);

        static std::unordered_map<jobject, java::websocket::WebSocketClient*>::iterator FindClientInstance(JNIEnv* env, jobject obj);
        static std::unordered_map<jobject, java::websocket::WebSocketClient*> s_objectMap;
        std::function<void()> m_openCallback;
        std::function<void(std::string)> m_messageCallback;
        std::function<void()> m_closeCallback;
        std::function<void()> m_errorCallback;

    };
}

namespace java::io
{
    class ByteArrayOutputStream : public lang::Object
    {
    public:
        ByteArrayOutputStream();
        ByteArrayOutputStream(int size);
        ByteArrayOutputStream(jobject object);

        void Write(lang::ByteArray b, int off, int len);

        lang::ByteArray ToByteArray() const;

        lang::String ToString(const char* charsetName) const;
    };

    class InputStream : public lang::Object
    {
    public:
        InputStream(jobject object);

        int Read(lang::ByteArray byteArray) const;
    };

    class OutputStream : public lang::Object
    {
    public:
        OutputStream(jobject object);
    };

    class OutputStreamWriter : public lang::Object
    {
    public:
        OutputStreamWriter(jobject object);

        void Write(std::string postBody);

        void Close();
    };
}

namespace java::net
{
    class HttpURLConnection : public lang::Object
    {
    public:
        static lang::Class Class();

        HttpURLConnection(jobject object);

        int GetResponseCode() const;

        void SetRequestMethod(const std::string& requestMethod);
    };

    class URL : public lang::Object
    {
    public:
        URL(jobject object);
        URL(lang::String url);

        URLConnection OpenConnection();

        lang::String ToString();
    };

    class URLConnection : public lang::Object
    {
    public:
        URLConnection(jobject object);

        void Connect();

        bool GetDoOutput() const;

        void SetDoOutput(bool n);

        void SetRequestProperty(const std::string& key, const std::string& value);

        URL GetURL() const;

        int GetContentLength() const;

        lang::String GetHeaderField(int n) const;

        lang::String GetHeaderFieldKey(int n) const;

        io::InputStream GetInputStream() const;

        io::OutputStream GetOutputStream() const;

        explicit operator HttpURLConnection() const;
    };
}

namespace android
{
    class ManifestPermission
    {
    public:
        static jstring CAMERA();

    private:
        static jstring getPermissionName(const char* permissionName);
    };
}

namespace android::app
{
    class Activity : public java::lang::Object
    {
    public:
        Activity(jobject object);

        void requestPermissions(jstring systemPermissionName, int permissionRequestID);
    };
}

namespace android::content
{
    class Context : public java::lang::Object
    {
    public:
        Context(jobject object);

        Context getApplicationContext();

        res::AssetManager getAssets() const;

        res::Resources getResources();

        template<typename ServiceT>
        ServiceT getSystemService()
        {
            return {getSystemService(ServiceT::ServiceName)};
        };

        jobject getSystemService(const char* serviceName);

        bool checkSelfPermission(jstring systemPermissionName);
    };
}

namespace android::content::res
{
    class AssetManager : public java::lang::Object
    {
    public:
        AssetManager(jobject object);

        operator AAssetManager*() const;
    };

    class Resources : public java::lang::Object
    {
    public:
        Resources(jobject object);

        Configuration getConfiguration();
    };

    class Configuration : public java::lang::Object
    {
    public:
        Configuration(jobject object);

        int getDensityDpi();
    };
}

namespace android::graphics
{
    class SurfaceTexture : public java::lang::Object
    {
    public:
        SurfaceTexture();
        void InitWithTexture(int texture);
        void updateTexImage() const;
        void setDefaultBufferSize(int width, int height);
    };
}

namespace android::view
{
    class Display : public java::lang::Object
    {
    public:
        Display(jobject object);

        int getRotation();
    };

    class WindowManager : public java::lang::Object
    {
    public:
        static constexpr const char* ServiceName{"window"};
        WindowManager(jobject object);

        Display getDefaultDisplay();
    };

    class Surface : public java::lang::Object
    {
    public:
        Surface(android::graphics::SurfaceTexture& surfaceTexture);
    };
}

namespace android::net
{
    class Uri : public java::lang::Object
    {
    public:
        Uri(jobject object);

        java::lang::String getScheme() const;

        java::lang::String getPath() const;

        static Uri Parse(java::lang::String uriString);
    };
}
