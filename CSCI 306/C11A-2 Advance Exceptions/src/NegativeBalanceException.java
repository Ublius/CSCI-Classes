/**
 *  
 * @author Tristen Erpelding
 * 
 * Purpose: This class is used to simulate an ATM machine. It creates a BankAccount object and then attempts to withdraw money from the account.
 */
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class NegativeBalanceException extends Exception{
    private double negativeBalance;

    public NegativeBalanceException() {
        // Call the super constructor
        super("Error: negative balance");
    }

    public NegativeBalanceException(double negativeBalance) {
        // Call the super constructor
        super("Amount exceeds balance by " + negativeBalance);
        this.negativeBalance = negativeBalance;

        try {
            // Write the exception message to a file
            PrintWriter writer = new PrintWriter("logfile.txt");
            writer.println(getMessage());
            writer.close();
        } catch (FileNotFoundException e) {
            // Print the stack trace
            e.printStackTrace();
        }

    }

    @Override
    public String toString() {
        return "Balance of " + negativeBalance + " not allowed";
    }
}



