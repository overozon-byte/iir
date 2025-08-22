///////////////////////////////////////////////////////////////////////////////
//
// Company:			Polytech Students Inovations
// Engineer:        Zelenin Aleksey Aleksandrovich
//
// Create Date:     
// Design Name:
// Module Name:     filter
// Project Name:
// Target Devices:
// Tool versions:
// Description:     lowpass filter
//
////////////////////////////////////////////////////////////////////////////////


`timescale 1 ns/1 ns
`default_nettype none

module TB_iir;

	//Inputs
	reg nrst = 1'b0; // NRESET
	reg clk = 1'b1; // CLK
	reg signed [ 15 :  0 ] id = 0; // ID
	//reg signed [ 15 :  0 ] id_f;
	wire signed  [ 15 :  0 ] od; // OD
	reg i_dv = 0; // I_DV
	wire o_dv; //O_DV

integer fp_r, fp_w, size, i, j, pos_r, pos_w, r;

initial begin // CLK
	forever
	//50 MHz
		#10 
		clk <= ~clk;
end

initial begin // NRESET
	forever
	// >= 200 ns
	#1790
	nrst = 1'b1;
end

initial begin 
	
	//Open for read and write
	#5
	fp_r = $fopen("E:\\digital_fitler_lp\\audio\\wn.wav", "r+b");
	fp_w = $fopen("E:\\digital_fitler_lp\\audio\\mono_verilog.wav", "w+b");
	
	// Size	
	r = $fseek(fp_r, 0, 2);
	size = $ftell(fp_r);
	$display("size is ","%d", size);
	
	//Check the position
	pos_r = $ftell(fp_r);
	$display("position in the moment when we know the size: ","%d", pos_r);
	r = $fseek(fp_r, 0, 0);
	pos_r = $ftell(fp_r);
	$display("position after we know the size: ","%d", pos_r);
	
	// First 44 bytes to .wav
	for(i = 0; i < 44; i = i + 1) begin
		@(posedge clk)
		r = $fread(id[ 7 :  0 ], fp_r, 1);
		@(posedge clk)
		$fwrite(fp_w, "%c", id[ 7 :  0 ]);
	end
	
	//Reading and writing from and in the files
	while (!$feof(fp_r))begin
		i_dv = 1;
		@(posedge clk) begin
			r = $fread(id[  7 :  0 ], fp_r, 1);
			r = $fread(id[ 15 :  8 ], fp_r, 1);
		end
		if(o_dv) begin
		@(posedge clk) begin
			$fwrite(fp_w, "%c", od[  7 :  0 ]);
			$fwrite(fp_w, "%c", od[ 15 :  8 ]);
			end
		end
		i_dv = 0;
	end
	//i_dv = 0;
	
	$fclose(fp_r);
	$fclose(fp_w);
	
end

// Instance of iir
iir
	inst_iir
		(
			.NRESET                    (nrst),
			.CLK                       (clk),
		
			.I_DV                      (i_dv),
			.DI                        (id),
			.O_DV                      (o_dv),
			.DO                        (od)
		);

endmodule
