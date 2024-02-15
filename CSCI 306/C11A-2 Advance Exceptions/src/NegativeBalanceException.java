import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class NegativeBalanceException extends Exception{
    private double negativeBalance;

    public NegativeBalanceException() {
        super("Error: negative balance");
    }

    public NegativeBalanceException(double negativeBalance) {
        super("Amount exceeds balance by " + negativeBalance);
        this.negativeBalance = negativeBalance;

        try {
            PrintWriter writer = new PrintWriter("logfile.txt");
            writer.println(getMessage());
            writer.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

    }

    @Override
    public String toString() {
        return "Balance of " + negativeBalance + " not allowed";
    }
}



