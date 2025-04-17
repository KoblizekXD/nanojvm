package io.nanojvm.intrinsics;

/**
 * Marks a native method as naked. Parameters will be passed to naked methods directly, instead of using
 * wrapper parameters.
 * <p>
 *  For example, imagine a native function with following definition:
 *  {@code int putchar(int c);}
 *  Making a static native method with following definition:
 *  {@code public static native int putchar(int c);}
 *  Will result in directly calling the C standard library putchar function.
 *  If given function is not loaded, it can be done so with {@link java.lang.System#loadLibrary}.
 * </p>
 */
public @interface Naked {
}
