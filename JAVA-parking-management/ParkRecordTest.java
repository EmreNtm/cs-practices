package myproject;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

class ParkRecordTest {

	@Test
	void getParkingDurationTest() {
		AutoPark otopark = new AutoPark(10, 3);
		Time t = new Time(12, 15);
		Time exit = new Time(11, 20);
		boolean isOfficial = true;
		String plate = "test";
		
		otopark.vehicleEnters(plate, t, isOfficial);
		otopark.vehicleExits(plate, exit);
		ParkRecord r = otopark.findRecord(new OfficialVehicle(plate));
		int output = r.getParkingDuration();
		assertEquals(55, output);
	}

}
