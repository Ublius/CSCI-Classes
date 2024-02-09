/** 
 * Piece: Stores piece information 
 * Author: Caleb Bartel 
 */

public class Piece {
	private int row;
	private int col;
	public Color color;
	public boolean king = false;

	/*
	 * Piece: Constructor for the Piece class
	 */
	public Piece(int row, int col, Color color) {
		this.row = row;
		this.col = col;
		this.color = color;
	}

	/** 
	 * draw: Draws the piece depending on the color
	 * Following the contract described in the drawBoard method of the Board class
	 */
	public void draw() {
		String symbol;
		if (color == Color.RED) {
			symbol = "r";
		}
		else {
			symbol = "b";
		}
		if (king) {
			symbol = symbol.toUpperCase();
		}
		System.out.print(symbol);
	}
}
