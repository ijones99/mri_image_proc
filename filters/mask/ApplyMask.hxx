// --------- March 2017, Ian L Jones ---------
//
//ApplyMask
//    ->SetInput(DOUBLE VOL, wc1);
//    ->SetInputMask(DOUBLE VOL, maskNorm);
//    ->GetOutput(DOUBLE VOL);
//


#ifndef __ApplyMask_txx
#define __ApplyMask_txx
#include "ApplyMask.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "math.h"

namespace itk
{

template< typename TInput, typename TMask, typename TOutput>
ApplyMask<TInput, TMask, TOutput>::ApplyMask()
{

    this->SetNumberOfRequiredInputs(2);

    this->SetNthOutput( 0, this->MakeOutput(0) );

}

template< typename TInput, typename TMask, typename TOutput>
void ApplyMask<TInput, TMask, TOutput>::SetInput(const TInput* image)
{
    this->SetNthInput(0, const_cast<TInput*>(image));
}
template< typename TInput, typename TMask, typename TOutput>
void ApplyMask<TInput, TMask, TOutput>::SetInputMask(const TMask* mask)
{
    this->SetNthInput(1, const_cast<TMask*>(mask));
}

template< typename TInput, typename TMask, typename TOutput >
typename TInput::ConstPointer ApplyMask<TInput, TMask, TOutput>::GetInput()
{
    return static_cast< const TInput * >
           ( this->ProcessObject::GetInput(0) );
}

template< typename TInput, typename TMask, typename TOutput >
typename TMask::ConstPointer ApplyMask<TInput, TMask, TOutput>::GetInputMask()
{
    return static_cast< const TMask * >
           ( this->ProcessObject::GetInput(1) );
}


template< typename TInput, typename TMask, typename TOutput>
void ApplyMask<TInput, TMask, TOutput>::GenerateData()
{

    typename TInput::ConstPointer image = this->GetInput();
    typename TMask::ConstPointer mask = this->GetInputMask();
    typename TMask::RegionType region = mask->GetLargestPossibleRegion();
    typename TMask::SizeType regionSize = region.GetSize();

    typename TOutput::Pointer output = this->GetOutput();

    output->SetRegions(regionSize);
    output->Allocate();

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
            typename TInput::PixelType valIm = image->GetPixel(Pixel1);
            typename TMask::PixelType valMask = mask->GetPixel(Pixel1);

            double aOut = valIm*valMask;

            output->SetPixel(Pixel1, aOut);
	    }
        }
    }
}



template< typename TInput, typename TMask, typename TOutput>
DataObject::Pointer ApplyMask<TInput, TMask, TOutput>::
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

template< typename TInput, typename TMask, typename TOutput>
TOutput* ApplyMask<TInput, TMask, TOutput>::GetOutput()
{
    return dynamic_cast< TOutput * >(this->ProcessObject::GetOutput(0) );
}


}// end namespace



#endif
