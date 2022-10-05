#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <adios2.h>

template <class T>
void GenData(std::vector<T> &data, const size_t step, const adios2::Dims &start,
             const adios2::Dims &count, const adios2::Dims &shape)
{
    if (start.size() == 2)
    {
        for (size_t i = 0; i < count[0]; ++i)
        {
            for (size_t j = 0; j < count[1]; ++j)
            {
                data[i * count[1] + j] =
                    (i + start[1]) * shape[1] + j + start[0];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    size_t variable_size = 10;
    
    adios2::Dims shape({variable_size});
    adios2::Dims start({0});
    adios2::Dims count({variable_size});
    
    adios2::Dims shape2D({variable_size,variable_size});
    adios2::Dims start2D({0,0});
    adios2::Dims count2D({variable_size,variable_size});

    std::vector<float> myFloats(variable_size);
    for(size_t i=0; i<myFloats.size(); i++)
    {
        myFloats[i]=i;
    }
    
    std::vector<float> myRandoms(variable_size);
    for(size_t i=0; i<myRandoms.size(); i++)
    {
        myRandoms[i]=i+100;
    }
    
    size_t datasize = std::accumulate(count2D.begin(), count2D.end(), 1, std::multiplies<size_t>());
    std::cout<<"******* datasize: "<<datasize<<std::endl;
    std::vector<float> myTwoDims(datasize);
  

    adios2::Params engineParams;
    engineParams["IPAddress"] = "10.0.0.31";
    engineParams["Port"] = "9030";

    adios2::ADIOS adios;
    adios2::IO io = adios.DeclareIO("TestIO");
    io.SetEngine("mhs");
    io.SetParameters(engineParams);

    auto varFloats = io.DefineVariable<float>("myfloats", shape, start, count);
    auto varRandom = io.DefineVariable<float>("myrandoms", shape, start, count);
    auto varTwoDims = io.DefineVariable<float>("mytwodims", shape2D, start2D, count2D);

    adios2::Engine engine = io.Open("Test", adios2::Mode::Write);

    for(size_t i=0; i<10; i++)
    {
        engine.BeginStep();
        GenData(myTwoDims, i, start2D, count2D, shape2D);
        std::cout<<"\n Write Step: "<<engine.CurrentStep()<<std::endl;
        for(auto a: myTwoDims)
            std::cout<<a<<" ";
        std::cout<<"\n";
        engine.Put<float>(varFloats, myFloats.data());
        engine.Put<float>(varRandom, myRandoms.data());
        engine.Put<float>(varTwoDims, myTwoDims.data());
        engine.EndStep();
    }

    engine.Close();

    return 0;
}



