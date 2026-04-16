package clueGame;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;
import java.awt.GridLayout;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Color;

public class GameCardPanel extends JPanel {
    private JPanel peoplePanel;
	private JPanel roomsPanel;
	private JPanel weaponsPanel;
	
	private Board board;
	private Map<Card, Color> cardColorMap = new HashMap<Card, Color>();
	private Set<Card> seenCards = new HashSet<Card>();
	private ArrayList<Card> hand = new ArrayList<Card>();

    // private static Card baseballBatCard, chairCard, pillowCard, penCard, hammerCard, knifeCard, johnsonCard, morganCard, stoverCard, parkerCard, hannaCard, maxwellCard, mediaRoomCard;

	/**
	 * Constructor for the panel, it does 90% of the work
	 */
	public GameCardPanel()  {
        setLayout(new GridLayout(3,1));
        setBorder(new TitledBorder(new EtchedBorder(), "Known Cards"));

        board = Board.getInstance();
		hand = board.getHumanPlayer().getCardList();

		peoplePanel = makePanel("People", CardType.PERSON);
		roomsPanel = makePanel("Rooms", CardType.ROOM);
		weaponsPanel = makePanel("Weapons", CardType.WEAPON);
		
		add(peoplePanel);
		add(roomsPanel);
		add(weaponsPanel);


	}

    private JPanel makePanel(String name, CardType type) {
		JPanel panel = new JPanel();
		panel.setPreferredSize(new Dimension(128, 576));
		panel.setLayout(new GridLayout(2, 1));
		panel.setSize(1, 2);
		panel.setBorder(new TitledBorder (new EtchedBorder(), name));

		
		JPanel handPanel = createPanel("In Hand:");

        boolean hasCardsOfType = false;
        for (Card card : hand) {
            if (card.getCardType() == type) {
                hasCardsOfType = true;
                JTextField cardText = new JTextField(card.getCardName());
				cardText.setBackground(board.getHumanPlayer().getColor());
                cardText.setEditable(false);
                handPanel.add(cardText, BorderLayout.SOUTH);
            }
        }

        if (!hasCardsOfType) {
            getNoneText(handPanel);
        }
        
		panel.add(handPanel, BorderLayout.NORTH);

		JPanel seenPanel = createPanel("Seen:");

        Iterator<Card> value = seenCards.iterator();
        hasCardsOfType = false;
        while (value.hasNext()) {
            Card nextCard = value.next();
            if (nextCard.getCardType() == type) {
                hasCardsOfType = true;
                JTextField cardText = new JTextField(nextCard.getCardName());
                cardText.setEditable(false);
                cardText.setBackground(cardColorMap.get(nextCard));
                seenPanel.add(cardText, BorderLayout.SOUTH);
            }
        }

        if (!hasCardsOfType) {
            getNoneText(seenPanel);
        }
		panel.add(seenPanel, BorderLayout.SOUTH);
		add(panel, BorderLayout.NORTH);
		
		return panel;
	}

	private JPanel createPanel(String text) {
		JPanel panel = new JPanel();
		panel.setLayout(new GridLayout(0, 1));
		panel.setSize(1, 2);
		JLabel label = new JLabel(text);
		panel.add(label, BorderLayout.NORTH);
		return panel;
	}

	private void getNoneText(JPanel seenPanel) {
		JTextField noneText = new JTextField("None");
		noneText.setEditable(false);
		seenPanel.add(noneText, BorderLayout.SOUTH);
	}

    public void updateSeen(Card card, Color color) {
		seenCards.add(card);
		cardColorMap.put(card, color);
		removeAll();
		peoplePanel = makePanel("People", CardType.PERSON);
		roomsPanel = makePanel("Rooms", CardType.ROOM);
		weaponsPanel = makePanel("Weapons", CardType.WEAPON);
		
		add(peoplePanel);
		add(roomsPanel);
		add(weaponsPanel);
		
		validate();
		repaint();
	}
	
	/**
	 * Main to test the panel
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		GameCardPanel panel = new GameCardPanel();  // create the panel
		JFrame frame = new JFrame();  // create the frame 
		frame.setContentPane(panel); // put the panel in the frame
		frame.setSize(180, 24 * 24);  // size the frame
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // allow it to close
		frame.setVisible(true); // make it visible
		
        // test filling in the data
		// panel.updateSeen(knifeCard, Color.RED);
        // panel.updateSeen(mediaRoomCard, Color.CYAN);
        // panel.updateSeen(pillowCard, Color.MAGENTA);
        // panel.updateSeen(chairCard, Color.GREEN);

	}
}
