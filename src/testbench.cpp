#include <iostream>
#include <string>

using namespace std;

#include "systemc.h"
#include "spi.h"
#include "bus_ahb.h"
#include "test_bus.h"
#include "test_spi.h"
#include "test_jstk.h"

#define TRACE_FILE "system"

void bus_tb( );
void spi_tb( );
void jstk_tb( );

int sc_main ( int argc, char** argv ) {
  //bus_tb( );
  //spi_tb( );
  jstk_tb( );
  return 0;
}

void bus_tb( ) {
  int i;
  
  // Main clock
  sc_clock clk_m ( "MAIN", 10, SC_NS, 0.5, 10, SC_NS, true );

  // AMBA ports
  sc_signal<bool, SC_MANY_WRITERS> hwrite;
  sc_signal<bool, SC_MANY_WRITERS> hsel[ dev_cnt ];
  sc_signal<bool, SC_MANY_WRITERS> hreset[ dev_cnt ];
  sc_signal<sc_uint<32>, SC_MANY_WRITERS> haddr;
  sc_signal<sc_uint<32>, SC_MANY_WRITERS> hwdata;
  sc_signal<sc_uint<32>, SC_MANY_WRITERS> hrdata;

  // Connect interconnect bus
  bus_ahb bus( "BUS_INTER" );
  bus.hclk( clk_m );
  bus.haddr( haddr );
  bus.hwrite( hwrite );
  bus.hwdata( hwdata );
  bus.hrdata( hrdata );
  for( i = 0; i < dev_cnt; i++ ) {
    bus.hsel[i]( hsel[i] );
  }
  for( i = 0; i < dev_cnt; i++ ) {
    bus.hreset[i]( hreset[i] );
  }

  test_bus bus_t( "BUS_TEST" );
  bus_t.clk( clk_m );
  bus_t.haddr( haddr ); 
  bus_t.hwrite( hwrite );
  bus_t.hwdata( hwdata);
  bus_t.hrdata( hrdata);
  
  // Open VCD file
  sc_trace_file *wf = sc_create_vcd_trace_file( TRACE_FILE );

  // Dum main clock
  sc_trace( wf, clk_m, "clk_m" );

  // Dump AMBA signals
  sc_trace( wf, haddr, "haddr" );
  sc_trace( wf, hwrite, "hwrite" );
  sc_trace( wf, hwdata, "hwdata" );
  sc_trace( wf, hrdata, "hrdata" );
  for( i = 0; i < dev_cnt; i++ ) {
    sc_trace( wf, hsel[i], "hsel_" + to_string(i) );
  }
  for( i = 0; i < dev_cnt; i++ ) {
    sc_trace( wf, hreset[i], "hreset_" + to_string(i) );
  }

  sc_start( );

  sc_close_vcd_trace_file( wf );

}

void spi_tb( ) {
  
  // Main clock
  sc_clock clock ( "MAIN", 10, SC_NS, 0.5, 10, SC_NS, true );

  // SPI ports
  sc_signal<bool, SC_MANY_WRITERS> miso;
  sc_signal<bool, SC_MANY_WRITERS> mosi;
  sc_signal<bool> rst;
  sc_signal<bool> start;
  sc_signal<bool> ss;
  sc_signal<bool> sclk;
  sc_signal<bool> busy_m;
  sc_signal<bool> busy_s;

  sc_signal<sc_uint<SPI_BIT_CAP> > data_in_m;
  sc_signal<sc_uint<SPI_BIT_CAP> > data_out_m;

  sc_signal<sc_uint<SPI_BIT_CAP> > data_in_s;
  sc_signal<sc_uint<SPI_BIT_CAP> > data_out_s;

  // Connect the DUT
  test_spi spi_t( "SPI_TEST" );
    spi_t.clock( clock );
    spi_t.miso( miso );
    spi_t.mosi( mosi );
    spi_t.busy_m( busy_m );
    spi_t.busy_s( busy_s );
    spi_t.rst( rst );
    spi_t.start( start );
    spi_t.ss( ss );
    spi_t.sclk( sclk );

    spi_t.data_in_m( data_in_m );
    spi_t.data_in_s( data_in_s );

    spi_t.data_out_m( data_out_m );
    spi_t.data_out_s( data_out_s );

  // Open VCD file
  sc_trace_file *wf = sc_create_vcd_trace_file( TRACE_FILE );

  // Dump the desired signals
  sc_trace( wf, clock, "clock" );

  sc_trace( wf, miso, "miso" );
  sc_trace( wf, mosi, "mosi" );
  sc_trace( wf, rst, "rst" );
  sc_trace( wf, start, "start" );
  sc_trace( wf, ss, "ss" );
  sc_trace( wf, sclk, "sclk" );
  sc_trace( wf, busy_m, "busy_m" );
  sc_trace( wf, busy_s, "busy_s" );

  sc_trace( wf, data_in_m, "data_in_m" );
  sc_trace( wf, data_out_m, "data_out_m" );

  sc_trace( wf, data_in_s, "data_in_s" );
  sc_trace( wf, data_out_s, "data_out_s" );

  sc_start( );
}

void jstk_tb( ) {
  
  // Main clock
  sc_clock clock ( "MAIN", 10, SC_NS, 0.5, 10, SC_NS, true );

  // SPI ports
  sc_signal<bool, SC_MANY_WRITERS> miso;
  sc_signal<bool, SC_MANY_WRITERS> mosi;
  sc_signal<bool> rst;
  sc_signal<bool> start;
  sc_signal<bool> ss;
  sc_signal<bool> sclk;
  sc_signal<bool> busy_m;
  sc_signal<bool> busy_s;

  sc_signal<sc_uint<SPI_BIT_CAP> > data_in_m;
  sc_signal<sc_uint<SPI_BIT_CAP> > data_out_m;

  sc_signal<sc_uint<SPI_BIT_CAP> > data_in_s;
  sc_signal<sc_uint<SPI_BIT_CAP> > data_out_s;

  // Connect the DUT
  test_jstk jstk_t( "SPI_TEST" );
    jstk_t.clock( clock );
    jstk_t.miso( miso );
    jstk_t.mosi( mosi );
    jstk_t.busy_m( busy_m );
    jstk_t.busy_s( busy_s );
    jstk_t.rst( rst );
    jstk_t.start( start );
    jstk_t.ss( ss );
    jstk_t.sclk( sclk );

    jstk_t.data_in_m( data_in_m );
    jstk_t.data_in_s( data_in_s );

    jstk_t.data_out_m( data_out_m );
    jstk_t.data_out_s( data_out_s );

  // Open VCD file
  sc_trace_file *wf = sc_create_vcd_trace_file( TRACE_FILE );

  // Dump the desired signals
  sc_trace( wf, clock, "clock" );

  sc_trace( wf, miso, "miso" );
  sc_trace( wf, mosi, "mosi" );
  sc_trace( wf, rst, "rst" );
  sc_trace( wf, start, "start" );
  sc_trace( wf, ss, "ss" );
  sc_trace( wf, sclk, "sclk" );
  sc_trace( wf, busy_m, "busy_m" );
  sc_trace( wf, busy_s, "busy_s" );

  sc_trace( wf, data_in_m, "data_in_m" );
  sc_trace( wf, data_out_m, "data_out_m" );

  sc_trace( wf, data_in_s, "data_in_s" );
  sc_trace( wf, data_out_s, "data_out_s" );

  sc_start( );
}

