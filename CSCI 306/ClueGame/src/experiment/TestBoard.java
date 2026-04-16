package experiment;

import java.util.HashSet;
import java.util.Set;

public class TestBoard {
	private TestBoardCell[][] board;
	private Set<TestBoardCell> visited;
	private Set<TestBoardCell> targets;
	public static final int BOARD_HEIGHT = 4; // rows
	public static final int BOARD_WIDTH = 4; // cols
	
	public TestBoard() {
		super();
		
		// put cells in board so getAdjList can be called on those cells
		this.board = new TestBoardCell[BOARD_HEIGHT][BOARD_WIDTH];
		for (int row = 0; row < BOARD_HEIGHT; row++) {
			for (int col = 0; col < BOARD_WIDTH; col++) {
				this.board[row][col] = new TestBoardCell(row, col);
			}
		}

		// set up adjacency lists for each cell
		// this allows for target finding later on
		for (int row = 0; row < BOARD_HEIGHT; row++) {
			for (int col = 0; col < BOARD_WIDTH; col++) {
				if (row + 1 < BOARD_HEIGHT) {
					board[row][col].addAdjacency(board[row+1][col]);
				}
				if (row - 1 >= 0) {
					board[row][col].addAdjacency(board[row-1][col]);
				}
				if (col + 1 < BOARD_WIDTH) {
					board[row][col].addAdjacency(board[row][col+1]);
				}
				if (col - 1 >= 0) {
					board[row][col].addAdjacency(board[row][col-1]);
				}
			}
		}
	}
	
	public void calcTargets(TestBoardCell startCell, int pathLength) {
		visited = new HashSet<TestBoardCell>();
		targets = new HashSet<TestBoardCell>();
	
		visited.add(startCell);
		
		findAllTargets(startCell, pathLength);
	}
	
	private void findAllTargets(TestBoardCell startCell, int pathLength) {
		for (TestBoardCell adjCell : startCell.getAdjList()) {
			if (!visited.contains(adjCell) && !adjCell.getOccupied()) {
				visited.add(adjCell);
				if (pathLength == 1 || adjCell.getRoom()) {
					targets.add(adjCell);
				} else {
					findAllTargets(adjCell, pathLength - 1);
				}
				visited.remove(adjCell);
			}
		}
	}

	public Set<TestBoardCell> getTargets() {
		return targets;
	}

	public TestBoardCell getCell(int row, int col) {
		return board[row][col];
	}
}
