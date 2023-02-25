package com.test.cmaketest;

public interface ForeverChangedCallback {
    // Called from JNI
    @SuppressWarnings("unused")
    void onForeverChangedChanged();
}
