package tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.jupiter.api.Assertions.*;

import org.junit.Assert;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import clueGame.Board;
import clueGame.BoardCell;
import clueGame.DoorDirection;
import clueGame.Room;

class FileInitTests {
	private static Board board;
	
	public static final int NUM_ROWS = 18;
	public static final int NUM_COLUMNS = 18;
	public static final int LEGEND_SIZE = 10;

	@BeforeEach
	void setUp() throws Exception {
		board = Board.getInstance();
		board.setConfigFiles("ClueLayout.csv", "ClueSetup.txt");
		board.initialize();
	}
	
	@Test
	void testRoomLabels() {
		assertEquals("Cellar", board.getRoom('C').getName() );
		assertEquals("Living Room", board.getRoom('L').getName() );
		assertEquals("Nursery", board.getRoom('N').getName() );
		assertEquals("Bedroom", board.getRoom('B').getName() );
		assertEquals("Family Room", board.getRoom('F').getName() );
	}
	
	@Test
	void testBoardSize() {
		assertEquals(NUM_ROWS, board.getNumRows());
		assertEquals(NUM_COLUMNS, board.getNumColumns());
	}
	
	@Test
	void testDoorDirections() {
		// test directions
		BoardCell cell = board.getCell(4, 3);
		assertTrue(cell.isDoorway());
		assertEquals(DoorDirection.LEFT, cell.getDoorDirection());
		
		cell = board.getCell(10, 8);
		assertTrue(cell.isDoorway());
		assertEquals(DoorDirection.UP, cell.getDoorDirection());
		
		
		cell = board.getCell(9, 12);
		assertTrue(cell.isDoorway());
		assertEquals(DoorDirection.RIGHT, cell.getDoorDirection());
		cell = board.getCell(12, 9);
		assertTrue(cell.isDoorway());
		assertEquals(DoorDirection.DOWN, cell.getDoorDirection());
		
		// test that cells that aren't doorways in file aren't doorways on board
		cell = board.getCell(7, 11);
		assertFalse(cell.isDoorway());
		
		cell = board.getCell(3, 3);
		assertFalse(cell.isDoorway());
	}
	
	@Test
	void testDoorAmount() {
		int numDoors = 0;
		
		for (int rows = 0; rows < NUM_ROWS; rows++) {
			for (int columns = 0; columns < NUM_COLUMNS; columns++) {
				BoardCell cell = board.getCell(rows, columns);
				if (cell.isDoorway()) {
					numDoors++;
				}
			}
		}
		
		Assert.assertEquals(9, numDoors);
	}
	
	@Test
	void testRooms() {
		// test room centers and label positions
		//fail("Not yet implemented");
		
		// Family Room
		BoardCell cell = board.getCell( 15, 8);
		Room room = board.getRoom( cell ) ;
		assertTrue( room != null );
		assertEquals( room.getName(), "Family Room" ) ;
		assertFalse( cell.isLabel() );
		assertTrue( cell.isRoomCenter() ) ;
		assertFalse( cell.isDoorway()) ;
		
		// Kitchen
		cell = board.getCell( 6, 7);
		room = board.getRoom( cell ) ;
		assertTrue( room != null );
		assertEquals( room.getName(), "Kitchen" ) ;
		assertTrue( cell.isLabel() );
		assertFalse( cell.isRoomCenter() ) ;
		assertFalse( cell.isDoorway()) ;
		
		// Bedroom
		cell = board.getCell( 9, 15);
		room = board.getRoom( cell ) ;
		assertTrue( room != null );
		assertEquals( room.getName(), "Bedroom" ) ;
		assertFalse( cell.isLabel() );
		assertFalse( cell.isRoomCenter() ) ;
		assertFalse( cell.isDoorway()) ;
		
		// Cellar
		cell = board.getCell( 5, 1);
		room = board.getRoom( cell ) ;
		assertTrue( room != null );
		assertEquals( room.getName(), "Cellar" ) ;
		assertFalse( cell.isLabel() );
		assertFalse( cell.isRoomCenter() ) ;
		assertFalse( cell.isDoorway()) ;
	}

}
