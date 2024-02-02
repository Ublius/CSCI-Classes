/**
 * Authors: Ben Isenhart & Tristen Erpelding
 * Date: 1 - 23 - 2024
 * Collaborators: None
 * Sources: None
 * 
 * Detective: Creates an object storing a name and badge number. These detectives are created via
 * hiring from the Station class
 */
public class Detective {
	private String detectiveName;
	private int detectiveBadgeNumber;
	
	/**
	 * Parameterized Constructor for the Detective class
	 * @param name An argument to be provided to name the new detective
	 * @param badgeNumber An argument to be provided to identify the detective numerically
	 */
	public Detective(String name, int badgeNumber) {
		detectiveName = name;
		detectiveBadgeNumber = badgeNumber;
	}
	
	@Override
	public String toString() {
		return "Detective [Badge=" + detectiveBadgeNumber + ", Name=" + detectiveName + "]";
	}
}
