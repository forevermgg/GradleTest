package com.test.cmaketest;

import androidx.annotation.Nullable;

public interface Capabilities {
    boolean canDeliverNotification();
    void checkCanDeliverNotification(@Nullable String exceptionMessage);
    boolean isMainThread();
}
