#include <iostream>
#include <thread>
#include <numeric>
#include <vector>
#include <adios2.h>

int main(int argc, char **argv)
{
    std::vector<float> myFloats;
    std::vector<float> myRandom;
    std::vector<float> myTwoDims;
    
    adios2::Params engineParams;
    engineParams["IPAddress"] = "127.0.0.1";
    engineParams["Port"] = "12320";
    engineParams["Mode"] = "inquirer";
    
    adios2::ADIOS adios;
    adios2::IO io = adios.DeclareIO("TestIO");
    io.SetEngine("mhs");
    io.SetParameters(engineParams);
    int inquire_step =3;
    adios2::Engine engine = io.Open("Test", adios2::Mode::Read);
    auto status = engine.BeginStep();
    for(int i=0; i<2; i++){
        if(status == adios2::StepStatus::OK ){
            auto varFloats = io.InquireVariable<float>("myfloats");
            auto varRandom = io.InquireVariable<float>("myrandoms");
            auto varTwoDims = io.InquireVariable<float>("mytwodims");
            
            auto shape = varFloats.Shape();
            auto shapeRandom = varRandom.Shape();
            auto shapeTwoDims = varTwoDims.Shape();
           
            varTwoDims.SetStepSelection({inquire_step,1});
            varFloats.SetStepSelection({inquire_step,1});
            varRandom.SetStepSelection({inquire_step,1});
        
            varFloats.SetSelection({{0}, {7}});
            varRandom.SetSelection({{4}, {2}});
            varTwoDims.SetSelection({{3,3}, {5,5}});
            
            myFloats.resize(std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<size_t>() ));
            myRandom.resize(std::accumulate(shapeRandom.begin(), shapeRandom.end(), 1, std::multiplies<size_t>() ));
            myTwoDims.resize(std::accumulate(shapeTwoDims.begin(), shapeTwoDims.end(), 1, std::multiplies<size_t>()));

            engine.Get<float>(varFloats, myFloats.data());
            engine.Get<float>(varRandom, myRandom.data());
            engine.Get<float>(varTwoDims, myTwoDims.data());
            engine.EndStep();
            
            for(const auto &f : myFloats)
            {
                std::cout << f << ", ";
            }
            std::cout << "\n***************"<<std::endl;
            
            for(const auto &f : myRandom)
            {
                std::cout << f << ", ";
            }
            std::cout << "\n***************"<<std::endl;
        
            for( auto  &f : myTwoDims)
            {
                std::cout << f << ", ";
                
            }
            std::cout << "\n*****receive done**********"<< std::endl;
            inquire_step++;
        }
       else if (status == adios2::StepStatus::EndOfStream)
        {
            std::cout << "********** Sreaming end **********" << std::endl;
            break;
        }
        else if (status == adios2::StepStatus::NotReady)
        {
            std::cerr << "********** Error : NotReady ********** " << std::endl;
            continue;
        }
   }
    engine.Close();
    return 0;
}
