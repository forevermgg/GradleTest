package com.test.cmaketest;

import androidx.annotation.Keep;

@Keep
public final class ForeverError extends Error {
    public ForeverError(String detailMessage) {
        super(detailMessage);
    }
}
