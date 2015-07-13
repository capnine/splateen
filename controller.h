#ifndef __gltest__controller__
#define __gltest__controller__


extern int mySpecialValue;
extern int myKeyboardValue;

void initController(void);
void myKeyboardFunc(unsigned char key, int x, int y);
void myKeyboardUpFunc(unsigned char key, int x, int y);
void mySpcialFunc(int key, int x, int y);
void mySpcialUpFunc(int key, int x, int y);

#endif /* defined(__gltest__controller__) */
