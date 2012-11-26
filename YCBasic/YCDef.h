#ifndef _INCLUDE_YCDEFINE_H_
#define _INCLUDE_YCDEFINE_H_

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef _DEBUG
  #pragma warning (disable : 4996)
#endif

//
// �������Ͷ���
//
#define IN              // ���
#define OUT				// ����
#define IN_OUT          // ���


#ifndef NULL
  #define NULL 0
#endif

#define SAFE_DELETE(p)  if(p) {delete p; p=NULL;}
#define SAFE_DELETE_ARRAY(p) if(p) {delete[] p; p=NULL;}

#define SAFE_RELEASE(p) if(p) {p->Release(); p=NULL;}  

#ifndef EPSINON
  #define EPSINON 0.00001f
#endif

#define BUFFER_16 16
#define BUFFER_32 16

#endif