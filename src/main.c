/**
  ******************************************************************************
  * @file    STemWin/STemWin_HelloWorld/Src/main.c
  * @author  MCD Application Team
  * @version V1.3.1
  * @date    09-October-2015
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "WM.h"
//#include "board.h"
#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "ff.h"
#include "DIALOG.h"
#include "Constant.h"
#include "Modbus.h"
//#include "uart_17xx_40xx.h"
#include "Display.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define KEY_Enter_Pin 			GPIO_PIN_4
#define KEY_Enter_GPIO_Port 	GPIOC
#define KEY_Up_Pin 				GPIO_PIN_5
#define KEY_Up_GPIO_Port 		GPIOC

/* Private variables ---------------------------------------------------------*/
uint8_t GUI_Initialized = 0;
TIM_HandleTypeDef TimHandle;
uint32_t uwPrescalerValue = 0;

/* Private function prototypes -----------------------------------------------*/
static void BSP_Config(void);
static void SystemClock_Config(void);
//void BSP_Pointer_Update(void);
//void BSP_Background(void);

extern void MainTask(void);

void key_cfg();

#define SDRAM_BANK_ADDR                 ((uint32_t)0xC0000000)
extern SDRAM_HandleTypeDef SdramHandle;


extern unsigned short page;


typedef struct
{
	unsigned char strt;
	unsigned char cmd;
	unsigned short data;
	unsigned char stop;
}keypad;



/*static const buf_arry Lookup_PLC[] = {
  { 1, 1, 0},
  { 2, 1, 28, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57,58},
  { 3, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 4, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 5, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 6, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 7, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 8, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 9, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 10, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 11, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 180, 181, 182, 183, 184, 185, 186, 187},
  { 12, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 190, 191, 192, 193, 194, 195, 196, 197},
  { 13, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 200, 201, 202, 203, 204, 205, 206, 207},
  { 14, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 210, 211, 212, 213, 214, 215, 216, 217},
  { 15, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 220, 221, 222, 223, 224, 225, 226, 227},
  { 16, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 230, 231, 232, 233, 234, 235, 236, 237},
  { 17, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 240, 241, 242, 243, 244, 245, 246, 247},
  { 18, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 250, 251, 252, 253, 254, 255, 256, 257},
  { 19, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 260, 261, 262, 263, 264, 265, 266, 267},
  { 20, 1, 43, 1, 2, 3, 4, 5, 6, 7, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323},
  { 21, 1, 43, 1, 2, 3, 4, 5, 6, 8, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353},
  { 22, 1, 43, 1, 2, 3, 4, 5, 6, 9, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383},
  { 23, 1, 43, 1, 2, 3, 4, 5, 6, 10, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413},
  { 24, 1, 43, 1, 2, 3, 4, 5, 6, 11, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443},
  { 25, 1, 43, 1, 2, 3, 4, 5, 6, 12, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473},
  { 26, 1, 43, 1, 2, 3, 4, 5, 6, 13, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503},
  { 27, 1, 43, 1, 2, 3, 4, 5, 6, 14, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523},
  { 28, 1, 43, 1, 2, 3, 4, 5, 6, 15, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 341, 542, 543, 544, 545, 546, 547, 548, 559, 560, 561, 562, 563},
  { 29, 1, 43, 1, 2, 3, 4, 5, 6, 16, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593},
  { 30, 1, 32, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113},
  { 31, 1, 43, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144},
  { 32, 1, 24, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 150, 151, 152, 153, 154, 155},
  { 33, 1, 121,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,	621,622,623,624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,
		  641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,
	      681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,720},
};

static const buf_arry Lookup_HMI[] = {
  { 1, 1, 0},
  { 2, 1, 28, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57,58},
  { 3, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 4, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 5, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 6, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 7, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 8, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 9, 1, 18, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58},
  { 10, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 11, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 12, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 13, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 14, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 15, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 16, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 17, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 18, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 19, 1, 33, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 160, 161, 162, 163, 164, 165, 166, 170, 171, 172, 173, 174, 175, 176, 177},
  { 20, 1, 43, 1, 2, 3, 4, 5, 6, 7, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 21, 1, 43, 1, 2, 3, 4, 5, 6, 8, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 22, 1, 43, 1, 2, 3, 4, 5, 6, 9, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 23, 1, 43, 1, 2, 3, 4, 5, 6, 10, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 24, 1, 43, 1, 2, 3, 4, 5, 6, 11, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 25, 1, 43, 1, 2, 3, 4, 5, 6, 12, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 26, 1, 43, 1, 2, 3, 4, 5, 6, 13, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 27, 1, 43, 1, 2, 3, 4, 5, 6, 14, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 28, 1, 43, 1, 2, 3, 4, 5, 6, 15, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 29, 1, 43, 1, 2, 3, 4, 5, 6, 16, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
  { 30, 1, 32, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113},
  { 31, 1, 43, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144},
  { 32, 1, 24, 1, 2, 3, 4, 5, 6, 17, 18, 19, 50, 51, 52, 53, 54, 55, 56, 57, 58, 150, 151, 152, 153, 154, 155},
  { 33, 1, 121,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,	321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,
		  341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,
	      381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420},
};
*/
const unsigned char Pagenum_list[Pic_count]={1,2,3,4,5,6,7,8,9,10,20,30,31,32,33};
const unsigned char Spic_Pagenum_list[Spic_count]={36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55};
const  char MyAdd = 1;
const  char rTry = 3;
const  unsigned short Timeout = 100;
const  unsigned short PoolTime=1000;
const  unsigned short ScanTimeMs =100;



//*******************private functions*************************
void HW_Config();

extern void view_Image(int page1, U32 hmg);

unsigned char ModPool_send (unsigned char ch);
unsigned char ModPool_rcv (void);

unsigned char ModSlave_send (unsigned char ch);
unsigned char ModSlave_rcv (void);

//*************************************************************


keypad *keyvalue;
//buf_arry *pPlcaray=(buf_arry *) Lookup_PLC,*pPlcTemp;
GUI_WIDGET_CREATE_INFO *NowPage;


 FATFS fatFS;	// File system object //

Bool readkey_flag,Display_flag,startup_flag,tout_flag,rcv_keyvalue_flag;


unsigned short page_number;
WORD Buff_hmi[500];
int Release;
int count_lcd_scan;
int count_key;
unsigned short PollTimeMs;
unsigned short TxTimeMs;
unsigned short ledscantime;
WORD cnttt;

unsigned char iii;
unsigned char tout_cnt;
unsigned char rcv_buf[5],rcv_byte_cnt=0;


extern unsigned char tr_buf[5],*pntr;
extern volatile int TimeMS;  			// Defined in GUI_X.c
extern volatile uint32_t timerCntms; 	// Defined in sdmmc
extern unsigned short page;
extern unsigned short num_feeder;
extern unsigned short temp_f;
extern unsigned short keybyte;

extern Bool Txt_Exist_flag;
extern Bool Init_dis_flag,key_flag;

int loc_xf1[100];




/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */ 
int main(void)
{  
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  //printf("emWin...\n");
  
  /* Initialize LCD and LEDs */
  BSP_Config();
  key_cfg();
  
  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
  
  /***********************************************************/

   /* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */
  uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
  
  /* Set TIMx instance */
  TimHandle.Instance = TIM3;
   
  /* Initialize TIM3 peripheral as follows:
       + Period = 500 - 1
       + Prescaler = ((SystemCoreClock/2)/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period = 500 - 1;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    while(1) 
    {
    }
  }
  
  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
  {
    while(1) 
    {
    }
  }
  
//*** memory test *************************************************
//  uint16_t dr,dw;
//  uint16_t *idx;
//  for(;;)
//	  for(uint32_t i =0 ; i < 1024 ; i += 1 )
//	  {
//		  for(uint16_t d =0 ; d < 255 ; ++d )
//		  {
//			  idx = (uint16_t*)SDRAM_BANK_ADDR + i;
//			  dw =(d << 8) | d;
//			  *(volatile uint16_t*)idx++ = dw;
//
//			  idx = (uint16_t*)SDRAM_BANK_ADDR + i;
//			  dr = (*(volatile uint16_t*) idx++ );
//
//			  if( dw != dr )
//				  printf("mismatch @ %i wr: 0x%0.4X rd: 0x%0.4X\n",i,dw,dr);
//		  }
//	  }
  
  /* Init the STemWin GUI Library */
  GUI_Init();
  GUI_Initialized = 1;

  int16_t x,y,dx,dy;
  x =0;
  y =0;
  dx =50;
  dy =50;

  /* Activate the use of memory device feature */
  WM_SetCreateFlags(WM_CF_MEMDEV);

  // set bg color
  GUI_SetBkColor(0x00FFFFFF);
  // clear LCD with bg color
  GUI_Clear();
  GUI_DispStringAt("Hello world!",x,y);
  GUI_Delay(500);

  uint16_t lcd_x_sz =LCD_GetXSize();
  uint16_t lcd_y_sz =LCD_GetYSize();

  for(;;)
  {
//	  ********************************************************
//	  GUI_SetColor(GUI_LIGHTBLUE);
//	  GUI_DrawGradientV(0 , lcd_y_sz , lcd_x_sz - 1 , lcd_y_sz * 2 - 1 , 0xFFFFFF , 0x000000 );
//	  if (  HAL_GPIO_ReadPin(KEY_Enter_GPIO_Port,KEY_Enter_Pin) )
//	  {
//		  GUI_DrawBitmap(&bmHydrangeas ,0 			 , lcd_y_sz + 0 		   );
//		  GUI_DrawBitmap(&bmHydrangeas ,lcd_x_sz / 2 , lcd_y_sz + lcd_y_sz / 2 );
//		  GUI_DrawBitmap(&bmPenguans   ,lcd_x_sz / 2 , lcd_y_sz + 0 		   );
//		  GUI_DrawBitmap(&bmPenguans   ,0 			 , lcd_y_sz + lcd_y_sz / 2 );
//		  GUI_SetOrg(0,lcd_y_sz);
//	  }

//	  ********************************************************

//	  GUI_SetColor(GUI_LIGHTGRAY);
//	  GUI_FillRect(0 , 0 , lcd_x_sz - 1 , lcd_y_sz - 1 );

	  if (  HAL_GPIO_ReadPin(KEY_Up_GPIO_Port,KEY_Up_Pin) )
	  {
//		  GUI_DrawBitmap(&bmDisplay12   , 0 			  , 0 			 );
//		  GUI_DrawBitmap(&bmPenguans   , lcd_x_sz / 2 , lcd_y_sz / 2 );
//		  GUI_DrawBitmap(&bmHydrangeas , lcd_x_sz / 2 , 0 			 );
//		  GUI_DrawBitmap(&bmHydrangeas , 0 			  , lcd_y_sz / 2 );
//		  GUI_DrawBitmap(&bmPenguansIco, 380 , 200 );
		  GUI_SetOrg(0,0);
		  intlcd();
		  page = page_Display;
		  lcdscan();
	  }

	 // GUI_Delay(5000);

  }


  for(;;)
  {
	  GUI_Clear();
	 // GUI_DrawBitmap(&bmHydrangeas,x,y);
	  x += dx;
	  y += dy;
	  GUI_Delay(2000);

	  if ( x > 600 )
		  dx = -50;
	  if ( x < 100 )
		  dx = +50;

	  if ( y > 300 )
		  dy = -50;
	  if ( y < 100 )
		  dy = +50;
  }
  /* Activate the use of memory device feature */
//  WM_SetCreateFlags(WM_CF_MEMDEV);
  
//  MainTask();

  /* Infinite loop */  
  while (1) 
  {
  }
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//  BSP_Background();
}

/**
  * @brief TIM MSP Initialization 
  *        This function configures the hardware resources used in This application: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM3_CLK_ENABLE();

  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
  
  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

/**
  * @brief  Initializes the STM32F429I-DISCO's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
static void BSP_Config(void)
{
  /* Initializes the SDRAM device */
  BSP_SDRAM_Init();
  
  /* Enable the CRC Module */
  __HAL_RCC_CRC_CLK_ENABLE();
}


void SystemClock_Config(void)
{


  RCC_OscInitTypeDef RCC_OscInitStruct;


  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  HAL_PWREx_ActivateOverDrive();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 50;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  //HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  //HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  //HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void key_cfg()
{

	  GPIO_InitTypeDef GPIO_InitStruct;

	  /* GPIO Ports Clock Enable */
//	  __GPIOA_CLK_ENABLE();
//	  __GPIOB_CLK_ENABLE();
	  __GPIOC_CLK_ENABLE();
//	  __GPIOG_CLK_ENABLE();
//	  __GPIOH_CLK_ENABLE();

	  /*Configure GPIO pins : Enter_Pin Up_Pin */
	  GPIO_InitStruct.Pin = KEY_Enter_Pin |KEY_Up_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init( GPIOC , & GPIO_InitStruct );
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

