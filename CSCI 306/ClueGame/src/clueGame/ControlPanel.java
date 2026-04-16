package clueGame;

import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;

public class ControlPanel extends JPanel implements ActionListener {
	private JTextField activePlayer; // whose turn it is
	private JTextField rollNum;
	private JTextField guess;
	private JTextField guessResult;
	private JButton next;
	private JButton accusation;
	
	public ControlPanel()  {
		setLayout(new GridLayout(2,0));
		
		JPanel top = new JPanel();
		top.setLayout(new GridLayout(1,4));
		
		// top left: whose turn and roll
		JPanel turn = new JPanel();
		turn.setLayout(new GridLayout(2,1));
		JLabel whoseTurn = new JLabel("Whose turn?");
		activePlayer = new JTextField(20);
		activePlayer.setEditable(false);
		turn.add(whoseTurn);
		turn.add(activePlayer);
		top.add(turn);
		
		JPanel roll = new JPanel();
		JLabel rollLabel = new JLabel("Roll");
		roll.setLayout(new GridLayout(2,1));
		rollNum = new JTextField(20);
		rollNum.setEditable(false);
		roll.add(rollLabel);
		roll.add(rollNum);
		top.add(roll);
		
		// top right: buttons
		accusation = new JButton("Make Accusation");
		next = new JButton("NEXT!");
		
		accusation.addActionListener(this);
		next.addActionListener(this);
		
		top.add(accusation);
		top.add(next);
		
		add(top);
		
		
		JPanel bottom = new JPanel();
		bottom.setLayout(new GridLayout(0,2));
		
		// bottom left: guess
		JPanel guessPanel = new JPanel();
		guessPanel.setLayout(new GridLayout(1,0));
		guessPanel.setBorder(new TitledBorder (new EtchedBorder(), "Guess"));
		guess = new JTextField(20);
		guess.setEditable(false);
		guessPanel.add(guess);
		
		// bottom right: guess result
		JPanel guessResultPanel = new JPanel();
		guessResultPanel.setLayout(new GridLayout(1,0));
		guessResultPanel.setBorder(new TitledBorder (new EtchedBorder(), "Guess Result"));
		guessResult = new JTextField(20);
		guessResult.setEditable(false);
		guessResultPanel.add(guessResult);
		
		bottom.add(guessPanel);
		bottom.add(guessResultPanel);
		
		add(bottom);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		Board board = Board.getInstance();
		
		if (e.getSource() == next) {
			/* the board is responsible for knowing whose turn it is
			* and if a turn is still in-progress
			*/
			if(board.roundOver()) {
				// The round is over and move onto the next player.
				board.nextTurn();
			}
			else {
				JOptionPane.showMessageDialog(null, "Please finish your turn.", "Sorry!", JOptionPane.INFORMATION_MESSAGE);
			}

		}
		else { // accusation button
			if(!board.roundOver() && !board.getHumanPlayer().getMadeMove()) { // it's the human's turn and the human hasn't moved yet
				// TODO: accusation
				SuggestionAccusationDialog suggestionPanel = new SuggestionAccusationDialog("Make a Accusation", false);
				suggestionPanel.setSize(500,300);
				suggestionPanel.setVisible(true);
				
			}
			else {
				JOptionPane.showMessageDialog(null, "You can't make an accusation now.", "Sorry!", JOptionPane.INFORMATION_MESSAGE);
			}
		}
		
	}

	
	public void setTurn(Player player, int roll) {
		String playerName = player.getName();
		activePlayer.setText(playerName);
		
		activePlayer.setBackground(player.getColor());
		
		rollNum.setText("" + roll);
	}
	
	public void setGuess(String guess, Player player) {
		this.guess.setText(guess);
		if (player != null) {
			this.guess.setBackground(player.getColor());
		}
		else {
			this.guess.setBackground(null);
		}
		
	}
	
	public void setGuessResult(String guessResult, Player player) {
		this.guessResult.setText(guessResult);
		if (player != null) {
			this.guessResult.setBackground(player.getColor());
		}
		else {
			this.guessResult.setBackground(null);
		}
		
	}
	
	
	/**
	 * Main to test the panel
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		ControlPanel panel = new ControlPanel();  // create the panel
		JFrame frame = new JFrame();  // create the frame 
		frame.setContentPane(panel); // put the panel in the frame
		frame.setSize(750, 180);  // size the frame
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // allow it to close
		frame.setVisible(true); // make it visible
		
		// test filling in the data
		ComputerPlayer testPlayer = new ComputerPlayer( "Col. Mustard", "orange", 0, 0);
		panel.setTurn(testPlayer, 5);
		panel.setGuess( "I have no guess!", testPlayer);
		panel.setGuessResult( "So you have nothing?", testPlayer);
	}

}
