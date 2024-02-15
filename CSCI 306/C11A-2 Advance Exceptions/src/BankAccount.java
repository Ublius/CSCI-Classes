public class BankAccount {
    private double balance;

    public BankAccount(double balance) {
        this.balance = balance;
    }

    public void withdraw(double amount) throws NegativeBalanceException {
        if (amount > balance) {
            throw new NegativeBalanceException(amount - balance);
        } else {
            balance -= amount;
        }
    }

    public void quickWithdraw(double amount) throws NegativeBalanceException {
        if (amount > balance) {
            throw new NegativeBalanceException();
        } else {
            balance -= amount;
        }
    }

}
