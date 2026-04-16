package clueGame;

import java.awt.GridLayout;
import java.util.Vector;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;

public class SuggestionAccusationDialog extends JDialog implements ActionListener {
    private Board board;
    private Vector<String> roomCardNames = new Vector<String>();
    private Vector<String> playerCardNames = new Vector<String>();
    private Vector<String> weaponCardNames = new Vector<String>();
    private JButton submitButton;
    private JButton cancelButton;
    private JTextField currentRoomName;

    private boolean suggestion;

    private JComboBox<String> roomNames;
    private JComboBox<String> playerNames;
    private JComboBox<String> weaponNames;

    public SuggestionAccusationDialog(String title, boolean isSuggestion) {
        setTitle(title);
        setModal(true);
        setLayout(new GridLayout(4,2));

        suggestion = isSuggestion;

        board = Board.getInstance();
        
        board.getRoomCardList().forEach(roomCard -> roomCardNames.add(roomCard.getCardName()));
        board.getSuspectCardList().forEach(playerCard -> playerCardNames.add(playerCard.getCardName()));
        board.getWeaponCardList().forEach(weaponCard -> weaponCardNames.add(weaponCard.getCardName()));

        JLabel roomLabel = new JLabel("Room");
        JLabel personLabel = new JLabel("Person");
        JLabel weaponLabel = new JLabel("Weapon");

        roomNames = new JComboBox<String>(roomCardNames);
        playerNames = new JComboBox<String>(playerCardNames);
        weaponNames = new JComboBox<String>(weaponCardNames);

        submitButton = new JButton("Submit");
        cancelButton = new JButton("Cancel");

        submitButton.addActionListener(this);
        cancelButton.addActionListener(this);

        
        add(roomLabel);

        if(isSuggestion) {
            currentRoomName = new JTextField(board.getHumanPlayer().getRoom().getName());
            currentRoomName.setEditable(false);
            add(currentRoomName);
        }
        else{
            add(roomNames);
        }
        
        add(personLabel);
        add(playerNames);
        add(weaponLabel);
        add(weaponNames);
        add(submitButton);
        add(cancelButton);

    }

    @Override
	public void actionPerformed(ActionEvent e) {
		Board board = Board.getInstance();
		
		if (e.getSource() == submitButton) {
            String selectedRoom = (String) roomNames.getSelectedItem();
            String selectedPerson = (String) playerNames.getSelectedItem();
            String selectedWeapon = (String) weaponNames.getSelectedItem();
            if(suggestion)
            { // Is a suggestion dialog
                board.handleSuggestion(board.getHumanPlayer(), board.getCard(selectedPerson), board.getCard(selectedWeapon), board.getCard(board.getHumanPlayer().getRoom().getName()));
            }
            else {
                // Is a accusation dialog

                Solution possibleSolution = new Solution(board.getCard(selectedRoom), board.getCard(selectedPerson), board.getCard(selectedWeapon));
                board.handleAccusation(board.getHumanPlayer(), possibleSolution);
            }
            dispose();
            setVisible(false);
		}
        else if (e.getSource() == cancelButton) {
            dispose();
            setVisible(false);
		}
		
	}

    public static void main(String[] args) {
		SuggestionAccusationDialog panel = new SuggestionAccusationDialog("Make a Suggestion", true);  // create the panel
		JFrame frame = new JFrame();  // create the frame 
		frame.setContentPane(panel); // put the panel in the frame
		frame.setSize(500, 300);  // size the frame
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // allow it to close
		frame.setVisible(true); // make it visible


		
	}
}
