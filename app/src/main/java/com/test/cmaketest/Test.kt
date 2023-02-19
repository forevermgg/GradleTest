package com.test.cmaketest

import android.util.Log

object Test {
    fun notifyConnectionListeners(oldState: Long, newState: Long) {
        Log.e("ConnectionState", "oldState:$oldState newState:$newState")
    }
}