
// --------- March 2017, Ian L Jones ---------
//
//GetContentRatio
//    ->SetInput1(DOUBLE VOL, fcMask);
//    ->SetInput2(DOUBLE VOL, wcMask);
//    ->GetOutput(DOUBLE VOL);
//


#ifndef __GetContentRatio_txx
#define __GetContentRatio_txx
#include "GetContentRatio.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "math.h"

namespace itk
{

template< typename TInput1, typename TInput2, typename TOutput>
GetContentRatio<TInput1, TInput2, TOutput>::GetContentRatio()
{

    this->SetNumberOfRequiredInputs(2);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TInput1, typename TInput2, typename TOutput>
void GetContentRatio<TInput1, TInput2, TOutput>::SetInput1(const TInput1* image1)
{
    this->SetNthInput(0, const_cast<TInput1*>(image1));
}
template< typename TInput1, typename TInput2, typename TOutput>
void GetContentRatio<TInput1, TInput2, TOutput>::SetInput2(const TInput2* image2)
{
    this->SetNthInput(1, const_cast<TInput2*>(image2));
}

template< typename TInput1, typename TInput2, typename TOutput >
typename TInput1::ConstPointer GetContentRatio<TInput1, TInput2, TOutput>::GetInput1()
{
    return static_cast< const TInput1 * >
           ( this->ProcessObject::GetInput(0) );
}

template< typename TInput1, typename TInput2, typename TOutput >
typename TInput2::ConstPointer GetContentRatio<TInput1, TInput2, TOutput>::GetInput2()
{
    return static_cast< const TInput2 * >
           ( this->ProcessObject::GetInput(1) );
}


template< typename TInput1, typename TInput2, typename TOutput>
void GetContentRatio<TInput1, TInput2, TOutput>::GenerateData()
{

    typename TInput1::ConstPointer image1 = this->GetInput1();
    typename TInput2::ConstPointer image2 = this->GetInput2();
    typename TInput2::RegionType region = image2->GetLargestPossibleRegion();
    typename TInput2::SizeType regionSize = region.GetSize();

    typename TOutput::Pointer output = this->GetOutput();

    output->SetRegions(regionSize);
    output->Allocate();

    for (int i=0; i<=regionSize[0]-1; i++)
    {
        for (int j=0; j<=regionSize[1]-1; j++)
        {
	    for (int m=0; m<=regionSize[2]-1; m++)
            {
            // get pixel value
            typename TInput1::IndexType Pixel1 ;
            Pixel1[0] = i;
            Pixel1[1] = j;
            Pixel1[2] = m;
            // get pixel value
            typename TInput1::PixelType valIm1 = image1->GetPixel(Pixel1);
            typename TInput2::PixelType valIm2 = image2->GetPixel(Pixel1);

            // fr = fc./(fc+wc)*100;
            double aOut = 100*valIm1/(valIm1+valIm2);

            output->SetPixel(Pixel1, aOut);
	    }
        }
    }
}



template< typename TInput1, typename TInput2, typename TOutput>
DataObject::Pointer GetContentRatio<TInput1, TInput2, TOutput>::
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

template< typename TInput1, typename TInput2, typename TOutput>
TOutput* GetContentRatio<TInput1, TInput2, TOutput>::GetOutput()
{
    return dynamic_cast< TOutput * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace



#endif
