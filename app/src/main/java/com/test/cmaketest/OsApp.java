package com.test.cmaketest;

public class OsApp implements NativeObject {
    private static final long nativeFinalizerPtr = nativeGetFinalizerMethodPtr();
    private final long nativePtr;

    public OsApp() {
        synchronized (OsApp.class) { // We need to synchronize access as OS caches the App instance
            nativePtr = nativeCreate(
                    "AppId",
                    "AppName",
                    "AppVersion",
                    "android",
                    android.os.Build.VERSION.RELEASE,
                    "sdkVersion");
        }
    }

    @Override
    public long getNativePtr() {
        return this.nativePtr;
    }

    @Override
    public long getNativeFinalizerPtr() {
        return nativeFinalizerPtr;
    }

    private static native long nativeGetFinalizerMethodPtr();

    private native long nativeCreate(String appId,
                                     String appName,
                                     String appVersion,
                                     String platform,
                                     String platformVersion,
                                     String sdkVersion);
}
