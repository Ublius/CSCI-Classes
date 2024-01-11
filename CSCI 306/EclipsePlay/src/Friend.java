/**
 * @author Tristen Erpelding
 *
 */
public class Friend {
	private String name;
	private String email;
	
	public Friend(String name, String email) {
		this.name = name;
		this.email = email;
	}

	// @Override
	// public String toString() {
	// 	return "Person [name=" + name + ", email=" + email + "]";
	// }

	public String getName() {
		return name;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getEmail() {
		return email;
	}
}
