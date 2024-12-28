package myproject;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

class TimeTest {

	//Fark dakika cinsinden.
	@Test
	void getDifferenceTest() {
		Time t1 = new Time(12, 30);
		Time t2 = new Time(11, 33);
		
		int output = t1.getDifference(t2);
		
		assertEquals(57, output);
	}

}
