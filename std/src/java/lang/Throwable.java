package java.lang;

import java.io.Serializable;

public class Throwable implements Serializable {
    private String detailMessage;

    public Throwable() {
    }

    public Throwable(String message) {
        this.detailMessage = message;
    }
}
