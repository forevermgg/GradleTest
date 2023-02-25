package com.test.cmaketest;

import android.util.Log;

import java.io.Closeable;

public abstract class ForeverNotifier implements Closeable {

    protected void RealmNotifier() {
    }

    // Called from JavaBindingContext::did_change.
    @SuppressWarnings("unused")
    // called from java_binding_context.cpp
    void didChange() {
    }

    // Called from JavaBindingContext::before_notify.
    // Package protected to avoid finding class by name in JNI.
    @SuppressWarnings("unused")
    void beforeNotify() {
        Log.e("mgg", "beforeNotify");
    }

    // Called from JavaBindingContext::will_send_notifications
    // This will be called before any change notifications are delivered when updating a
    // Realm version. This will be triggered even if no change listeners are registered.
    void willSendNotifications() {
    }

    // Called from JavaBindingContext::will_send_notifications
    void didSendNotifications() {
    }

    /**
     * Called when close OsSharedRealm to clean up any event left in to queue.
     */
    @Override
    public void close() {
    }

    /**
     * For current implementation of async transaction only. See comments for {@link #transactionCallbacks}.
     *
     * @param runnable to be executed in the following event loop.
     */
    public abstract boolean post(Runnable runnable);
}
