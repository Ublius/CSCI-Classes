package clueGame;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Random;
import java.util.Set;

import javax.swing.JOptionPane;


public class ComputerPlayer extends Player {
	private Solution lastSuggestion; // if not disproved, use for accusation
	private Solution lastSeenSuggestion;
	private Set<Solution> badSolutions;
	

    public ComputerPlayer(String name, String color, int row, int column) {
        super(name, color, row, column);
        badSolutions = new HashSet<Solution>();
    }

    public Solution createSuggestion(Room room) {
    	Card roomCard = room.getCard();
    	
    	Board board = Board.getInstance();
    	
    	Set<Card> players = board.getSuspectCardList();
    	Set<Card> weapons = board.getWeaponCardList();
    	
    	Set<Card> possiblePlayers = new HashSet<Card>();
		Set<Card> possibleWeapons = new HashSet<Card>();
		
		// get cards to choose from
    	for (Card player : players) {
    		if (!super.hasSeen(player)) {
    			possiblePlayers.add(player);
    		}
    	}
    	
    	for (Card weapon : weapons) {
    		if (!super.hasSeen(weapon)) {
    			possibleWeapons.add(weapon);
    		}
    	}
    	
    	Card player = Board.getInstance().chooseOneCard(possiblePlayers);
    	Card weapon = Board.getInstance().chooseOneCard(possibleWeapons);
    	
    	Solution suggestion = new Solution(roomCard, player, weapon);
    	lastSuggestion = suggestion;
    	
        return suggestion;
    }

	public BoardCell selectTarget(Set<BoardCell> targets) {

		ArrayList<BoardCell> rooms = new ArrayList<>();
		Random random = new Random();
		Set<Card> seenCards = getSeen();
		
		// Filter out rooms from the targets
		for (BoardCell nextCell : targets) {
			if (nextCell.getIsRoom()) {
				rooms.add(nextCell);
			}
		}

		for (BoardCell room : rooms) {
			boolean roomInSeen = false;
			for (Card seenCard : seenCards) {
				if (seenCard.equals(room.getRoomCard())) {
					roomInSeen = true;
					break;
				}
			}
			// If the room is unseen, return it
			if (!roomInSeen) {
				return room;
			}
		}
		
		BoardCell[] targetsArray = targets.toArray(new BoardCell[targets.size()]);
		int randInt = random.nextInt(targetsArray.length);
		return targetsArray[randInt];
	}
	
	@Override
	public void playTurn(Set<BoardCell> targets) {
		Board board = Board.getInstance();
		ArrayList<BoardCell> walkwayTargets = new ArrayList<BoardCell>();
		
		// make accusation, unless it has been disproved before
		if (lastSuggestion != null) {
			boolean validSolution = true;
			for (Solution solution : badSolutions) {
				if (solution != null && solution.equals(lastSuggestion)) {
					validSolution = false;
				}
			}
			if (validSolution) {
				board.handleAccusation(this, lastSuggestion);
			}
		}

		if(targets.isEmpty()) {
			JOptionPane.showMessageDialog(null, "Computer Player had no where to move, Turn Skipped.", "Information!", JOptionPane.INFORMATION_MESSAGE);
			// The round is over if the player can't move
			board.setRoundOver(true);
			return;
		}
		
		for (BoardCell target : targets) {
			// always choose unseen rooms when available
			if (target.getIsRoom() && !super.hasSeen(board.getCard(board.getRoom(target).getName()))) {
				createSuggestion(board.getRoom(target));
				// Move the player to the new location
				board.movePlayer(this, target);
				
				// after moving into room, make a suggestion
				Solution suggestion = createSuggestion(board.getRoom(target));
				board.handleSuggestion(this, suggestion.getPerson(), suggestion.getWeapon(), suggestion.getRoom());
				// The player has made a move and can't move again this round
				setMadeMove(true);
				// board.setRoundOver(true);
			}
			// store walkway targets to pick randomly
			else {
				walkwayTargets.add(target);
			}
		}
		if(!getMadeMove())
		{
			// The player has made a move and cant move again this round
			board.movePlayer(this, walkwayTargets.get(new Random().nextInt(walkwayTargets.size())));
		}
		// The round is over if the player has made a move
		board.setRoundOver(true);

	}
	
	public void seeGuess(Solution guess, boolean disproved) { // see a suggestion someone else made
		if (!disproved) {
			lastSeenSuggestion = guess; // assume someone else will guess this
			// if next person loses, consider it disproved
			
			if (!super.getSeen().contains(guess.getPerson()) && !super.getSeen().contains(guess.getRoom()) && !super.getSeen().contains(guess.getWeapon())) {
				lastSuggestion = guess; // consider it disproved if you've seen a card in it before
			}
		}
		else { // not disproved
			badSolutions.add(guess);
		}
	}
	
	public void disproveSuggestion(Card card) {
		final boolean FORCE_ACCUSATION = false; // debugging
		if (!FORCE_ACCUSATION) {
			// if suggestion was disproved, don't hold onto it
			lastSuggestion = null;
		}
		super.updateSeen(card);
	}
	
}
