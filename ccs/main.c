#include <main.h>
#FUSES HS
#USE DELAY(CLOCK = 20M)
#BIT TMR1IF = 0X0C.0

int8 c_g, dv_g, c_p, dv_p, c_h, dv_h, thu, c_d, dv_d, c_t, dv_t, c_n, dv_n; // khai bao ca bien chua gia tri thap phan cua cac don vi thoi gian
int8 mang_tg_rtc[7];                                                        // mang chua giu lieu thoi gian cua rtc
int8 h1 = 0, p1 = 0, g1 = 0;                                                // khai bao cac bien chua thoi gian bao thuc
int8 gio = 0, phut = 0, giay = 0, BDT = 0;                                  // khai bao cac bien chua thoi gian bao thuc
int8 dang_bao_thuc = 0;
unsigned int8 dh_nhiet_do, dh_nhiet_do2, dh_do_am, dh_do_am2;
unsigned int8 str[20];

void convert_bcd();        // chuen doi so 2 bcd thanh 2 so thap phan rieng le bao gom chuc va don vi rieng cua cac gia tri thoi gian
void ReadRTC(int8 *buff);  // doc cac gia tri thoi gian tu RTC vao mang
void WriteRTC(int8 *buff); // ghi mang chua gia tri thoi gian vao RTC
void rtc_init(void);
void chinh_gio(); // khai bao ham chinh thoi gian
void chinh_bt();  // khai bao ham chinh thoi gian bao thuc

//========================================
void hienthi1(int8 m) // hien thi chinh cai dat thoi gian vao 1307
{
   LCD_putcmd(0x80); // lenh dua con tro den hang 1 cot 1 LCD
   printf(LCD_putchar, "TIME: ");
   if (m == 1)
   {
      printf(LCD_putchar, "__"); // hien thi dau __ thay v? gia tri de bien minh dang chon gia tri can chinh nao
   }
   else
   {
      LCD_putchar(c_h + 0x30);  // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(dv_h + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar, ":");
   if (m == 2)
   {
      printf(LCD_putchar, "__");
   }
   else
   {
      LCD_putchar(c_p + 0x30);  // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(dv_p + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar, ":");
   if (m == 3)
   {
      printf(LCD_putchar, "__");
   }
   else
   {
      LCD_putchar(c_g + 0x30);  // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(dv_g + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar, "    ");
   LCD_putcmd(0xC0); // lenh dua con tro den hang 2 cot 1 LCD
   printf(LCD_putchar, "DATE: ");
   if (m == 4)
   {
      printf(LCD_putchar, "__");
   }
   else
   {
      LCD_putchar(c_d + 0x30);  // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(dv_d + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar, "/");
   if (m == 5)
   {
      printf(LCD_putchar, "__");
   }
   else
   {
      LCD_putchar(c_t + 0x30);  // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(dv_t + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar, "/");
   printf(LCD_putchar, "20");
   if (m == 6)
   {
      printf(LCD_putchar, "__");
   }
   else
   {
      LCD_putchar(c_n + 0x30);  // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(dv_n + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar, "    ");
}

//========================================
void hienthi2(int8 m) // hien thi cai dat bao thuc
{
   LCD_putcmd(0xc0); // lenh dua con tro den hang 2 cot 1 LCD
   printf(LCD_putchar, "    ");
   if (m == 1) // ne khi
   {
      printf(LCD_putchar, "__");
   }
   else
   {
      LCD_putchar(h1 / 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(h1 % 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar, ":");
   if (m == 2)
   {
      printf(LCD_putchar, "__");
   }
   else
   {
      LCD_putchar(p1 / 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(p1 % 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar, ":");
   if (m == 3)
   {
      printf(LCD_putchar, "__");
   }
   else
   {
      LCD_putchar(g1 / 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(g1 % 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar, "    ");

   // eep
   write_eeprom(0x01, h1);
   write_eeprom(0x02, p1);
   write_eeprom(0x03, g1);
}

//========================================
void hienthinhietdo()
{

   if (DHT_GetTemHumi(dh_nhiet_do, dh_nhiet_do2, dh_do_am, dh_do_am2))
   {
      sprintf(str, "HUMIDITY: %u,%u      ", dh_do_am, dh_do_am2);
      LCD_putcmd(0x94);
      printf(LCD_putchar, str);

      sprintf(str, "TEMPERATURE: %u,%u", dh_nhiet_do, dh_nhiet_do2);
      LCD_putcmd(0xD4);
      printf(LCD_putchar, str);
   }
   delay_ms(500);
}

//========================================
void main()
{
   output_high(led);
   int8 nho_giay; // nho giay hien tai

   // timer1
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
   set_timer1(3036);

   // PWM
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_16, 255, 1); // nhanh hon
   set_timer2(0);
   set_pwm1_duty(0);

   lcd_init(); // khoi tao LCD
   rtc_init();
   LCD_putcmd(0x80);                            // lenh dua con tro den dong 1 cot 1
   printf(LCD_putchar, " CLOCK LCD ");          // xuat hien thi chuoi len LCD
   LCD_putcmd(0xC0);                            // lenh dua co tro  den dong 2 cot 1
   printf(LCD_putchar, " SON + TRINH + VIET "); // xuat hien thi chuoi len LCD
   LCD_putcmd(0x94);
   DELAY_MS(1000);
   printf(LCD_putchar, " READING DHT11... ");
   DELAY_MS(3000); // tao thoi gian tre

   //========================================
   while (true)
   {
      // timer1
      gio = 0;
      phut = 0;
      giay = 0;
      if (input(k6) == 0)
      {
         BDT = 0;
         set_timer1(3036);
         LCD_putcmd(0x80);
         printf(LCD_putchar, " TIMER:             ");
         LCD_putcmd(0xC0);
         printf(LCD_putchar, "    ");
         LCD_putchar(gio / 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
         LCD_putchar(gio % 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
         printf(LCD_putchar, ":");
         LCD_putchar(phut / 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
         LCD_putchar(phut % 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
         printf(LCD_putchar, ":");
         LCD_putchar(giay / 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
         LCD_putchar(giay % 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
         printf(LCD_putchar, "        ");
         LCD_putcmd(0x94);
         printf(LCD_putchar, "                    ");
         LCD_putcmd(0xD4);
         printf(LCD_putchar, "                    ");
         while (input(k6) == 0)
         {
            {
               if (TMR1IF == 1)
               {
                  TMR1IF = 0;
                  BDT++;
                  set_timer1(3036);
                  LCD_putcmd(0xC0);
                  printf(LCD_putchar, "    ");
                  LCD_putchar(gio / 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
                  LCD_putchar(gio % 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
                  printf(LCD_putchar, ":");
                  LCD_putchar(phut / 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
                  LCD_putchar(phut % 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
                  printf(LCD_putchar, ":");
                  LCD_putchar(giay / 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
                  LCD_putchar(giay % 10 + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
                  printf(LCD_putchar, "        ");
                  if (BDT == 10)
                  {
                     giay++;
                     if (giay == 60)
                     {
                        giay = 0;
                        phut++;
                        if (phut = 60)
                        {
                           phut = 0;
                           gio++;
                        }
                     }
                     BDT = 0;
                  }
               }
            }
         }
      }
      // end

      h1 = read_eeprom(0x01);
      p1 = read_eeprom(0x02);
      g1 = read_eeprom(0x03);

      ReadRTC(&mang_tg_rtc[0]); // do thoi gian tu RTC
      convert_bcd();            // chuyen doi thoi gian trong RTC sang so thap phan
      hienthi1(0);              // xuat hien thi len LCD
      hienthinhietdo();
      if (nho_giay != dv_g) // neu giay nho v? don vi giay moi doc khac nhau. tuc giay da tang len
      {
         nho_giay = dv_g; // nho lai giay da thay doi

         // PWM
         if ((c_h * 10 + dv_h) < 22 || (c_h * 10 + dv_h) > 5) // dat den thoi gian bao thuc dat
         {
            set_pwm1_duty((int16)500);
         }
         if (h1 == (c_h * 10 + dv_h) && p1 == (c_p * 10 + dv_p) && g1 == (c_g * 10 + dv_g)) // dat den thoi gian bao thuc dat
         {
            output_high(coi);  // bat coi khi thoi gian thuc = thoi gian bao thuc dat
            dang_bao_thuc = 1; // bao da bat coi bao thuc
         }
         if ((c_h * 10 + dv_h) >= 22 || (c_h * 10 + dv_h) <= 5) // dat den thoi gian bao thuc dat
         {
            set_pwm1_duty((int16)1000);
         }
      }
      if (input(k2) == 0) // khi nhan phim chinh thoi gian
      {
         while (input(k2) == 0)
            ;         // cho cho toi khi nut nhan duoc nha
         chinh_gio(); // thuc hien ham chinh thoi gian
      }
      if (input(k5) == 0) // khi nhan phim chinh bao thuc
      {
         while (input(k5) == 0)
            ; // cho cho toi khi nut nhan duoc nha
         if (dang_bao_thuc == 1)
         {
            output_low(coi);   // tat coi neu dang bao
            dang_bao_thuc = 0; // bao da tat coi bao thuc
         }
         else
            chinh_bt(); // thuc hien ham chinh bao thuc
      }
   }
}

//========================================
void convert_bcd() // BCD sang so thap phan
{
   c_g = mang_tg_rtc[0] >> 4;    // dich phai 4 bit de lay hang chuc
   dv_g = mang_tg_rtc[0] & 15;   // xoa 4 vit cao de lay hang don vi
   c_p = mang_tg_rtc[1] >> 4;    // dich phai 4 bit de lay hang chuc
   dv_p = mang_tg_rtc[1] & 0x0f; // xoa 4 vit cao de lay hang don vi
   c_h = mang_tg_rtc[2] >> 4;    // dich phai 4 bit de lay hang chuc
   dv_h = mang_tg_rtc[2] & 0x0f; // xoa 4 vit cao de lay hang don vi
   thu = mang_tg_rtc[3];
   c_d = mang_tg_rtc[4] >> 4;    // dich phai 4 bit de lay hang chuc
   dv_d = mang_tg_rtc[4] & 0x0f; // xoa 4 vit cao de lay hang don vi
   c_t = mang_tg_rtc[5] >> 4;    // dich phai 4 bit de lay hang chuc
   dv_t = mang_tg_rtc[5] & 0x0f; // xoa 4 vit cao de lay hang don vi
   c_n = mang_tg_rtc[6] >> 4;    // dich phai 4 bit de lay hang chuc
   dv_n = mang_tg_rtc[6] & 0x0f; // xoa 4 vit cao de lay hang don vi
}

//========================================
void chinh_bt() // chuong trinh con chinh bao thuc
{
   int8 mode = 1, i = 0;
   LCD_putcmd(0x80);                   // dua con tro hien thi len dong 1 cot 1
   printf(LCD_putchar, " SET CLOCK "); // hien thi thong tin len dong
   while (true)
   {
      if (i < 5)         // khi bien lap i tu 0-4 thi hien thi gia tri can chinh
         hienthi2(0);    // hien thi binh thuong thoi gian bao thuc
      else               // khi bien lap i trong lan quet 5-9 thi hien thi dau '?' cho bien minh dang chon gia tri chinh nao
         hienthi2(mode); // hien thi dau '?' tai gia tri duoc chon
      i++;
      if (i >= 10)
         i = 0; // tang bien i sau moi lan quet (gioi han 0-9)

      // Tang gia tri
      if (input(k3) == 0) // khi nhan nut tang
      {
         while (input(k3) == 0)
            ; // cho nha nut tang
         i = 0;
         switch (mode)
         {
         case 1:
            if (h1 < 23)
               h1++;
            else
               h1 = 0;
            break;
         case 2:
            if (p1 < 59)
               p1++;
            else
               p1 = 0;
            break;
         case 3:
            if (g1 < 59)
               g1++;
            else
               g1 = 0;
            break;
         }
      }
      // Giam gia tri
      if (input(k4) == 0) // nhan nut giam gia tri
      {
         while (input(k4) == 0)
            ; // cho nha nut giam gia tri
         i = 0;
         switch (mode)
         {
         case 1:
            if (h1 > 0)
               h1--;
            else
               h1 = 23; // gio lon hon 0 thi giam 1 don vi con be hon 0 thi nhay ve 23
            break;
         case 2:
            if (p1 > 0)
               p1--;
            else
               p1 = 59; // phut lon hon 0 thi giam 1 don vi con be hon 0 thi nhay ve 59
            break;
         case 3:
            if (g1 > 0)
               g1--;
            else
               g1 = 59; // giay lon hon 0 thi giam 1 don vi con be hon 0 thi nhay ve 59
            break;
         }
      }

      if (input(k5) == 0) // khi nhan nut chinh bao thuc
      {
         while (input(k5) == 0)
            ;
         i = 5;
         mode++; // tang bien chinh thoi gian bao thuc
         if (mode > 3)
            break; // thoat khoi ham chinh bao thuc sau khi chinh xong  3 gia tri bao thuc
      }
      if (input(k2) == 0) // nhan nut chinh thoi gian
      {
         while (input(k2) == 0)
            ;   // cho nut chinh thoi gian nha
         break; // thoat khoi ham khi nhan nut chinh thoi gian
      }
   }
}

//========================================
void chinh_gio() // ham chinh thoi gian thuc
{
   int8 mode = 1, i = 0, t;
   ReadRTC(&mang_tg_rtc[0]);
   convert_bcd(); // chuyen doi thoi gian trong rtc ra so thap phan
   while (true)   // vong lap vo han. chi thoat neu co lenh break cua ham
   {
      if (i < 5)         // khi bien lap i tu 0-4 thi hien thi gia tri can chinh
         hienthi1(0);    // hien thi nhu binh thuong cac gia tri thoi gian
      else               // khi bien lap i trong lan quet 5-9 thi hien thi dau '?' cho bien minh dang chon gia tri chinh nao
         hienthi1(mode); // hien thi dau '?' tai gia tri duoc chon
      i++;
      if (i >= 10)
         i = 0;           // tang bien i sau moi lan quet (gioi han 0-9)
      if (input(k3) == 0) // nut tang duoc nhan
      {
         while (input(k3) == 0)
            ;          // cho nut tang duoc nha
         i = 0;        // cho i = 0 de tiep tuc hien thi gia tri duoc chon
         switch (mode) // lua chon tang gia tri tuy thuoc vao bien mode
         {
         case 1:                 // mode = 1 // tang gio gio
            t = c_h * 10 + dv_h; // ghep 2 so thap phan rieng le vao mot bien tam
            if (t < 23)          // neu gio <23 thi tang gio
               t++;
            else
               t = 0; // neu gio >23 thi nap lai gio =0

            c_h = t / 10;        // tach roi hang chuc cua bien tam gio
            dv_h = t % 10;       // tach toi hang don vi cua bien tam gio
            break;               // thoat khoi ham chon
         case 2:                 // mode = 2 // tang phut
            t = c_p * 10 + dv_p; // tuong tu nhu voi gio
            if (t < 59)
               t++;
            else
               t = 0;
            c_p = t / 10;
            dv_p = t % 10;
            break;
         case 3:
            t = c_g * 10 + dv_g;
            if (t < 59)
               t++;
            else
               t = 0;
            c_g = t / 10;
            dv_g = t % 10;
            break;
         case 4:
            t = c_d * 10 + dv_d;
            if (t < 31)
               t++;
            else
               t = 1;
            c_d = t / 10;
            dv_d = t % 10;
            break;
         case 5:
            t = c_t * 10 + dv_t;
            if (t < 12)
               t++;
            else
               t = 1;
            c_t = t / 10;
            dv_t = t % 10;
            break;
         case 6:
            t = c_n * 10 + dv_n;
            if (t < 99)
               t++;
            else
               t = 0;
            c_n = t / 10;
            dv_n = t % 10;
            break;
         }
      }
      
      if (input(k4) == 0) // khi nhan nut giam
      {
         while (input(k4) == 0)
            ; // cho nha nut giam
         i = 0;
         switch (mode)
         {
         case 1:
            t = c_h * 10 + dv_h;
            if (t > 0)
               t--;
            else
               t = 23;
            c_h = t / 10;
            dv_h = t % 10; // chuc gio + dvi gio nho hon 0 thi nhay ve 23
            break;
         case 2:
            t = c_p * 10 + dv_p;
            if (t > 0)
               t--;
            else
               t = 59;
            c_p = t / 10;
            dv_p = t % 10; // chuc phuc + dvi phut gio nho hon 0 thi nhay ve 59
            break;
         case 3:
            t = c_g * 10 + dv_g;
            if (t > 0)
               t--;
            else
               t = 59;
            c_g = t / 10;
            dv_g = t % 10; // chuc giay + dvi giay gio nho hon 0 thi nhay ve 59
            break;
         case 4:
            t = c_d * 10 + dv_d;
            if (t > 1)
               t--;
            else
               t = 31;
            c_d = t / 10;
            dv_d = t % 10; // chuc ngay + dvi ngay nho hon 1 thi nhay ve 31
            break;
         case 5:
            t =
                c_t * 10 + dv_t;
            if (t > 1)
               t--;
            else
               t = 12;
            c_t = t / 10;
            dv_t = t % 10; // chuc thang + dvi thango nho hon 1 thi nhay ve 12
            break;
         case 6:
            t = c_n * 10 + dv_n;
            if (t > 0)
               t--;
            else
               t = 99;
            c_n = t / 10;
            dv_n = t % 10; // chuc nam + dvi nam nho hon 0 thi nhay ve 99
            break;
         }
      }

      if (input(k2) == 0) // khi nhan nut set
      {
         while (input(k2) == 0)
            ; // cho nha nut set
         i = 5;
         mode++; // tang bien chon gia tri chinh
         if (mode > 6)
            break; // khi nhan chinh het 6 gia tri thi thoat khoi ham chinh thoi gian
      }
      if (input(k5) == 0) // khi nhan nut chinh bao thuc thi thoat khoi ham chinh thoi gian
      {
         while (input(k5) == 0)
            ;   // cho nha nut chinh bao thuc
         break; // thoat khoi ham chinh thoi gian
      }
   }

   //========================================
   mang_tg_rtc[0] = (c_g << 4) | dv_g; // chuyen 2 so thap phan cua giay thanh BCD luu vao mang thoi gian
   mang_tg_rtc[1] = (c_p << 4) | dv_p; // chuyen 2 so thap phan cua phut thanh BCD luu vao mang thoi gian
   mang_tg_rtc[2] = (c_h << 4) | dv_h; // chuyen 2 so thap phan cua gio thanh BCD luu vao mang thoi gian
   mang_tg_rtc[3] = thu;
   mang_tg_rtc[4] = (c_d << 4) | dv_d; // chuyen 2 so thap phan cua ngay thanh BCD luu vao mang thoi gian
   mang_tg_rtc[5] = (c_t << 4) | dv_t; // chuyen 2 so thap phan cua thang thanh BCD luu vao mang thoi gian
   mang_tg_rtc[6] = (c_n << 4) | dv_n; // chuyen 2 so thap phan cua nam thanh BCD luu vao mang thoi gian
   WriteRTC(&mang_tg_rtc[0]);          // ghi mang thoi gian vao DS1307
}

//========================================
void ReadRTC(int8 *buff) // doc du lieu tu 1307
{
   i2c_Start();               // Lenh bat dau i2c
   i2c_Write(0xD0);           // ghi ma ghi vao DS1307
   i2c_Write(0x00);           // Ghi dia chi 0 vao DS
   i2c_Start();               // Lenh restart i2c
   i2c_Write(0xD1);           // chi ma doc vao DS
   *(buff + 0) = i2c_Read(1); // Doc giay voi bit thua nhan (ACK) de dia chi tang tu dong
   *(buff + 1) = i2c_Read(1); // doc phut
   *(buff + 2) = i2c_Read(1); // doc gio
   *(buff + 3) = i2c_Read(1); // doc thu
   *(buff + 4) = i2c_Read(1); // doc ngay
   *(buff + 5) = i2c_Read(1); // doc thang
   *(buff + 6) = i2c_Read(0); // doc nam khong bit thua nhan de ket thuc viec doc
   i2c_Stop();                // Lenh dung i2c
}

//========================================
void WriteRTC(int8 *buff) // ghi du lieu vao 1307
{
   i2c_Start();            // lenh bat dau i2c
   i2c_Write(0xD0);        // ghi ma ghi vao DS
   i2c_Write(0x00);        // nap dia chi ban dau vao DS
   i2c_Write(*(buff + 0)); // ghi giay
   i2c_Write(*(buff + 1)); // ghi phut
   i2c_Write(*(buff + 2)); // ghi gio
   i2c_Write(*(buff + 3)); // ghi thu
   i2c_Write(*(buff + 4)); // ghi ngay
   i2c_Write(*(buff + 5)); // ghi thang
   i2c_Write(*(buff + 6)); // ghi nam
   i2c_Stop();             // lenh dung i2c
}

//========================================
void rtc_init(void) // khoi tao cho ds1307
{
   int8 a; // byte luu tam gia tri o nho 0 cua ds
   i2c_start();
   i2c_write(0xD0); // ghi vao ds1307 chieu ghi
   i2c_write(0);    // ghi dia chi can doc
   i2c_start();     // start bit i2c
   i2c_write(0xD1); // ghi vao ds1307 chieu doc
   a = i2c_read(0);
   i2c_stop();
   if ((a & 0x80) != 0)
   {
      a &= 0x7f;
      i2c_start();
      i2c_write(0); // ghi vao ds1307 chieu ghi
      i2c_write(0); // ghi dia chi can doc
      i2c_write(a); //
      i2c_stop();
   }
}
