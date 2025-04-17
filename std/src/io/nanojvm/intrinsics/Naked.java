package io.nanojvm.intrinsics;

/**
 * Indicates that a static native method should be invoked in a "naked" form,
 * where parameters are passed directly in their native representation without
 * any Java object wrapping or translation.
 * <p>
 * This allows for high-performance interop between Java and native code, 
 * bypassing typical JNI object conversions. For example, a {@code String}
 * parameter may be passed as a {@code const char*}, and primitive types
 * will be passed directly as their corresponding C types.
 * </p>
 *
 * <p><b>Usage Requirements:</b></p>
 * <ul>
 *   <li>This annotation is only applicable to {@code static native} methods.</li>
 *   <li>The native side must handle raw native parameters and understand the
 *       expected layout and memory representation.</li>
 * </ul>
 *
 * <p><b>Example:</b></p>
 * <pre>{@code
 * @Naked
 * public static native int doSomething(String input, int count);
 * }</pre>
 *
 * <p>In the example above, {@code input} may be passed as a {@code const char*}
 * directly, and {@code count} as an {@code int32_t} in native code.</p>
 *
 * <p><b>Warning:</b> Improper use can lead to memory corruption or crashes.
 * Ensure native methods are carefully implemented to match the calling convention
 * and parameter formats expected by this annotation.</p>
 *
 * @see Memory#malloc(long) 
 * @implNote This annotation is part of the NanoJVM project and is experimental.
 */
// @Target(ElementType.METHOD)
// @Retention(RetentionPolicy.CLASS)
public @interface Naked {
}
