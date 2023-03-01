/* CSCI 200: Assignment 3: L3B - Pointers
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        Used a lot of the CPP reference website, https://cplusplus.com/reference/string/string/     
 *
 * gain familiarity with using the concepts learned on pointers, addresses, and dynamic memory.
 */

#include <iostream>

using namespace std;

int main()
{
    int iNum = 9;
    double dNum = 14.25;
    int *pINum1, *pINum2;
    double *pDNum;

    pINum1 = &iNum;
    cout << "The address of iNum is: " << &iNum << endl;
    cout << "The address of iNum stored in pINum1 is: " << pINum1 << endl;
    cout << "The contents of iNum: " << *pINum1 << endl;

    // pINum2 = &dNum; Error
    // D:\Repos\CSCI-Classes\CSCI 200\Set3\L3B\main.cpp:27:14: error: cannot convert 'double*' to 'int*' in assignment

    pDNum = &dNum;
    cout << "The address of dNum is: " << pDNum << endl;
    cout << "The contents of dNum: " << *pDNum << endl;

    iNum = 7;
    cout << "The contents of iNum using pINum1: " << *pINum1 << endl;

    pINum2 = pINum1;
    cout << "The address stored in pINum2: " << pINum2 << endl;
    cout << "The contents of iNum using pINum2: " << *pINum2 << endl;

    *pINum2 = 12;
    cout << "The contents of iNum using pINum1: " << *pINum1 << endl;
    cout << "The contents of iNum using pINum2: " << *pINum2 << endl;
    cout << "The contents of iNum directly: " << iNum << endl;

    return 0;
}