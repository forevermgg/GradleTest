package com.test.cmaketest;

import androidx.annotation.Nullable;

public class AndroidCapabilities implements Capabilities {
    @Override
    public boolean canDeliverNotification() {
        return false;
    }

    @Override
    public void checkCanDeliverNotification(@Nullable String exceptionMessage) {

    }

    @Override
    public boolean isMainThread() {
        return false;
    }
}
