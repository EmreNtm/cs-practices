package myproject;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

class SubscriptionTest {

	@Test
	void isValidTest() {
		String plate = "test";
		Date begin = new Date(15, 05, 2000);
		Date end = new Date(15, 05, 2030);
		Subscription sub = new Subscription(begin, end, plate);
		SubscribedVehicle v = new SubscribedVehicle(plate, sub);
		
		boolean output = v.getSubscription().isValid();
		assertEquals(true, output);
	}

}
