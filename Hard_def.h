/*** HARD_DEF.H *******************************************
 *
 *********************************************************/
#define HW_INT_DIS				__disable_irq()
#define HW_INT_ENA				__enable_irq()
//*** RCC Reset and clock control ************************
#define HW_CPU_CLK_RD(ptr)		RCC_GetClocksFreq(ptr)
#define HW_CPU_CLK_INS \
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE), \
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE), \
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE), \
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE), \
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE)
//*** GPIO General purpose I/O ***************************
// Ultrasound US, infrared IR
// PA0  (PIN 10) AGND(US) 			AIN
// PA1  (PIN 11) ECHO(US) 			AIN
// PA2  (PIN 12) ECHO_LEV(US)		AIN
// PA3  (PIN 13) FREF_OUT_AN(IR_RX) AIN
// PA4  (PIN 14)
// PA5  (PIN 15)
// PA6  (PIN 16) ECHO(US)			COMP_OUT PA1 - DAC1 AlternateFx = 7
// PA7  (PIN 17)
// PA8  (PIN 29) HW_LEV0			AIN o DEBUG_TIMING_OUT
// PA9  (PIN 30) TXD/
// PA10 (PIN 31) RXD/
// PA11 (PIN 32) HW_LEV1			AIN o DEBUG_TIMING_OUT
// PA12 (PIN 33) LED				OUT active high LED
// PA13 (PIN 34) SWDIO				Emulator
// PA14 (PIN 37) SWCLK				Emulator
// PA15 (PIN 38)
//
// PB0  (PIN 18) A2_G0(US)			OUT active high Amplify
// PB1  (PIN 19) A2_G1(US)			OUT active high Amplify
// PB2  (PIN 20) A2_G2(US)			OUT active high Amplify
// PB3  (PIN 39) A1_G0(US)			OUT active high Amplify
// PB4  (PIN 40) A1_G1(US)			OUT active high Amplify
// PB5  (PIN 41) A1_G2(US)			OUT active high Amplify
// PB6  (PIN 42) ENA(US)			OUT open collector high, low to activate
// PB7  (PIN 43) T2_IN(US)			OUT negative T1_IN to operate
// PB8  (PIN 45) FREF_PWM(IR_TX)	OUT PWM AlternateFx = 2
// PB9  (PIN 46) T1_IN(US)			OUT positive T2_IN to operate
// PB10 (PIN 21) FREF_OUT_DIG(IR_RX)IN sensing IR
// PB11 (PIN 22) SEL3(IR_TX)		OUT anode active high
// PB12 (PIN 25) SEL2(IR_TX)		OUT anode active high
// PB13 (PIN 26) SEL1(IR_TX)		OUT anode active high
// PB14 (PIN 27) VBURST(IR_TX)		OUT active low (shorts diode)
// PB15 (PIN 28)
//
// PC13 (PIN 2)
//
// PF6  (PIN 35) SCL/(I2C)			OUT clock I2C
// PF7  (PIN 36) SDA/(I2C)			I/O data I2C
//*** IO_INS ************************************************
// Macro HW_DEBUG_TIMING_IO_INS inside HW_IO_INS
#if (DEBUG_TIMING)
#define HW_DEBUG_TIMING_IO_INS \
	loc.GPIO_Mode = GPIO_Mode_OUT, 	/* PIN digital output: */ \
	loc.GPIO_OType = GPIO_OType_PP, \
	loc.GPIO_PuPd = GPIO_PuPd_NOPULL, \
	loc.GPIO_Speed = GPIO_Speed_10MHz, \
									/* ... Debug A active high */ \
	loc.GPIO_Pin = GPIO_Pin_8, GPIOA->BRR = GPIO_Pin_8, \
	GPIO_Init(GPIOA, &loc), \
									/* ... Debug B active high */ \
	loc.GPIO_Pin = GPIO_Pin_11, GPIOA->BRR = GPIO_Pin_11, \
	GPIO_Init(GPIOA, &loc)
#else
#define HW_DEBUG_TIMING_IO_INS \
 	loc.GPIO_Mode = GPIO_Mode_AN,	/* PIN analog input: */ \
	loc.GPIO_PuPd = GPIO_PuPd_NOPULL, \
	loc.GPIO_OType = GPIO_OType_PP, \
	loc.GPIO_Speed = GPIO_Speed_10MHz, \
													/* ... HW_LEV_0 */ \
	loc.GPIO_Pin = GPIO_Pin_8, \
	GPIO_Init(GPIOA, &loc), \
													/* ... HW_LEV_1 */ \
	loc.GPIO_Pin = GPIO_Pin_11, \
	GPIO_Init(GPIOA, &loc)
#endif	/* DEBUG_TIMING */
#define HW_IO_INS \
{ \
	GPIO_InitTypeDef  loc; \
 													/* PIN digital output push-pull no pull: */ \
	loc.GPIO_Mode = GPIO_Mode_OUT; \
	loc.GPIO_OType = GPIO_OType_PP; \
	loc.GPIO_PuPd = GPIO_PuPd_NOPULL; \
	loc.GPIO_Speed = GPIO_Speed_10MHz; \
													/* ... Led active high */ \
	loc.GPIO_Pin = GPIO_Pin_12, GPIOA->BRR = GPIO_Pin_12; \
	GPIO_Init(GPIOA, &loc); \
													/* ... A2_G0 active high */ \
	loc.GPIO_Pin = GPIO_Pin_0, GPIOB->BRR = GPIO_Pin_0; \
	GPIO_Init(GPIOB, &loc); \
													/* ... A2_G1 active high */ \
	loc.GPIO_Pin = GPIO_Pin_1, GPIOB->BRR = GPIO_Pin_1; \
	GPIO_Init(GPIOB, &loc); \
													/* ... A2_G2 active high */ \
	loc.GPIO_Pin = GPIO_Pin_2, GPIOB->BRR = GPIO_Pin_2; \
	GPIO_Init(GPIOB, &loc); \
													/* ... A1_G0 active high */ \
	loc.GPIO_Pin = GPIO_Pin_3, GPIOB->BRR = GPIO_Pin_3; \
	GPIO_Init(GPIOB, &loc); \
													/* ... A1_G1 active high */ \
	loc.GPIO_Pin = GPIO_Pin_4, GPIOB->BRR = GPIO_Pin_4; \
	GPIO_Init(GPIOB, &loc); \
													/* ... A1_G2 active high */ \
	loc.GPIO_Pin = GPIO_Pin_5, GPIOB->BRR = GPIO_Pin_5; \
	GPIO_Init(GPIOB, &loc); \
													/* ... SEL3 active high */ \
	loc.GPIO_Pin = GPIO_Pin_11, GPIOB->BRR = GPIO_Pin_11; \
	GPIO_Init(GPIOB, &loc); \
													/* ... SEL2 active high */ \
	loc.GPIO_Pin = GPIO_Pin_12, GPIOB->BRR = GPIO_Pin_12; \
	GPIO_Init(GPIOB, &loc); \
													/* ... SEL1 active high */ \
	loc.GPIO_Pin = GPIO_Pin_13, GPIOB->BRR = GPIO_Pin_13; \
	GPIO_Init(GPIOB, &loc); \
													/* ... VBURST active low (shorts diode) */ \
	loc.GPIO_Pin = GPIO_Pin_14, GPIOB->BSRR = GPIO_Pin_14; \
	GPIO_Init(GPIOB, &loc); \
													/* ... ENA Open drain high (Must be last) */ \
	loc.GPIO_OType = GPIO_OType_OD; \
	loc.GPIO_Pin = GPIO_Pin_6, GPIOB->BSRR = GPIO_Pin_6; \
	GPIO_Init(GPIOB, &loc); \
													/* PIN digital input */ \
	loc.GPIO_OType = GPIO_OType_PP; \
	loc.GPIO_Mode = GPIO_Mode_IN; \
													/* ... FREF_OUT_DIG */ \
	loc.GPIO_Pin = GPIO_Pin_10; \
	GPIO_Init(GPIOB, &loc); \
													/* PIN analog input: */ \
	loc.GPIO_Mode = GPIO_Mode_AN; \
	loc.GPIO_OType = GPIO_OType_PP; \
													/* ... AGND */ \
	loc.GPIO_Pin = GPIO_Pin_0; \
	GPIO_Init(GPIOA, &loc); \
													/* ... ECHO */ \
	loc.GPIO_Pin = GPIO_Pin_1; \
	GPIO_Init(GPIOA, &loc); \
													/* ... ECHO LEV */ \
	loc.GPIO_Pin = GPIO_Pin_2; \
	GPIO_Init(GPIOA, &loc); \
													/* ... FREF_OUT_AN */ \
	loc.GPIO_Pin = GPIO_Pin_3; \
	GPIO_Init(GPIOA, &loc); \
													/* PIN Alternate FX: */ \
	loc.GPIO_Mode = GPIO_Mode_AF; \
	loc.GPIO_OType = GPIO_OType_PP; \
	loc.GPIO_PuPd = GPIO_PuPd_NOPULL; \
													/* ... FREF_PWM */ \
	loc.GPIO_Pin = GPIO_Pin_8; \
	GPIO_Init(GPIOB, &loc); \
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2); \
													/* ... COMP */ \
	loc.GPIO_Pin = GPIO_Pin_6; \
	GPIO_Init(GPIOA, &loc); \
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_7); \
													/* ... T2_IN TIMx_CH1N */ \
	loc.GPIO_Pin = GPIO_Pin_7; \
	GPIO_Init(GPIOB, &loc); \
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_2); \
													/* ... T1_IN TIMx_CH1 */ \
	loc.GPIO_Pin = GPIO_Pin_9; \
	GPIO_Init(GPIOB, &loc); \
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_2); \
													/* Debug */ \
	HW_DEBUG_TIMING_IO_INS; \
}
//*** I/O ***************************************************
#define HW_IR_RX_TST					(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10))
#define HW_IR_SEL_A_ON					(GPIOB->BSRR = GPIO_Pin_13)			// VSEL1..3
#define HW_IR_SEL_A_OFF					(GPIOB->BRR = GPIO_Pin_13)
#define HW_IR_SEL_B_ON					(GPIOB->BSRR = GPIO_Pin_12)
#define HW_IR_SEL_B_OFF					(GPIOB->BRR = GPIO_Pin_12)
#define HW_IR_SEL_C_ON					(GPIOB->BSRR = GPIO_Pin_11)
#define HW_IR_SEL_C_OFF					(GPIOB->BRR = GPIO_Pin_11)
#define HW_IR_TX_ON						(GPIOB->BRR = GPIO_Pin_14)			// VBURST
#define HW_IR_TX_OFF					(GPIOB->BSRR = GPIO_Pin_14)
#define HW_IR_ON(led_ind) \
{ \
	(led_ind == IR_A_ID) ? HW_IR_SEL_A_ON : HW_IR_SEL_A_OFF; \
	(led_ind == IR_B_ID) ? HW_IR_SEL_B_ON : HW_IR_SEL_B_OFF; \
	(led_ind == IR_C_ID) ? HW_IR_SEL_C_ON : HW_IR_SEL_C_OFF; \
}
#define HW_IR_OFF		HW_IR_SEL_A_OFF, HW_IR_SEL_B_OFF, HW_IR_SEL_C_OFF
//
#define HW_US_POW_ENA					GPIOB->BRR = GPIO_Pin_6
#define HW_US_POW_DIS					GPIOB->BSRR = GPIO_Pin_6
#define HW_US_AMP_SET(x) \
{ \
	if (((x) & 0x01) > 0) \
		GPIOB->BSRR = GPIO_Pin_0; \
	else \
		GPIOB->BRR = GPIO_Pin_0; \
	if (((x) & 0x02) > 0) \
		GPIOB->BSRR = GPIO_Pin_1; \
	else \
		GPIOB->BRR = GPIO_Pin_1; \
	if (((x) & 0x04) > 0) \
		GPIOB->BSRR = GPIO_Pin_2; \
	else \
		GPIOB->BRR = GPIO_Pin_2; \
	if (((x) & 0x08) > 0) \
		GPIOB->BSRR = GPIO_Pin_3; \
	else \
		GPIOB->BRR = GPIO_Pin_3; \
	if (((x) & 0x10) > 0) \
		GPIOB->BSRR = GPIO_Pin_4; \
	else \
		GPIOB->BRR = GPIO_Pin_4; \
	if (((x) & 0x20) > 0) \
		GPIOB->BSRR = GPIO_Pin_5; \
	else \
		GPIOB->BRR = GPIO_Pin_5; \
}
//*** Ticker ************************************************
#define HW_TIC_INS(tic_freq, cpu_clk_freq)		SysTick_Config((cpu_clk_freq) / (tic_freq))
#define HW_TIC_VAL		SysTick->VAL
// Handler prototyped in startup
#define HW_TIC_HANDLER() void SysTick_Handler(void)
//*** ADC ***************************************************
#if (DEBUG_TIMING)
#define ADC_CHAN_TBL {ADC_CHSELR_CHSEL0, ADC_CHSELR_CHSEL1, ADC_CHSELR_CHSEL2, ADC_CHSELR_CHSEL3}
#else
#define ADC_CHAN_TBL {ADC_CHSELR_CHSEL0, ADC_CHSELR_CHSEL1, ADC_CHSELR_CHSEL2, ADC_CHSELR_CHSEL3, ADC_CHSELR_CHSEL8, ADC_CHSELR_CHSEL11}
#endif
#define HW_ADC_INS(chan_tbl) \
{ \
	ADC_InitTypeDef adcInit; \
	byte i; \
 \
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); \
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE); \
	/* Configurazione dell'ADC */ \
	adcInit.ADC_ContinuousConvMode = DISABLE; \
	/* adcInit.ADC_ContinuousConvMode = ENABLE; */ \
	adcInit.ADC_DataAlign = ADC_DataAlign_Right; \
	adcInit.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO; \
	adcInit.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; \
	adcInit.ADC_Resolution = ADC_Resolution_10b; \
	adcInit.ADC_ScanDirection = ADC_ScanDirection_Upward; \
	/* adcInit.ADC_ScanDirection = ADC_ScanDirection_Backward; */ \
	ADC_Init(ADC1, &adcInit); \
	for (i = 0; i < ADC_N; i++) \
		ADC_ChannelConfig(ADC1, chan_tbl[i], ADC_SampleTime_7_5Cycles); \
	ADC_GetCalibrationFactor(ADC1); \
	ADC_Cmd(ADC1, ENABLE); \
}
/* Configurazione del DMA1 Channel1 */
//DMA_DeInit(DMA1_Channel1);
#define HW_ADC_DMA_INS(adc_buf) \
{ \
	DMA_InitTypeDef dmaInit; \
 \
	dmaInit.DMA_PeripheralBaseAddr = (dword)&(ADC1->DR),	/*(uint32_t)ADC1_DR_Address */ \
	dmaInit.DMA_MemoryBaseAddr = (dword)adc_buf, \
	dmaInit.DMA_DIR = DMA_DIR_PeripheralSRC, \
	dmaInit.DMA_BufferSize = ADC_N, \
	dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable, \
	dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable, \
	dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord, \
	dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord, \
	dmaInit.DMA_Mode = DMA_Mode_Circular, \
	dmaInit.DMA_Priority = DMA_Priority_High, \
	dmaInit.DMA_M2M = DMA_M2M_Disable, \
	DMA_Init(DMA1_Channel1, &dmaInit), \
	DMA_Cmd(DMA1_Channel1, ENABLE); \
}
/* ADC DMA request in circular mode */
//ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
#define HW_ADC_ENA \
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_OneShot), \
	ADC_DMACmd(ADC1, ENABLE)
//
#define HW_ADC_START			ADC_StartOfConversion(ADC1)
#define HW_ADC_DONE_TST			(!(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET))
#define HW_ADC_DONE_ACK			DMA_ClearFlag(DMA1_FLAG_TC1)
//*** DAC ***************************************************
// DAC mode soft triggered. right aligned 12 bit
#define HW_DAC_INS \
{ \
  DAC_InitTypeDef  loc; \
 \
  	  	  	  	  	  	  	  	  	  	  /* DAC clock enable */ \
  	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); \
  	 loc.DAC_Trigger = DAC_Trigger_Software; \
  	 loc.DAC_OutputBuffer = DAC_OutputBuffer_Disable; \
  	 DAC_Init(DAC_Channel_1, &loc); \
}
#define HW_DAC_UNINS
#define HW_DAC_ENA				DAC_Cmd(DAC_Channel_1, ENABLE)
#define HW_DAC_DIS				DAC_Cmd(DAC_Channel_1, DISABLE)
#define HW_DAC_VAL(x)			DAC_SetChannel1Data(DAC_Align_12b_R, x)
//*** COMP **************************************************
// COMP
#define HW_COMP_INS \
{ \
	  COMP_InitTypeDef loc; \
 \
  	  	  	  	  	  	  	  	  	  	  /* Configure PA1: PA1 is used as COMP1 non inveting input */ \
  	  	  	  	  	  	  	  	  	  	  /* COMP Peripheral clock enable */ \
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); \
     	 	 	 	 	 	 	 	 	  /* COMP1 Init: DAC1 output is used COMP1 inverting input */ \
     loc.COMP_InvertingInput = COMP_InvertingInput_DAC1; \
     	 	 	 	 	 	 	 	 	  /* no redirect COMP1 output */ \
     loc.COMP_Output = COMP_Output_None; \
     loc.COMP_Mode = COMP_Mode_HighSpeed; \
     loc.COMP_Hysteresis = COMP_Hysteresis_No; \
     COMP_Init(COMP_Selection_COMP1, &loc); \
}
#define HW_COMP_ENA				COMP_Cmd(COMP_Selection_COMP1, ENABLE)
#define HW_COMP_DIS				COMP_Cmd(COMP_Selection_COMP1, DISABLE)
//*** RSA ***************************************************
#define HW_RSA_ASYNC_INS(rate, parity, bit_n, stop_n) \
{ \
	GPIO_InitTypeDef loc; \
	USART_InitTypeDef uloc; \
 \
	uloc.USART_BaudRate = rate; \
	uloc.USART_WordLength = (bit_n == '8') ? USART_WordLength_8b : USART_WordLength_9b; \
	uloc.USART_StopBits = (stop_n == '1') ? USART_StopBits_1 : USART_StopBits_2; \
	if (parity == 'N') \
		uloc.USART_Parity = USART_Parity_No; \
	else \
		uloc.USART_Parity = (parity == 'E') ? USART_Parity_Even : USART_Parity_Odd; \
	uloc.USART_HardwareFlowControl = USART_HardwareFlowControl_None; \
	uloc.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; \
																				/* Enable GPIO clock */ \
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); \
																				/* Enable USART clock */ \
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); \
																				/* Connect PXx to USARTx_Tx */ \
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); \
																				/* Connect PXx to USARTx_Rx */ \
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1); \
																				/* Configure USART Tx, Rx as alternate function push-pull */ \
	loc.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; \
	loc.GPIO_Mode = GPIO_Mode_AF; \
	loc.GPIO_Speed = GPIO_Speed_50MHz; \
	loc.GPIO_OType = GPIO_OType_PP; \
	loc.GPIO_PuPd = GPIO_PuPd_UP; \
	GPIO_Init(GPIOA, &loc); \
																				/* USART configuration */ \
	USART_Init(USART1, &uloc); \
																				/* Enable USART */ \
	USART_Cmd(USART1, ENABLE); \
}
#define HW_RSA_INT_INS \
{ \
	NVIC_InitTypeDef loc; \
 \
									/* Enable the USART1 Interrupt */ \
	loc.NVIC_IRQChannel = USART1_IRQn; \
	loc.NVIC_IRQChannelPriority = 0; \
	loc.NVIC_IRQChannelCmd = ENABLE; \
	NVIC_Init(&loc); \
}
#define HW_RSA_RX_INT_ENA 		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE)
#define HW_RSA_RX_INT_DIS		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE)
#define HW_RSA_TX_INT_ENA		USART_ITConfig(USART1, USART_IT_TXE, ENABLE)
#define HW_RSA_TX_INT_DIS		USART_ITConfig(USART1, USART_IT_TXE, DISABLE)
#define HW_RSA_RX_CRT_TST		(!(USART_GetITStatus(USART1, USART_IT_RXNE) == RESET))
#define HW_RSA_RX_CRT(crt)		crt = USART_ReceiveData(USART1)
#define HW_RSA_TX_CRT(crt)		USART_SendData(USART1, crt)
#define HW_RSA_TX_IDLE_TST		(!(USART_GetITStatus(USART1, USART_IT_TXE) == RESET))
// Handler prototyped in startup
#define HW_RSA_HANDLER()		void USART1_IRQHandler(void)
//*** TIM_IR_PWM **********************************************
#define HW_TIM_IR_PWM_DIS		TIM_Cmd(TIM16, DISABLE)
#define HW_TIM_IR_PWM_ENA		TIM_Cmd(TIM16, ENABLE)
#define HW_TIM_IR_PWM_CLR		TIM_SetCounter(TIM16, 0)
#define HW_TIM_IR_PWM_INS \
{ \
	TIM_TimeBaseInitTypeDef tloc; \
 \
												/* TIM clock enable */ \
 	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE); \
 	 	 	 	 	 	 	 	 	 	 	 	/* Time base configuration */ \
	tloc.TIM_Period = IR_CTRL_PERIOD; \
	tloc.TIM_Prescaler = (word) (SystemCoreClock  / IR_CTRL_TIC_FREQ) - 1; \
	tloc.TIM_ClockDivision = TIM_CKD_DIV1; \
	tloc.TIM_CounterMode = TIM_CounterMode_Up; \
	TIM_TimeBaseInit(TIM16, &tloc); \
	TIM_ARRPreloadConfig(TIM16, ENABLE); \
}
//TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable); \
//#define HW_TIM_IR_PWM_RELOAD_ENA   TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable)		/* _FDL_ */
//#define HW_TIM_IR_PWM_RELOAD_DIS   TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Disable)		/* _FDL_ */
#define HW_TIM_IR_PWM_UPD(duty)		TIM_SetCompare1(TIM16, ((dword)IR_CTRL_PERIOD * (dword)(duty) + 128UL) / 256UL)
#define HW_TIM_IR_PWM_OUT_INS(duty) \
{ \
	TIM_OCInitTypeDef loc; \
 \
												/* Output compare PWM 1 */ \
	TIM_OCStructInit(&loc); \
	loc.TIM_OCMode = TIM_OCMode_PWM1; \
	loc.TIM_Pulse = ((dword)IR_CTRL_PERIOD * (dword)(duty) + 128UL) / 256UL; \
	loc.TIM_OutputState = TIM_OutputState_Enable; \
	TIM_OC1Init(TIM16, &loc); \
}
#define HW_TIM_IR_PWM_OUT_ENA		TIM_CtrlPWMOutputs(TIM16, ENABLE)
#define HW_TIM_IR_PWM_OUT_DIS		TIM_CtrlPWMOutputs(TIM16, DISABLE)
//*** TIM_ADC ***********************************************
#define HW_TIM_ADC_INS(freq) \
{ \
	  TIM_TimeBaseInitTypeDef loc; \
													/* TIM3 clock enable */ \
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); \
													/* Time base configuration */ \
	  loc.TIM_Period = (6000000UL / (freq)); \
	  loc.TIM_Prescaler = (word) (SystemCoreClock  / 6000000UL) - 1; \
	  loc.TIM_ClockDivision = TIM_CKD_DIV1; \
	  loc.TIM_CounterMode = TIM_CounterMode_Up; \
	  TIM_TimeBaseInit(TIM3, &loc); \
}
#define HW_TIM_ADC_UNINS
#define HW_TIM_ADC_INT_INS \
{ \
	NVIC_InitTypeDef loc; \
						/* Enable the TIM3 gloabal Interrupt */ \
	loc.NVIC_IRQChannel = TIM3_IRQn; \
	loc.NVIC_IRQChannelPriority = 0x03; \
	loc.NVIC_IRQChannelCmd = ENABLE; \
	NVIC_Init(&loc); \
}
#define HW_TIM_ADC_INT_PEND_CLR \
		DMA_ClearFlag(DMA1_FLAG_TC1), TIM_ClearITPendingBit(TIM3, TIM_IT_CC3)
#define HW_TIM_ADC_CLR			TIM_SetCounter(TIM3, 0)
#define HW_TIM_ADC_INT_ENA		TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE)
#define HW_TIM_ADC_ENA			TIM_Cmd(TIM3, ENABLE)
//
#define HW_TIM_ADC_INT_DIS		TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE)
#define HW_TIM_ADC_DIS			TIM_Cmd(TIM3, DISABLE)
#define HW_TIM_ADC_INT_UNINS \
{ \
	NVIC_InitTypeDef loc; \
 \
	loc.NVIC_IRQChannel = TIM3_IRQn; \
	loc.NVIC_IRQChannelPriority = 0x03; \
	loc.NVIC_IRQChannelCmd = DISABLE; \
	NVIC_Init(&loc); \
}
#define HW_TIM_ADC_HANDLER()		void TIM3_IRQHandler(void)
#define HW_TIM_ADC_INT_TST			(!(TIM_GetITStatus(TIM3, TIM_IT_CC3) == RESET))
#define HW_TIM_ADC_INT_ACK			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3)
//*** HW_TIM_IR *********************************************
#define HW_TIM_IR_INS(freq) \
{ \
	TIM_TimeBaseInitTypeDef loc; \
 \
											/* TIM2 clock enable */ \
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); \
											/* Time base configuration */ \
	loc.TIM_Period = (6000000UL / (freq)); \
	loc.TIM_Prescaler = (word) (SystemCoreClock  / 6000000UL) - 1; \
	loc.TIM_ClockDivision = TIM_CKD_DIV1; \
	loc.TIM_CounterMode = TIM_CounterMode_Up; \
	TIM_TimeBaseInit(TIM2, &loc); \
}
#define HW_TIM_IR_UNINS
#define HW_TIM_IR_ENA				TIM_Cmd(TIM2, ENABLE)
#define HW_TIM_IR_DIS				TIM_Cmd(TIM2, DISABLE)
//
#define HW_TIM_IR_UPD(freq)			TIM_SetAutoreload(TIM2, (6000000UL / (freq)))
#define HW_TIM_IR_CLR				TIM_SetCounter(TIM2, 0)
#define HW_TIM_IR_HANDLER()			void TIM2_IRQHandler(void)
#define HW_TIM_IR_INT_TST			(!(TIM_GetITStatus(TIM2, TIM_IT_CC3) == RESET))
#define HW_TIM_IR_INT_ACK			TIM_ClearITPendingBit(TIM2, TIM_IT_CC3)
#define HW_TIM_IR_INT_PEND_CLR		HW_TIM_IR_INT_ACK
#define HW_TIM_IR_INT_ENA			TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE)
#define HW_TIM_IR_INT_DIS			TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE)
#define HW_TIM_IR_INT_INS \
{ \
	NVIC_InitTypeDef iloc; \
 \
	iloc.NVIC_IRQChannel = TIM2_IRQn; \
	iloc.NVIC_IRQChannelPriority = 0x03; \
	iloc.NVIC_IRQChannelCmd = ENABLE; \
	NVIC_Init(&iloc); \
}
#define HW_TIM_IR_INT_UNINS \
{ \
	NVIC_InitTypeDef iloc; \
 \
	iloc.NVIC_IRQChannel = TIM2_IRQn; \
	iloc.NVIC_IRQChannelPriority = 0x03; \
	iloc.NVIC_IRQChannelCmd = DISABLE; \
	NVIC_Init(&iloc); \
}
//*** TIM_US_PWM **********************************************
#define HW_TIM_US_PWM_DIS		TIM_Cmd(TIM17, DISABLE)
#define HW_TIM_US_PWM_ENA		TIM_Cmd(TIM17, ENABLE)
#define HW_TIM_US_PWM_CLR		TIM_SetCounter(TIM17, 0)
#define HW_TIM_US_PWM_INS \
{ \
	TIM_TimeBaseInitTypeDef tloc; \
 \
												/* TIM clock enable */ \
 	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE); \
 	 	 	 	 	 	 	 	 	 	 	 	/* Time base configuration */ \
	tloc.TIM_Period = US_CTRL_PERIOD; \
	tloc.TIM_Prescaler = (word) (SystemCoreClock  / US_CTRL_TIC_FREQ) - 1; \
	tloc.TIM_ClockDivision = TIM_CKD_DIV1; \
	tloc.TIM_CounterMode = TIM_CounterMode_Up; \
	TIM_TimeBaseInit(TIM17, &tloc); \
	TIM_ARRPreloadConfig(TIM17, ENABLE); \
	TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Enable); \
}
#define HW_TIM_US_PWM_UNINS
#define HW_TIM_US_PWM_UPD(duty)		TIM_SetCompare1(TIM17, ((dword)US_CTRL_PERIOD * (dword)(duty) + 128UL) / 256UL)
#define HW_TIM_US_PWM_OUT_INS(duty) \
{ \
	TIM_OCInitTypeDef loc; \
 \
												/* Output compare PWM 1 */ \
	TIM_OCStructInit(&loc); \
	loc.TIM_OCMode = TIM_OCMode_PWM1; \
	loc.TIM_Pulse = ((dword)US_CTRL_PERIOD * (dword)(duty) + 128UL) / 256UL; \
	loc.TIM_OutputState = TIM_OutputState_Enable; \
	TIM_OC1Init(TIM17, &loc); \
	TIM_CCxCmd(TIM17, TIM_Channel_1, TIM_CCx_Enable); \
	TIM_CCxNCmd(TIM17, TIM_Channel_1, TIM_CCxN_Enable); \
	TIM_OC1NPolarityConfig(TIM17, TIM_OCNPolarity_High); \
	TIM_OC1PolarityConfig(TIM17, TIM_OCPolarity_High); \
}
#define HW_TIM_US_PWM_OUT_ENA		TIM_CtrlPWMOutputs(TIM17, ENABLE)
#define HW_TIM_US_PWM_OUT_DIS		TIM_OC1NPolarityConfig(TIM17, TIM_OCNPolarity_Low), TIM_CtrlPWMOutputs(TIM17, DISABLE)
#define HW_TIM_US_PWM_HANDLER()		void TIM17_IRQHandler(void)
#define HW_TIM_US_PWM_INT_TST		(!(TIM_GetITStatus(TIM17, TIM_IT_CC1) == RESET))
#define HW_TIM_US_PWM_INT_ACK		TIM_ClearITPendingBit(TIM17, TIM_IT_CC1)
#define HW_TIM_US_PWM_INT_PEND_CLR	HW_TIM_US_PWM_INT_ACK
#define HW_TIM_US_PWM_INT_ENA		TIM_ITConfig(TIM17, TIM_IT_CC1, ENABLE)
#define HW_TIM_US_PWM_INT_DIS		TIM_ITConfig(TIM17, TIM_IT_CC1, DISABLE)
#define HW_TIM_US_PWM_INT_INS \
{ \
	NVIC_InitTypeDef iloc; \
 \
	iloc.NVIC_IRQChannel = TIM17_IRQn; \
	iloc.NVIC_IRQChannelPriority = 0x03; \
	iloc.NVIC_IRQChannelCmd = ENABLE; \
	NVIC_Init(&iloc); \
}
#define HW_TIM_US_PWM_INT_UNINS \
{ \
	NVIC_InitTypeDef iloc; \
 \
	iloc.NVIC_IRQChannel = TIM17_IRQn; \
	iloc.NVIC_IRQChannelPriority = 0x03; \
	iloc.NVIC_IRQChannelCmd = DISABLE; \
	NVIC_Init(&iloc); \
}
//*** FLA ***************************************************
#define HW_FLA_UNLOCK					FLASH_Unlock()
#define HW_FLA_LOCK						FLASH_Lock()
#define HW_FLA_PAGE_ERASE_TST(adr)		(FLASH_ErasePage((adr)) == FLASH_COMPLETE)
#define HW_FLA_WORD_WRITE_TST(adr, dw)	(FLASH_ProgramHalfWord((adr), (dw)) == FLASH_COMPLETE)
#define HW_FLA_DWORD_WRITE_TST(adr, dw)	(FLASH_ProgramWord((adr), (dw)) == FLASH_COMPLETE)
//*** Debug timing ******************************************
//*** I2C ***************************************************
// I2C2 (PF7, PF6 = SDA/, SCL/)
#define HW_I2C_INS \
{ \
	I2C_InitTypeDef i2cInit; \
	GPIO_InitTypeDef gpioInit; \
										/* Inizializzazione dei pin della porta (SDA, SCL)*/ \
										/* *(DWORD *)(0x48001400) = 0x5555AAAA; */ \
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE); \
	gpioInit.GPIO_Pin = GPIO_Pin_6; \
	gpioInit.GPIO_Mode = GPIO_Mode_AF; \
	gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL; \
	gpioInit.GPIO_OType = GPIO_OType_OD; \
	gpioInit.GPIO_Speed = GPIO_Speed_2MHz; \
	GPIO_Init(GPIOF, &gpioInit); \
	gpioInit.GPIO_Pin = GPIO_Pin_7; \
	GPIO_Init(GPIOF, &gpioInit); \
										/* Impostazione della I2C */ \
	i2cInit.I2C_Ack = I2C_Ack_Enable; \
	i2cInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; \
	i2cInit.I2C_AnalogFilter = I2C_AnalogFilter_Enable; \
	i2cInit.I2C_Mode = I2C_Mode_I2C; \
	i2cInit.I2C_Timing = 0x5033060A; \
	i2cInit.I2C_DigitalFilter = 0; \
	i2cInit.I2C_OwnAddress1 = (I2C_SLA_GRP_1 << 4) | (I2C_SLA_GRP_2 << 1); \
	I2C_Init(I2C2, &i2cInit); \
	I2C_Cmd(I2C2, ENABLE); \
}
#define HW_I2C_INT_INS \
    NVIC_SetPriority(I2C2_IRQn, 0x00), \
    NVIC_EnableIRQ(I2C2_IRQn)
//
#define HW_I2C_INT_ENA 									I2C_ITConfig(I2C2, I2C_IT_ADDRI | I2C_IT_STOPI, ENABLE)
#define HW_I2C_INT_DIS 									I2C_ITConfig(I2C2, I2C_IT_ADDRI | I2C_IT_STOPI, DISABLE)
//
#define HW_I2C_RX_INT_ENA								I2C_ITConfig(I2C2, I2C_IT_RXI, ENABLE)
#define HW_I2C_RX_INT_DIS								I2C_ITConfig(I2C2, I2C_IT_RXI, DISABLE)
#define HW_I2C_TX_INT_ENA								I2C_ITConfig(I2C2, I2C_IT_TXI, ENABLE)
#define HW_I2C_TX_INT_DIS								I2C_ITConfig(I2C2, I2C_IT_TXI, DISABLE)
#define HW_I2C_RX_ADR_TST								(!(I2C_GetFlagStatus(I2C2, I2C_ISR_ADDR) == RESET))
#define HW_I2C_RX_ADR_ACK								I2C_ClearFlag(I2C2, I2C_ICR_ADDRCF)
#define HW_I2C_RX_NAK_TST								(!(I2C_GetFlagStatus(I2C2, I2C_ISR_NACKF) == RESET))
#define HW_I2C_RX_NAK_ACK								I2C_ClearFlag(I2C2, I2C_ICR_NACKCF)
#define HW_I2C_RX_STOP_TST								(!(I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF) == RESET))
#define HW_I2C_RX_STOP_ACK								I2C_ClearFlag(I2C2, I2C_ICR_STOPCF)
//
#define HW_I2C_RX_ADR_GET(adr)							adr = I2C_GetAddressMatched(I2C2)
#define HW_I2C_TX_CRT(crt) 								I2C_SendData(I2C2, (crt))
#define HW_I2C_TX_IDLE_TST								(!(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET))
#define HW_I2C_TX_EMPTY_TST								(!(I2C_GetFlagStatus(I2C2, I2C_ISR_TCR) == RESET))
#define HW_I2C_RX_CRT_TST								(!(I2C_GetFlagStatus(I2C2, I2C_ISR_RXNE) == RESET))
#define HW_I2C_RX_CRT(crt)								crt = I2C_ReceiveData(I2C2)
#define HW_I2C_STOP										I2C_ClearFlag(I2C2, I2C_ICR_STOPCF)
#define HW_I2C_RUN_TST									(I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF) == RESET)
#define HW_I2C_EVE_HANDLER() 							void I2C2_IRQHandler(void)
