myss	SEGMENT PARA STACK 'yigin'
		DW 50 DUP(?)
myss	ENDS
myds	SEGMENT PARA 'veri'
CR		EQU 13
LF		EQU 10
MSG1    DB CR, LF, 'Dizinin boyutunu giriniz: ', 0
HATA	DB CR, LF, 'Dikkat !!! Sayi vermediniz yeniden giris yapiniz.!!! ', 0
MSG2	DB CR, LF, 'Elemani giriniz: ', 0
MSG3	DB ' ', 0
MSG4	DB CR, LF, 'Lutfen [-128, 127] araliginda deger girin!', 0
MSG5    DB CR, LF, 'Lutfen (0, 100] araliginda bir sayi girin!', 0
arr		DB 100 DUP(?)
n		DW ?
myds	ENDS
mycs	SEGMENT PARA 'kod'
		ASSUME SS:myss, DS:myds, CS:mycs

ANA 	PROC FAR
		PUSH DS
		XOR AX, AX
		PUSH AX
		MOV AX, myds
		MOV DS, AX
		
bas:	MOV AX, OFFSET MSG1 ;("Dizinin boyutunu giriniz: ")
		CALL PUT_STR
		CALL GETN
		CMP AX, 0
		JLE hata1
		CMP AX, 100
		JG hata1 ;Aralık dışı boyut girilmiş mi kontrolleri
		JMP hatayok
hata1:	MOV AX, OFFSET MSG5 ;("Lutfen (0, 100] araliginda bir sayi giriniz")
		CALL PUT_STR
		JMP bas
hatayok:MOV n, AX ;Dizinin boyutunu aldım.
		
		MOV CX, n
		XOR SI, SI ;Döngü indisi
L1:		MOV AX, OFFSET MSG2 ;("Elemani giriniz: ")
		CALL PUT_STR
		CALL GETN
		CMP AX, -128       
		JL hata2
		CMP AX, 127  ;Girilen sayı [-128, 127] aralığında mı kontrolleri.
		JG hata2
		JMP hatasiz
hata2:	MOV AX, OFFSET MSG4  ;("Lutfen [-128, 127] araliginda deger girin!")
		CALL PUT_STR
		JMP L1 ;Sayı istediğimiz aralığın dışında olduğu için en başa dönüp tekrar sayı istedim.
hatasiz:MOV arr[SI], AL ;Sayının verilen aralıkta olduğunu anladıktan sonra sayıyı diziye aldım.
		INC SI
		LOOP L1 ;Elemanları alan döngü.
		
		XOR DX, DX ;QuickSort algoritmasının ihtiyacı olan first değerini DX olarak oluşturdum. İlk first :0
		MOV BX, n ;last değerini BX olarak ayarladım. İlk last : n-1
		DEC BX 
		CALL QSRT ;first ve last değerleri ile qsrt fonksiyonunu çağırdım.
		
		MOV CX, n
		XOR SI, SI ;Döngü indisi
L2:		MOV AX, OFFSET MSG3 ;(" ")
		CALL PUT_STR
		MOV AL, arr[SI]
		CBW  ;PUTN fonksiyonu sayıyı AX üzerinden yazdığı için eğer işareti kaybetmek istemiyorsak CBW yapmalıyız.
		CALL PUTN
		INC SI
		LOOP L2 ;Elemanları yazdım
		RETF
ANA		ENDP

QSRT	PROC NEAR
		CMP DX, BX ;first ve last değeri buluştuğunda/first lastı geçtiğinde rekürsifi sonlandıracak f < l kontrolü
		JGE son 
		CALL ISLEM ;pivotun indisini SI üzerinden döndüren işlem fonksiyonu
		PUSH BX
		MOV BX, SI 
		DEC BX
		CALL QSRT ;QSRT , first = first ve last = pivotindisi-1 parametreleri ile çağırılıyor.
		POP BX
		MOV DX, SI
		INC DX
		CALL QSRT ;QSRT, first = pivotindisi + 1 ve last = last parametreleri ile çağırılıyor.
son:	RET
QSRT	ENDP

ISLEM	PROC NEAR
		MOV SI, DX
		DEC SI      ;DX first değeriydi. SI first-1 oldu.
		MOV CX, BX  ;BX last'ı tutuyordu.
		SUB CX, DX  ;Dönüş sayısı: last - first
		MOV DI, DX  ;DI first değerden başlayan, DI < last olana kadar dönen döngü değişkeni
L3:		MOV AL, arr[BX] ;pivot olarak son elemanı seçtim.
		CMP arr[DI], AL ;swap işleminin yapılıp yapılmayacağını belirleyen kontrol
		JGE atla		
		INC SI			 ;buradaki işlemler ile pivotun geleceği konumun sol tarafına
		MOV AL, arr[DI]	 ;pivottan küçük,sağ tarafına pivottan büyük
		XCHG arr[SI], AL ;sayılar getiriliyor fakat oivot hala en sonda.
		MOV arr[DI], AL 
atla:	INC DI	
		LOOP L3
		INC SI
		MOV AL, arr[DI]  ;pivot, sol tarafında kendisinden küçük,
		XCHG arr[SI], AL ;sağ tarafında kendisinden büyük sayıların
		MOV arr[DI], AL  ;olduğu konuma getiriliyor ve indisi SI'da oluşuyor.
		RET
ISLEM 	ENDP

GETC	PROC NEAR  ;Karakter okuma fonksiyonu
		MOV AH, 1h 
		INT 21H
		RET
GETC 	ENDP

PUTC 	PROC NEAR  ;Karakter yazma fonksiyonu
		PUSH AX
		PUSH DX
		MOV DL, AL
		MOV AH, 2
		INT 21H
		POP DX
		POP AX
		RET
PUTC	ENDP

GETN 	PROC NEAR ;Sayı okuma fonksiyonu
		PUSH BX
		PUSH CX
		PUSH DX
GETN_START:
		MOV DX, 1
		XOR BX, BX
		XOR CX, CX
NEW:
		CALL GETC
		CMP AL, CR
		JE FIN_READ
		CMP AL, '-'
		JNE CTRL_NUM
NEGATIVE:
		MOV DX, -1
		JMP NEW
CTRL_NUM:
		CMP AL, '0'
		JB error
		CMP AL, '9'
		JA error
		SUB AL, '0'
		MOV BL, AL
		MOV AX, 10
		PUSH DX
		MUL CX
		POP DX
		MOV CX, AX
		ADD CX, BX
		JMP NEW
ERROR:
		MOV AX, OFFSET HATA
		CALL PUT_STR
		JMP GETN_START
FIN_READ:
		MOV AX, CX
		CMP DX, 1
		JE FIN_GETN
		NEG AX
FIN_GETN:
		POP DX
		POP CX
		POP DX
		RET
GETN	ENDP

PUTN	PROC NEAR ;Sayı yazma fonksiyonu
		PUSH CX
		PUSH DX
		XOR DX, DX
		PUSH DX
		MOV CX, 10
		CMP AX, 0
		JGE CALC_DIGITS
		NEG AX
		PUSH AX
		MOV AL, '-'
		CALL PUTC
		POP AX
CALC_DIGITS:
		DIV CX
		ADD DX, '0'
		PUSH DX
		XOR DX, DX
		CMP AX, 0
		JNE CALC_DIGITS
DISP_LOOP:
		POP AX
		CMP AX, 0
		JE END_DISP_LOOP
		CALL PUTC
		JMP DISP_LOOP
END_DISP_LOOP:
		POP DX
		POP CX
		RET
PUTN	ENDP
		
PUT_STR PROC NEAR ;String yazma fonksiyonu
		PUSH BX
		MOV BX, AX
		MOV AL, BYTE PTR[BX]
PUT_LOOP:
		CMP AL, 0
		JE PUT_FIN
		CALL PUTC
		INC BX
		MOV AL, BYTE PTR[BX]
		JMP PUT_LOOP
PUT_FIN:
		POP BX
		RET
PUT_STR ENDP
		
mycs	ENDS
		END ANA