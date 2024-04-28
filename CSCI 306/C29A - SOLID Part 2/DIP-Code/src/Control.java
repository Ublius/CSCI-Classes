public class Control {
	public static void main(String[] args) {
		LightBulb lightBulb = new LightBulb() ;
		ElectricPowerSwitch bulbSwitch = new ElectricPowerSwitch( lightBulb ) ;
		
		bulbSwitch.press() ;
		bulbSwitch.press() ;


		Fan fan = new Fan() ;
		ElectricPowerSwitch fanSwitch = new ElectricPowerSwitch( fan ) ;
		
		fanSwitch.press() ;
		fanSwitch.press() ;
	}
}
