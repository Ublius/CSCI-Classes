package clueGame;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;

public class Room {
    private String name;
    private Card card;
    private BoardCell labelCell;
    private BoardCell centerCell;
    private BoardCell secretCell;

   public Room(String name) {
		super();
		this.name = name;
	}
   
   public Room(String name, Card card) {
	   super();
	   this.name = name;
	   this.card = card;
   }

   public void drawName(Graphics graphics, int x, int y) {
	   graphics.setColor(Color.BLACK);
      graphics.setFont(new Font("TimesRoman", Font.BOLD, 12));
	   graphics.drawString(name, x, y);
   }
   
	public void setLabelCell(BoardCell labelCell) {
		this.labelCell = labelCell;
	}

	public void setCenterCell(BoardCell centerCell) {
		this.centerCell = centerCell;
	}

	public String getName() {
       return name;
    }

    public BoardCell getLabelCell() {
       return labelCell;
    }

    public BoardCell getCenterCell() {
       return centerCell;
    }

   public void setSecretCell(BoardCell secretCell) {
      this.secretCell = secretCell;
   }

   public BoardCell getSecretCell() {
      return secretCell;
   }
   
   public Card getCard() {
	   return card;
   }
}
