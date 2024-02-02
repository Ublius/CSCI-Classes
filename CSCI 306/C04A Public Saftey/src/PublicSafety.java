/**
 * Authors: Ben Isenhart & Tristen Erpelding
 * Date: 1 - 23 - 2024
 * Collaborators: None
 * Sources: None
 * 
 * PublicSafety: The PublicSafety class creates a pair of Station classes, one in a city and the
 * other in the university of that city. These stations can be queried via the ControlCenter class
 */
public class PublicSafety {
	private Station cityStation;
	private Station universityStation;
	
	/**
	 * Parameterized Constructor for the PublicSafety class
	 * @param universityName Given name for the university's station
	 * @param cityName Given name for the city's station
	 */
	public PublicSafety(String universityName, String cityName) {
		universityStation = new Station(universityName);
		cityStation = new Station(cityName);
	}
	
	/**
	 * Accessible method from ControlCenter for hiring a new detective at one of the two stations.
	 * The actual hiring process is done by the Station itself
	 * @param cityStatus True represents a city hire, False represents a university hire
	 */
	public void doHire(boolean cityStatus) {
		if (cityStatus) {
			cityStation.newHire();
		} else {
			universityStation.newHire();
		}
	}
	
	/**
	 * Outputs the list of all detectives working in both the city and university station via the
	 * Station class implementation for displaying this list
	 */
	public void printDetectiveLists() {
		cityStation.printDetectiveList();
		universityStation.printDetectiveList();
	}
}
