package myproject;

public class Time {
	
	private int hour;
	private int minute;
	
	public Time(int hour, int minute) {
		setHour(hour);
		setMinute(minute);
	}

	//Dakika cinsinden sonuç döndürür.
	public int getDifference(Time t) {
		if(t.getHour() > this.hour) {
			if(t.getMinute() >= this.minute) {
				return (t.getHour() - this.getHour()) * 60 + (t.getMinute() - this.getMinute());
			} else {
				return (t.getHour() - this.getHour() - 1) * 60 + (60 + t.getMinute() - this.getMinute());
			}
		} else if (t.getHour() < this.hour){
			if(this.minute >= t.getMinute()) {
				return (this.getHour() - t.getHour()) * 60 + (this.getMinute() - t.getMinute());
			} else {
				return (this.getHour() - t.getHour() - 1) * 60 + (60 + this.getMinute() - t.getMinute());
			}
		} else {
			if(t.getMinute() >= this.minute) {
				return t.getMinute() - this.getMinute();
			} else {
				return this.getMinute() - t.getMinute();
			}
		}
	}
	
	public int getHour() {
		return hour;
	}

	public void setHour(int hour) {
		if (hour >= 0 && hour <= 23)
			this.hour = hour;
		else 
			System.out.println("Saat girilemedi. Lutfen 0-23 arasi bir saat giriniz.");
	}

	public int getMinute() {
		return minute;
	}

	public void setMinute(int minute) {
		if (minute >= 0 && minute <= 59)
			this.minute = minute;
		else 
			System.out.println("Dakika girilemedi. Lutfen 0-59 arasi bir dakika giriniz.");
	}
	
}
