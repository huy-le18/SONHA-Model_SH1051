;;;;;///*********************************************************************************************************//**
;;;;;// * @file    MAIN_PROGRAM.INC
;;;;;// * @version V1.00
;;;;;// * @date    11/13/2020
;;;;;// * @brief   The header file of the MAIN_PROGRAM library.
;;;;;// *************************************************************************************************************
;;;;;// * @attention
;;;;;// * <h2><center>Copyright (C) Best Solution Inc. All rights reserved</center></h2>
;;;;;// ************************************************************************************************************/
;;;;;///* ---------------------------------------------------------------------------------------------------------*/
;;;;;//
;;;;;///* ==================================================================================================================================================*/
;;;;;///** @addtogroup MAIN_PROGRAM
;;;;;//  * @{
;;;;;//  */
;;;;;///* ---------------------------------------------------------------------------------------------------------*/
;;;;;//
;;;;;///* ---------------------------------------------------------------------------------------------------------*/
;;;;;///* Exported functions --------------------------------------------------------------------------------------*/
;;;;;///* ---------------------------------------------------------------------------------------------------------*/
;;;;;///** @defgroup  MAIN_PROGRAM_exported_functions MAIN_PROGRAM exported functions
;;;;;//  * @{
;;;;;//  */
;;;;;//
;;;;;///* ---------------------------------------------------------------------- */
;;;;;///* assembly exported function,register                                    */
;;;;;///* ---------------------------------------------------------------------- */
;;;;;/*
;;;;;//-SUBROUTIN

#define MAIN_PROGRAM_DEF
#include "..\GLOBE\GLOBE_VARIES.INC"
#include "..\GLOBE\GLOBE_INCLUDE.INC"

;;;;;//-FUNCTION DEFINE               ;-FUNCTION NAME
#include "..\BS84B08C_CTOUCH\BS84B08C_CTOUCH.INI"
#define EXTEND_FUNCTION_1A_INITIAL          _BS84B08C_CTOUCH_INITIAL          
#define EXTEND_FUNCTION_1A                  _BS84B08C_CTOUCH                
#define EXTEND_FUNCTION_1A_HALT_PREPARE     _BS84B08C_CTOUCH_HALT_PREPARE      
#define EXTEND_FUNCTION_1A_RETURN_MAIN      _BS84B08C_CTOUCH_RETURN_MAIN      
     
;//;;;;;//--
;//#define EXTEND_FUNCTION_1C_INITIAL          _IIC_SLAVE_INITIAL          
;//#define EXTEND_FUNCTION_1C                  _IIC_SLAVE                  
;//#define EXTEND_FUNCTION_1C_HALT_PREPARE     _IIC_SLAVE_HALT_PREPARE     
;//#define EXTEND_FUNCTION_1C_HALT_WAKEUP      _IIC_SLAVE_HALT_WAKEUP      
;//#define EXTEND_FUNCTION_1C_RETURN_MAIN      _IIC_SLAVE_RETURN_MAIN      
      
;;;;;//--
#define EXTEND_FUNCTION_1D_INITIAL          _USER_PROGRAM_INITIAL          
#define EXTEND_FUNCTION_1D                  _USER_PROGRAM                  
#define EXTEND_FUNCTION_1D_HALT_PREPARE     _USER_PROGRAM_HALT_PREPARE     
#define EXTEND_FUNCTION_1D_HALT_WAKEUP      _USER_PROGRAM_HALT_WAKEUP      
#define EXTEND_FUNCTION_1D_RETURN_MAIN      _USER_PROGRAM_RETURN_MAIN      
 
;;;;;//-FLAG DEFINE
;;;;;*/
;;;;;////--------------------------------------------------------------------------
;;;;;//
;;;;;///**
;;;;;//  * @}
;;;;;//  */
;;;;;///* ==================================================================================================================================================*/


