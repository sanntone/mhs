#include <iostream>
#include <thread>
#include <numeric>
#include <vector>
#include <adios2.h>

int main(int argc, char **argv)
{
    std::vector<float> myFloats;

    adios2::Params engineParams;
    engineParams["IPAddress"] = "127.0.0.1";
    engineParams["Port"] = "12320";
    engineParams["Mode"] = "daemon";

    adios2::ADIOS adios;
    adios2::IO io = adios.DeclareIO("TestIO");
    io.SetEngine("mhs");
    io.SetParameters(engineParams);

    adios2::Engine engine = io.Open("Test", adios2::Mode::Read);

    while(true)
    {
    }

    engine.Close();
    return 0;
}



