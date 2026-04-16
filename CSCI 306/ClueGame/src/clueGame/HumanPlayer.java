package clueGame;

import java.util.Set;

import javax.swing.JOptionPane;

public class HumanPlayer extends Player {

    public HumanPlayer(String name, String color, int row, int column) {
        super(name, color, row, column);
    }

    public Solution createSuggestion(Room room) {
		SuggestionAccusationDialog suggestionPanel = new SuggestionAccusationDialog("Make a Suggestion", true);
		suggestionPanel.setSize(500,300);
		suggestionPanel.setVisible(true);
    	return null;
    }

	@Override
	public void playTurn(Set<BoardCell> targets) {
		Board board = Board.getInstance();
		if (targets.isEmpty()) {
			JOptionPane.showMessageDialog(null, "No movement targets available.", "Sorry!", JOptionPane.INFORMATION_MESSAGE);
			// The round is over if the player can't move
			board.setRoundOver(true);
		}
	}

}
