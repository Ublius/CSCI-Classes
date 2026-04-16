package clueGame;

public class Card {
    private String cardName;
    private CardType cardType;
    private Player player = null;
    private Room room = null;

    public Card(String cardName, CardType cardType) {
        this.cardName = cardName;
        this.cardType = cardType;
    }
    
    public Card (String cardName, CardType cardType, Player player) {
    	this.cardName = cardName;
        this.cardType = cardType;
        this.player = player;
    }
    
    public Card (String cardName, CardType cardType, Room room) {
    	this.cardName = cardName;
        this.cardType = cardType;
        this.room = room;
    }

    public boolean equals(Card target) {
    	return (this.cardName.equals(target.getCardName()) && this.cardType == target.getCardType());
    }

    public CardType getCardType() {
        return cardType;
    }


    public String getCardName() {
        return cardName;
    }
    
    public Player getPlayer() {
    	return player;
    }
    
    public Room getRoom() {
    	return room;
    }
    
    public void setRoom(Room room) {
    	this.room = room;
    }
}
