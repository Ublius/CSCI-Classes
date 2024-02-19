/**
 *  
 * @author Tristen Erpelding
 * 
 * Purpose: This class is used to simulate an ATM machine. It creates a BankAccount object and then attempts to withdraw money from the account.
 */
public class BankAccount {
    private double balance;

    public BankAccount(double balance) {
        // Set the balance
        this.balance = balance;
    }

    public void withdraw(double amount) throws NegativeBalanceException {
        if (amount > balance) {
            // Throw a new exception with the amount that exceeds the balance
            throw new NegativeBalanceException(amount - balance);
        } else {
            balance -= amount;
        }
    }

    public void quickWithdraw(double amount) throws NegativeBalanceException {
        if (amount > balance) {
            // Throw a new exception with the amount that exceeds the balance
            throw new NegativeBalanceException();
        } else {
            balance -= amount;
        }
    }

}
