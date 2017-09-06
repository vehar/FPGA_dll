//-------------- interface description file

//MAX 128 Addr
#ifndef REGISTER_DEFINES
#define REGISTER_DEFINES

//>>REG_DEF_START<<

//SYSTEM
#define FMC_SYNC_nEN_b	(0)
#define FMC_SYNC_SRC_b	(1)
#define FMC_ADC_EN_b	(2)
#define FMC_DAC_EN_b	(3)
#define FMC_SCAN_MODE_b	(4)

#define CONTROL_REG 1 //0-SYNC_nEN_CR 1-SYNC_CR 2-ADC_EN 3-DAC_EN 4-SCAN_MODE_CR
//----------------------------------------------------------------------------------


#define SYSTEM_BASE_ADR 1										
#define SYSTEM_RESET_CR				 (SYSTEM_BASE_ADR + 1)        //_FreqSync1Addr
//-CONTROL_REG #define SCAN_MODE_CR 			(SYSTEM_BASE_ADR + 2)        //
//-CONTROL_REG #define SYNC_nEN_CR	 			(SYSTEM_BASE_ADR + 3)        //_SyncCtrlAddr
#define GRSQ_RUN					 (SYSTEM_BASE_ADR + 4)        //	_GRSQ_RUN //Generator-reciever run sequence configuration reg
#define FSYNC_DR 					 (SYSTEM_BASE_ADR + 5)        //
//CONTROL_REG #define SYNC_CR 					 (SYSTEM_BASE_ADR + 6)        //
#define MAIN_CR 					 (SYSTEM_BASE_ADR + 7)        //
#define LPM_CR 						 (SYSTEM_BASE_ADR + 8)        //control reg 0-adc Npwdn; 1- ...
                                                                
//GENERATOR                                                     
#define GEN_BASE_ADR 10

#define DAC_GAIN_CH1 (0x0A)
#define DAC_GAIN_CH2 (0x0B)
#define DAC_GAIN_CH3 (0x0C)
#define DAC_GAIN_CH4 (0x0D)
#define DAC_GAIN_CH5 (0x0E)
#define DAC_GAIN_CH6 (0x0F)
#define DAC_GAIN_CH7 (0x10)
#define DAC_GAIN_CH8 (0x11)

#define GEN_EN		 (0x12)
/*
#define GEN_DELAY_DR_1				 (GEN_BASE_ADR + 1)           //_GEN_DELAY_DR0_Adr //GenBuffAddr1
#define GEN_DELAY_DR_2				 (GEN_BASE_ADR + 2)           //	
#define GEN_DELAY_DR_3				 (GEN_BASE_ADR + 3)           // 
#define GEN_DELAY_DR_4				 (GEN_BASE_ADR + 4)           //
#define GEN_DELAY_DR_5				 (GEN_BASE_ADR + 5)           //
#define GEN_DELAY_DR_6				 (GEN_BASE_ADR + 6)           //
#define GEN_DELAY_DR_7				 (GEN_BASE_ADR + 7)           //
		                                                        
#define GEN_EN					 (GEN_BASE_ADR + 8)           //_GEN_EN_MR_Adr //GenCSAddr
*/
#define GEN_DURATION_DR_Adr			 (GEN_BASE_ADR + 9)           //_GEN_DURATION_DR_Adr //GenStartAddrWr

//ANALOG                                                        
#define ANALOG_BASE_ADR 20                                      
#define INTEGR_COEF_DR				 (ANALOG_BASE_ADR + 1)        //
#define COMPRESS_DR					 (ANALOG_BASE_ADR + 2)        //
#define DETECT_CR					 (ANALOG_BASE_ADR + 3)        //
#define AN_CH_CSR					 (ANALOG_BASE_ADR + 4)        //	
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
#define ACGAIN_DR_4					 (ACGAIN_BASE_ADR + 4)         //
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
#define ASCAN_EN_MR					 (ASCAN_BASE_ADR + 2)         //		
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
typedef enum {OFF, NEG_SIG, POS_SIG, NEG_POS_SIG} Detector_reg; 		//positive-negative signal detection
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