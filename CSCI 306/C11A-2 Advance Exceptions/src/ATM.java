public class ATM {
    private BankAccount account;

    public ATM() {
        account = new BankAccount(500);
    }

    public void handleTransaction() {
        try {
            account.withdraw(600);
        } catch (NegativeBalanceException e) {
            System.out.println(e);
            System.out.println(e.getMessage());
        }

        try {
            account.quickWithdraw(600);
        } catch (NegativeBalanceException e) {
            System.out.println(e);
            System.out.println(e.getMessage());
        }
    }

    public static void main(String[] args) {
        ATM atm = new ATM();
        atm.handleTransaction();
    }

}
