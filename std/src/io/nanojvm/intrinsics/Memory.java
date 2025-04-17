package io.nanojvm.intrinsics;

/**
 * Internal class for managing memory native memory states from Java in NanoJVM.
 */
public final class Memory {
    private Memory() {
    }

    /**
     * @return the address of the beginning of the heap area. 
     */
    public static native long getHeapArea();

    /**
     * Returns the address of the object in the heap.
     * @param obj the object to get the address of
     * @return the address of the object in the heap, can be {@link #NULL} if the object is null 
     */
    public static native long ref(Object obj);
    
    public static final long NULL = 0L;
    
    @Naked
    public static native long malloc(long size);
}
