/*=========================================================================
 *
 *  Ian L Jones 30.01.2017
 *  IN: vector (complex number representation)
 *  OUT: scalar
 *
 *  MATLAB code to translate
 *      mask = 255*ones(size(S(1).complex));
 *      mask(abs(S(1).complex) < mask_threshold) = NaN;
 *
 *=========================================================================*/

#ifndef __CreateMask_txx
#define __CreateMask_txx
#include "CreateMask.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "math.h"
#include "cmath"

namespace itk
{

template< typename TInput, typename TOutput>
CreateMask<TInput, TOutput>::CreateMask()
{

    this->SetNumberOfRequiredInputs(1);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TInput, typename TOutput>
void CreateMask<TInput, TOutput>::SetInput(const TInput* imageI0)
{
    this->SetNthInput(0, const_cast<TInput*>(imageI0));
}

template< typename TInput, typename TOutput >
typename TInput::ConstPointer CreateMask<TInput, TOutput>::GetInput()
{
    return static_cast< const TInput * >
           ( this->ProcessObject::GetInput(0) );
}

template< typename TInput, typename TOutput>
void CreateMask<TInput, TOutput>::GenerateData()
{

    double MASK_THRESHOLD  = this->GetMaskThreshold();
    //double MASK_THRESHOLD = 40;
    // std::cout << ">>>>>>>>>>>>>>>>>>>this far..."<< std::endl;
    typename TInput::ConstPointer imageI0 = this->GetInput();
    typename TInput::RegionType region = imageI0->GetLargestPossibleRegion();
    typename TInput::SizeType regionSize = region.GetSize();

    typename TOutput::Pointer output = this->GetOutput();

    // Setup output 1
    output->SetRegions(regionSize);
    output->Allocate();

    double c = 0;
    int MaskCtr = 0;
    int NanCtr = 0;
    for (int i=0; i<regionSize[0]; i++)
    {
        for (int j=0; j<regionSize[1]; j++)
        {
            for (int m=0; m<regionSize[2]; m++)
            {
                // get phase & mag values
                typename TInput::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                typename TInput::PixelType phiVal = imageI0->GetPixel(Pixel1);

                double absVal = std::sqrt(std::pow( phiVal[0],2) + std::pow(phiVal[1],2));


                if(absVal < MASK_THRESHOLD)
                {
                    c = 0;
                    NanCtr++;
                    //std::cout <<   "UNDER =" << (int)c<< std::endl;
                }
                else
                {
                    c = 255;
                    MaskCtr++;

                }

                /*if (i==102-1 && j==74-1)
                {
                    std::cout <<   "first vals: X=" << X << " " << "Y=" << Y << ": " <<ComplexNum[0] <<" " << ComplexNum[1] << "i"<<std::endl;
                }
                */
                 //std::cout <<  "c " << c<<  std::endl;
                output->SetPixel(Pixel1, c);

            }
        }
    }
    std::cout <<   "MaskCtr in CreateMask =" << MaskCtr << std::endl;
    std::cout <<   "NanCtr in CreateMask =" << NanCtr << std::endl;
}



template< typename TInput, typename TOutput>
DataObject::Pointer CreateMask<TInput, TOutput>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TOutput::New() ).GetPointer();
        break;
    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template< typename TInput, typename TOutput>
TOutput* CreateMask<TInput, TOutput>::GetOutput()
{
    return dynamic_cast< TOutput * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace



#endif
