import io.nanojvm.Console;

public class Loops {

    private int i;

    public Loops() {
        i = 12;
    }

    public static void main(String[] args) {
        Loops l = new Loops();
        Console.puts(String.valueOf(l.getI()));
    }

    public void newI() {
        i = 24;
    }

    public int getI() {
        return i;
    }
}
