package tests;

import static org.junit.jupiter.api.Assertions.*;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import clueGame.Board;
import clueGame.Card;
import clueGame.CardType;
import clueGame.Solution;
import clueGame.Player;
import clueGame.ComputerPlayer;
import clueGame.HumanPlayer;

class GameSolutionTest {
	private static Board board;

	@BeforeAll
	public static void setUp() {
		board = Board.getInstance();
		board.setConfigFiles("ClueLayout.csv", "ClueSetup.txt");		
		board.initialize();
	}

	@Test
	void testCheckAccusation() {
		Solution solution = board.getSolution();
		Card person = solution.getPerson();
		Card weapon = solution.getWeapon();
		Card room = solution.getRoom();
		
		Card wrong = new Card("Incorrect", CardType.PERSON);
		
		assertTrue(board.checkAccusation(person, weapon, room));
		assertFalse(board.checkAccusation(wrong, weapon, room));
		assertFalse(board.checkAccusation(person, wrong, room));
		assertFalse(board.checkAccusation(person, weapon, wrong));
	}
	
	@Test
	void testDisproveSuggestion() {
		// create example player with known cards
		Player player = new ComputerPlayer("Example", "orange", 0, 0);
		Card person = new Card("suspect", CardType.PERSON);
		player.updateHand(person);
		Card weapon = new Card("weapon", CardType.WEAPON);
		player.updateHand(weapon);
		Card room = new Card("room", CardType.ROOM);
		player.updateHand(room);
		
		// example non-matching cards
		Card wrongPerson = board.getSuspectCardList().iterator().next();
		Card wrongWeapon = board.getWeaponCardList().iterator().next();
		Card wrongRoom = board.getRoomCardList().iterator().next();
		
		assertEquals(player.disproveSuggestion(person, wrongWeapon, wrongRoom), person);
		boolean gotPerson = false, gotWeapon = false, gotRoom = false;
		for (int tries = 0; tries < 20; tries++) { // should choose card randomly
			Card result = player.disproveSuggestion(person, weapon, room);
			if (result.equals(person)) {
				gotPerson = true;
			}
			else if (result.equals(weapon)) {
				gotWeapon = true;
			}
			else if (result.equals(room)) {
				gotRoom = true;
			}
			else {
				fail("Player returned card they don't have");
			}
		}
		assertTrue(gotPerson);
		assertTrue(gotWeapon);
		assertTrue(gotRoom);
		
		// no match
		assertEquals(player.disproveSuggestion(wrongPerson, wrongWeapon, wrongRoom), null);
	}
	
	@Test
	void testHandleSuggestion() {
		ArrayList<Player> playerList = new ArrayList<Player>(board.getPlayerList());
		
		
		Card person = new Card("correct", CardType.PERSON);
		Card weapon = new Card("correct", CardType.WEAPON);
		Card room  = new Card("correct", CardType.ROOM);
		
		playerList.get(0).updateHand(person); // accuser
		
		// accuser can't disprove, nobody else can disprove
		assertEquals(board.handleSuggestion(playerList.get(0), person, weapon, room),null);
		
		playerList.get(1).updateHand(person);
		playerList.get(2).updateHand(person);
		
		// player 1 should disprove
		assertEquals(board.handleSuggestion(playerList.get(0), person, weapon, room),person);
	}

}
