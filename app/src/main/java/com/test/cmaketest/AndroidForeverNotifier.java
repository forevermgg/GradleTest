package com.test.cmaketest;

import android.os.Handler;
import android.os.Looper;

public class AndroidForeverNotifier extends ForeverNotifier{
    private Handler handler;

    public AndroidForeverNotifier(Capabilities capabilities) {
        if (capabilities.canDeliverNotification()) {
            handler = new Handler(Looper.myLooper());
        } else {
            handler = null;
        }
    }

    @Override
    public boolean post(Runnable runnable) {
        return handler != null && handler.post(runnable);
    }
}
