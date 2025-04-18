package io.nanojvm.shenenigans;

public final class RuntimeInstrumentation {
    private RuntimeInstrumentation() {}
    
    public static native void instrumentMethodEntrance(String methodName, String methodDescriptor, MethodHook hook);
}
