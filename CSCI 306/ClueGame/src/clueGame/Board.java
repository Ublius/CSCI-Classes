package clueGame;

import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;

import javax.imageio.ImageIO;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

public class Board extends JPanel implements MouseListener {
	private BoardCell[][] board;
	private Set<BoardCell> visited;
	private Set<BoardCell> targets;
	private String layoutConfigFile;
	private String setupConfigFile;
	private Map<Character, Room> roomMap;
	private static Board theInstance = new Board();
	private int boardHeight; // rows
	private int boardWidth; // cols
	private ArrayList<Player> playerList;
	private int selectedPlayerNum;
	private ControlPanel controlPanel;
	private GameCardPanel cardPanel;
	private Set<Card> cardList;
	private Set<Card> peopleCardList;
	private Set<Card> roomCardList;
	private Set<Card> weaponCardList;
	private Solution solution;
	private static Random random = new Random(); // for dealing cards in helper functions
	private static final int HUMAN_PLAYER = 0; // human is first player
	private boolean roundOver;
	private BufferedImage playerPiece;
	private BufferedImage backgroundImage;

	private Board() {
		super();
	}

	/*
	 * initialize the board (since we are using singleton pattern)
	 */
	public void initialize() {
		try {
			loadSetupConfig();
		} catch (FileNotFoundException | BadConfigFormatException e) {
			System.out.println(e.getMessage());
		}
		try {
			loadLayoutConfig();
		} catch (FileNotFoundException | BadConfigFormatException e) {
			System.out.println(e.getMessage());
		}

		calcAdjacencies();

		dealCards();

		addMouseListener(this);
		
		// load images
		try {
			playerPiece = ImageIO.read(getClass().getResourceAsStream("/data/chessPiece.png"));
			
		} catch (IOException e) {
			System.out.println("Player image missing. Error message:");
			System.out.println(e.getMessage());
			
			playerPiece = null;
		}
		
		try {
			playerPiece = ImageIO.read(getClass().getResourceAsStream("/data/chessPiece.png"));
			backgroundImage = ImageIO.read(getClass().getResourceAsStream("/data/backgroundTexture.jpg"));
			
		} catch (IOException e) {
			System.out.println("Background image missing. Error message:");
			System.out.println(e.getMessage());
			
			backgroundImage = null;
		}
	}

	public void loadSetupConfig() throws FileNotFoundException, BadConfigFormatException {
		roomMap = new HashMap<Character, Room>();
		playerList = new ArrayList<Player>();
		cardList = new HashSet<Card>();
		final String FAILED_PREFIX = "Failed on line: ";
		final String INVALID_CELL_TYPE = "\nIncorrect amount of elements in line of setup file";
		
		InputStream input = getClass().getResourceAsStream(setupConfigFile);
		if (input == null) {
			throw new FileNotFoundException("Couldn't find setup config file.");
		}
		try (Scanner in = new Scanner(input)) {
			while (in.hasNextLine()) {
				String nextLine = in.nextLine();

				if (nextLine.startsWith("//")) { // skip comments
					continue;
				}

				String[] line = nextLine.split(",");

				// trim whitespace
				for (int section = 0; section < line.length; section++) {
					line[section] = line[section].trim();
				}

				if (line[0].equals("Room") || line[0].equals("Space")) {
					// must have three elements in line
					if (line.length != 3) {
						throw new BadConfigFormatException(
								FAILED_PREFIX + nextLine + INVALID_CELL_TYPE);
					}

					// did not provide an initial
					if (line[2].length() != 1) {
						throw new BadConfigFormatException(
								FAILED_PREFIX + nextLine + "\nInvalid initial found in setup file");
					}

					// the center and label cells won't be known until
					// the layout is read
					if (line[0].equals("Room")) {
						Card card = new Card(line[1], CardType.ROOM);
						cardList.add(card);
						Room room = new Room(line[1], card);
						roomMap.put(line[2].charAt(0), room);
						card.setRoom(room);
					} else {
						roomMap.put(line[2].charAt(0), new Room(line[1]));
					}

				} else if (line[0].equals("Player")) {
					// must have five elements in line
					if (line.length != 5) {
						throw new BadConfigFormatException(
								FAILED_PREFIX + nextLine + INVALID_CELL_TYPE);
					}
					// Player, name, color, row, column
					int row = Integer.parseInt(line[3]);
					int col = Integer.parseInt(line[4]);

					if (playerList.isEmpty()) {
						playerList.add(new HumanPlayer(line[1], line[2], row, col));
					} else {
						playerList.add(new ComputerPlayer(line[1], line[2], row, col));
					}
					cardList.add(new Card(line[1], CardType.PERSON, playerList.get(playerList.size() - 1)));
				}

				else if (line[0].equals("Weapon")) {
					// must have two elements in line
					if (line.length != 2) {
						throw new BadConfigFormatException(
								FAILED_PREFIX + nextLine + INVALID_CELL_TYPE);
					}

					cardList.add(new Card(line[1], CardType.WEAPON));
				}

			}
		//}
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}

	public void loadLayoutConfig() throws FileNotFoundException, BadConfigFormatException {
		InputStream input = getClass().getResourceAsStream(layoutConfigFile);
		if (input == null) {
			throw new FileNotFoundException("Couldn't find layout config file.");
		}
		Scanner in = new Scanner(input);
		
		List<String[]> lines = processFirstPass(in);
		processSecondPass(lines);
	}

	private void calcAdjacencies() {
		// add adjacencies for doorways and add room adjacencies
		for (int row = 0; row < boardHeight; row++) {
			for (int col = 0; col < boardWidth; col++) {
				BoardCell cell = board[row][col];
				if (cell.getInitial() == 'W') { // walkway adjacencies
					try {
						if (row + 1 < boardHeight) {
							addWalkwayAdjacencies(row + 1, col, cell, DoorDirection.DOWN);
						}
						if (row - 1 >= 0) {
							addWalkwayAdjacencies(row - 1, col, cell, DoorDirection.UP);
						}
						if (col + 1 < boardWidth) {
							addWalkwayAdjacencies(row, col + 1, cell, DoorDirection.RIGHT);
						}
						if (col - 1 >= 0) {
							addWalkwayAdjacencies(row, col - 1, cell, DoorDirection.LEFT);
						}
					} catch (Exception e) {
						e.printStackTrace();
					}
				}

				// secret passages
				else if (cell.isRoomCenter() && (roomMap.get(cell.getInitial()).getSecretCell() != null)) {
					cell.addAdjacency(roomMap.get(roomMap.get(cell.getInitial()).getSecretCell().getSecretPassage())
							.getCenterCell());
				}
			}
		}
	}

	private void addWalkwayAdjacencies(int row, int col, BoardCell cell, DoorDirection direction) throws Exception {
		BoardCell otherCell = board[row][col];

		if (otherCell.getInitial() == 'W') { // other cell is a walkway
			cell.addAdjacency(otherCell);
		} else if (otherCell.getIsRoom() && cell.getDoorDirection() == direction) { // other cell is a room and this is a doorway
			// add room's center to adjacency list
			cell.addAdjacency(roomMap.get(otherCell.getInitial()).getCenterCell());

			// add this doorway to room's center cell's adjacency list
			roomMap.get(otherCell.getInitial()).getCenterCell().addAdjacency(cell);
		}
	}

	private void dealCards() {
		// temporary lists - dealt cards will be removed
		HashSet<Card> possibleWeapons = new HashSet<Card>();
		HashSet<Card> possiblePeople = new HashSet<Card>();
		HashSet<Card> possibleRooms = new HashSet<Card>();
		HashSet<Card> possibleCards = new HashSet<Card>();
		
		// saved lists, for getters
		peopleCardList = new HashSet<Card>();
		weaponCardList = new HashSet<Card>();
		roomCardList = new HashSet<Card>();

		for (Card card : cardList) {
			possibleCards.add(card);
			if (card.getCardType() == CardType.PERSON) {
				peopleCardList.add(card);
				possiblePeople.add(card);
			} else if (card.getCardType() == CardType.ROOM) {
				roomCardList.add(card);
				possibleRooms.add(card);
			} else if (card.getCardType() == CardType.WEAPON) {
				weaponCardList.add(card);
				possibleWeapons.add(card);
			}
		}

		// create solution
		Card person = chooseOneCard(possiblePeople);
		Card room = chooseOneCard(possibleRooms);
		Card weapon = chooseOneCard(possibleWeapons);

		solution = new Solution(room, person, weapon);

		possibleWeapons.remove(weapon);
		possiblePeople.remove(person);
		possibleRooms.remove(room);

		// deal remaining cards to players
		dealCardsToPlayers(possibleCards);
	}

	private void dealCardsToPlayers(HashSet<Card> possibleCards) {
		int cardsDealt = 0;

		// aim to give each player at least one card
		for (Player player : playerList) {
			if (cardsDealt < cardList.size() - 3) {
				Card card = chooseOneCard(possibleCards);
				player.updateHand(card);
				possibleCards.remove(card);
				cardsDealt++;
			} else {
				break;
			}
		}

		// randomly deal the rest until there are no cards remaining
		while (cardsDealt < cardList.size() - 3) {
			int chosen = random.nextInt(playerList.size());
			Player player = playerList.get(chosen);

			boolean alreadyHave = false;
			Card chosenCard = chooseOneCard(possibleCards);
			for (Card card : player.getCardList()) {
				// don't give a player multiple of the same type of card
				if (chosenCard.getCardType() == card.getCardType()) {
					alreadyHave = true;
					break;
				}
			}
			if (!alreadyHave) {
				player.updateHand(chosenCard);
				possibleCards.remove(chosenCard);
			}
			cardsDealt++;
		}
	}

	// used by unit tests without players
	public void calcTargets(BoardCell startCell, int pathLength) {
		createTargetList(startCell, pathLength);
	}
	
	public void calcTargets(Player player, BoardCell startCell, int pathLength) {
		createTargetList(startCell, pathLength);
		
		if (player.getCanStay()) { // player was moved into room by suggestion
			targets.add(startCell); // can stay in room
			player.setCanStay(false); // can only do this once
		}

		// flag new targets for highlighting if it's human's turn
		if (selectedPlayerNum == HUMAN_PLAYER) {
			for (BoardCell cell : targets) {
				cell.setTarget(true);
			}
		}

		// redraw
		this.repaint();
	}

	private void createTargetList(BoardCell startCell, int pathLength) {
		visited = new HashSet<BoardCell>();
		targets = new HashSet<BoardCell>();

		visited.add(startCell);

		findAllTargets(startCell, pathLength);
	}

	private void findAllTargets(BoardCell startCell, int pathLength) {
		for (BoardCell adjCell : startCell.getAdjList()) {
			if (!visited.contains(adjCell) && (!adjCell.getOccupied() || adjCell.getIsRoom())) {
				visited.add(adjCell);
				if (pathLength == 1 || adjCell.getIsRoom()) {
					targets.add(adjCell);
				} else {
					findAllTargets(adjCell, pathLength - 1);
				}
				visited.remove(adjCell);
			}
		}
	}

	// used for dealing and computers making suggestions
	public Card chooseOneCard(Set<Card> options) {
		int chosen = 0;
		int iterator = 0;
		if (!options.isEmpty()) {
			chosen = random.nextInt(options.size());
			for (Card card : options) {
				if (chosen == iterator) {
					return card;
				}
				iterator++;
			}
		}
		return null;
	}

	private List<String[]> processFirstPass(Scanner in) {
		List<String[]> lines = new ArrayList<>();
		int maxWidth = 0;
		int row = 0;

		while (in.hasNextLine()) { // Already read the first line to find width
			String[] line = in.nextLine().split(",");
			maxWidth = Math.max(maxWidth, line.length);
			lines.add(line);
			row++;
		}

		boardHeight = row;
		boardWidth = maxWidth;

		// Initialize the board
		board = new BoardCell[boardHeight][boardWidth];
		return lines;
	}

	private void processSecondPass(List<String[]> lines) throws BadConfigFormatException {
		for (int r = 0; r < boardHeight; r++) {
			String[] line = lines.get(r);

			if (line.length != boardWidth) {
				throw new BadConfigFormatException("Incorrect amount of columns per row");
			}

			for (int c = 0; c < boardWidth; c++) {
				char initial = Character.toUpperCase(line[c].charAt(0));
				String cellType = line[c].substring(1);

				processRoomCell(r, c, initial, cellType);
				processDoorwayCell(r, c, initial, cellType);
			}
		}
	}

	private void processRoomCell(int row, int col, char initial, String cellType) throws BadConfigFormatException {
		boolean isRoom = false;
		boolean roomCenter = false;
		boolean roomLabel = false;
		char passage = 'X';

		if (initial != 'W' && initial != 'X') { // if room cell
			if (!roomMap.containsKey(initial)) {
				throw new BadConfigFormatException("Unknown room in layout");
			}

			isRoom = true;

			if (cellType.length() == 1) {
				char cellTypeChar = cellType.charAt(0);
				if (cellTypeChar == '#') {
					roomLabel = true;
				} else if (cellTypeChar == '*') {
					roomCenter = true;
				} else if (roomMap.containsKey(cellTypeChar)) {
					passage = cellTypeChar; // it's a passage
				} else {
					throw new BadConfigFormatException("Invalid room cell type");
				}
			}
		}

		setBoardCellAndRoomFeatures(row, col, initial, isRoom, roomCenter, roomLabel, passage);
	}

	private void processDoorwayCell(int row, int col, char initial, String direction) throws BadConfigFormatException {
		if (initial == 'W' && direction.length() == 1) { // doorway
			DoorDirection doorDirection = getDoorDirection(direction.charAt(0));
			board[row][col] = new BoardCell(row, col, initial, false, false, false, 'X', doorDirection);
		}
	}

	private void setBoardCellAndRoomFeatures(int row, int col, char initial, boolean isRoom, boolean roomCenter,
			boolean roomLabel, char passage) {
		board[row][col] = new BoardCell(row, col, initial, isRoom, roomCenter, roomLabel, passage, DoorDirection.NONE);
		if (isRoom) {
			board[row][col].setRoomCard(roomMap.get(initial).getCard());
		}
		if (roomLabel) {
			roomMap.get(initial).setLabelCell(board[row][col]);
		} else if (roomCenter) {
			roomMap.get(initial).setCenterCell(board[row][col]);
		} else if (passage != 'X') {
			roomMap.get(initial).setSecretCell(board[row][col]);
		}
	}

	/**
	 * Begin the game after ClueGame finishes setting up
	 */
	public void begin() {
		// game begins as if player before human just finished
		selectedPlayerNum = 0;
		getHumanPlayer().setActive(true);
		nextTurn();
	}

	public void nextTurn() {
		if (playerList.size() == 1) { // only human remains
			JOptionPane.showMessageDialog(getRootPane(), "You (sort of) won!", "Congratulations?",
					JOptionPane.INFORMATION_MESSAGE);
			System.exit(0);
		}
		Player currentPlayer = playerList.get(selectedPlayerNum);
		// Set the round to not be over as it just started
		roundOver = false;
		if (currentPlayer.getActive()) { // currently selected player is ready to play
			// roll and get valid targets
			int roll = random.nextInt(1, 6);
			BoardCell currentLocation = getCell(currentPlayer.getRow(), currentPlayer.getColumn());
			calcTargets(currentPlayer, currentLocation, roll);

			// show turn information
			controlPanel.setTurn(currentPlayer, roll);

			// player hasn't moved yet; can't move twice in one turn
			currentPlayer.setMadeMove(false);
			
			// clear previous guess
			controlPanel.setGuess("", null);
			controlPanel.setGuessResult("", null);

			// prompt player to play
			currentPlayer.playTurn(targets);

			// if human just played, update known information
			if (selectedPlayerNum == HUMAN_PLAYER) {
				cardPanel.repaint();
			}
			// player finished turn; no longer active
			currentPlayer.setActive(false);

			// select next player
			selectedPlayerNum = (selectedPlayerNum + 1) % playerList.size();
			currentPlayer = playerList.get(selectedPlayerNum);
			currentPlayer.setActive(true);
		}
	}

	public void movePlayer(Player player, BoardCell location) {
		// The location the player is at is now unoccupied
		player.getLocation().setOccupied(false);
		// unflag previous targets
		if (targets != null) {
			for (BoardCell cell : targets) {
				cell.setTarget(false);
			}
		}
		// Move the player to the new location
		player.setLocation(location);
		// The location the player is at is now occupied
		player.getLocation().setOccupied(true);
		repaint();
		// After the player has moved the round is over
		roundOver = true;
	}

	public boolean checkAccusation(Card person, Card weapon, Card room) {
		Solution accusation = new Solution(room, person, weapon);
		return solution.equals(accusation);
	}
	
	public void handleAccusation(Player player, Solution accusation) {
		if (checkAccusation(accusation.getPerson(), accusation.getWeapon(), accusation.getRoom())) {
			// win message
			if (player == getHumanPlayer()) { // human won
				JOptionPane.showMessageDialog(getRootPane(), "You won with the guess: " + accusation, "Congratulations!",
						JOptionPane.INFORMATION_MESSAGE);
			}
			else { // computer won
				JOptionPane.showMessageDialog(getRootPane(), player.getName() + " won with the guess " + accusation, "Sorry...",
						JOptionPane.INFORMATION_MESSAGE);
			}
			// end game
			System.exit(0);
		}
		else {
			// human loses
			if (player == getHumanPlayer()) { // human won
				JOptionPane.showMessageDialog(getRootPane(), "You lost with the guess " + accusation, "Sorry...",
						JOptionPane.INFORMATION_MESSAGE);
				System.exit(0);
			}
			else { // computer loses
				JOptionPane.showMessageDialog(getRootPane(), player.getName() + " lost with the guess " + accusation, "Uh oh!",
						JOptionPane.INFORMATION_MESSAGE);
				playerList.remove(player); // remove player from game
				repaint(); // take player off board
				
				// other computer players should avoid losing
				// learn that last non-disproved suggestion wasn't the solution
				for (Player otherPlayer : playerList) {
					if (otherPlayer != getHumanPlayer()) {
						((ComputerPlayer) otherPlayer).seeGuess(accusation, true);
					}
				}
			}
			
		}
	}

	public Card handleSuggestion(Player accuser, Card person, Card weapon, Card room) {
		if (controlPanel != null) { // no control panel in unit tests
			controlPanel.setGuess(person.getCardName() + ", " + weapon.getCardName() + ", " + room.getCardName(),
					accuser);
			
			// move accused player to room
			// unit test for suggestions uses fake rooms with no centers
			movePlayer(person.getPlayer(), room.getRoom().getCenterCell());
			person.getPlayer().setCanStay(true); // player can choose to stay in room to make a suggestion
		}
		
		Solution suggestion = new Solution(room, person, weapon);
		
		Card disprovingCard = null;

		for (Player player : playerList) {
			// player can't disprove own suggestion
			if (player != accuser) {
				Card card = player.disproveSuggestion(person, weapon, room);
				if (card != null) {
					// suggestion disproved
					if (controlPanel != null) { // if human just played, show what disproved suggestion
						if (accuser == getHumanPlayer()) {
							controlPanel.setGuessResult("Disproved with " + card.getCardName(), player);
							cardPanel.updateSeen(card, player.getColor());
						}
						else { // if a computer made suggestion, don't give away what disproved suggestion
							controlPanel.setGuessResult("Disproved!", player);
							// tell computer players their suggestion was disproved
							((ComputerPlayer) accuser).disproveSuggestion(card);
						}
					}
					disprovingCard = card;
					break;
				}
			}
		}
		if (controlPanel != null && disprovingCard == null) {
			controlPanel.setGuessResult("Not disproved!", null);
		}
		
		// tell other computer players what last suggestion was
		for (Player player : playerList) {
			if (player != accuser && player != getHumanPlayer()) {
				// computers see suggestion and if it was disproved
				((ComputerPlayer) player).seeGuess(suggestion, disprovingCard != null);
			}
		}
		return disprovingCard;
	}

	@Override
	public void paintComponent(Graphics graphics) {
		super.paintComponent(graphics);

		int cellWidth = getWidth() / boardWidth;
		int cellHeight = getHeight() / boardHeight;
		
		if (backgroundImage != null) {
    		graphics.drawImage(backgroundImage,0,0,getWidth(),getHeight(), 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight(), null, null);
    	}

		for (int row = 0; row < boardHeight; row++) {
			for (int col = 0; col < boardHeight; col++) {
				board[row][col].drawCell(graphics, col * cellWidth, row * cellHeight, cellWidth, cellHeight);
			}
		}

		for (Map.Entry<Character, Room> entry : roomMap.entrySet()) {
			Room room = entry.getValue();
			BoardCell labelCell = room.getLabelCell();
			if (labelCell != null) {
				int nameX = room.getLabelCell().getColumn() * cellWidth;
				int nameY = room.getLabelCell().getRow() * cellHeight;
				room.drawName(graphics, nameX, nameY);
			}

		}

		// Create an offset for players in a room.
		int counter = 0;
		for (Player player : playerList) {
			counter++;
			int offsetX = player.getLocation().getIsRoom() ? (int) (counter * counter) : 0;
			player.drawPlayer(graphics, player.getColumn() * cellWidth + offsetX, player.getRow() * cellHeight,
					cellWidth, cellHeight);

		}

	}

	@Override
	public void mouseClicked(MouseEvent e) {
		// check if it's the human's turn
		if (selectedPlayerNum != (HUMAN_PLAYER + 1) % playerList.size() || playerList.get(HUMAN_PLAYER).getMadeMove()) {
			return;
		}

		BoardCell clickedCell = findClickedCell(e);

		// determine if cell is a valid target
		if (clickedCell == null) { // no cell matches location
			return;
		} else if (clickedCell.getIsRoom()
				&& targets.contains(roomMap.get(clickedCell.getRoomCard().getCardName().charAt(0)).getCenterCell())) { // clicked a room
			// can click anywhere in the room
			movePlayer(playerList.get(HUMAN_PLAYER),
					roomMap.get(clickedCell.getRoomCard().getCardName().charAt(0)).getCenterCell());
					playerList.get(HUMAN_PLAYER).createSuggestion(roomMap.get(clickedCell.getRoomCard().getCardName().charAt(0)));
		} else if (targets.contains(clickedCell)) { // clicked a walkway
			movePlayer(playerList.get(HUMAN_PLAYER), clickedCell);
		} else { // cell wasn't a target
			JOptionPane.showMessageDialog(getRootPane(), "Selected cell isn't a valid target", "Invalid",
					JOptionPane.INFORMATION_MESSAGE);
			return;
		}

		playerList.get(HUMAN_PLAYER).setMadeMove(true); // if move was successful, player can't move again during turn
	}

	private BoardCell findClickedCell(MouseEvent e) {
		// find what cell was clicked
		BoardCell clickedCell = null;
		int cellWidth = getWidth() / boardWidth;
		int cellHeight = getHeight() / boardHeight;

		if (targets != null) {
			// find clicked cell
			// need to check all cells, not just targets, in case a random spot in a room
			// was clicked
			for (int row = 0; row < boardHeight; row++) {
				for (int col = 0; col < boardWidth; col++) {
					BoardCell cell = this.getCell(row, col);
					if (cell.contains(e.getX(), e.getY(), cellWidth, cellHeight)) {
						clickedCell = cell;
					}
				}
			}
		}
		return clickedCell;
	}

	/**
	 * Required overrides
	 */
	@Override
	public void mouseEntered(MouseEvent e) {}

	@Override
	public void mouseExited(MouseEvent e) {}

	@Override
	public void mousePressed(MouseEvent e) {}

	@Override
	public void mouseReleased(MouseEvent e) {}

	public Room getRoom(char c) {
		return roomMap.get(c);
	}

	public Room getRoom(BoardCell cell) {
		return roomMap.get(cell.getInitial());
	}

	public int getNumRows() {
		return boardHeight;
	}

	public int getNumColumns() {
		return boardWidth;
	}

	public Set<BoardCell> getTargets() {
		return targets;
	}

	public BoardCell getCell(int row, int col) {
		return board[row][col];
	}

	public static Board getInstance() {
		return theInstance;
	}

	public void setConfigFiles(String layoutCSV, String setupTXT) {
		layoutConfigFile = "/data/" + layoutCSV;
		setupConfigFile = "/data/" + setupTXT;

	}

	public Set<BoardCell> getAdjList(int row, int col) {
		return board[row][col].getAdjList();
	}

	public ArrayList<Player> getPlayerList() {
		return playerList;
	}

	public Set<Card> getCardList() {
		return cardList;
	}

	public Card getCard(String name) {
		for (Card card : cardList) {
			if (card.getCardName().equals(name)) {
				return card;
			}
		}

		return null;
	}

	public Player getHumanPlayer() {
		return playerList.get(HUMAN_PLAYER);
	}

	private DoorDirection getDoorDirection(char directionChar) throws BadConfigFormatException {
		switch (directionChar) {
			case '^':
				return DoorDirection.UP;
			case 'v':
				return DoorDirection.DOWN;
			case '<':
				return DoorDirection.LEFT;
			case '>':
				return DoorDirection.RIGHT;
			default:
				throw new BadConfigFormatException("Invalid doorway direction in layout");
		}
	}

	public Set<Card> getWeaponCardList() {
		return weaponCardList;
	}

	public Set<Card> getRoomCardList() {
		return roomCardList;
	}

	public Set<Card> getSuspectCardList() {
		return peopleCardList;
	}

	public Solution getSolution() {
		return solution;
	}

	/**
	 * To update with turn information
	 */
	public void setControlPanel(ControlPanel panel) {
		controlPanel = panel;
	}

	/**
	 * To repaint when seen card list changes
	 */
	public void setCardPanel(GameCardPanel panel) {
		cardPanel = panel;
	}

	public boolean roundOver() {
		return roundOver;
	}

	public void setRoundOver(boolean b) {
		roundOver = b;
	}

	public BufferedImage getPlayerImage() {
		return playerPiece;
	}
}