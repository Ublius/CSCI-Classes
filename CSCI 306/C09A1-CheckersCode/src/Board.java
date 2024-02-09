/** 
 * Board: Creates tiles, pieces, and draws the board
 * Author: Caleb Bartel 
 */

import java.util.ArrayList;

public class Board {
	static final int BOARD_DIMENSION = 8;
	static final int NUM_PIECE_ROWS = 3;
	private Tile[][] tiles = new Tile[BOARD_DIMENSION][BOARD_DIMENSION];
	private ArrayList<Piece> piecesArrayList = new ArrayList<Piece>();
	
	/** 
	 * Default Constructor: Initializes tiles and pieces to default orientation
	 */
	public Board() {
		Color curTileColor = Color.RED;
		for (int row = 0; row < BOARD_DIMENSION; row++){
			for (int col = 0; col < BOARD_DIMENSION; col++){
				tiles[row][col] = new Tile(row,col, curTileColor);
				if (curTileColor == Color.BLACK && row < NUM_PIECE_ROWS) {
					newPiece(row, col, curTileColor);
				}
				if (curTileColor == Color.BLACK && row > BOARD_DIMENSION - NUM_PIECE_ROWS - 1) {
					newPiece(row, col, curTileColor);
				}
				curTileColor = switchColor(curTileColor);
			}
			curTileColor = switchColor(curTileColor);
		}
	}
	
	/*
	 * switchColor: switches the color of the tile
	 */
	private Color switchColor(Color curColor) {
		if (curColor == Color.RED) {
			return Color.BLACK;
		}
		else {
			return Color.RED;
		}
	}

	/*
	 * newPiece: creates a new piece and adds it to the piecesArrayList
	 */
	private void newPiece(int row, int col, Color color) {
		Piece piece = new Piece(row, col, color);
		piecesArrayList.add(piece);
		tiles[row][col].setPiece(piece);
	}

	/** 
	 * drawBoard: loops through the tiles and draws them with added styling
	 * 
	 * Drawing contract: 
	 * Print a single character to standard out
	 */
	private void drawBoard() {
		for (int i = 0; i < BOARD_DIMENSION; i++){
			System.out.println("");
			for (int j = 0; j < BOARD_DIMENSION; j++){
				System.out.print("----");
			}
			System.out.println("");
			for (int j = 0; j < BOARD_DIMENSION; j++){
				System.out.print("| ");
				tiles[i][j].draw();
				System.out.print(" ");
			}	
		}
	}

	/** 
	 * main: test main to draw initial board arrangement
	 */
	public static void main(String[] args) {
		Board board = new Board();
		board.drawBoard();
	}

}
