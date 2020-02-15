
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"
#include "RingBuffer.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{


    CRingBuffer <float>buf(10);
    for (int i=0; i<10; i++)
        buf.putPostInc((float)i*2);
//    buf.show();
//    cout << buf.getPostInc() << endl;
//    cout << buf.getPostInc() << endl;
//    buf.show();
//    buf.putPostInc((float)-1);
//    buf.show();
    //cout << buf.get(0.3) << endl;
    //cout << buf.get(3.9) << endl;
    buf.show();
    cout << buf.get(3) << endl;
    return 0;
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

