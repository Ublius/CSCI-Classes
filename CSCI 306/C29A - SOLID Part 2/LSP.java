class Bird {
    public void fly() {
        System.out.println("Bird is flying");
    }
}

class Ostrich extends Bird {
    @Override
    public void fly() {
        throw new UnsupportedOperationException("Ostrich cannot fly");
    }
}

public class Main {
    public static void main(String[] args) {
        Bird bird = new Ostrich();
        bird.fly(); // This will throw an UnsupportedOperationException
    }
}