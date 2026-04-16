package tests;

import experiment.*;
import static org.junit.jupiter.api.Assertions.*;

import java.util.Set;

import org.junit.Assert;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class BoardTestsExp {
	private TestBoard board;

	@BeforeEach
	void setUp() throws Exception {
		this.board = new TestBoard();
	}

	@Test
	void testTargetsNormal() {
		// test roll of 2
		TestBoardCell cell2 = board.getCell(0, 0);
		board.calcTargets(cell2, 2);
		Set<TestBoardCell> targets = board.getTargets();
		
		Assert.assertEquals(3, targets.size());
		Assert.assertTrue(targets.contains(board.getCell(0, 2)));
		Assert.assertTrue(targets.contains(board.getCell(1, 1)));
		Assert.assertTrue(targets.contains(board.getCell(2, 0)));
		
		// test roll of 3
		TestBoardCell cell3 = board.getCell(0, 0);
		board.calcTargets(cell3, 3);
		Set<TestBoardCell> targets3 = board.getTargets();
		
		Assert.assertEquals(6, targets3.size());
		Assert.assertTrue(targets3.contains(board.getCell(3, 0)));
		Assert.assertTrue(targets3.contains(board.getCell(2, 1)));
		Assert.assertTrue(targets3.contains(board.getCell(0, 1)));
		Assert.assertTrue(targets3.contains(board.getCell(1, 2)));
		Assert.assertTrue(targets3.contains(board.getCell(0, 3)));
		Assert.assertTrue(targets3.contains(board.getCell(1, 0)));
		
		// test roll of 4
		TestBoardCell cell4 = board.getCell(0, 0);
		board.calcTargets(cell4, 4);
		Set<TestBoardCell> targets4 = board.getTargets();
		
		Assert.assertEquals(6, targets4.size());
		Assert.assertTrue(targets4.contains(board.getCell(1, 1)));
		Assert.assertTrue(targets4.contains(board.getCell(1, 3)));
		Assert.assertTrue(targets4.contains(board.getCell(2, 0)));
		Assert.assertTrue(targets4.contains(board.getCell(2, 2)));
		Assert.assertTrue(targets4.contains(board.getCell(3, 1)));
		Assert.assertTrue(targets4.contains(board.getCell(0, 2)));
		
		// test roll of 5
		TestBoardCell cell5 = board.getCell(0, 0);
		board.calcTargets(cell5, 5);
		Set<TestBoardCell> targets5 = board.getTargets();
		
		Assert.assertEquals(8, targets5.size());
		Assert.assertTrue(targets5.contains(board.getCell(0, 1)));
		Assert.assertTrue(targets5.contains(board.getCell(0, 3)));
		Assert.assertTrue(targets5.contains(board.getCell(1, 0)));
		Assert.assertTrue(targets5.contains(board.getCell(1, 2)));
		Assert.assertTrue(targets5.contains(board.getCell(3, 0)));
		Assert.assertTrue(targets5.contains(board.getCell(3, 2)));
		Assert.assertTrue(targets5.contains(board.getCell(2, 1)));
		Assert.assertTrue(targets5.contains(board.getCell(2, 3)));
		
		// test roll of 6
		TestBoardCell cell6 = board.getCell(0, 0);
		board.calcTargets(cell6, 6);
		Set<TestBoardCell> targets6 = board.getTargets();
		
		Assert.assertEquals(7, targets6.size());
		Assert.assertTrue(targets6.contains(board.getCell(0, 2)));
		Assert.assertTrue(targets6.contains(board.getCell(1, 1)));
		Assert.assertTrue(targets6.contains(board.getCell(1, 3)));
		Assert.assertTrue(targets6.contains(board.getCell(2, 0)));
		Assert.assertTrue(targets6.contains(board.getCell(2, 2)));
		Assert.assertTrue(targets6.contains(board.getCell(3, 1)));
		Assert.assertTrue(targets6.contains(board.getCell(3, 3)));
		
		
		// test roll of 1 from middle
		TestBoardCell middle = board.getCell(2,2);
		board.calcTargets(middle, 1);
		Set<TestBoardCell> middleList = board.getTargets();;
		Assert.assertTrue(middleList.contains(board.getCell(2,3)));
		Assert.assertTrue(middleList.contains(board.getCell(2,1)));
		Assert.assertTrue(middleList.contains(board.getCell(3,2)));
		Assert.assertTrue(middleList.contains(board.getCell(1,2)));
		Assert.assertEquals(4, middleList.size());
		
	}
	
	@Test
	void testTargetsRoom() {
		board.getCell(2, 1).setRoom(true);
		board.getCell(3, 2).setRoom(true);
		TestBoardCell cell = board.getCell(0, 3);
		board.calcTargets(cell, 4);
		Set<TestBoardCell> targets = board.getTargets();
		Assert.assertEquals(6, targets.size());
		Assert.assertTrue(targets.contains(board.getCell(2, 1)));
		Assert.assertTrue(targets.contains(board.getCell(3, 2)));
		Assert.assertTrue(targets.contains(board.getCell(0, 1)));
		Assert.assertTrue(targets.contains(board.getCell(1, 2)));
		Assert.assertTrue(targets.contains(board.getCell(1, 0)));
		Assert.assertTrue(targets.contains(board.getCell(2, 3)));
	}

	@Test
	void testTargetsRoom6Rolls() {
		board.getCell(1, 2).setRoom(true);
		board.getCell(2, 2).setRoom(true);
		TestBoardCell cell = board.getCell(3, 3);
		board.calcTargets(cell, 6);
		Set<TestBoardCell> targets = board.getTargets();
		Assert.assertEquals(6, targets.size());
		Assert.assertTrue(targets.contains(board.getCell(2, 0)));
		Assert.assertTrue(targets.contains(board.getCell(1, 2)));
		Assert.assertTrue(targets.contains(board.getCell(2, 2)));
		Assert.assertTrue(targets.contains(board.getCell(0, 0)));
		Assert.assertTrue(targets.contains(board.getCell(1, 1)));
		Assert.assertTrue(targets.contains(board.getCell(0, 2)));
	}
	
	@Test
	void testTargetsOccupied() {
		//Set the bottom 3 rows of the game board to occupied
		for (int i = 1; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				board.getCell(i,j).setOccupied(true);
			}
		}
		//Calculate a 2-tile roll, and return the set of targets.
		TestBoardCell startCell = board.getCell(0, 0);
		board.calcTargets(startCell, 2);
		Set<TestBoardCell> targets = board.getTargets();
		//The set should only have 1 element being position (0,2)
		assertEquals(1, targets.size());
		assertTrue(targets.contains(board.getCell(0,2)));
		
	}
	
	@Test
	void testTargetsMixed() {
		// set up occupied cells
		board.getCell(0, 2).setOccupied(true);
		board.getCell(1, 2).setRoom(true);
		
		TestBoardCell cell = board.getCell(0, 3);
		board.calcTargets(cell, 3);
		Set<TestBoardCell> targets = board.getTargets();
		
		Assert.assertEquals(3, targets.size());
		Assert.assertTrue(targets.contains(board.getCell(1, 2)));
		Assert.assertTrue(targets.contains(board.getCell(2, 2)));
		Assert.assertTrue(targets.contains(board.getCell(3, 3)));
	}
	
	@Test
	void testAdjacency() {
		// top left
		TestBoardCell cell = board.getCell(0,0);
		Set<TestBoardCell> testList = cell.getAdjList();
		Assert.assertTrue(testList.contains(board.getCell(1,0)));
		Assert.assertTrue(testList.contains(board.getCell(0,1)));
		Assert.assertEquals(2, testList.size());
	
		// bottom right
		TestBoardCell bottomRight = board.getCell(3,3);
		Set<TestBoardCell> bottomRightList = bottomRight.getAdjList();
		Assert.assertTrue(bottomRightList.contains(board.getCell(2,3)));
		Assert.assertTrue(bottomRightList.contains(board.getCell(3,2)));
		Assert.assertEquals(2, bottomRightList.size());
		
		// right edge
		TestBoardCell rightEdge = board.getCell(1,3);
		Set<TestBoardCell> rightEdgeList = rightEdge.getAdjList();
		Assert.assertEquals(3, rightEdgeList.size());
		Assert.assertTrue(rightEdgeList.contains(board.getCell(0,3)));
		Assert.assertTrue(rightEdgeList.contains(board.getCell(2,3)));
		Assert.assertTrue(rightEdgeList.contains(board.getCell(1,2)));
	
		
		// left edge
		TestBoardCell leftEdge = board.getCell(3,1);
		Set<TestBoardCell> leftEdgeList = leftEdge.getAdjList();
		Assert.assertEquals(3, leftEdgeList.size());
		Assert.assertTrue(leftEdgeList.contains(board.getCell(3,0)));
		Assert.assertTrue(leftEdgeList.contains(board.getCell(3,2)));
		Assert.assertTrue(leftEdgeList.contains(board.getCell(2,1)));

		
		// middle
		TestBoardCell middle = board.getCell(2,2);
		Set<TestBoardCell> middleList = middle.getAdjList();
		Assert.assertTrue(middleList.contains(board.getCell(2,3)));
		Assert.assertTrue(middleList.contains(board.getCell(2,1)));
		Assert.assertTrue(middleList.contains(board.getCell(3,2)));
		Assert.assertTrue(middleList.contains(board.getCell(1,2)));
		Assert.assertEquals(4, middleList.size());
	}

}
