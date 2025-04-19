package io.nanojvm;

import io.nanojvm.intrinsics.Naked;

public final class Console {
    
    private Console() {
    }
    
    @Naked
    public static native void puts(String str);
}
