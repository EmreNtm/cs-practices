package myproject;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

class AutoParkTest {

	//Otoparka araç ekleme
	@Test
	void vehicleEntersTest() {
		AutoPark otopark = new AutoPark(10, 3);
		Time t = new Time(12, 15);
		boolean isOfficial = true;
		String plate = "test";
		
		boolean output = otopark.vehicleEnters(plate, t, isOfficial);
		assertEquals(true, output);
	}
	
	//Var olan aracý tekrar ekleme
	@Test
	void vehicleEntersTest2() {
		AutoPark otopark = new AutoPark(10, 3);
		Time t = new Time(12, 15);
		boolean isOfficial = true;
		String plate = "test";
		
		otopark.vehicleEnters(plate, t, isOfficial);
		boolean output = otopark.vehicleEnters(plate, t, isOfficial);
		assertEquals(false, output);
	}
	
	//Olmayan aracý çýkartma (Aracý çýkartamazsa fonksiyon false döndürür.)
	@Test
	void vehicleExitsTest() {
		AutoPark otopark = new AutoPark(10, 3);
		Time t = new Time(12, 15);
		
		boolean output = otopark.vehicleExits("deneme", t);
		assertEquals(false, output);
	}

	//Ücret ödemesi gerekmeyen aracý çýkarma. (Ücret ödemesi gerekmiyorsa fonksiyon false döndürür.)
	@Test
	void vehicleExitsTest2() {
		AutoPark otopark = new AutoPark(10, 3);
		Time t = new Time(12, 15);
		boolean isOfficial = true;
		String plate = "test";
		
		otopark.vehicleEnters(plate, t, isOfficial);
		boolean output = otopark.vehicleExits(plate, t);
		assertEquals(false, output);
	}
	
	//Ücret ödemesi gereken aracý çýkarma. (Ücret ödemesi gerekiyorsa fonksiyon true döndürür.)
	@Test
	void vehicleExitsTest3() {
		AutoPark otopark = new AutoPark(10, 3);
		Time t = new Time(12, 15);
		boolean isOfficial = false;
		String plate = "test";
		
		otopark.vehicleEnters(plate, t, isOfficial);
		boolean output = otopark.vehicleExits(plate, t);
		assertEquals(true, output);
	}
	
	//Subscriptionlu araçlar listesinde araç arama
	@Test
	void searchVehicleTest() {
		AutoPark otopark = new AutoPark(10, 3);
		String plate = "test";
		Date begin = new Date(15, 05, 2000);
		Date end = new Date(15, 05, 2030);
		Subscription sub = new Subscription(begin, end, plate);
		SubscribedVehicle v = new SubscribedVehicle(plate, sub);
		
		otopark.addVehicle(v);
		SubscribedVehicle output = otopark.searchVehicle(plate);
		assertEquals(v, output);
	}
		
}
