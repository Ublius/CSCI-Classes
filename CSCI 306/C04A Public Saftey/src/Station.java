import java.util.Scanner;

/**
 * Authors: Ben Isenhart & Tristen Erpelding
 * Date: 1 - 23 - 2024
 * Collaborators: None
 * Sources: Just in-class videos
 * 
 * Station: The station class creates and maintains a list of all detectives working for the station.
 * Alongside this, it keeps track of badge numbers for new detective fires to each specific Station instance.
 */
public class Station {
	private String stationName;
	private Detective[] detectiveArr;
	private int numDetectivesAtStation;
	public static final int MAX_DETECTIVES = 5;
	public static int lastBadgeNumber = 1;
	
	/**
	 * Parameterized Constructor for the Station class
	 * @param name Given name for the new Station
	 */
	public Station(String name) {
		stationName = name;
		detectiveArr = new Detective[MAX_DETECTIVES];
	}
	
	/**
	 * Prints out each detective's badge number and name that is employed by this station
	 */
	public void printDetectiveList() {
		System.out.println("List of detectives for " + stationName);
		for (int i = 0; i < numDetectivesAtStation; i++) {
			System.out.println(detectiveArr[i].toString());
		}
		System.out.println();
	}
	
	/**
	 * Hires a new detective from user input for the detective's name. If there are already
	 * 5 employed detectives, the method will reject new inputs. Otherwise, it creates a new
	 * detective and adds them to the list of detectives working for the station
	 */
	public void newHire() {
		if (numDetectivesAtStation < 5) {
			Scanner scan = new Scanner(System.in);
			System.out.print("New hire for " + stationName + "...Enter detective's name: ");
			String newHireName = scan.nextLine();
			
			detectiveArr[numDetectivesAtStation] = new Detective(newHireName, lastBadgeNumber);
			lastBadgeNumber += 1;
			numDetectivesAtStation += 1;
		} else {
			System.out.println("Can't hire any more detectives for " + stationName);
		}
	}
}
