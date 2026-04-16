package experiment;

import java.util.HashSet;
import java.util.Set;

public class TestBoardCell {
    private int row;
    private int column;
    private Set<TestBoardCell> adjacencyList;
    private boolean isRoom;
    private boolean isOccupied;


    public TestBoardCell(int row, int column) {
        this.row = row;
        this.column = column;
        this.adjacencyList = new HashSet<TestBoardCell>();
    }

    public void addAdjacency(TestBoardCell cell) {
        adjacencyList.add(cell);
    }

    public Set<TestBoardCell> getAdjList() {
        return adjacencyList;
    }

    public void setRoom(boolean val) {
        isRoom = val;
    }

    public boolean getRoom() {
        return isRoom;
    }

    public void setOccupied(boolean val) {
        isOccupied = val;
    }

    public boolean getOccupied() {
        return isOccupied;
    }

}
