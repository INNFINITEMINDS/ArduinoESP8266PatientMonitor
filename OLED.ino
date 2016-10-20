#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(5, 2, 3, 4);// SW SPI Com: SCK = 5, MOSI = 11, CS = 10, A0 = 9

void init_OLED() {
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void print_med(int column , int row , char* text1 , char* text2 , char* text3 , char* title1 , char* title2 , char* title3) {
  // picture loop
  u8g.firstPage();  
  do {
      // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, row, title1);
  u8g.drawStr( 0, row+17 ,title2);
  u8g.drawStr( 0, row+17*2,title3);
  u8g.drawStr( column, row, text1);
  u8g.drawStr( column, row+17 , text2);
  u8g.drawStr( column, row+17*2, text3);
    
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
}

void print_text(int column , int row , char* text1 , char* text2 , char* text3) {
  // picture loop
  u8g.firstPage();  
  do {
      // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( column, row, text1);
  u8g.drawStr( column, row+17 , text2);
  u8g.drawStr( column, row+17*2, text3);
    
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
}


