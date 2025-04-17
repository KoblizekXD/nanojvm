package io.nanojvm.intrinsics;

/**
 * Internal class for managing memory native memory states from Java in NanoJVM.
 */
public final class Memory {
    private Memory() {
    }

    public static native long getHeapArea();

    // Returns an address of given object in heap memory.
    public static native long ref(Object obj);
}
