package com.test.cmaketest;

import com.test.cmaketest.impl.Managed;
import com.test.cmaketest.impl.Stats;

import java.lang.ref.*;

/**
 * Java mirror of std::vector&lt;jchar&gt; (UTF-16)
 */
public class U16String extends Managed {

    
    public U16String(long ptr) {
        super(ptr, _FinalizerHolder.PTR);
    }

    public U16String(String s) {
        this(_nMake(s));
        Stats.onNativeCall();
    }

    @Override
    public String toString() {
        try {
            Stats.onNativeCall();
            return _nToString(_ptr);
        } finally {
            Reference.reachabilityFence(this);
        }
    }

    
    public static class _FinalizerHolder {
        public static final long PTR = _nGetFinalizer();
    }

    public static native long _nMake(String s);
    public static native long _nGetFinalizer();
    public static native String _nToString(long ptr);
}