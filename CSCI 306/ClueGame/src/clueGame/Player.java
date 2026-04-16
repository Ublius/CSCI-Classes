package clueGame;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.Set;

public abstract class Player {
    private String name;
    private Color color;
    private int row, column;
    private ArrayList<Card> hand;
    private Set<Card> seen;
    protected boolean active; // true if it's this player's turn
    private boolean canStayOnTile;
    private boolean madeMove = false;

    
    public Player(String name, String color, int row, int column) {
        this.name = name;
        
        try {
            Field field = Color.class.getField(color);
            this.color = (Color)field.get(null);
        } catch (Exception e) {
            this.color = null;
        }
        
        this.row = row;
        this.column = column;
        
        this.hand = new ArrayList<Card>();
        this.seen = new HashSet<Card>();
        this.active = false;
    }
    
    public void drawPlayer(Graphics g, int x, int y, int width, int height) {
    	Board board = Board.getInstance();
    	
    	BufferedImage image = board.getPlayerImage();
    	
    	g.setColor(color);
    	g.fillOval(x, y, width, height);
    	if (image != null) {
    		g.drawImage(image,x,y,x + width,y + height, 0, 0, image.getWidth(), image.getHeight(), null, null);
    	}	
    }
    
    /**
     * Decide where to go, given a list of valid targets
     * Has to call movePlayer itself instead of returning target location
     * Because suggestions can only be made after moving
     * @param targets: set of valid boardcells for movement
     */
    public abstract void playTurn(Set<BoardCell> targets);
    
    public String getName() {
    	return name;
    }
    
    public Color getColor() {
    	return color;
    }
    
    public boolean hasSeen(Card card) {
    	return false;
    }

    public void updateHand(Card card) {
    	hand.add(card);
    }
    
    public void updateSeen(Card card) {
    	seen.add(card);
    }

    public void clearSeen() {
		seen.clear();
	}
    
    /**
     * Create a suggestion based on seen cards and current room
     * Return a Solution containing suggested cards
     * 
     * @param room
     * @return
     */
    public abstract Solution createSuggestion(Room room);
    
    public Card disproveSuggestion(Card person, Card weapon, Card room) {
        List<Card> matchingCards = new ArrayList<>();
    
        if (!hand.isEmpty()) {
        	for (Card card : hand) {
	            if (card.equals(person) || card.equals(weapon) || card.equals(room)) {
	                matchingCards.add(card);
	            }
        	}
        }
        
    
        return matchingCards.isEmpty() ? null : matchingCards.get(new Random().nextInt(matchingCards.size()));
    }

	public ArrayList<Card> getCardList() {
		return hand;
	}
	
	public Room getRoom() {
		Board board = Board.getInstance();
		return board.getRoom(board.getCell(this.row, this.column));
	}

    public int getRow() {
        return row;
    }

    public int getColumn() {
        return column;
    }

    public Set<Card> getSeen() {
        return seen;
    }
    
    public void setLocation(BoardCell location) {
    	this.row = location.getRow();
    	this.column = location.getColumn();
    }

    public BoardCell getLocation() {
        Board board = Board.getInstance();
    	return board.getCell(row, column);
    }
    
    public boolean getActive() {
    	return active;
    }
    
    public void setActive(boolean bool) {
    	active = bool;
    }

    public void setCanStay(boolean canStay) {
		canStayOnTile = canStay;
	}
	
	public boolean getCanStay() {
		return canStayOnTile;
	}

    public boolean getMadeMove() {
        return madeMove;
    }

    public void setMadeMove(boolean madeMove) {
        this.madeMove = madeMove;
    }


}
