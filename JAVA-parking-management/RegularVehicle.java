package myproject;

public class RegularVehicle implements Vehicle {

	private String plate;
	
	public RegularVehicle(String plate) {
		this.plate = plate;
	}
	
	@Override
	public String getPlate() {
		return plate;
	}

	@Override
	public boolean isSpecial() {
		return false;
	}

	@Override
	public Subscription getSubscription() {
		return null;
	}

}
