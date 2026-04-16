package tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.Set;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import clueGame.Board;
import clueGame.BoardCell;

public class BoardAdjTargetTest {
	private static Board board;
	
	@BeforeAll
	public static void setUp() {
		board = Board.getInstance();
		board.setConfigFiles("ClueLayout.csv", "ClueSetup.txt");		
		board.initialize();
	}

	// Ensure that player does not move around within room
	// Spreadsheet color: Yellow
	@Test
	public void testAdjacenciesRooms()
	{
		// Cellar
		// One doorway and one secret passage
		Set<BoardCell> testList = board.getAdjList(3, 1);
		assertEquals(2, testList.size());
		assertTrue(testList.contains(board.getCell(15, 15)));
		assertTrue(testList.contains(board.getCell(4,3)));
		
		// Nursery
		// One doorway and one secret passage
		testList = board.getAdjList(15, 15);
		assertEquals(2, testList.size());
		assertTrue(testList.contains(board.getCell(3, 1)));
		assertTrue(testList.contains(board.getCell(12, 15)));
		
		// Office
		// One doorway
		testList = board.getAdjList(2, 15);
		assertEquals(1, testList.size());
		assertTrue(testList.contains(board.getCell(2, 12)));
	}

	
	// Ensure door locations include their rooms and also additional walkways
	// Spreadsheet color: Yellow
	@Test
	public void testAdjacencyDoor()
	{
		Set<BoardCell> testList = board.getAdjList(4, 6);
		assertEquals(3, testList.size());
		assertTrue(testList.contains(board.getCell(1, 7))); // room
		assertTrue(testList.contains(board.getCell(4, 5)));
		assertTrue(testList.contains(board.getCell(4, 7)));

		testList = board.getAdjList(11, 4);
		assertEquals(4, testList.size());
		assertTrue(testList.contains(board.getCell(11, 1))); // room
		assertTrue(testList.contains(board.getCell(10, 4)));
		assertTrue(testList.contains(board.getCell(12, 4)));
		assertTrue(testList.contains(board.getCell(11, 5)));
		
		testList = board.getAdjList(9, 12);
		assertEquals(4, testList.size());
		assertTrue(testList.contains(board.getCell(8, 15))); // room
		assertTrue(testList.contains(board.getCell(9, 11)));
		assertTrue(testList.contains(board.getCell(8, 12)));
		assertTrue(testList.contains(board.getCell(10, 12)));
	}
	
	// Test a variety of walkway scenarios
	// Spreadsheet color: Dark purple
	@Test
	public void testAdjacencyWalkways()
	{
		// One adjacent walkway
		Set<BoardCell> testList = board.getAdjList(5, 16);
		assertEquals(1, testList.size());
		assertTrue(testList.contains(board.getCell(5, 15)));
		
		// Near a door
		testList = board.getAdjList(4, 4);
		assertEquals(3, testList.size());
		assertTrue(testList.contains(board.getCell(4, 3)));
		assertTrue(testList.contains(board.getCell(4, 5)));
		assertTrue(testList.contains(board.getCell(5, 4)));

		// Two adjacent
		testList = board.getAdjList(13, 2);
		assertEquals(2, testList.size());
		assertTrue(testList.contains(board.getCell(13, 1)));
		assertTrue(testList.contains(board.getCell(13, 3)));

		// Four adjacent
		testList = board.getAdjList(11,11);
		assertEquals(4, testList.size());
		assertTrue(testList.contains(board.getCell(11, 10)));
		assertTrue(testList.contains(board.getCell(11, 12)));
		assertTrue(testList.contains(board.getCell(10, 11)));
		assertTrue(testList.contains(board.getCell(12, 11)));
	
	}
	
	
	// Tests out of room center
	// Roll amounts: 1, 3, 5
	// Spreadsheet color: Light orange
	@Test
	public void testTargetsInFamilyRoom() {
		// test a roll of 1
		// can only get to doorway
		board.calcTargets(board.getCell(15, 8), 1);
		Set<BoardCell> targets= board.getTargets();
		assertEquals(1, targets.size());
		assertTrue(targets.contains(board.getCell(12, 9)));
		
		// test a roll of 3
		board.calcTargets(board.getCell(15, 8), 3);
		targets= board.getTargets();
		assertEquals(5, targets.size());
		assertTrue(targets.contains(board.getCell(12, 7)));
		assertTrue(targets.contains(board.getCell(11, 8)));	
		assertTrue(targets.contains(board.getCell(10, 9)));
		assertTrue(targets.contains(board.getCell(11, 10)));
		assertTrue(targets.contains(board.getCell(12, 11)));
		
		
		// test a roll of 5
		board.calcTargets(board.getCell(15, 8), 5);
		targets= board.getTargets();
		assertEquals(13, targets.size());
		assertTrue(targets.contains(board.getCell(12, 5)));
		assertTrue(targets.contains(board.getCell(11, 6)));	
		assertTrue(targets.contains(board.getCell(10, 7)));
		assertTrue(targets.contains(board.getCell(11, 8)));
		assertTrue(targets.contains(board.getCell(12, 7)));
		assertTrue(targets.contains(board.getCell(13, 12)));
		assertTrue(targets.contains(board.getCell(12, 13)));
		assertTrue(targets.contains(board.getCell(11, 12)));
		assertTrue(targets.contains(board.getCell(10, 11)));
		assertTrue(targets.contains(board.getCell(11, 10)));
		assertTrue(targets.contains(board.getCell(10, 9)));
	}
	
	// Roll amount: 1, 3, 4
	@Test
	public void testTargetsInKitchen() {
		// test a roll of 1
		board.calcTargets(board.getCell(7, 7), 1);
		Set<BoardCell> targets= board.getTargets();
		assertEquals(1, targets.size());
		assertTrue(targets.contains(board.getCell(10, 8)));
		
		// test a roll of 3
		board.calcTargets(board.getCell(7, 7), 3);
		targets= board.getTargets();
		assertEquals(5, targets.size());
		assertTrue(targets.contains(board.getCell(12, 8)));
		assertTrue(targets.contains(board.getCell(11, 9)));	
		assertTrue(targets.contains(board.getCell(11, 7)));
		assertTrue(targets.contains(board.getCell(10, 10)));
		assertTrue(targets.contains(board.getCell(10, 6)));
		
		
		// test a roll of 4
		board.calcTargets(board.getCell(7, 7), 4);
		targets= board.getTargets();
		assertEquals(9, targets.size());
		assertTrue(targets.contains(board.getCell(12, 9)));
		assertTrue(targets.contains(board.getCell(12, 7)));
		assertTrue(targets.contains(board.getCell(10, 9)));
		assertTrue(targets.contains(board.getCell(10, 7)));
		assertTrue(targets.contains(board.getCell(11, 10)));
		assertTrue(targets.contains(board.getCell(10, 11)));
		assertTrue(targets.contains(board.getCell(10, 5)));
		assertTrue(targets.contains(board.getCell(11, 6)));
		assertTrue(targets.contains(board.getCell(12, 9)));
	}

	// Tests out of room center
	// Roll amounts: 1, 3, 4
	// Spreadsheet color: Light orange
	@Test
	public void testTargetsAtDoor() {
		// test a roll of 3
		board.calcTargets(board.getCell(12, 15), 3);
		Set<BoardCell> targets= board.getTargets();
		assertEquals(7, targets.size());
		assertTrue(targets.contains(board.getCell(15, 15)));
		assertTrue(targets.contains(board.getCell(11, 15)));
		assertTrue(targets.contains(board.getCell(12, 14)));
		assertTrue(targets.contains(board.getCell(12, 16)));
		assertTrue(targets.contains(board.getCell(11, 17)));

		// test a roll of 1, at door
		board.calcTargets(board.getCell(12, 15), 1);
		targets= board.getTargets();
		assertEquals(4, targets.size());
		assertTrue(targets.contains(board.getCell(15, 15)));
		assertTrue(targets.contains(board.getCell(11, 15)));
		assertTrue(targets.contains(board.getCell(12, 14)));
		assertTrue(targets.contains(board.getCell(12, 16)));

		// test a roll of 4
		board.calcTargets(board.getCell(12, 15), 4);
		targets= board.getTargets();
		assertEquals(8, targets.size());
		assertTrue(targets.contains(board.getCell(15, 15)));
		assertTrue(targets.contains(board.getCell(11, 12)));
		assertTrue(targets.contains(board.getCell(11, 16)));
		assertTrue(targets.contains(board.getCell(12, 13)));

	}

	@Test
	public void testTargetsInWalkway1() {
		// test a roll of 1
		board.calcTargets(board.getCell(5, 14), 1);
		Set<BoardCell> targets= board.getTargets();
		assertEquals(2, targets.size());
		assertTrue(targets.contains(board.getCell(5, 13)));
		assertTrue(targets.contains(board.getCell(5, 15)));
		
		// test a roll of 3
		board.calcTargets(board.getCell(5, 14), 3);
		targets= board.getTargets();
		assertEquals(3, targets.size());
		assertTrue(targets.contains(board.getCell(5, 11)));
		assertTrue(targets.contains(board.getCell(4, 12)));
		assertTrue(targets.contains(board.getCell(6, 12)));

		
		// test a roll of 4
		board.calcTargets(board.getCell(5, 14), 4);
		targets= board.getTargets();
		assertEquals(4, targets.size());
		assertTrue(targets.contains(board.getCell(4, 11)));
		assertTrue(targets.contains(board.getCell(6, 11)));
		assertTrue(targets.contains(board.getCell(3, 12)));
		assertTrue(targets.contains(board.getCell(7, 12)));
	}

	@Test
	public void testTargetsInWalkway2() {
		// test a roll of 1
		board.calcTargets(board.getCell(11, 7), 1);
		Set<BoardCell> targets= board.getTargets();
		assertEquals(4, targets.size());
		assertTrue(targets.contains(board.getCell(10, 7)));
		assertTrue(targets.contains(board.getCell(11, 6)));
		assertTrue(targets.contains(board.getCell(11, 8)));
		assertTrue(targets.contains(board.getCell(12, 7)));

		// test a roll of 3
		board.calcTargets(board.getCell(11, 7), 3);
		targets= board.getTargets();
		assertEquals(11, targets.size());
		assertTrue(targets.contains(board.getCell(7, 7)));
		assertTrue(targets.contains(board.getCell(10, 7)));
		assertTrue(targets.contains(board.getCell(11, 4)));
		assertTrue(targets.contains(board.getCell(12, 9)));
		
		// test a roll of 4
		board.calcTargets(board.getCell(11, 7), 4);
		targets= board.getTargets();
		assertEquals(15, targets.size());
		assertTrue(targets.contains(board.getCell(7, 7)));
		assertTrue(targets.contains(board.getCell(11, 1)));
		assertTrue(targets.contains(board.getCell(15, 8)));
		assertTrue(targets.contains(board.getCell(10, 10)));
		assertTrue(targets.contains(board.getCell(10, 4)));
	}

	@Test
	// test to make sure occupied locations do not cause problems
	public void testTargetsOccupied() {
		// test a roll of 3 blocked 1 up and 2 left
		board.getCell(10, 5).setOccupied(true);
		board.calcTargets(board.getCell(11, 7), 3);
		board.getCell(10, 5).setOccupied(false);
		Set<BoardCell> targets = board.getTargets();
		assertEquals(10, targets.size());
		assertTrue(targets.contains(board.getCell(7, 7)));
		assertTrue(targets.contains(board.getCell(10, 7)));
		assertTrue(targets.contains(board.getCell(11, 4)));
		assertTrue(targets.contains(board.getCell(12, 9)));
		assertFalse(targets.contains(board.getCell(10, 5)));

	
		// we want to make sure we can get into a room, even if flagged as occupied
		board.getCell(16, 4).setOccupied(true);
		board.getCell(16, 1).setOccupied(true);
		board.calcTargets(board.getCell(15, 4), 1);
		board.getCell(16, 4).setOccupied(false);
		board.getCell(16, 1).setOccupied(false);
		targets = board.getTargets();
		assertEquals(3, targets.size());
		assertTrue(targets.contains(board.getCell(14, 4)));	
		assertTrue(targets.contains(board.getCell(15, 5)));	
		assertFalse(targets.contains(board.getCell(16, 4)));	
		
		// check leaving a room with a blocked doorway
		board.getCell(12, 9).setOccupied(true);
		board.calcTargets(board.getCell(15, 8), 3);
		board.getCell(12, 9).setOccupied(false);
		targets= board.getTargets();
		assertEquals(0, targets.size());
		assertFalse(targets.contains(board.getCell(12, 7)));
		assertFalse(targets.contains(board.getCell(12, 11)));	
		assertFalse(targets.contains(board.getCell(10, 9)));

	}
}
