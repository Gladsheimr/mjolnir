module blinky (output wire led_blue,
               output wire led_green,
               output wire led_red,
               output wire gpio_2);

   wire clk;
   SB_HFOSC inthosc(.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(clk));

   localparam N = 27;
   reg [N:0] counter;

   always @(posedge clk)
     counter <= counter + 1;

   assign led_blue = counter[N];
   assign led_green = counter[N-1];
   assign led_red = counter[N-2];
   assign gpio_2 = counter[N];
endmodule