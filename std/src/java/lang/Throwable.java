package java.lang;

import java.io.Serializable;

public class Throwable implements Serializable {
    
    private final String detailMessage;
    private final Throwable cause;

    public Throwable() {
        cause = null;
        detailMessage = null;
    }

    public Throwable(String message) {
        this.detailMessage = message;
        cause = null;
    }
    
    public Throwable(String message, Throwable cause) {
        this.detailMessage = message;
        this.cause = cause;
    }

    public String getMessage() {
        return detailMessage;
    }
    
    public Throwable getCause() {
        return cause;
    }
}
