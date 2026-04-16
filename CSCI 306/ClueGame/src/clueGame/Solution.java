package clueGame;

public class Solution {
    private Card room;
    private Card person;
    private Card weapon;

    public Solution(Card room, Card person, Card weapon) {
        this.room = room;
        this.person = person;
        this.weapon = weapon;
    }

    public Card getRoom() {
        return room;
    }

    public Card getPerson() {
        return person;
    }

    public Card getWeapon() {
        return weapon;
    }
    
    public boolean equals(Solution other) {
    	return (person.equals(other.getPerson()) && weapon.equals(other.getWeapon())
				&& room.equals(other.getRoom()));
    }
    
    // for debugging
	@Override
	public String toString() {
		return room.getCardName() + ", " + person.getCardName() + ", " + weapon.getCardName();
	}
}
