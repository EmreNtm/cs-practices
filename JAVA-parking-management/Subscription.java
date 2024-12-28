package myproject;

public class Subscription {

	Date begin;
	Date end;
	private SubscribedVehicle vehicle;
	
	public Subscription(Date begin, Date end, String plate) {
		this.begin = begin;
		this.end = end;
		this.vehicle = new SubscribedVehicle(plate, this);
	}
	
	//Baþlangýç ve bitiþ gününü de dahil ettim.
	public boolean isValid() {
		return ((Date.getToday().isBeforeThan(end) || Date.getToday().isEqualsWith(end)) && 
				(Date.getToday().isAfterThan(begin) || Date.getToday().isEqualsWith(begin)));
	}

	public Date getBegin() {
		return begin;
	}

	public void setBegin(Date begin) {
		this.begin = begin;
	}

	public Date getEnd() {
		return end;
	}

	public void setEnd(Date end) {
		this.end = end;
	}

	public void setVehicle(SubscribedVehicle vehicle) {
		this.vehicle = vehicle;
	}
	
	public SubscribedVehicle getVehicle() {
		return vehicle;
	}
	
}
