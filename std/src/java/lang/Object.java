package java.lang;

public class Object {

    public Object() {}

    public final native Class<?> getClass();

    public native int hashCode();

    public boolean equals(Object obj) {
        return this == obj;
    }

    protected void finalize() throws Throwable { } // Deprecated in favor of #destruct()

    // Calls on GC cleanup
    protected void destruct() throws Throwable {
    }
}
