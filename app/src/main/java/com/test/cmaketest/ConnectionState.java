package com.test.cmaketest;

public enum ConnectionState {

    DISCONNECTED(SyncNetWorkState.CONNECTION_VALUE_DISCONNECTED),

    /**
     * A connection is currently in progress of being established. If successful the next
     * state is {@link #CONNECTED}. If the connection fails it will be {@link #DISCONNECTED}.
     */
    CONNECTING(SyncNetWorkState.CONNECTION_VALUE_CONNECTING),

    /**
     * A connection was successfully established to the server. If the SyncSession is {@link SyncSession.State#ACTIVE}
     * data will now be transferred between the device and the server.
     */
    CONNECTED(SyncNetWorkState.CONNECTION_VALUE_CONNECTED);

    final int value;

    ConnectionState(int value) {
        this.value = value;
    }

    static ConnectionState fromNativeValue(long value) {
        ConnectionState[] stateCodes = values();
        for (ConnectionState state : stateCodes) {
            if (state.value == value) {
                return state;
            }
        }
        throw new IllegalArgumentException("Unknown connection state code: " + value);
    }
}
