import java.awt.Point;

public class StringPlay {

    public static void main(String[] args) {
        String cpp = "C++ is cool";
        String java = "I love Java";
        String newString = cpp.replace("C++", "Java");
        
        System.out.println("String Manipulation");
        System.out.println(cpp);
        System.out.println(java);
        System.out.println(newString);

        Point p = new Point(10, 20);
        Point q = new Point(10, 20);

        System.out.println("Point Comparison");
        System.out.println(p == q);
        System.out.println(p.equals(q));

        String name = "Tristen";
        String nameAdjusted = "tRiStEn";

        System.out.println("String Comparison");
        System.out.println(name == nameAdjusted);
        System.out.println(name.equals(nameAdjusted));
        System.out.println(name.equalsIgnoreCase(nameAdjusted));

    }

}
