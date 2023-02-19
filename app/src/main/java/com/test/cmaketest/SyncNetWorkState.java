package com.test.cmaketest;

import android.util.Log;

import java.util.concurrent.CopyOnWriteArrayList;

public class SyncNetWorkState {
    static final byte CONNECTION_VALUE_DISCONNECTED = 0;
    static final byte CONNECTION_VALUE_CONNECTING = 1;
    static final byte CONNECTION_VALUE_CONNECTED = 2;
    private final long appNativePointer;
    private final CopyOnWriteArrayList<ConnectionListener> connectionListeners = new CopyOnWriteArrayList<>();
    private long nativeConnectionListenerToken;

    SyncNetWorkState(long appNativePointer) {
        this.appNativePointer = appNativePointer;
    }

    private static native void nativeRemoveConnectionListener(long appNativePointer, long listenerId);

    private static native byte nativeGetConnectionState(long appNativePointer);

    private native long nativeAddConnectionListener(long appNativePointer);

    /**
     * Get the current state of the connection used by the session as defined in {@link ConnectionState}.
     *
     * @return the state of connection used by the session.
     * @see ConnectionState
     */
    public ConnectionState getConnectionState() {
        byte state = nativeGetConnectionState(appNativePointer);
        if (state == -1) {
            // session was not found, probably the Realm was closed
            throw new IllegalStateException("Could not find session, Realm was probably closed");
        }
        return ConnectionState.fromNativeValue(state);
    }

    public boolean isConnected() {
        ConnectionState connectionState = ConnectionState.fromNativeValue(nativeGetConnectionState(appNativePointer));
        return connectionState == ConnectionState.CONNECTED;
    }

    void notifyConnectionListeners(long oldState, long newState) {
        for (ConnectionListener listener : connectionListeners) {
            try {
                listener.onChange(ConnectionState.fromNativeValue(oldState), ConnectionState.fromNativeValue(newState));
            } catch (Exception exception) {
                Log.e("SyncNetWorkState", exception.getLocalizedMessage());
            }
        }
    }

    public synchronized void addConnectionChangeListener(ConnectionListener listener) {
        if (connectionListeners.isEmpty()) {
            nativeConnectionListenerToken = nativeAddConnectionListener(appNativePointer);
        }
        connectionListeners.add(listener);
    }

    public synchronized void removeConnectionChangeListener(ConnectionListener listener) {
        connectionListeners.remove(listener);
        if (connectionListeners.isEmpty()) {
            nativeRemoveConnectionListener(appNativePointer, nativeConnectionListenerToken);
        }
    }
}
