package io.nanojvm.shenenigans;

/**
 * Utilities for runtime VM instrumentation.
 */
public final class RuntimeInstrumentation {
    private RuntimeInstrumentation() {}

    /**
     * Adds a method entrance instrumentation hook.
     * @param methodName the name of the method to instrument
     * @param methodDescriptor the method descriptor
     * @param hook the hook to be called when the method is entered
     */
    public static native void instrumentMethodEntrance(String methodName, String methodDescriptor, MethodHook hook);
}
