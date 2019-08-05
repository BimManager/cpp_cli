/*
 *  itr_arr.cpp
 */

# using <mscorlib.dll>

int main(void)
{
    array<int, 1>           ^arr = { 1, 4, 5, 1324, 2, 3 };
    cli::interior_ptr<int>  pb;
    cli::interior_ptr<int>  pe;

    pb = &(arr[0]);
    pe = pb + arr->Length;
    while (pb != pe)
    {
        *pb = *pb * 10;
        ++pb;
    }
    --pb;
    System::Console::WriteLine("The last element " + *pb);
    return (0);
}