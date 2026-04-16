package clueGame;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.util.HashSet;
import java.util.Set;

public class BoardCell {
    private int row;
    private int column;
    private char initial;
    private DoorDirection doorDirection;
    private boolean isTarget;
    private boolean roomLabel;
    private boolean roomCenter;
    private char secretPassage; // default is X for when there's no passage
    private Set<BoardCell> adjacencyList;
    private boolean isRoom;
    private boolean isOccupied;
    private Card roomCard;


    public BoardCell(int row, int column) {
        this.row = row;
        this.column = column;
        this.adjacencyList = new HashSet<BoardCell>();
        this.isOccupied = false;
    }
    
    public BoardCell(int row, int column, char initial, boolean isRoom, boolean roomCenter, boolean roomLabel, char secretPassage, DoorDirection doorDirection) {
        this.row = row;
        this.column = column;
        this.adjacencyList = new HashSet<BoardCell>();
        this.isOccupied = false;
        this.initial = initial;
        this.isRoom = isRoom;
        this.roomCenter = roomCenter;
        this.roomLabel = roomLabel;
        this.secretPassage = secretPassage;
        this.doorDirection = doorDirection;
        this.roomCard = null;
        this.isTarget = false;
    }
    
    public void drawCell(Graphics graphics, int x, int y, int width, int height) {
    	Color color = null;
    	Board board = Board.getInstance();
    	
    	final int ALPHA = 255 * 4 / 5;
    	
    	// check if room is a target
    	BoardCell roomCenterCell = null;
    	if (isRoom) {
    		roomCenterCell = board.getRoom(initial).getCenterCell();
    	}
    	
    	// color targets
    	if (isTarget || (roomCenterCell != null && roomCenterCell.isTarget())) {
            // The cell is a target than can be moved to.
    		//color = Color.YELLOW;
    		color = new Color(252,252,108, ALPHA);
    	}
    	else if (isRoom) {
    		color = new Color(169,209,142, ALPHA);
    	}
    	else if (initial == 'W') { // walkway
    		color = new Color(180,199,231, ALPHA);
    	}
    	else if (initial == 'X') { // unused
    		//color = Color.GRAY;
    		color = new Color(122,122,122,ALPHA);
    	}
    	graphics.setColor(color);
    	graphics.fillRect(x, y, width, height);
    	
    	
    	
    	// draw doorway indicators
    	graphics.setColor(new Color(143,170,220));
    	switch (doorDirection) {
		case DOWN:
			graphics.fillRect(x, y + height - (height/5), width, height/5);
			break;
		case LEFT:
			graphics.fillRect(x, y, width/5, height);
			break;
		case RIGHT:
			graphics.fillRect(x + width - (width/5), y, width/5, height);
			break;
		case UP:
			graphics.fillRect(x, y, width, (height/5));
			break;
		default: // covers NONE
			break;
    	}
    	
    	// draw border over fill, so it doesn't get covered
    	if (color == null) {
    		graphics.setColor(Color.BLACK);
    	}
    	else {
    		graphics.setColor(color.darker());
    	}
    	graphics.drawRect(x, y, width, height);
    }
    
    public boolean contains(int x, int y, int width, int height) {
    	Rectangle area = new Rectangle(column*width,row*height,width,height);
    	return (area.contains(new Point(x,y)));
    }

    public void addAdjacency(BoardCell cell) {
        adjacencyList.add(cell);
    }

    public Set<BoardCell> getAdjList() {
        return adjacencyList;
    }

    public void setRoom(boolean val) {
        isRoom = val;
    }

    public boolean getIsRoom() {
        return isRoom;
    }

    public void setOccupied(boolean val) {
        isOccupied = val;
    }
    
	public void clearAdjacencyList() {
		adjacencyList.clear();
	}

    public void setTarget(boolean val) {
    	isTarget = val;
    }
    
    public boolean isTarget() {
    	return isTarget;
    }

    public void setRoomCard(Card roomCard) {
        this.roomCard = roomCard;
    }

    public boolean getOccupied() {
        return isOccupied;
    }

    public boolean isDoorway() {
    	return doorDirection != DoorDirection.NONE;
    }

    public DoorDirection getDoorDirection() {
        return doorDirection;
    }

    public boolean isLabel() {
        return roomLabel;
    }

    public boolean isRoomCenter() {
        return roomCenter;
    }

    public char getSecretPassage() {
       return secretPassage;
    }

    public char getInitial() {
        return initial;
    }

    public Card getRoomCard() {
        return roomCard;
    }
    
    public int getRow() {
    	return row;
    }
    
    public int getColumn() {
    	return column;
    }
}