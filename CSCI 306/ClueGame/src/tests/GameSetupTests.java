package tests;

import static org.junit.jupiter.api.Assertions.*;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import clueGame.Board;
import clueGame.Card;
import clueGame.CardType;
import clueGame.Player;

class GameSetupTests {
	private static Board board;
	private static Set<Card> cardsNotDealt;

	@BeforeAll
	public static void setUp() {
		board = Board.getInstance();
		board.setConfigFiles("ClueLayout.csv", "ClueSetup.txt");		
		board.initialize();
		cardsNotDealt = new HashSet<Card>(board.getCardList());
	}

	@Test
	void testNumberOfPlayers() {
		assertEquals(6, board.getPlayerList().size());
	}

	@Test
	void testNumberOfCards() {
		assertEquals(21, board.getCardList().size());
	}

	@Test
	void testNumberOfRoomCards() {
		assertEquals(9, board.getRoomCardList().size());
		int numRoomCards = 0;
		for (Card card : board.getCardList()) {
			if(card.getCardType() == CardType.ROOM) {
				numRoomCards++;
			}
		}
		assertEquals(9, numRoomCards);
	}

	@Test
	void testNumberOfSuspectCards() {
		assertEquals(6, board.getSuspectCardList().size());
	}

	@Test
	void testNumberOfWeaponCards() {
		assertEquals(6, board.getWeaponCardList().size());
	}

	@Test
	void testDealCardsSolution() {
		assertNotEquals(null, board.getSolution().getRoom());
		assertNotEquals(null, board.getSolution().getWeapon());
		assertNotEquals(null, board.getSolution().getPerson());
		
		cardsNotDealt.remove(board.getSolution().getRoom());
		cardsNotDealt.remove(board.getSolution().getWeapon());
		cardsNotDealt.remove(board.getSolution().getPerson());
	}

	@Test
	void testDealCards() {
		List<Player> playerList = new ArrayList<>(board.getPlayerList());

		for (Player player : playerList) {
			// ensure player has at least one card
			assertNotEquals(0, player.getCardList().size());
		}

		for (int i = 0; i < playerList.size(); i++) {
			for (int j = i + 1; j < playerList.size(); j++) {
				int numCardsToCheck = Math.min(playerList.get(i).getCardList().size(), playerList.get(j).getCardList().size());
				for (int k = 0; k < numCardsToCheck; k++) {
					assertFalse(playerList.get(i).getCardList().get(k).equals(playerList.get(j).getCardList().get(k)));
				}
			}
		}
	}

	@Test
	void testEqualDistributionCards() {
		// distributing to n players + solution
		int hands = board.getPlayerList().size() + 1;
		
		int numCardsExpected = board.getCardList().size() / hands;

		for (Player player : board.getPlayerList()) {
			// each hand should have numCardsExpected cards, give or take one
			assertEquals(numCardsExpected, player.getCardList().size(), 2);
		}
	}

}
