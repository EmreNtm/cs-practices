package myproject;

import java.util.Scanner;

public class Ana {

	//Emre Nitim 17011079
	
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		System.out.print("Otopark olusturuluyor.\nSaatlik otopark ucretini giriniz: ");
		double fee = in.nextDouble();
		System.out.print("Otopark kapasitesini giriniz: ");
		int cap = in.nextInt();
		
		AutoPark otopark = new AutoPark(fee, cap);
		
		//0 girilene kadar programýn bitmemesini saðlar.
		while(true) {
			System.out.print("\nIslem seciniz:\n_____________________________________\n"
					+ "1.Yeni abone arac ekle\n"
					+ "2.Otoparka arac ekle\n"
					+ "3.Otoparktan arac cikar\n"
					+ "4.Arac otoparkta mi\n"
					+ "5.Arac abone mi\n"
					+ "6.Otopark bilgisi goruntule\n"
					+ "0.Cikis");
			int secim;
			do {
				System.out.print("\n\nSeciminiz: ");
				secim = in.nextInt();
			} while (secim < 0 || secim > 6);
			
			if(secim != 0) {
				switch(secim) {
				case 1:
					System.out.print("\nAracin plakasini girin: ");
					in.nextLine();
					String plate = in.nextLine();
					System.out.print("Aboneligin baslangic yilini girin: ");
					int year = in.nextInt();
					int month;
					do {
						System.out.print("Aboneligin baslangic ayini girin: ");
						month = in.nextInt();
					} while (month < 0 || month > 12);
					int day;
					do {
						System.out.print("Aboneligin baslangic gununu girin: ");
						day = in.nextInt();
					} while (day < 0 || day > 31);
					System.out.print("Aboneligin bitis yilini girin: ");
					int fyear = in.nextInt();
					int fmonth;
					do {
						System.out.print("Aboneligin bitis ayini girin: ");
						fmonth = in.nextInt();
					} while (fmonth < 0 || fmonth > 12);
					int fday;
					do {
						System.out.print("Aboneligin bitis gununu girin: ");
						fday = in.nextInt();
					} while (fday < 0 || fday > 31);
					
					Date s = new Date(day, month, year);
					Date f = new Date(fday, fmonth, fyear);
					Subscription sub = new Subscription(s, f, plate);
					SubscribedVehicle v = new SubscribedVehicle(plate, sub);
					
					if(otopark.addVehicle(v))
						System.out.print("\nArac sisteme eklendi!\n");
					break;
				case 2:
					System.out.print("\nAracin plakasini girin: ");
					in.nextLine();
					String plate1 = in.nextLine();
					int hour;
					do {
						System.out.print("Aracin giris saatini girin: ");
						hour = in.nextInt();
					} while (hour < 0 || hour >= 24);
					int min;
					do {
						System.out.print("Aracin giris dakikasini girin: ");
						min = in.nextInt();
					} while (min < 0 || min >= 60);
					int o;
					do {
						System.out.print("Arac official mi (1->evet, 0->hayir): ");
						o = in.nextInt();
					} while (o != 1 && o != 0);
					boolean isOfficial;
					if(o == 1)
						isOfficial = true;
					else
						isOfficial = false;
					
					Time t = new Time(hour, min);
					
					if(otopark.vehicleEnters(plate1, t, isOfficial))
						System.out.print("\nArac otoparka eklendi!\n");
					break;
				case 3:
					System.out.print("\nAracin plakasini girin: ");
					in.nextLine();
					String plate2 = in.nextLine();
					int hour1;
					do {
						System.out.print("Aracin cikis saatini girin: ");
						hour1 = in.nextInt();
					} while (hour1 < 0 || hour1 >= 24);
					int min1;
					do {
						System.out.print("Aracin cikis dakikasini girin: ");
						min1 = in.nextInt();
					} while (min1 < 0 || min1 >= 60);
					
					Time t1 = new Time(hour1, min1);
					
					otopark.vehicleExits(plate2, t1);
					System.out.print("Arac artik otoparkda degil!\n");
					break;
				case 4:
					System.out.print("\nAracin plakasini girin: ");
					in.nextLine();
					String plate3 = in.nextLine();
					
					if(otopark.isParked(plate3))
						System.out.print("\nArac otoparkta!\n");
					else
						System.out.print("\nArac otoparkta degil!\n");
					break;
				case 5:
					System.out.print("\nAracin plakasini girin: ");
					in.nextLine();
					String plate4 = in.nextLine();
					
					if(otopark.searchVehicle(plate4) != null && otopark.searchVehicle(plate4).getSubscription().isValid())
						System.out.print("\nArac abone!\n");
					else if(otopark.searchVehicle(plate4) != null)
						System.out.print("\nAracin aboneligi bitmis!\n");
					else
						System.out.print("\nArac abone degil!\n");
					break;
				case 6:
					System.out.println(otopark.toString());
					break;
				}
			} else {
				break;
			}
		}
		in.close();
	}

}
