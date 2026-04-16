package tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.fail;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import clueGame.Board;
import clueGame.BoardCell;
import clueGame.ComputerPlayer;
import clueGame.Player;
import clueGame.Room;
import clueGame.Solution;
import clueGame.Card;
import clueGame.CardType;

public class ComputerAITest {
    private static Board board;

    @BeforeAll
	public static void setUp() {
		board = Board.getInstance();
		board.setConfigFiles("ClueLayout.csv", "ClueSetup.txt");		
		board.initialize();
	}

    @Test
	public void testComputerCreateSuggestion() {
        Player player = new ComputerPlayer("computer", "orange", 0, 0);
        Room room = board.getRoom(player.getRoom().getName().charAt(0));
        Card weapon = board.getWeaponCardList().iterator().next();
        Card person = board.getSuspectCardList().iterator().next();
        
        player.updateSeen(person);
        player.updateSeen(weapon);
        
        int duplicatePerson = 0;
        int duplicateWeapon = 0;
        Card lastPerson = null;
        Card lastWeapon = null;
        
        final int TRIES = 20;
        
        for (int i = 0; i < TRIES; i++) {
        	Solution suggestion = player.createSuggestion(room);
        	
        	// returned a card
        	assertFalse(suggestion.getPerson() == null);
        	assertFalse(suggestion.getRoom() == null);
        	assertFalse(suggestion.getWeapon() == null);
        	
        	// returned given room, returned card not already seen
        	assertEquals(room.getCard(), suggestion.getRoom());
        	assertFalse(player.hasSeen(person));
        	assertFalse(player.hasSeen(weapon));
        	
        	// test if repeating card or picking randomly
        	if (lastPerson != null && lastPerson.equals(suggestion.getPerson())) {
        		duplicatePerson++;
        	}
        	else {
        		duplicatePerson = 0;
        		lastPerson = suggestion.getPerson();
        	}
        	if (lastWeapon != null && lastWeapon.equals(suggestion.getWeapon())) {
        		duplicateWeapon++;
        	}
        	else {
        		duplicateWeapon = 0;
        		lastWeapon = suggestion.getWeapon();
        	}
        }
        if (duplicatePerson == TRIES || duplicateWeapon == TRIES) {
        	fail("Didn't suggest random cards");
        }
    }

    @Test
	public void testComputerSelectTarget() {
       /*
		 * tests when there are no rooms in range, that the computer
		 * randomly selects a target
		 */
        Card officeCard = new Card("Office", CardType.ROOM);
        ComputerPlayer player = new ComputerPlayer("computer", "orange", 11, 7);

		board.calcTargets(board.getCell(player.getRow(), player.getColumn()), 1);
		int spot1Selected = 0;
		int spot2Selected = 0;
		for (int i = 0; i < 1000; i++) {
			BoardCell targetCell = player.selectTarget(board.getTargets());
			if (targetCell == board.getCell(10, 7)) {
				spot1Selected++;
			}
			else if (targetCell == board.getCell(11, 6)) {
				spot2Selected++;
			}	
		}
		assertTrue(spot1Selected >= 100);
		assertTrue(spot2Selected >= 100);
		
		/*
		 * If there is a room in the targets that the player has not seen,
		 * select that room
		 */

		player = new ComputerPlayer("computer", "orange", 11, 7);
		board.calcTargets(board.getCell(player.getRow(), player.getColumn()), 3);

		BoardCell targetCell = player.selectTarget(board.getTargets());
		assertEquals(board.getCell(7, 7), targetCell);
		
		/*
		 * If there is a room in the targets that the player has seen, choose
		 * any target including the room randomly
		 */

        player = new ComputerPlayer("computer", "orange", 2, 12);
		player.updateSeen(officeCard);

		board.calcTargets(board.getCell(player.getRow(), player.getColumn()), 1);
		spot1Selected = 0;
		spot2Selected = 0;
		for (int i = 0; i < 1000; i++) {
			targetCell = player.selectTarget(board.getTargets());
			if (targetCell == board.getCell(2, 15)) {
				spot1Selected++;
			}
			else if (targetCell == board.getCell(3, 12)) {
				spot2Selected++;
			}	
		}
		assertTrue(spot1Selected >= 100);
		assertTrue(spot2Selected >= 100);
	}
    
}
