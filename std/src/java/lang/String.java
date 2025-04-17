package java.lang;

public class String {
    private final byte coder;
    private final char[] value;
    
    public String() {
        this.coder = 0;
        this.value = new char[0];
    }
    
    public byte[] getBytes() {
        return getBytes(0, value.length);
    }
    
    public byte[] getBytes(int beginIndex, int endIndex) {
        if (beginIndex < 0 || endIndex > value.length || beginIndex > endIndex) {
            throw new RuntimeException("Index out of bounds");
        }
        byte[] bytes = new byte[endIndex - beginIndex];
        for (int i = beginIndex; i < endIndex; i++) {
            bytes[i - beginIndex] = (byte) value[i];
        }
        return bytes;
    }
}
