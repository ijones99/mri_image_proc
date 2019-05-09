//
// --------- March 2017, Ian L Jones ---------
//    
//PhaseToRad
//    SetInput(DOUBLE VOL); 
//    SetMaxPhaseVal(DOUBLE VAL);
//


#ifndef __itkPhaseToRad_hxx
#define __itkPhaseToRad_hxx

#include "PhaseToRad.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

//MATLAB :   S(i).phase = S(i).phase/max_phase*2*pi - pi;      % phase -> rad

namespace itk
{

template< class TImage>
void PhaseToRad< TImage>
::GenerateData()
{
    typename TImage::ConstPointer input = this->GetInput();
    typename TImage::Pointer output = this->GetOutput();
    //int MAX_PHASE_VAL = 4096;
    int MAX_PHASE_VAL  = this->GetMaxPhaseVal();

    double PI = 3.14159265358979323846;


    this->AllocateOutputs();

    ImageAlgorithm::Copy(input.GetPointer(), output.GetPointer(), output->GetRequestedRegion(),
                         output->GetRequestedRegion() );

    typename TImage::RegionType region = input->GetLargestPossibleRegion();
    typename TImage::SizeType regionSize = region.GetSize();

    // std::cout << "region size: " << regionSize << std::endl;
    typename TImage::IndexType Pixel1 ;
    for (int i=0; i<regionSize[0]; i++)
    {
        //std::cout << "i " << i << std::endl;
        for (int j=0; j<regionSize[1]; j++)
        {
            for (int m=0; m<regionSize[2]; m++)
            {
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                //MATLAB :   S(i).phase = S(i).phase/max_phase*2*pi - pi;      % phase -> rad
                typename TImage::PixelType PixelValIn = input->GetPixel(Pixel1);
                typename TImage::PixelType PixelValOut;
                PixelValOut = ((PixelValIn/MAX_PHASE_VAL)*2*PI)-PI;
                //PixelValOut = PixelValIn/MAX_PHASE_VAL*2*PI-PI;

                output->SetPixel( Pixel1, PixelValOut );
            }
        }
    }



}



}// end namespace


#endif
