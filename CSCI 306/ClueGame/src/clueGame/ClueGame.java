package clueGame;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import java.awt.BorderLayout;

public class ClueGame extends JFrame {
    private static GameCardPanel gameCardPanel;
    private ControlPanel controlPanel;
    private Board board;

    public ClueGame() {
        board = Board.getInstance();
        board.setConfigFiles("ClueLayout.csv", "ClueSetup.txt");
        board.initialize();

        gameCardPanel = new GameCardPanel();
        controlPanel = new ControlPanel();

        // board needs to be able to request repaints when game status changes
        board.setCardPanel(gameCardPanel);
        board.setControlPanel(controlPanel);

        add(gameCardPanel, BorderLayout.EAST);
        add(controlPanel, BorderLayout.SOUTH);
        add(board, BorderLayout.CENTER);

        // Display the splash screen and who the player is playing as.
        JOptionPane.showMessageDialog(rootPane, "You are " + board.getHumanPlayer().getName() + ".\n"
                + "Can you find the solution\n" + "before the Computer players?", "Welcome to Clue",
                JOptionPane.INFORMATION_MESSAGE);

        // Process first player's turn
        board.begin();
    }

    public static void main(String[] args) {
        JFrame frame = new ClueGame();
        frame.setTitle("Clue Game");
        frame.setSize(800, 800); // size the frame
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // allow it to close
        frame.setVisible(true); // make it visible
        
        // start music
        MusicPlayer music = new MusicPlayer();
        music.play();
    }
}
