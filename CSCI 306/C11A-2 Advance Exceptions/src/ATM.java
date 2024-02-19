/**
 *  
 * @author Tristen Erpelding
 * 
 * Purpose: This class is used to simulate an ATM machine. It creates a BankAccount object and then attempts to withdraw money from the account.
 */
public class ATM {
    private BankAccount account;

    public ATM() {
        // Create a new bank account with a balance of 500
        account = new BankAccount(500);
    }

    public void handleTransaction() {
        try {
            // Withdraw 100 from the account
            account.withdraw(600);
        } catch (NegativeBalanceException e) {
            // If the balance is negative, print the exception and the message
            System.out.println(e);
            System.out.println(e.getMessage());
        }

        try {
            // Withdraw 100 from the account
            account.quickWithdraw(600);
        } catch (NegativeBalanceException e) {
            // If the balance is negative, print the exception and the message
            System.out.println(e);
            System.out.println(e.getMessage());
        }
    }

    public static void main(String[] args) {
        // Create a new ATM object and handle the transaction
        ATM atm = new ATM();
        atm.handleTransaction();
    }

}
