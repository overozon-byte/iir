///////////////////////////////////////////////////////////////////////////////
//
// Company:			Polytech Students Inovations
// Engineer:        Zelenin Aleksey Aleksandrovich
//
// Create Date:     
// Design Name:
// Module Name:     iir
// Project Name:
// Target Devices:
// Tool versions:
// Description:     lowpass filter
//
////////////////////////////////////////////////////////////////////////////////

module iir
		(
		input	wire                    NRESET,
		input	wire                    CLK,
		
		input	wire                    I_DV, // ready to read
		input	wire signed	[ 15 :  0 ] DI,   // input data
		
		output	wire                    O_DV, // ready to write
		output	wire signed [ 15 :  0 ] DO   // output data
		);
		
		reg signed [ 15 :  0 ] r_DO;
		reg r_O_DV;
		
		assign DO = r_DO;
		assign O_DV = r_O_DV;
		
		// //3000 Hz
		//A coefficients (1.23 format)
		wire signed [ 23 :  0 ] ACoef0 = 24'd583557;
		wire signed [ 23 :  0 ] ACoef1 = 24'd1167114;
		wire signed [ 23 :  0 ] ACoef2 = 24'd583557;
		//B coefficients (1.23 format)
		wire signed [ 23 :  0 ] BCoef0 = 24'd16777216;
		wire signed [ 23 :  0 ] BCoef1 = -24'd23614021;
		wire signed [ 23 :  0 ] BCoef2 = 24'd9171256;
		
		//5000 Hz
		//A coefficients (1.23 format)
		// wire signed [ 23 :  0 ] ACoef0 = 24'd1395190;
		// wire signed [ 23 :  0 ] ACoef1 = 24'd2790381;
		// wire signed [ 23 :  0 ] ACoef2 = 24'd1395190;
		// //B coefficients (1.23 format)
		// wire signed [ 23 :  0 ] BCoef0 = 24'd16777216;
		// wire signed [ 23 :  0 ] BCoef1 = -24'd17367290;
		// wire signed [ 23 :  0 ] BCoef2 = 24'd6170839;
		
		// //1000 Hz
		// //A coefficients (1.23 format)
		// wire signed [ 23 :  0 ] ACoef0 = 24'd77387;
		// wire signed [ 23 :  0 ] ACoef1 = 24'd154774;
		// wire signed [ 23 :  0 ] ACoef2 = 24'd77387;
		// //B coefficients (1.23 format)
		// wire signed [ 23 :  0 ] BCoef0 = 24'd16777216;
		// wire signed [ 23 :  0 ] BCoef1 = -24'd30183829;
		// wire signed [ 23 :  0 ] BCoef2 = 24'd13715582;
		
		//Regs
		reg signed [ 15 :  0 ] r_old_data;
		reg signed [ 15 :  0 ] intermed;
		reg signed [ 39 :  0 ] add;
		
		//D-latch
		always @(posedge CLK or negedge NRESET) begin
			if (!NRESET)
				r_old_data <= 0;
			else
				r_old_data <= r_DO;
		end
		
		//IIR
		//intermed <= (add >> 24);
		always @(posedge CLK) begin
			if(I_DV) begin
				if (DI == 0) begin
					r_DO <= 0;
				end
				else begin
					//First addition
					add <= (ACoef0 * DI - BCoef0 * r_old_data);
					intermed <= add[ 38 :  23 ];
				
					//Second addition
					add <= (ACoef1 * DI - BCoef1 * intermed);
					intermed <= add[ 38 :  23 ];
			
					//Third addition
					add <= (ACoef2 * DI - BCoef2 * intermed);
					r_DO <= add[ 38 :  23 ];
				
					r_O_DV <= 1;
				end
			end
			else begin
				add <= 0;
				intermed <= 0;
				r_O_DV <= 0;
			end
		end

endmodule
