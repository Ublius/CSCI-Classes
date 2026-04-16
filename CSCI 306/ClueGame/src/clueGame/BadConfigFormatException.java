package clueGame;

import java.io.PrintWriter;
import java.io.FileNotFoundException;

public class BadConfigFormatException extends Exception {
    public BadConfigFormatException() {
        super("Error: Config File Incorrectly Formated");
        try {
            PrintWriter writer = new PrintWriter("log.txt");
            writer.println(getMessage());
            writer.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
    
    public BadConfigFormatException(String message) {
        super("Error: " + message);
        try {
            PrintWriter writer = new PrintWriter("log.txt");
            writer.println(getMessage());
            writer.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
