package io.nanojvm.shenenigans;

/**
 * Interface for method hooks. This interface is used to intercept method calls and
 * perform custom actions before or after the method is executed.
 */
public interface MethodHook {
    /**
     * @param instance the instance of the invoking object. Will be null for static methods. Otherwise, never null.
     * @param args the arguments of the method. Will have length of 0 if the method has no arguments.
     * @return true if the method should be invoked, false otherwise. Cancelling the execution of the method will
     * result in no value being returned. Meaning the operand stack will be missing an entry.
     */
    boolean onInvoke(Object instance, Object[] args);
}
