import java.util.ArrayList;

public class TheMotivator {
    private ArrayList<String> names = new ArrayList<String>();

    public void createNames() {
        names.add("TheMotivator");
        names.add("TheMotivator2");
        names.add("TheMotivator3");
    }

	public void feedback(int score) {
		if (score == 100)
			System.out.println("You're awesome");
		else if (score > 90)
			System.out.println("That's great");
		else if (score > 60)
			System.out.println("That's good ");
		else
			System.out.println("Well, what can I say?");
	}

    public void name() {
        for (String name : names) {
            System.out.println(name);
        }
        System.out.println("I motivate");
    }

	public static void main(String[] args) {
		TheMotivator mtoive = new TheMotivator();
		mtoive.feedback(60);
        mtoive.createNames();
        mtoive.name();
	}
}