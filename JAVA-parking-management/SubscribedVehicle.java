package myproject;

public class SubscribedVehicle implements Vehicle {

	private String plate;
	private Subscription sub;
	
	public SubscribedVehicle(String plate, Subscription sub) {
		this.plate = plate;
		this.sub = sub;
	}
	
	@Override
	public String getPlate() {
		return plate;
	}

	@Override
	public boolean isSpecial() {
		return true;
	}

	@Override
	public Subscription getSubscription() {
		return sub;
	}
	
}
