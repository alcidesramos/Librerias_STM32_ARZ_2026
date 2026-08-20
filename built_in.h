#ifndef __built_in_h
#define __built_in_h

//--- 8-bit
#define Lo(param) ((char *)&param)[0]
#define Hi(param) ((char *)&param)[1]
#define Higher(param) ((char *)&param)[2]
#define Highest(param) ((char *)&param)[3]
#define LOBYTE(x)  ((uint8_t)((x) & 0x00FFU))
#define HIBYTE(x)  ((uint8_t)(((x) & 0xFF00U) >> 8U))

#define lo(param) ((char *)&param)[0]
#define hi(param) ((char *)&param)[1]
#define higher(param) ((char *)&param)[2]
#define highest(param) ((char *)&param)[3]


//--- 16-bit
#define LoWord(param) ((unsigned *)&param)[0]
#define HiWord(param) ((unsigned *)&param)[1]

#define loword(param) ((unsigned *)&param)[0]
#define hiword(param) ((unsigned *)&param)[1]

#endif //__built_in_h
