package com.test.cmaketest;

public interface ConnectionListener {

    /**
     * A change in the connection to the server was detected.
     *
     * @param oldState the state the connection transitioned from.
     * @param newState the state the connection transitioned to.
     */
    void onChange(ConnectionState oldState, ConnectionState newState);
}
