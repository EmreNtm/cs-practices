package myproject;

public class ParkRecord {
	
	Time enterTime;
	Time exitTime;
	Vehicle vehicle;
	
	public ParkRecord(int enterHour, int enterMinute, int exitHour, int exitMinute, Vehicle vehicle) {
		enterTime = new Time(enterHour, enterMinute);
		exitTime = new Time(exitHour, exitMinute);
		this.vehicle = vehicle;
	}
	
	public ParkRecord(int enterHour, int enterMinute, Vehicle vehicle) {
		enterTime = new Time(enterHour, enterMinute);
		exitTime = null;
		this.vehicle = vehicle;
	}
	
	public ParkRecord(int enterHour, int enterMinute, int exitHour, int exitMinute) {
		enterTime = new Time(enterHour, enterMinute);
		exitTime = new Time(exitHour, exitMinute);
		this.vehicle = null;
	}
	
	public ParkRecord(int enterHour, int enterMinute) {
		enterTime = new Time(enterHour, enterMinute);
		exitTime = null;
		this.vehicle = null;
	}
	
	//Araç hala çýkmadýysa -1 döner.
	public int getParkingDuration() {
		if(exitTime != null)
			return enterTime.getDifference(exitTime);
		else {
			System.out.println("Arac henüz cikis yapmadi!");
			return -1;
		}
	}
	
	public String toString() {
		String s = "";
		s +=  "	Aracin plakasi: " + vehicle.getPlate() + "\n"; 
		if(vehicle.isSpecial()) {
			s += "	Arac özel (Resmi veya Abone)\n"; 
		}
		s +=	"	Giris zamani (Hour / Min): " + enterTime.getHour() + " / " + enterTime.getMinute() + "\n";
		if(exitTime != null) {
			s +=	"	Cikis zamani (Hour / Min): " + exitTime.getHour() + " / " + exitTime.getMinute() + "\n";
			s +=	"	Toplam park zamani (Hour / Min): " + (getParkingDuration() / 60) + " / " + (getParkingDuration() % 60) + "\n";
		} else {
			s += 	"	Cikis yapilmadi\n";
		}
		return s;
	}
	
	public Time getExitTime() {
		return exitTime;
	}

	public void setExitTime(int exitHour, int exitMinute) {
		this.exitTime = new Time(exitHour, exitMinute);
	}
	
	public Time getEnterTime() {
		return enterTime;
	}

	public void setEnterTime(int enterHour, int enterMinute) {
		this.enterTime = new Time(enterHour, enterMinute);
	}
	
	public Vehicle getVehicle() {
		return vehicle;
	}
	
	public void setVehicle(Vehicle vehicle) {
		this.vehicle = vehicle;
	}
}
