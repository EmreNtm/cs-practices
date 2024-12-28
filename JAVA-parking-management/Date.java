package myproject;

import java.util.Calendar;

public class Date {
	
	private int day;
	private int month;
	private int year;
	
	public Date(int day, int month, int year) {
		this.day = day;
		this.month = month;
		this.year = year;
	}

	public boolean isAfterThan(Date d) {
		if(this.year > d.getYear())
			return true;
		else if(this.year < d.getYear())
			return false;
		else
			if(this.month > d.getMonth())
				return true;
			else if(this.month < d.getMonth())
				return false;
			else
				if(this.day > d.getDay())
					return true;
				else
					return false;
	}
	
	public boolean isBeforeThan(Date d) {
		return (!isAfterThan(d) && !isEqualsWith(d));
	}
	
	public boolean isEqualsWith(Date d) {
		if(this.year == d.getYear())
			if(this.month == d.getMonth())
				if(this.day == d.getDay())
					return true;
		return false;
	}
	
	public static Date getToday() {
		Calendar cal = Calendar.getInstance();
		Date today = new Date(cal.get(Calendar.DAY_OF_MONTH), cal.get(Calendar.MONTH) + 1, cal.get(Calendar.YEAR));
		return today;
	}
	
	public void setDay(int day) {
		if(day > 0 && day < 31)
			this.day = day;
		else 
			System.out.println("Gun: " + day + " girilmesi mümkün deðildir!");
	}

	public void setMonth(int month) {
		if(month > 0 && month < 13)
			this.month = month;
		else 
			System.out.println("Ay: " + month + " girilmesi mümkün deðildir!");
	}

	public void setYear(int year) {
		this.year = year;
	}

	public int getDay() {
		return day;
	}

	public int getMonth() {
		return month;
	}

	public int getYear() {
		return year;
	}
	
}
