package myproject;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

class DateTest {

	@Test
	void isAfterThanTest() {
		Date begin = new Date(15, 05, 2000);
		Date end = new Date(15, 05, 2030);
		
		boolean output = begin.isAfterThan(end);
		assertEquals(false, output);
	}
	
	@Test
	void isBeforeThanTest() {
		Date begin = new Date(15, 05, 2000);
		Date end = new Date(15, 05, 2030);
		
		boolean output = begin.isBeforeThan(end);
		assertEquals(true, output);
	}
	
	@Test
	void isEqualsWithTest() {
		Date begin = new Date(15, 05, 2000);
		Date end = new Date(15, 05, 2030);
		
		boolean output = begin.isEqualsWith(end);
		assertEquals(false, output);
	}
	
	@Test
	void getTodayTest() {
		Date d = new Date(15, 05, 2000);
		Date today = Date.getToday();
		
		boolean output = (d.getDay() == today.getDay()) && (d.getMonth() == today.getMonth()) && (d.getYear() == today.getYear());
		assertEquals(false, output);
	}

}
