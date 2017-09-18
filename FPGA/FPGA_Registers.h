//-------------- interface description file

//MAX 128 Addr
#ifndef REGISTER_DEFINES
#define REGISTER_DEFINES

//>>REG_DEF_START<<

#define ON  (1)
#define OFF (0)

//SYSTEM CONTROL_REG bits
#define FMC_SYNC_nEN_b	(0)
#define FMC_SYNC_SRC_b	(1)
#define FMC_ADC_EN_b	(2)
#define FMC_DAC_EN_b	(3)
#define FMC_SCAN_MODE_b	(4)
#define GEN_HW_EN_b    	(5) //old GEN_EN
#define ASCAN_EN_b    	(6) //old ASCAN_EN_MR

//----------------------------------------------------------------------------------

//SYSTEM
#define SYSTEM_BASE_ADR 1
#define CONTROL_REG					SYSTEM_BASE_ADR + 0	// //0-SYNC_nEN_CR 1-SYNC_CR 2-ADC_EN 3-DAC_EN 4-SCAN_MODE_CR
#define SYSTEM_RESET_CR				SYSTEM_BASE_ADR + 1	//
#define AN_CH_CSR					SYSTEM_BASE_ADR + 2 //	
//#define ___________				SYSTEM_BASE_ADR + 3	//
//#define ___________				SYSTEM_BASE_ADR + 4	//
#define FSYNC_DR 					SYSTEM_BASE_ADR + 5	//
#define DRAW_STARTTIME_DR			SYSTEM_BASE_ADR + 6 //
#define DRAW_ENDTIME_DR				SYSTEM_BASE_ADR + 7 //
#define DRAW_COMPRESS_DR			SYSTEM_BASE_ADR + 8 //

//DAC                                                                                              
#define  DAC_BASE_ADR 10
#define  DAC_GAIN_DR_1				 DAC_BASE_ADR + 0		//
#define  DAC_GAIN_DR_2				 DAC_BASE_ADR + 1		//	
#define  DAC_GAIN_DR_3				 DAC_BASE_ADR + 2		// 
#define  DAC_GAIN_DR_4				 DAC_BASE_ADR + 3		//
#define  DAC_GAIN_DR_5				 DAC_BASE_ADR + 4		//
#define  DAC_GAIN_DR_6				 DAC_BASE_ADR + 5		//
#define  DAC_GAIN_DR_7				 DAC_BASE_ADR + 6		//
#define  DAC_GAIN_DR_8				 DAC_BASE_ADR + 7		//
///////////////////////////////////////////////////////
#define  COMPRESS_BASE_ADR			 DAC_GAIN_DR_8 + 1
#define  COMPRESS_DR_1_2				 COMPRESS_BASE_ADR + 0	//
#define  COMPRESS_DR_3_4				 COMPRESS_BASE_ADR + 1	//
#define  COMPRESS_DR_5_6				 COMPRESS_BASE_ADR + 2	//
#define  COMPRESS_DR_7_8				 COMPRESS_BASE_ADR + 3	//

///////////////////////////////////////////////////////
//Control Zone Delay Channel n
#define  ADC_DELAY_BASE_ADR			 COMPRESS_DR_7_8 + 1//  22
#define  ADC_DELAY_DR_1				 ADC_DELAY_BASE_ADR + 0
#define  ADC_DELAY_DR_2				 ADC_DELAY_BASE_ADR + 1
#define  ADC_DELAY_DR_3				 ADC_DELAY_BASE_ADR + 2
#define  ADC_DELAY_DR_4				 ADC_DELAY_BASE_ADR + 3
#define  ADC_DELAY_DR_5				 ADC_DELAY_BASE_ADR + 4
#define  ADC_DELAY_DR_6				 ADC_DELAY_BASE_ADR + 5
#define  ADC_DELAY_DR_7				 ADC_DELAY_BASE_ADR + 6
#define  ADC_DELAY_DR_8				 ADC_DELAY_BASE_ADR + 7

///////////////////////////////////////////////////////
//Control Zone Delay Duration n
#define ADC_DURATION_BASE_ADR  ADC_DELAY_DR_8 + 1   // 30
#define ADC_DURATION_DR_1   ADC_DURATION_BASE_ADR + 0
#define ADC_DURATION_DR_2   ADC_DURATION_BASE_ADR + 1
#define ADC_DURATION_DR_3   ADC_DURATION_BASE_ADR + 2
#define ADC_DURATION_DR_4   ADC_DURATION_BASE_ADR + 3
#define ADC_DURATION_DR_5   ADC_DURATION_BASE_ADR + 4
#define ADC_DURATION_DR_6   ADC_DURATION_BASE_ADR + 5
#define ADC_DURATION_DR_7   ADC_DURATION_BASE_ADR + 6
#define ADC_DURATION_DR_8   ADC_DURATION_BASE_ADR + 7
		    
// TODO
#define GEN_CH_ACCORD_BASE_ADR  0x40
#define GEN_CH_ACCORD_DR_1   GEN_CH_ACCORD_BASE_ADR + 0
#define GEN_CH_ACCORD_DR_2   GEN_CH_ACCORD_BASE_ADR + 1
#define GEN_CH_ACCORD_DR_3   GEN_CH_ACCORD_BASE_ADR + 2
#define GEN_CH_ACCORD_DR_4   GEN_CH_ACCORD_BASE_ADR + 3
#define GEN_CH_ACCORD_DR_5   GEN_CH_ACCORD_BASE_ADR + 4
#define GEN_CH_ACCORD_DR_6   GEN_CH_ACCORD_BASE_ADR + 5
#define GEN_CH_ACCORD_DR_7   GEN_CH_ACCORD_BASE_ADR + 6
#define GEN_CH_ACCORD_DR_8   GEN_CH_ACCORD_BASE_ADR + 7

#define ASCAN_BUFF_SIZE   0x50
#define ASCAN_BUFF		    0x50


//ANALOG                                                        
#define ANALOG_BASE_ADR 20                                      
//#define INTEGR_COEF_DR				 (ANALOG_BASE_ADR + 1)        //
#define COMPRESS_DR					 (ANALOG_BASE_ADR + 2)        //
#define DETECT_CR					 (ANALOG_BASE_ADR + 3)        //
#define AN_CH_CSR					 (3) //(ANALOG_BASE_ADR + 4)        //	
#define AC_SUM_DR					 (ANALOG_BASE_ADR + 5)        //R
#define CH_SET_CR					 (ANALOG_BASE_ADR + 6)  //R
#define DAC_GAIN_DR					 (ANALOG_BASE_ADR + 7)  // W - curent amplification

//FILTER	                                                    
#define FILT_BASE_ADR 30	                                    
#define FILT_COEFS_RST_CR			 (FILT_BASE_ADR + 1)          //
#define FILT_COMPRES_DR				 (FILT_BASE_ADR + 2)          // 
#define FILT_COEFS_DR				 (FILT_BASE_ADR + 3)          //
#define FILT_EN_CR					 (FILT_BASE_ADR + 4)          //
                                                                
//TGC                                                           
#define TGC_BASE_ADR 40                                         
#define TGC_RAM_START_AR			 (TGC_BASE_ADR + 1)           //		
#define TGC_RAM_DR 					 (TGC_BASE_ADR + 2)           //
#define TGC_EN_MR					 (TGC_BASE_ADR + 3)           //	
//ACUST CONTACT GAIG	                                        
#define ACGAIN_BASE_ADR 50			 					        
#define ACGAIN_DR_1					 (ACGAIN_BASE_ADR + 1)        //	
#define ACGAIN_DR_2					 (ACGAIN_BASE_ADR + 2)        // 
#define ACGAIN_DR_3					 (ACGAIN_BASE_ADR + 3)        //
#define ACGAIN_DR_4					 (ACGAIN_BASE_ADR + 4)        //
#define ACGAIN_DR_5					 (ACGAIN_BASE_ADR + 5)        //
#define ACGAIN_DR_6					 (ACGAIN_BASE_ADR + 6)        //
#define ACGAIN_DR_7					 (ACGAIN_BASE_ADR + 7)        //
#define ACGAIN_DR_8					 (ACGAIN_BASE_ADR + 8)        //
                                                                
//ADC                                                           
#define ADC_BASE_ADR 60		                                    
#define ADC_PEC_DELAY_DR			 (ADC_BASE_ADR + 1)           //	
#define ADC_DELAY_DR				 (ADC_BASE_ADR + 2)           //		  
#define ADC_DATA_DR			         (ADC_BASE_ADR + 3)           //	
#define ADC_CZONEDUR_DR				 (ADC_BASE_ADR + 4)     // duration of control zone
                                                            
//ASCAN 	                                                    
#define ASCAN_BASE_ADR 70		                                
#define ASCAN_WR_CSR				 (ASCAN_BASE_ADR + 1)         //		
//#define ASCAN_EN_MR					 (ASCAN_BASE_ADR + 2)         //		
#define ASCAN_COLOR 				 (ASCAN_BASE_ADR + 3)         // !!!
#define LCD_CR						 (ASCAN_BASE_ADR + 4)         // CPU lcd bus -on/off
                                                                
//CURSORS 	                                                    
#define CURSOR_BASE_ADR 80		                                
#define CURSOR_X_ADR				 (CURSOR_BASE_ADR + 1)        //	
#define CURSOR_Y_ADR				 (CURSOR_BASE_ADR + 5)        //
	                                                            
#define CURSOR_X_DR_1				 (CURSOR_X_ADR + 1)           // 	
#define CURSOR_X_DR_2				 (CURSOR_X_ADR + 2)           //	
#define CURSOR_Y_DR_1				 (CURSOR_Y_ADR + 1)           //	
#define CURSOR_Y_DR_2				 (CURSOR_Y_ADR + 2)           //	
                                                                
//STROBE                                                        
#define STROBE_BASE_ADR 90                                      
#define STROBE_1_BASE 				 (STROBE_BASE_ADR + 1)        	

#define STROBE_1_STR_DR 			 (STROBE_1_BASE + 1)       //
#define STROBE_1_END_DR 			 (STROBE_1_BASE + 2)       //
#define STROBE_1_LVL_DR				 (STROBE_1_BASE + 3)	        //
		                                                          
#define STROBE_1_1EL_DR  			 (STROBE_1_BASE + 4) 	        //R
#define STROBE_1_ZERO_DR 			 (STROBE_1_BASE + 6) 	        //R
#define STROBE_1_MAX_DR	 			 (STROBE_1_BASE + 5) 	        //R
//---------------------------------- --------------------       -------------------------
#define STROBE_2_BASE  				 (STROBE_BASE_ADR + 10)       //(!!!)
		                                                        
#define STROBE_2_STR_DR 			 (STROBE_2_BASE + 1)	        //
#define STROBE_2_END_DR 			 (STROBE_2_BASE + 2)	        //
#define STROBE_2_LVL_DR				 (STROBE_2_BASE + 3)	        //
		                                                         
#define STROBE_2_1EL_DR  			 (STROBE_2_BASE + 4) 	        //R
#define STROBE_2_MAX_DR	 			 (STROBE_2_BASE + 5) 	        //R
#define STROBE_2_ZERO_DR 			 (STROBE_2_BASE + 6) 	        //R
//---------------------------------- --------------------       -------------------------
                                                                
//TESTS                                                         
#define TESTS_BASE_ADR 110                                      
#define SPEED_TEST_DR				 (TESTS_BASE_ADR + 1)         //R/W
#define ABUS_TEST_DR				 (TESTS_BASE_ADR + 2)         //R                             
#define DBUS_TEST_DR				 (TESTS_BASE_ADR + 3)         //
#define TEST_CNT_DR					 (TESTS_BASE_ADR + 4)          //R
#define TEST_IRQ_CR					 (TESTS_BASE_ADR + 5)         //	  
#define VERSION_DR 					 (TESTS_BASE_ADR + 6)         // 			                                    
#define SITEP_CR					 (TESTS_BASE_ADR + 7)         //SignalTestPattern_ControlReg// rdm 11 - generated in FPGA  
                                     
//TRACK_SENSOR (encoder)             
#define TRACK_SENSOR_BASE_ADR 120    
#define TRACK_SPEED_DR 				 (TRACK_SENSOR_BASE_ADR + 1)   //R
#define TRACK_POS_DR 				 (TRACK_SENSOR_BASE_ADR + 2)   //R
          
//bit defines

typedef enum {STOP = 0, INT_SYNC, EXT_SYNC, EXT_CONTROLLER} SyncCtr_reg;
//typedef enum {OFF, NEG_SIG, POS_SIG, NEG_POS_SIG} Detector_reg; 		//positive-negative signal detection
typedef enum {CH1_COMB, CH1_SEPAR, CH2_COMB, CH2_SEPAR} AN_CH_CSR_reg; //COMBINED MODE, SEPARATED MODE
typedef enum {ATTEN_OFF, ATTEN_ON} AttenSw_reg;							//Input signal attenuator
typedef enum {INVERTION_OFF, INVERTION_ON} InvertSignal_reg; 
typedef enum {TGC_OFF, TGC_ON} TgcEn_reg; 
typedef enum {DAC_CH_1, DAC_CH_2} DacCh_reg; 

typedef enum {FILT_OFF, FILT_ON} FiltEn_reg; //filter
//!
/*
typedef enum {OFF, GS0,GS1,GS2,GS3,GS4,GS5,GS6,GS7} GenCS_reg; //WR to selected gen_buf 
typedef enum {OFF, GE0,GE1,GE2,GE3,GE4,GE5,GE6,GE7} GenEn_reg; //Enable selected generator

typedef enum {TWO_POINTS, ONE_POINT} AScanDrawMode_reg; //
typedef enum {OFF, GE0,GE1,GE2,GE3,GE4,GE5,GE6,GE7} AScanEn_reg; //Enable selected A-skan
typedef enum {OFF, GE0,GE1,GE2,GE3,GE4,GE5,GE6,GE7} AScanWrCS_reg; //Enable selected A-skan
*/

#endif //REGISTER_DEFINES
//>>REG_DEF_END<<