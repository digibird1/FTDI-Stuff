#include <iostream>
#include <ftdi.h>
#include <string.h>
#include <unistd.h>
#include <ctime>

/*
	read from FPGA only: ~20MB/s
	write to FPGA ~16-17 MB/s
	

*/

int EXIT_FAILURE = -1;

int init_ftdi(struct ftdi_context *ftdi){


	if (ftdi_set_interface(ftdi, INTERFACE_A) < 0)
	{
	   std::cerr<<"ftdi_set_interface failed\n";
	   ftdi_free(ftdi);
	   return EXIT_FAILURE;
	}
	

	char *descstring = NULL;
	if (ftdi_usb_open_desc(ftdi, 0x0403, 0x6014, descstring, NULL) < 0)
	{
	   std::cerr<<"Can't open ftdi device: "<<ftdi_get_error_string(ftdi)<<std::endl;
	   ftdi_free(ftdi);
	   return EXIT_FAILURE;
	}


	/* A timeout value of 1 results in may skipped blocks */
	if(ftdi_set_latency_timer(ftdi, 4))
	{
	   std::cerr<<"Can't set latency, Error "<<ftdi_get_error_string(ftdi)<<std::endl;
	   ftdi_usb_close(ftdi);
	   ftdi_free(ftdi);
	   return EXIT_FAILURE;
	}

	if(ftdi_usb_purge_rx_buffer(ftdi) < 0)
	{
	   std::cerr<<"Can't rx purge "<<ftdi_get_error_string(ftdi)<<std::endl;
	   return EXIT_FAILURE;
	}

	if(ftdi_usb_purge_tx_buffer(ftdi) < 0)
	{
	   std::cerr<<"Can't rx purge "<<ftdi_get_error_string(ftdi)<<std::endl;
	   return EXIT_FAILURE;
	}

	if (ftdi_set_bitmode(ftdi,  0xff, BITMODE_SYNCFF ) < 0)
	{
	   std::cerr<<"Can't set synchronous fifo mode, Error "<<ftdi_get_error_string(ftdi)<<std::endl;
	   ftdi_usb_close(ftdi);
	   ftdi_free(ftdi);
	   return EXIT_FAILURE;
	}


	return 0;
}

int close_ftdi(struct ftdi_context *ftdi){
	ftdi_set_bitmode(ftdi,  0xff, BITMODE_RESET);
	ftdi_usb_close(ftdi);
   	ftdi_free(ftdi);

	return 0;
}






int main(int argc, char **argv)
{
   	struct ftdi_context *ftdi;
	if ((ftdi = ftdi_new()) == 0)
	{
	   std::cerr<<"ftdi_new failed"<<std::endl;;
	   return EXIT_FAILURE;
	}

	init_ftdi(ftdi);


	

//--------------------------------------------------------------------
	int f;



		//write
		const unsigned int n= 1024;//If we go higher than 1024 we start loosing data!!!
		unsigned char buf[n]; 
		char Array[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		unsigned int j=0;
		for(int i=0;i<n;i++){
			buf[i]=Array[j];
			j++;
			if(j==16) j=0;
		}


		const unsigned int m= 1024;
		unsigned char buf2[m];

		time_t now;//Current time
		time(&now);

		long unsigned int counter=0;
		int DataReceived=0;

		for(int i=0;i<1e6;i++){
		
			if( ftdi_write_data(ftdi, buf, n)<n){
				std::cerr<<"write failed"<< f <<"\t"<< ftdi_get_error_string(ftdi)<<std::endl;
			}

			DataReceived=0;
			while(DataReceived<m){
				f=ftdi_read_data 	( ftdi, buf2, m );

				DataReceived+=f;

				if(f>0){//Data available and no error
					for(int i=0;i<f;i++){
						//std::cout<<buf2[i];
						if(buf2[i]!=Array[counter%16]){
							std::cout<<buf2[i]<<"!="<<Array[counter%16]<<std::endl;
						}
						counter++;
					} 
					//std::cout<<":End"<<std::endl;
				}

				if(f<m){
					std::cerr<<"Data received smaller than buffer! "<<DataReceived<<std::endl;
					return 0;
				} 

			}	
			

			if(i%1000==0){
				double seconds = difftime(time(NULL),now);
				std::cout<<seconds<<std::endl;
				
			 	std::cout<<2.*(double)(i*n)/seconds<<" byte/s"<<std::endl;
			}

		}
		





//----------------------------------------------------------------------------

		close_ftdi(ftdi);




	return 0;
}
