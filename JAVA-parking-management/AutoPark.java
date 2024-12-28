package myproject;

import java.util.Vector;

public class AutoPark {
	
	private SubscribedVehicle[] subscribedVehicles;
	private ParkRecord[] parkRecords;
	private Vector<Vehicle> inPark;
	private int vehicleNumber;
	private int parkIndex;
	private double hourlyFee;
	private double incomeDaily;
	
	public AutoPark(double hourlyFee, int capacity) {
		this.hourlyFee = hourlyFee;
		parkRecords = new ParkRecord[0];
		inPark = new Vector<Vehicle>(capacity);
		vehicleNumber = 0;
		parkIndex = 0;
		subscribedVehicles = new SubscribedVehicle[0];
	}
	
	public SubscribedVehicle searchVehicle(String plate) {
		for(SubscribedVehicle v : subscribedVehicles) {
			if(v.getPlate().compareTo(plate) == 0)
				return v;
		}
		return null;
	}
	
	public boolean isParked(String plate) {
		for(Vehicle v : inPark) {
			if(v.getPlate().compareTo(plate) == 0) {
				return true;
			}
		}
		return false;
	}

	private void enlargeVehicleArray() {
		SubscribedVehicle[] tmp = new SubscribedVehicle[subscribedVehicles.length + 1];
		java.lang.System.arraycopy(subscribedVehicles, 0, tmp, 0, subscribedVehicles.length);
		subscribedVehicles = tmp;
	}
	
	private void enlargeParkRecordsArray() {
		ParkRecord[] tmp = new ParkRecord[parkRecords.length + 1];
		java.lang.System.arraycopy(parkRecords, 0, tmp, 0, parkRecords.length);
		parkRecords = tmp;
	}
	
	public boolean addVehicle(SubscribedVehicle v) {
		if(searchVehicle(v.getPlate()) != null) {
			System.out.println("Araç zaten ekli!");
			return false;
		} else {
			enlargeVehicleArray();
			subscribedVehicles[subscribedVehicles.length-1] = v;
			return true;
		}
			
	}
	
	public boolean vehicleEnters(String plate, Time enter, boolean isOfficial) {
		if(!isParked(plate)) {
			if(vehicleNumber >= inPark.capacity()) {
				System.out.println("Park kapasitesi dolu!");
				return false;
			}
			Vehicle v = null;
			if(isOfficial) 
				v = new OfficialVehicle(plate);
			else {
				for(Vehicle a : subscribedVehicles)
					if(a.getPlate().compareTo(plate) == 0)
						v = a;
				if(v == null) {
					v = new RegularVehicle(plate);
				}
			}
			enlargeParkRecordsArray();
			parkRecords[parkIndex] = new ParkRecord(enter.getHour(), enter.getMinute(), v);
			inPark.add(v);
			parkIndex++;
			vehicleNumber++;
			return true;
		}
		System.out.println("Arac zaten otoparkta!");
		return false;
	}
	
	//Projeden anladýðým kadarýyla eðer aracýn para ödemesi gerekiyorsa true, gerekmyiorsa false döndürmesi gerekiyor.
	//Çözümü bu þekilde yaptým. Ayrýca araç eðer otoparkta deðil ise yine false döndürüyor.
	public boolean vehicleExits(String plate, Time exit) {
		if(isParked(plate)) {
			Vehicle tmpVehicle = null;
			ParkRecord tmpRecord = null;
			for(Vehicle v : inPark) 
				if(v.getPlate().compareTo(plate) == 0) {
					tmpVehicle = v;
					break;
				}
			tmpRecord = findRecord(tmpVehicle);
			if(tmpVehicle instanceof RegularVehicle) {
				tmpRecord.setExitTime(exit.getHour(), exit.getMinute());
				double fee = (int)(tmpRecord.getParkingDuration() / 60) * hourlyFee;
				incomeDaily += fee;
				System.out.println("\nAracin cikis yapmasi icin " + fee + " odemesi gerekmektedir.");
				inPark.remove(tmpVehicle);
				vehicleNumber--;
				return true;
			} else {
				if(tmpVehicle instanceof SubscribedVehicle && ((SubscribedVehicle) tmpVehicle).getSubscription().isValid() ||
						tmpVehicle instanceof OfficialVehicle) {
					tmpRecord.setExitTime(exit.getHour(), exit.getMinute());
					System.out.println("\nAracin cikis yapmasi icin odeme yapmasina gerek yoktur.");
					inPark.remove(tmpVehicle);
					vehicleNumber--;
					return false;
				} else {
					tmpRecord.setExitTime(exit.getHour(), exit.getMinute());
					double fee = (int)(tmpRecord.getParkingDuration() / 60) * hourlyFee;
					incomeDaily += fee;
					System.out.println("\nAbonelik bitmiþ! Aracin cikis yapmasi icin " + fee + " odemesi gerekmektedir.");
					inPark.remove(tmpVehicle);
					vehicleNumber--;
					return true;
				}
			}
		}
		System.out.println("\nArac zaten otoparkta degildi!");
		return false;
	}
	
	public String toString() {
		String s = "";
		s = s + "\nOtoparkin gunluk geliri: " + incomeDaily + "\n";
		s += "Otoparka abone araclar:\n";
		int i;
		for(i = 0; i < subscribedVehicles.length; i++) {
			s = s + (i+1) + ". aracin plakasi: " + subscribedVehicles[i].getPlate();
			if(!subscribedVehicles[i].getSubscription().isValid())
				s += " (Aracin aboneligi bitmis!)";
			s += "\n";
		}
		s += "\nOtopark kayitlari:\n";
		for(i = 0; i < parkRecords.length; i++) {
			if(parkRecords[i] != null)
				s = s + (i+1) + ". kayit: \n" + parkRecords[i].toString() + "\n";
		}
		return s;
	}
	
	public ParkRecord findRecord(Vehicle v) {
		for(ParkRecord r : parkRecords)
			if(r.getVehicle().getPlate().compareTo(v.getPlate()) == 0)
				return r;
		return null;
	}
	
	public SubscribedVehicle[] getSubscribedVehicles() {
		return subscribedVehicles;
	}

	public void setSubscribedVehicles(SubscribedVehicle[] subscribedVehicles) {
		this.subscribedVehicles = subscribedVehicles;
	}

	public ParkRecord[] getParkRecords() {
		return parkRecords;
	}

	public void setParkRecords(ParkRecord[] parkRecords) {
		this.parkRecords = parkRecords;
	}

	public double getHourlyFee() {
		return hourlyFee;
	}

	public void setHourlyFee(double hourlyFee) {
		this.hourlyFee = hourlyFee;
	}

	public double getIncomeDaily() {
		return incomeDaily;
	}

	public void setIncomeDaily(double incomeDaily) {
		this.incomeDaily = incomeDaily;
	}
	
}
