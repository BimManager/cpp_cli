
/*
 *  debug.c 
 */

#using <System.dll>
#using <System.Diagnostics.Debug.dll>

 using System::Diagnostics::Debug;

int main(void)
{
    #ifdef DEBUG
    System::Console::WriteLine("Debug");
    Debug::WriteLine("Foo");
    #endif
    return (0);
}