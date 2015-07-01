#ifndef gltest_controller_h
#define gltest_controller_h


extern int mySpecialValue;
extern int myKeyboardValue;

void myKeyboardFunc(unsigned char key, int x, int y);
void myKeyboardUpFunc(unsigned char key, int x, int y);
void mySpcialFunc(int key, int x, int y);
void mySpcialUpFunc(int key, int x, int y);

#endif
