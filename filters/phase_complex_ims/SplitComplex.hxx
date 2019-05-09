
// --------- March 2017, Ian L Jones ---------
//   
//SplitComplex
//    ->SetInput(DOUBLE VOL VEC)
//    ->GetOutput1(DOUBLE VOL);
//    ->GetOutput2(DOUBLE VOL);
//
//

#ifndef __SplitComplex_txx
#define __SplitComplex_txx
#include "SplitComplex.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "math.h"

namespace itk
{

template< typename TInput, typename TOutput1, typename TOutput2>
SplitComplex<TInput, TOutput1, TOutput2>::SplitComplex()
{

    this->SetNumberOfRequiredInputs(1);

    this->SetNthOutput( 0, this->MakeOutput(0) );
    this->SetNthOutput( 1, this->MakeOutput(1) );

}

template< typename TInput, typename TOutput1, typename TOutput2>
void SplitComplex<TInput, TOutput1, TOutput2>::SetInput(const TInput* imageComplex)
{
    this->SetNthInput(0, const_cast<TInput*>(imageComplex));
}


template< typename TInput, typename TOutput1, typename TOutput2 >
typename TInput::ConstPointer SplitComplex<TInput, TOutput1, TOutput2>::GetInput()
{
    return static_cast< const TInput * >
           ( this->ProcessObject::GetInput(0) );
}


template< typename TInput, typename TOutput1, typename TOutput2>
void SplitComplex<TInput, TOutput1, TOutput2>::GenerateData()
{

    typename TInput::ConstPointer imageComplex = this->GetInput();
    typename TOutput1::RegionType region = imageComplex->GetLargestPossibleRegion();
    typename TOutput1::SizeType regionSize = region.GetSize();

    typename TOutput1::Pointer outputReal = this->GetOutput1();
    typename TOutput2::Pointer outputImaginary = this->GetOutput2();

    outputReal->SetRegions(regionSize);
    outputReal->Allocate();
    outputImaginary->SetRegions(regionSize);
    outputImaginary->Allocate();

    typename TInput::PixelType inputData;

    for (int i=0; i<regionSize[0]; i++)
    {
        for (int j=0; j<regionSize[1]; j++)
        {

            for (int m=0; m<regionSize[2]; m++)
            {
                // get pixel value
                typename TInput::IndexType Pixel1 ;
                Pixel1[0] = i;
                Pixel1[1] = j;
                Pixel1[2] = m;
                // get pixel value
                inputData = imageComplex->GetPixel(Pixel1);
                //typename TOutput1::PixelType valI0 = imageI0->GetPixel(Pixel1);

                outputReal->SetPixel(Pixel1, inputData[0]);
                outputImaginary->SetPixel(Pixel1, inputData[1]);
            }


        }
    }
}



template< typename TInput, typename TOutput1, typename TOutput2>
DataObject::Pointer SplitComplex<TInput, TOutput1, TOutput2>::
MakeOutput(unsigned int idx)
{
    DataObject::Pointer output;

    switch ( idx )
    {
    case 0:
        output = ( TOutput1::New() ).GetPointer();
        break;
    case 1:
        output = ( TOutput2::New() ).GetPointer();
        break;

    default:
        std::cerr << "No output " << idx << std::endl;
        output = NULL;
        break;
    }
    return output.GetPointer();
}

template< typename TInput, typename TOutput1, typename TOutput2>
TOutput1* SplitComplex<TInput, TOutput1, TOutput2>::GetOutput1()
{
    return dynamic_cast< TOutput1 * >(this->ProcessObject::GetOutput(0) );
}

template< typename TInput, typename TOutput1, typename TOutput2>
TOutput2* SplitComplex<TInput, TOutput1, TOutput2>::GetOutput2()
{
    return dynamic_cast< TOutput2 * >(this->ProcessObject::GetOutput(1) );
}

}// end namespace



#endif
